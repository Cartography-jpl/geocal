# Editable python install from the autotools `make install`

Status: implemented in geocal-repo (branch develop_2_0). This document is
written so the same change can be applied to other repositories that use the
same autotools layout ("add the editable python mode option like geocal did").

## Quick checklist for another repository

1. Find where the repo runs `pip install` from `make install` (here
   `python/python.am`, `python-install` rule on `INSTALL_EXEC_HOOK`).
2. Add the `AC_ARG_ENABLE([python-editable-mode], ...)` block to
   `configure.ac` (section "Changes", item 1).
3. Add `PYTHON_EDITABLE_MODE = @python_editable_mode_default@` and the shell
   `if test -n` rule to the `.am` file (item 2). Keep any pip options the
   repo already uses, and use the repo's own variable for the source dir.
4. Add a docs note (item 3), regenerate (item 4), and verify (below).
5. Use `--enable-python-editable-mode` and `PYTHON_EDITABLE_MODE` exactly, so
   the interface is the same across repos.

## Goal

`make install` runs `pip install` on the pure python code in `python/`
(the `geocal` package). Normally this copies the code into the install
prefix. For development we often want an *editable* install (`pip install -e`),
so edits to `python/` are picked up without reinstalling.

Only the pure python code is affected. The SWIG wrappers (`geocal_swig`) and
the `.so` files are installed by the normal automake rules and always get a
full install.

Two ways to ask for editable:

1. `./configure --enable-python-editable-mode`: every `make install` is
   editable.
2. `make install PYTHON_EDITABLE_MODE=t`: editable for just that run, even if
   configure was not given the option.

Both can be undone for one run with `make install PYTHON_EDITABLE_MODE=`
(empty value means normal install). Any non-empty value means editable.

## Background: how python is installed

- `python/python.am` has a `python-install` rule, hooked into `make install`
  with `INSTALL_EXEC_HOOK+= python-install`. (`INSTALL_EXEC_HOOK` is defined
  in `common.am`, which collects all hooks in one place so automake doesn't
  complain.)
- `python/Makefile` (hand written, not generated) is a separate developer
  convenience Makefile with `install` (editable) and `install2` targets. It is
  not part of the autotools build. Leave it alone.

## Changes

### 1. `configure.ac`

Add next to the other `AC_ARG_WITH`/`AC_ARG_ENABLE` options (e.g. after
`--with-conda-install-support`):

```
#=================================================================
# Install the python code in editable mode (pip install -e) when doing
# "make install". Can also be selected for a single run with
# "make install PYTHON_EDITABLE_MODE=t". See
# claude_documents/python_editable_install_plan.md
# (drop the pointer to this file if the repo doesn't carry it)

AC_ARG_ENABLE([python-editable-mode],
	      [AS_HELP_STRING([--enable-python-editable-mode],
	      [install the python code in editable mode (pip install -e) in make install])],
	      [enable_python_editable_mode=$enableval],
	      [enable_python_editable_mode=no])
if test "$enable_python_editable_mode" = "yes"; then
   python_editable_mode_default=t
else
   python_editable_mode_default=
fi
AC_SUBST([python_editable_mode_default])
```

Notes:
- The option has to be `--enable-...`. A bare `--python-editable-mode` is not
  a valid autoconf option form; configure stops with
  `error: unrecognized option: '--python-editable-mode'`.
- Put this in the repo's `configure.ac`, not in a shared m4 file. Other
  repos share the m4/autotools files, so don't change those.

### 2. `python/python.am`

Replace the `python-install` rule (and remove the
`# TODO Have a editable version of this` comment):

```
# Set to a non-empty value to install the python code in editable mode.
# Default comes from configure (--enable-python-editable-mode), and can be
# overridden for a single run with "make install PYTHON_EDITABLE_MODE=t"
# (or "PYTHON_EDITABLE_MODE=" to force a normal install).
PYTHON_EDITABLE_MODE = @python_editable_mode_default@

python-install:
	if test -n "$(PYTHON_EDITABLE_MODE)"; then \
	  pip install --prefix=@prefix@ --force-reinstall --no-deps -e $(abs_top_srcdir)/@srcpython@; \
	else \
	  pip install --prefix=@prefix@ --force-reinstall --no-deps $(srcdir)/@srcpython@; \
	fi
```

Notes:
- A plain `VAR = value` in the Makefile is overridden by `make VAR=x` on the
  command line, so no `?=` is needed (and `?=` gives automake portability
  warnings).
- Use a shell `if`, not GNU make `ifeq`/`$(if ...)`, so the `.am` file stays
  portable automake.
- An editable install records the source path, so it must be absolute
  (`$(abs_top_srcdir)`); `$(srcdir)` can be relative in an out-of-tree build.
- `--force-reinstall --no-deps` is there so switching between editable and
  normal modes really replaces the existing install (the version number is the
  same in both modes, so pip would otherwise say "already satisfied").
  `--no-deps` keeps this from reinstalling numpy etc.
- If the repo already passes extra pip options (e.g. `--no-build-isolation`),
  keep them in both branches.

### 3. Docs

Add a short note to `CLAUDE.md` (Development Build Options) describing the
configure option and the `PYTHON_EDITABLE_MODE` make variable, and that the
source tree must stay in place for an editable install.

### 4. Regenerate

Run `./bootstrap` (or use `--enable-maintainer-mode`) and re-run `configure`,
since `configure.ac` and a `.am` file changed.

In geocal the build directory was configured with `--enable-maintainer-mode`,
so just running `make` regenerated `configure`, `Makefile.in` and re-ran
`config.status` by itself (about 1.5 minutes, mostly an unrelated rebuild).
No manual bootstrap was needed. Check with
`grep PYTHON_EDITABLE_MODE Makefile` in the build directory afterwards; you
should see `PYTHON_EDITABLE_MODE =` (empty by default).

Things to know about the regenerated files:
- `configure` and `Makefile.in` are checked into git in geocal. After
  regenerating, `git add` them along with `configure.ac` and `python.am`,
  otherwise other checkouts won't see the option. If the other repo doesn't
  check them in, ignore this.
- `aminclude_static.am` is also regenerated, but only its timestamp comment
  changes. Don't stage that noise.

## Verification

Run from the build directory (`$SITE` is the `site-packages` directory of the
install prefix):

- `make -n python-install`, `make -n python-install PYTHON_EDITABLE_MODE=t`
  and `make -n python-install PYTHON_EDITABLE_MODE=` show the right pip
  command (the `if test -n "..."` line shows which value was picked up).
  `make -n install PYTHON_EDITABLE_MODE=t` shows the same editable pip line,
  confirming the variable reaches the real install hook.
- Run the real `make python-install PYTHON_EDITABLE_MODE=t`, then plain
  `make python-install`. After each, check:
  - `cat $SITE/<pkg>-*.dist-info/direct_url.json` has
    `"dir_info": {"editable": true}` only after the editable install.
  - From a directory outside the source tree,
    `python -c "import <pkg>, <pkg>_swig; print(<pkg>.__file__)"` prints the
    source tree path for editable, and the `site-packages` path for normal.
  - The reverse direction (editable then normal, and normal then editable)
    really switches, which is what `--force-reinstall` is for.
- Check that `--enable-python-editable-mode` flips the default without
  touching the real build: run configure in a scratch directory (e.g.
  `/tmp/cfgtest`) using the original configure arguments, which are on line
  7 of the existing build's `config.log` (drop `--no-create --no-recursion`),
  add `--enable-python-editable-mode --prefix=/tmp/cfgtest/inst`, and look for
  `PYTHON_EDITABLE_MODE = t` in the new `Makefile`. In geocal this configure
  takes about 2 minutes, so run it in the background. Delete the scratch
  directory afterwards.
- Optionally run the python test suite against an editable install.

## Caveats

- Editable pip installs ignore `DESTDIR`, as the existing rule already does.
  Staged/packaged installs should use the normal mode.
- Known mypy limitation: `py.typed` isn't visible through a pip editable
  install (see comment in `python/Makefile`). This is existing behavior.
- Editable mode needs the source tree to stay where it is.
- `--force-reinstall --no-deps` only affects the python package itself. It
  won't clean up stale generated files left in `site-packages` by older
  installs of the SWIG package (we hit an unrelated case of this in geocal, a
  leftover `geocal_swig/raw_raster_image.py` that broke `import` after the
  `.so` was rebuilt). That is a separate problem from this feature.
- In geocal the pixi environment keeps whatever mode was installed last, so
  after testing a normal install you may want to run
  `make install PYTHON_EDITABLE_MODE=t` again.
