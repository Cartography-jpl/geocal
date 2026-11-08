# GeoCal C++ Modernization and Third-Party Library Plan

**Date:** 2026-10-22 (revised the same day after maintainer feedback; updated 2026-11-07 from the finished skeleton
rehearsal, §0, then from an independent review of this plan, §0.5)
**Status:** Planning. Done in geocal so far: clang-format and `make format`/`format-check`/`lint` (Phase 1 item 7),
and the `dir_change.h` fix (§8.2). No other step of §5 has started.
**Where the work starts (2026-10-23):** every step is first rehearsed in the small `swig-rules-skeleton` repository, renamed
`cxx-python-skeleton` on 2026-11-06 (`/home/smyth/Local/cxx-python-skeleton` on rifle,
`/data2/smythdata/cxx-python-skeleton` on frenchpeas; branch
`modernize`), following
`claude_notes/implementation_plan.md` there (its §0 has the status and next step). geocal work begins once the skeleton
has worked out the problems.
**The skeleton has finished its phases (2026-11-07):** S0–S7, then three code review passes, each with a fix plan
(the third fix plan is written and decided but set aside). §0 says what that found and what it changes here; §0.4
maps every item of the skeleton's "Notes for geocal" table to a step of this plan. **Next (decided 2026-11-07):** the
skeleton's third fix plan, at least T0–T2, then geocal's Phase 0.
**Scope:** The C++ library (`lib/`), its build system, and the third-party C++
libraries it depends on. Python-only code is covered only where the C++ work
affects it (the SWIG bindings, pickling).

This document builds on `CODE_QUALITY_ANALYSIS.md` and `progress.md` (both
from 2026-05-12) and on the other notes in `claude_documents/`. Those earlier
documents get some facts wrong, so every number below was measured from the
current tree (`git ls-files`, generated `*_wrap.cc` excluded) and from the
reference pixi environment at `../geocal-build/build-pixi`.

## 0. What the skeleton rehearsal found (added 2026-11-07)

Sources, all in the skeleton's `claude_notes/`: `implementation_plan.md` (§0 status, "Notes for geocal" table,
progress log, §13 decisions), `code_review_2026-11-06.md` with `code_review_fix_plan.md` (steps R0–R9),
`code_review_2026-11-07.md` with `code_review_2026-11-07_fix_plan.md` (F0–F10), and
`code_review_2026-11-07_third.md` with `code_review_2026-11-07_third_fix_plan.md` (T0–T9). "Skeleton decision N"
below means item N of that plan's §13, or D1–D18 of the fix plans.

### 0.1 Where the skeleton is

* Phases S0–S7 are done. The skeleton is C++23 with gcc 14 as the minimum (skeleton decision 22), uses
  `std::shared_ptr`, `std::optional` and `std::array`, has nanobind bindings (SWIG was removed in S6.6), and uses
  `mdspan` for raw pointer plus size interfaces (S7).
* Everything named after SWIG was renamed on 2026-11-06 (skeleton decision 23): `swig_rules/` →
  `cxx_python_rules/`, `SWIG_MAPPER_NAMESPACE` → `CXX_PYTHON_RULES_NAMESPACE`, `skeleton_swig` →
  `skeleton_cxx._bindings`. Its notes from before the rename use the old names.
* The order that worked: S0 → ST (test hardening) → S1 → S2 → S3 → **S5 (samples of geocal's class patterns)
  before S4** → S4 (`std::shared_ptr`) → S6 (nanobind) → S7. S5 had to come before S4 because S4 breaks the python
  bindings until S6, and the samples' SWIG results were the S6 target. The bindings were broken for one day.
* After S7, three fresh code reviews, each followed by a fix plan. The first two fix plans are done (except CI,
  deferred to the JPL runner). The third is written and decided but **set aside, not started**.

### 0.2 What each review pass found that the phases had missed

All findings were confirmed with small test programs before being called bugs.

| Pass | Found | Fixed in the skeleton? | geocal today (checked 2026-11-07) |
|---|---|---|---|
| **1** (2026-11-06, after S7) | **Bugs in code that came from geocal and framework:** Blitz serialization scrambles column-major arrays, reads reversed arrays out of bounds, and scrambles a load into a column-major target, with no error in any case; copying an `Observer` copies its self pointer, so registering the copy registers the original; loading an archive `chdir`s the whole process, which is unsafe with threads. **Bugs from the conversion:** `None` for a `shared_ptr` argument crashes python; C++ writes into read-only numpy arrays; the rules library hard-coded `Skeleton::`. **Old idioms:** implicit converting constructors, `typedef`, `const_cast` where `mutable` belongs, `std::map` as a public base, `range_check` macros, two exception types for the same errors, autoconf macros leaking from installed headers, 13 uninitialized members (the review named 4). **Bindings** that still looked generated. | Yes (R1–R9). Bindings rewritten by hand (2,704 → 1,561 lines) and the converter changed to write that style; strict warning flags; `.clang-tidy` grew from 7 checks to 17 plus `bugprone-*`. | The Blitz serialization and Observer copy bugs and the `chdir` are in geocal's code now (same code). The Blitz ones are latent: geocal serializes no column-major, reversed or nonzero-base array (plan review). **`None` for a `shared_ptr` argument crashes geocal's SWIG bindings too** (`ScaleImage(None, 2.0)` segfaults, confirmed 2026-11-07). The rest is code geocal gets in Phases 1 and 6. |
| **2** (2026-11-07) | **Rules-library bugs:** nested serialization (pickling a python subclass object that holds a C++ object) loses every `weak_ptr` written after it; `serialize_shared_ptr.h` has no include guard; XML archives that hold a python subclass aren't text, so the python string functions failed; the type registry key was shared by all projects. **Tooling gaps:** no sanitizer option, and the hand-configured ASan build **turned out not to be compiled with ASan at all** (configure replaced the `CXXFLAGS` given by hand); no UBSan, no `_GLIBCXX_ASSERTIONS`, no clang build, no CI. Thread safety not written down. Comments full of phase and review history. `configure` accepted Boost 1.46. | Yes (F1–F10), except CI (deferred). Found on the way: pytest's temporary directory was in the source tree, shared by every build directory, so two builds testing at once broke each other. | The `weak_ptr` and include-guard bugs are in geocal's `swig_rules/lib` now (the same commented-out code and unused `ptr_serialize_depth`; no guard). No geocal class serializes a `weak_ptr`, so the first affects downstream code, not geocal's own classes. `configure.ac` still asks for Boost 1.46. |
| **3** (2026-11-07) | **Bugs at the boundaries, where the tests only ran the happy path:** a python file object whose `write`/`read` raises is silently ignored by the C++ stream adapters; **an archive write to a full disk reports success for small archives, and hangs forever for `.gz`**; python 3.12 passes `configure` but the code needs 3.13; the `.sample` files geocal copies had drifted from the live files; `from skeleton import *` missed the C++ classes. | Not yet: T0–T9 written, decisions D14–D18 made, set aside. | **The archive write bug is in geocal now, confirmed 2026-11-07** with the installed geocal on frenchpeas: `serialize_write` of an `ImageCoordinate` to `/dev/full`, plain or `.gz`, returns with no error; a 20,000-entry `GenericObjectMap` gives `RuntimeError: output stream error` as a plain file and **hangs** (killed after 25 s) as `.gz`. **Also confirmed** (plan review): writing to a nonexistent directory (`/nonexistent_dir_xyz/a.xml`, also `.xml.gz` and `.bin`) returns with no error; geocal's `OstreamCompress` never checks that the file opened (the skeleton fixed that earlier, in R3.3). Both geocal envs (linux, osx) are on python 3.13.7. geocal has no `__all__`. |

### 0.3 What the passes show, and what it means here

1. **Converting code doesn't find its bugs.** The phases changed types and tools across the whole code and kept all
   tests passing, but the serious bugs found afterwards were old ones the conversion carried along unexamined. geocal
   has the same code, so it has those bugs now. **Here:** fix them early, in a new Phase 0 step 9, rather than in
   whichever phase happens to touch the file.
2. **Each pass found less, and further out.** Pass 1 found bugs and old idioms everywhere, pass 2 the shared rules
   library and the tooling, pass 3 only the boundaries. The third pass still found two bugs that lose data, so one
   review would have been too few. **Here:** review passes are part of the plan (§5 "Review passes").
3. **Most bugs sat where the tests only ran the happy path:** a `BytesIO` that never fails, a disk that never fills,
   the newest python, the live file rather than its copy, valid arguments only. **Here:** when a phase converts a
   feature, add at least one failure-path test for it (a bad argument from python, a failing file, a wrong type, the
   copied file rather than the original).
4. **Checks that didn't check.** The ASan build that wasn't built with ASan; `make check` testing the installed
   library (§6.1); a warning flag tried with `-fsyntax-only`, which never runs it (`-Wnull-dereference`); samples
   nobody compared with the live files; a minimum python no build used. **Here:** when adding a check, break
   something on purpose once and see the check fail (the skeleton did this for `script/check_all.sh` and its
   type-registration test).
5. **Copies drift.** Files copied by hand (between the repositories, or into `.sample` files) drifted within a day.
   The skeleton will generate its samples (third fix plan T4). **Here:** copy from the skeleton only by diffing file by
   file (skeleton decision 15), and take samples only from generated ones.
6. **Baselines made each step checkable.** Each fix plan began with recorded test counts, an API snapshot
   (`script/api_snapshot.py`), a byte-for-byte golden archive comparison and strict-`xfail` reproductions of each bug,
   and ended with a table of checks (mostly `grep -r`) showing each problem gone. **Here:** the same tools come in
   Phase 0 step 4, and each phase records a baseline before it starts.
7. **Fix plans need their own review.** An independent review of the first fix plan found 13 errors and 11 omissions
   before any code changed, for example deleting typedefs that were in use, and a fix that covered only one of three
   related bugs. **Here:** have each geocal phase plan reviewed the same way before starting it.

### 0.4 The skeleton's "Notes for geocal", item by item

The skeleton plan §0 asks a geocal session to check each item of its "Notes for geocal" table against geocal's code
and record where it goes. The second and third fix plans have their own short lists, included here. "Step 4a" and
"step 9" are Phase 0 steps.

| Skeleton item | geocal today | Where in this plan |
|---|---|---|
| `python-install`: uninstall from the prefix first, then `--ignore-installed` (ST.7, decision 17) | Still `--force-reinstall` (`python/python.am`) | Phase 0 step 9 |
| `"blah"` seed in `print_to_string`/`Exception`; the `Exception` copy loses its position (ST.7) | Still in `printable.h`, `geocal_exception.h`, `lib/Msp/msp.h`, `lib/swig_quaternion.i`; the copy uses `s_.str(cp)` | Phase 0 step 9 |
| jsonpickle `DeprecationWarning`: pass `keys=True` | Not passed (`sqlite_shelf.py`) | Phase 0 step 9 |
| `swig_array.i` typemaps crash on bad input (ST.3) | Unfixed copy | Step 4a (regenerate the wrappers) |
| `skip_weak_ptr_handling(true)` used stale marks (ST.5) | Nobody calls it | Step 4a |
| doxygen `DOT_MULTI_TARGETS` obsolete in doxygen 1.18 | Still set (doxygen 1.13), in `swig_rules/doxygen_single.cfg` and `config/doxygen.cfg` | The step 4a copy drops it from `doxygen_single.cfg`; remove it from `config/doxygen.cfg` (which stays for the C++ API docs) when doxygen is updated |
| Measure test coverage (gcovr) | Not measured | Phase 0 step 4 |
| Diff `swig_rules/` and `config/m4/` file by file before copying (decision 15) | geocal-only settings exist (`swig_rules/__init__.py`) | Step 4a |
| `make check` tests the installed library: per-target rpath (ST.11, decision 18) | `geocal_test_all_LDFLAGS = -no-install $(OPENMP_CFLAGS)` (`geocal.am`); `LD_PRELOAD` workaround | Phase 0 step 9; closes §6.1 |
| `--enable-debug` with no flags; Blitz's `.pc` flags leak `-DNDEBUG -O2` into every build (`ac_blitz.m4`, `ac_enable_debug.m4`) | geocal's copies are the unfixed ones. Debug builds in activated envs also keep `-DNDEBUG` from conda's `CPPFLAGS` and get no `-DBZ_DEBUG`, in geocal and in the skeleton (plan review; skeleton third fix plan T6.11) | Step 4a, after the skeleton's T6.11 |
| `Exception` (S1.5); its stack trace resolved with `dladdr` and one `addr2line` per module (S6) | Vendored `backtrace.hpp` base | Phase 1 item 6 |
| clang-format adopted | Done | — |
| Boost → std inside `swig_rules` (S2) | — | Step 4a |
| `std::optional`/`std::array` typemaps (S3); the Boost #319 workaround in `serialize_shared_ptr.h`; `geocal_serialize_support.h` includes `boost/serialization/array.hpp`; a returned `std::array` is a numpy array | — | Typemaps in step 4a; the rest in Phase 4 |
| An observer registered in `load()` is added twice (S5) | geocal's `Observable` doesn't serialize its list | Nothing now; applies if geocal ever serializes observers |
| SWIG pinned to the committed wrappers' version | `4.4.*` | Until Phase 6 |
| `script/boost_to_std_ptr.py`, `serialize_shared_ptr.h`, aliasing `to_ptr` (S4) | — | Phase 5 |
| nanobind bindings and the eight things found in S6 | — | Phase 6 |
| Golden shelves with python subclass objects, written by the SWIG build | None | Phase 0 step 5 |
| C++23; `mdspan` for `read_ptr` (S7); the `AC_PROG_AWK` bootstrap warning; don't name a member `range_check` | C++17 | Phase 0 step 3a (C++23 and the bootstrap warning); `read_ptr` in Phase 7 |
| Rename `swig_rules` → `cxx_python_rules`, `geocal_swig` → `geocal_cxx` | — | Phase 6 |
| `write_shelve` with a `pathlib.Path` | Already converts (`str(Path(fn))`) | Nothing to do |
| Blitz serialization: column-major, reversed and strided arrays, the load side; throw on a nonzero base (R1.1, D1) | Same code (`isStorageContiguous()` and `resize`), latent in geocal; geocal also serializes `Array<Time,1>` and lacks 4 element types | Phase 0 step 9 |
| Copying an `Observer`/`Observable` (R1.2, D2: delete copying) | Same code. The skeleton's trial build found one failure in geocal: `%copyctor QuaternionCamera` | Phase 0 step 9 |
| `blitz_array.h` hard-coded the skeleton's namespace (R1.3) | — | Phase 6 (take the current files) |
| Archive loading `chdir`s the process → per-thread archive base directory (R4.1); `read_shelve` still changes directory (decision "option 2") | `serialize_read_generic` uses `DirChange` | Phase 6, class by class |
| nanobind: `None` crashes; writes into read-only arrays | `None` crashes the SWIG bindings too (`ScaleImage(None, 2.0)`) | Phase 6: the `None` audit (the converter adds no `.none()` by default) |
| Smaller rules-library fixes: python 3.12 error API, rank-generic Blitz serialization, deduplicated `serialize_function.cc` | — | The Blitz part in Phase 0 step 9; the rest with the Phase 6 copy |
| Configuration header with only project macros, carrying the namespace and serialization macros (R1.3, R2.11) | `geocal_config.h` comes from autoheader; `geocal_gdal.h` `#undef`s `PACKAGE_*`; `SWIG_MAPPER_NAMESPACE` is a `-D` flag in `bindings/python/python.am` and `#define`d by hand in 7 files | Phase 1 item 8 |
| `range_check` as functions with `source_location`; `explicit`, default member initializers, rule of zero, `mutable`, `#pragma once` (R1.8, R2) | Macros (`geocal_exception.h`) | Phase 1 items 6 and 9 |
| Hand-written binding style; updated converter (R6) | — | Phase 6 |
| Strict warnings and a wider `.clang-tidy` (R7, F6.8, F6.9) | geocal's `.clang-tidy` still has the 7 S1.3 checks; the skeleton's has 25 entries, including the groups `bugprone-*` and `performance-*` | Phase 1 items 3 and 5 |
| Second review, serialization (F1, F4): per-write `weak_ptr` marks, include guard, registry key per project; in-memory functions renamed | Unfixed. Callers of the old names: `sqlite_shelf.py`, `mars_igc_test.py`, `rsm_grid_test.cc`, `boost_pcall.h` | Marks and guard in Phase 0 step 9; renames and registry in Phase 6 |
| Second review, library and build (F2, F3, F5–F7): sanitizer option, `_GLIBCXX_ASSERTIONS`, `make golden-check`, `check_all.sh`, pytest temp directory, nothrow `Exception` copies, protected `GenericObject` copy (geocal trial: nothing slices), observer lists as `std::vector`, "Thread safety:" lines | — | Tooling in Phase 0 step 4; classes in Phase 1 items 6, 9, 10; observer lists in Phase 5 |
| Docstring table compile time (`doxy2nanobind.py`: 3 min 24 s → 0.6 s on geocal's headers) | — | Phase 6 |
| Third fix plan T1: python stream errors | — | Phase 6 (with `iostream.h` and the converter's flush) |
| T2: archive write errors and the gzip hang | **Confirmed in geocal** (§0.2) | Phase 0 step 9, after the skeleton does T2 |
| T3: python 3.13 minimum (step 0: check geocal's and framework's envs) | geocal: 3.13.7 (linux and osx locks). framework: `python<3.9`, which it must leave anyway | Phase 6 |
| T4: samples generated from the live files | — | Phase 6 (`277b657` predates T4, so step 4a diffs its samples by hand) |
| T6.6: qualify `polymorphic_[io]archive` | 5 files use the short names | Optional, Phase 1 |
| T6.9: `CacheInvalidatedObserver::this_obj()` returns `const&` | geocal has no `CacheInvalidatedObserver` | Nothing to do |
| T7.1: concepts on `serialize_read<T>`, `get<T>` | — | Phase 6 (run the grep the note asks for first) |
| T7.3: module statics set at import | — | Phase 6 |
| T7.4: configure check that Blitz is thread safe | frenchpeas's build env (conda-forge `libblitz`) defines `BZ_THREADSAFE`; the afids build the env files now name (`*_1000`) is unchecked | Phase 0 step 9 |

### 0.5 Independent review of this plan (2026-11-07)

A fresh reviewer (a subagent with no part in writing it) checked this plan
against geocal's code and build, and against the skeleton's notes, code and
git history. All findings were applied; the main ones were spot-checked again
before applying. What it changed:
* **Ordering.** The step 4a copy from `277b657` needs C++20, so the C++23
  switch moved from Phase 1 to Phase 0 step 3a. The step 9 ports are written
  in C++17 (they are backported, and the format-string `Exception` comes in
  Phase 1). `make golden-check` moved to step 5, which creates the corpus.
  `check_all.sh` starts from a warning and lint baseline, because geocal can't
  be warning- or lint-clean before Phase 1.
* **Bugs in geocal it found or confirmed:** archive writes to a directory that
  doesn't exist "succeed" (added to the T2 row); `None` crashes the SWIG
  bindings today (`ScaleImage(None, 2.0)`); a CRTP typo in `RayIntersect`;
  pytest's temporary directory is in the source tree (this plan had said it
  wasn't); debug builds still get `-DNDEBUG` from conda's `CPPFLAGS`, and no
  Blitz bounds checking (also in the skeleton, now its T6.11).
* **Wrong claims corrected:** the nanobind converter adds no `.none()` by
  default (Phase 6 now has a `None` audit); `.db`/`.json` shelves of C++
  objects break with the module rename (§10 open item 9);
  `--without-swig` still compiles the committed wrappers (Phases 5–6 test with
  `make geocal-check`); the rpath fix doesn't cover the build-tree python tests;
  the class-by-class move off `chdir` needs a temporary `DirChange`.
* **Scope:** the `weak_ptr` and Blitz serialization bugs are latent in geocal's
  own classes; the `maintain_1.28` backports are now specified per row; Phase 1
  item 8 also covers the namespace macros (R1.3); stale C++20, SWIG-pin and
  nanobind-"considered" text fixed.

## Branch strategy and constraints from the maintainer

**Branches.**
* **`develop_2_0`** is where the modernization work happens (the current
  branch, 59 commits ahead of `master` on 2026-11-07). GeoCal 2.0 is
  allowed to **break the API**: `std::shared_ptr`, nanobind, C++23.
* **`master` → `maintain_1.28`** (name confirmed; `configure.ac` on
  `master` is version 1.28). Branch it off before 2.0 lands. It serves the
  **closed gcc 11.2 system** and any other user who needs the 1.x API:
  **bug fixes only**, C++17, current dependencies. If a new feature is ever
  needed on 1.x, it is treated as a **backport**: implement it first on
  `develop_2_0`, then port it back to `maintain_1.28`.
* The gcc 11.2 system is a closed, tightly controlled system. It uses
  conda/pixi, but updating packages there takes a very long time. That makes
  a frozen maintenance branch a better fit than holding the whole project at
  gcc 11.2's feature level.
* Since `maintain_1.28` covers gcc 11.2, **2.0 no longer has the gcc 11.2
  constraint.** The rule for 2.0 is: **stable, current conda-forge
  compilers, nothing cutting-edge** (§7).

**Other constraints.**
* **macOS is best-effort.** No sponsor needs it, and it has broken from time
  to time over the years. Keep it working when that is cheap, but it must not
  drive decisions. In particular, libc++ gaps don't block using libstdc++
  features; where two options cost the same, pick the portable one.
* **Boost:** 2.0 can require a current Boost (≥ 1.84, the current
  conda-forge version).
* Replacing GSL is **desirable** if it isn't too expensive.
* Blitz++: we currently use the **conda-forge `libblitz`** package, which is
  sufficient. We maintain our own conda packages in `../afids-conda-package`,
  so we can build our own Blitz there if it is ever needed. (*2026-11-07:* the
  env files now ask for the afids build, `libblitz` `1.0.2.*` build `*_1000`
  from the afids channel, which patches the headers for gcc warnings.
  frenchpeas's current build env still has conda-forge's
  `libblitz-1.0.2-h9c3ff4c_3`.) GeoCal does not
  carry a copy of Blitz.
* General code cleanup is wanted where it is **reasonably mechanical**. We
  don't want a large manual rewrite.

---

## 1. Corrections to the earlier analysis

| Earlier claim (CODE_QUALITY_ANALYSIS.md) | Actual state |
|---|---|
| "Pre-C++11 codebase" / "Adopt C++14" | `config/m4/afids_common.m4` already has `AX_CXX_COMPILE_STDCXX([17], [ext], [mandatory])`, so the build requires **gnu++17**. The *idioms* are still C++98-era, but the language level is not the problem. |
| "No lambda functions", "no `override`" | There are a few of each (for example, `geocal_thread_pool.h` uses a lambda, and `override` appears 31 times). Neither is used consistently. |
| Replace `boost::function`, `boost::bind`, `boost::unordered_map` | **None of these appear in the code at all.** The Boost features that actually need attention are different (§3.2). |
| "Keep Boost for Blitz++ integration" | Blitz++ does not use Boost. The real tie between Boost and Blitz is the custom serialization of `blitz::Array` (`blitz_array_serialize_support.h`). |
| Boost ≥ 1.46 | `configure.ac` still says 1.46, but the code needs much newer: `boost::asio::thread_pool` needs ≥1.66, and `boost_pcall.h` has Boost.Process v1/v2 logic. The `boost_fix/1.56–1.58` workaround directories are dead code. |
| C++17 is "too new" (comment in `geocal.am`, line ~22, about `BOOST_FILESYSTEM_VERSION=3`) | Out of date. C++17 is already mandatory, so `std::filesystem` can be used. |

Recommendations from the earlier documents that **this plan does not
endorse**, or treats as separate decisions to make later: switching SWIG to
pybind11 as a near-term step, switching autotools to CMake as a prerequisite,
moving to "C++14" (we are already past that), and replacing Blitz++. The
reasons are in §3, §4, §6 and §7.

---

## 2. Current state

### 2.1 Code size (lib/, excluding generated wrappers)

* 655 `.h`/`.cc` files, about 106.5k lines; 230 `.i` SWIG files; 181 `*_test.cc` files.
* About 707 source files in `GeoCalCore` (Implementation 431, Interface 166,
  Support 110). There are smaller trees for CartLab, Mspi, Msp, and Oco3.
* Generated SWIG wrappers are **committed to git** on purpose, so that
  downstream users don't need SWIG (a deliberate trade-off; other repos made
  the opposite choice). They live in `bindings/python/swig/`,
  with 227 `*_wrap.cc` and 226 `.py` files, about 107 MB. Any mass rename
  in the C++ code (for example `boost::shared_ptr` → `std::shared_ptr`)
  regenerates every one of these files, which produces a huge diff.

### 2.2 Toolchains

**Reference development environment** (pixi env `../geocal-build/build-pixi`):

| Component | Version | Notes |
|---|---|---|
| gcc/g++ (conda-forge) | 15.1.0 | |
| Boost | 1.84.0 | conda-forge. |
| Blitz++ | 1.0.2 (conda-forge `libblitz`) | Upstream is in maintenance mode. We can build our own in `../afids-conda-package` if needed. *2026-11-07:* the env files now use the afids build (`*_1000`, patched headers). |
| GSL | 2.8 | **GPL-3.0.** |
| GDAL | 3.11.4 | PROJ 9.6.2. |
| CSPICE | N0067 | |
| HDF5 / HDFEOS5 | 1.14.6 / 5.1.16 | |
| FFTW | 3.3.10 | **GPL-2.0+.** Optional. |
| SWIG | 4.3.1 (4.4.0 in the env since) | `env/pixi.toml` still pins `<4.4.0`, but the 4.4 problem has been fixed and **4.4 can be used now**, so the pin can be dropped. *Update 2026-11-07:* the env files now pin `swig = "4.4.*"`, matching the committed wrappers. |
| Doxygen | 1.13.2 | |
| Python / numpy | 3.13.7 / 2.3.3 | |
| autoconf / automake / libtool | 2.72 / 1.17 / 2.5.4 | |
| fmt, spdlog | 11.2 / 1.15 | Already in the env; GeoCal does not use them. |
| Eigen, nanobind, `mdspan` | **not present** | Add to the pixi env when needed. The skeleton's env has `nanobind` 3.1.0 (pinned `>=3.1,<4`; its forked caster has a `static_assert` on the version) and the Kokkos `mdspan`. `clang-tools` 22 and `clang-22` were added to geocal's env on 2026-11-05. |
| bear | **don't use** | `bear` (and `intercept-build`) don't run on this host (glibc 2.28 is too old for the conda build). Use the skeleton's `script/make_compile_commands.py` and `script/run_clang_tidy.sh` instead (skeleton S0.4). |

**Other environments:**
* The `env/linux` lock file is on gcc/libstdc++ **15.2**, and `env/osx` is on
  **clang 21 / libc++ 21** (osx-arm64). libc++ is slower than libstdc++ to
  implement some library features, but because macOS is best-effort this
  doesn't constrain 2.0.
* **gcc 11.2 (closed system)** is supported by the `maintain_1.28` branch
  only, not by `develop_2_0`.

**Proposed minimum compiler for 2.0:** **gcc ≥ 14**, in **C++23** mode
(decided 2026-11-07; §7.4, §7.5). clang/libc++ is best-effort.

gcc 14 has been available in conda-forge for a long time. Development uses
whatever the pixi locks give (currently gcc 15 / clang 21).

**Newer compilers are an option.** conda-forge has **gcc 16.2.0** (built
2026-09-16), and the maintainer can move the pixi environment to a newer gcc
if it matters. I tested gcc 16.2 in a throwaway env (§7.3, §7.5).

### 2.3 Third-party usage (measured)

Count = occurrences / number of files.

**Boost**

| Feature | Uses / files | Where it appears in the API |
|---|---|---|
| `boost::shared_ptr` | 2917 / 427 | **Everywhere:** the public API, SWIG typemaps (`swig_rules/include/shared_ptr_type_mapper.i`, `my_shared_ptr.i`), and a custom serializer (`swig_rules/lib/serialize_shared_ptr.h`, which handles SWIG directors). Downstream repos use it too. |
| `boost::make_shared` / `dynamic_pointer_cast` / `weak_ptr` | 283 / 64 / 6 | Goes with `shared_ptr`. |
| `boost::serialization` | 428 / 216 | Core feature. Python pickling (`%pickle_serialization`, 245 classes) goes through it. Persisted archives are used in production and in `unit_test_data`. |
| `boost::array` | 701 / 105 | Mostly `array<double,3>` and `array<AutoDerivative<double>,3>`. SWIG typemaps in `swig_boost_array.i`. Serialized. |
| `boost::math::quaternion` | 458 / 70 | Core math type, also used with `AutoDerivative`. |
| `BOOST_FOREACH` | 106 / 39 | |
| `boost::filesystem` | 70 / 23 | Pinned to `BOOST_FILESYSTEM_VERSION=3`. |
| `boost::format` / `lexical_cast` | 62 / 16, 55 / 20 | |
| `boost::multi_array` | 51 / 21 | Mostly tests. |
| `boost::regex` | 37 / 12 | Parsing (time strings, CartLab orbit files, SPICE helper, Mspi config). |
| `boost::optional` | 33 / 7 | RSM classes, `tre_support.h`. SWIG typemap `swig_boost_optional.i`. Serialized. |
| `boost::noncopyable`, `scoped_ptr` | 19 / 16, 7 / 3 | |
| `boost::date_time` | 14 / 4 | `geocal_time.cc` (string parsing) and Msp. |
| `boost::iostreams` | 8 / 3 | `ostream_pad.h` and Mspi (gzip). |
| `boost::asio`, `boost::process` | 1–2 files | `geocal_thread_pool.h`, `boost_pcall.h`. |
| `boost::operators` | 4 headers | Header-only and harmless. |
| `boost::math::tools::toms748_solve` | 1 | `geocal_gsl_root.cc`. |
| Boost.Test | all 181 tests | |
| Vendored "boost::backtrace" | `lib/GeoCalCore/Support/boost/backtrace.{hpp,cpp}` | An old, unofficial library that was never accepted into Boost. `geocal_exception.h` inherits from it. |

**Blitz++**: 1843 `blitz::` occurrences in 232 files, and 2045 `Array<` in
267 files. The most common types are `Array<double,2>` (830) and
`Array<double,1>` (612), but ranks up to 7 are used. Blitz is also part of
the zero-copy numpy typemaps (`swig_rules/include/swig_array_inc.i`),
`AutoDerivative<T>` (1126 uses), and serialization.

**GSL**: 15 files outside the `geocal_gsl_*` wrappers. Details are in §3.4.

**FFTW** (3 files): `geocal_fftw.{h,cc}` and `phase_correlation_matcher.cc`. Optional.

**libsdbx** (`thirdparty/libsdbx`, 467 files / about 68k lines, vendored, 2008):
used **only** by `lib/GeoCalCore/Implementation/location_to_file.h` for an
R*-tree. That class is used by `GdalMultiFile` and `CartLabMultifile`. See §3.6.

**HDF5 C++ API** (`H5::`, 2 files), **GDAL** (528 / 60, already updated for
GDAL 3 axis order), **CSPICE** (31 files, kept behind the
`toolkit_*_interface` classes, not thread-safe), VICAR RTL (optional),
GeoTIFF, MSP (optional), MSPI-shared/HDFEOS5 (optional),
`thirdparty/libraw` (disabled).

---

## 3. What should change: library-by-library trade-offs

### 3.1 Recommendation summary

| Library | Recommendation | Risk | Effort |
|---|---|---|---|
| Boost features that have direct `std` equivalents (filesystem, regex, optional, format, lexical_cast, foreach, noncopyable, scoped_ptr, multi_array) | **Replace** | Low | S–M |
| Vendored `boost::backtrace` | **Replace** with Boost.Stacktrace | Low | S |
| `boost::array` → `std::array` | **Replace** (separate step, because it touches SWIG and serialization) | Medium | M |
| `boost::shared_ptr` → `std::shared_ptr` | **Replace**, as a coordinated, versioned break (GeoCal 2.0) | High (downstream impact) | L |
| `boost::date_time` → `std::chrono` | Replace in `geocal_time`; leave the Msp code alone | Low | S |
| Boost.Asio thread pool / Boost.Process / Boost.Iostreams | Keep | — | — |
| `boost::math::quaternion`, Boost.Serialization, Boost.Test | **Keep** | — | — |
| **libsdbx** | **Replace** with `boost::geometry::index::rtree` (header-only, already part of Boost) | Low | S |
| **GSL** | **Replace** with Eigen + Boost.Math | Low–Medium | M (small to medium) |
| FFTW | Replace with pocketfft (§3.5), mainly so `PhaseCorrelationMatcher` can run in threads | Low | S |
| Blitz++ | **Keep** (conda-forge `libblitz`; our own build in `afids-conda-package` as a fallback) | — | — |
| HDF5 C++ API | Optional / low priority: HighFive | Low | S |
| GDAL, PROJ, GeoTIFF, CSPICE, VICAR RTL | Keep; modernize call sites only | — | S |
| SWIG | Keep until Phase 6, then **replace with nanobind** (the skeleton's pilot worked and SWIG is gone there, §4) | High (whole python layer) | L |
| Autotools | Keep for now (§6) | — | — |

### 3.2 Boost: details

These changes are made on `develop_2_0`, with the 2.0 compiler minimums (§7.4).

* `boost::filesystem` → `std::filesystem` (gcc ≥9 needs no `-lstdc++fs`).
  Mechanical. Drop `-DBOOST_FILESYSTEM_VERSION=3`.
* `boost::regex` → `std::regex`. Slower, but every use is parsing small
  headers or config files, so speed doesn't matter. Each regex needs to be
  checked (`std::regex` has no lookbehind, for example).
* `BOOST_FOREACH` → range-for. `noncopyable` → `= delete`. `scoped_ptr` →
  `std::unique_ptr` (§8).
* `boost::format` → **`std::format`** (C++20; gcc ≥13, libc++ ≥17).
  `lexical_cast` → `std::to_string` / `std::from_chars` (or `std::stod`
  where speed doesn't matter, which is also the portable choice for macOS).
* `boost::multi_array` → `blitz::Array` / `std::vector` (mostly in tests).
* `boost::optional` → `std::optional`. Needs a new SWIG typemap, and a
  serializer that writes the same archive layout as `boost::optional`.
* `boost::date_time` → C++20 `std::chrono` calendar types
  (`year_month_day`, `hh_mm_ss`, `sys_time`) for formatting and arithmetic,
  and `std::chrono::parse` (libstdc++ ≥14, which is why the minimum is gcc 14,
  §7.4) for parsing. libc++ has no `parse` yet; because macOS is
  best-effort that is acceptable, but keep the parsing in one function in
  `geocal_time.cc`, so a small fallback parser can be added if macOS ever
  matters.
* **Backtrace**: use Boost.Stacktrace, stored as a member of `Exception`
  rather than as a base class, and add `std::source_location` (C++20) for the
  throw site (§7.1). `std::stacktrace` (C++23) works in gcc 15 but needs the
  separate static `libstdc++exp` library, whose ABI gcc still calls
  experimental. Not something to link into a shared library yet (§7.5).
* **`boost::array` → `std::array`**: the C++ change is mechanical. It needs new
  `swig_rules` typemaps, and the archive layout must be checked with the
  golden archive tests (Phase 0).
* **`boost::shared_ptr` → `std::shared_ptr`**: the largest change. It affects
  the public API, `swig_rules` (shared subtree), the custom
  `serialize_shared_ptr.h` director handling, and `weak_ptr_serialize_support`.
  Do it as GeoCal 2.0, with a migration script for downstream repos and
  wrappers regenerated in a separate commit. Archive compatibility is likely
  (both formats write a `px` NVP) but must be verified.
* **Keep**: Boost.Serialization (no replacement can read existing archives),
  `boost::math::quaternion` (no `std` type; works with `AutoDerivative`),
  Boost.Test, Asio thread pool, Process, Iostreams.
* **`--without-boost-serialization`**: nobody uses it. It is future-proofing,
  so that if GeoCal ever moves to another serialization library, it is clear
  which code to remove. The pattern is cheap: one
  `#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION` per `.cc` file (218 guards in 199
  files, only 5 headers), with `serialize()` only *declared* in headers.
  **But it is currently broken.** I syntax-checked the tree with the macro
  undefined, and **12 files fail**: `argus_orbit.cc`, `vicar_raster_image.cc`,
  `combine_orbit.cc` and its test, `eci_tod_burl.cc`,
  `iterative_morphological_dilation.cc`, `phase_correlation_matcher.cc`,
  `spice_kepler_orbit.cc`, `spice_orbit.cc`, `tle_orbit.cc`, `map_info.cc`,
  `generic_object.cc`. Errors are serialization code outside the guard
  (`GEOCAL_NVP` etc.), plus `geocal_serialize_support.h` and
  `swig_rules/lib/serialize_shared_ptr.h` assuming Boost serialization.
  **Recommendation:** keep the option, because the cost is small. Fix the
  12 files once, and add a **`--without-boost-serialization` build to the
  regular builds** (just compiling the C++ tests is enough). That cheap
  check is what stops it from rotting again. If it keeps breaking in ways
  that are a pain to fix, that is the signal to drop it.

After this, the compiled Boost libraries GeoCal needs drop to
**serialization, unit_test_framework, iostreams (Mspi only)**.

### 3.3 Blitz++

**Current situation:** GeoCal uses the conda-forge `libblitz` 1.0.2 package,
which is sufficient; if a patch is ever needed, we can build our own in
`../afids-conda-package`. Blitz is used everywhere (public API,
`AutoDerivative`, zero-copy numpy typemaps, serialization).

**C++20 compatibility check (done, §7.3):** Blitz headers compile cleanly
in gnu++20 with gcc 15. They produce many *deprecation warnings*
(`-Wdeprecated-copy`, and `-Wvolatile` in `memblock.h`) but **no errors**.

| Option | Pros | Cons |
|---|---|---|
| **Keep Blitz** | No work; behaviour already validated; zero-copy numpy typemaps already work. | Small, ageing upstream. |
| Eigen | Large, active upstream; header-only. | `Matrix` is 2-D only (GeoCal uses ranks 3–7); different slicing; column-major by default; would touch about 270 files plus the typemaps, `AutoDerivative`, and serialization. |
| xtensor | numpy-like API. | Uncertain upstream; heavy compile times; the same amount of rewriting. |
| `mdspan` (via the Kokkos reference implementation, §3.3.1) | Standard design (C++23 `std::mdspan`); available **now** for C++17/20 through the conda-forge `mdspan` package; also provides `submdspan` slicing and an owning `mdarray`. | No expression templates or arithmetic, no `AutoDerivative` integration, so it complements Blitz rather than replacing it. |

**Recommendation:** keep Blitz. Do not add new ways for it to leak into the
API. Where new linear-algebra code is needed (including the GSL
replacement), use Eigen locally through `Eigen::Map` over the Blitz data
(no copy), the same way `GslMatrix` wraps Blitz data today. Treat Blitz
headers as system headers (`-isystem`) so their warnings don't hide GeoCal's
own (§8).

#### 3.3.1 mdspan as an interop layer (reference implementation)

conda-forge's **`mdspan`** package is the Kokkos team's reference
implementation of `std::mdspan` (Apache-2.0 with LLVM exception,
header-only, needs C++14 or later). There are two versions:

| Version | Header / namespace | Contents |
|---|---|---|
| `0.6.0` (the last tagged release, older) | `<experimental/mdspan>`, hard-coded `std::experimental` | mdspan, layouts, submdspan, mdarray |
| `0.6.1.dev20260223` (snapshot, 2026) | `<mdspan/mdspan.hpp>`, configurable `MDSPAN_IMPL_STANDARD_NAMESPACE` (default `Kokkos`) and `MDSPAN_IMPL_PROPOSED_NAMESPACE` (`Experimental`) | mdspan, layouts, `submdspan` (P2630), `mdarray` (P1684, owning), `layout_padded` (P2642), `dims` (P2389) |

**Tested** (in a throwaway env, using the 0.6.1 snapshot together with Blitz 1.0.2):
contiguous row-major views of `blitz::Array<double,3>` storage (writes visible
through Blitz), **`layout_stride` views of non-contiguous Blitz slices built
from `stride()`**, `submdspan`, and an owning `mdarray`. All worked with gcc
15.1 in **C++17** and **C++20** and gcc 16.2 in C++23. In C++23 mode it uses
`m[i, j]`; before that it uses `m(i, j)` (macro `MDSPAN_USE_BRACKET_OPERATOR`).

It doesn't forward to the compiler's `std::mdspan` when that exists; it
always defines its own types. So, if we adopt it:
* Add a tiny GeoCal header (for example `geocal_mdspan.h`) with a namespace
  alias, e.g. `namespace gc_md = Kokkos;`. Later it can point at `std` once
  every supported compiler has `std::mdspan` *and* we don't need the extras
  (`submdspan` is C++26, and `mdarray` isn't standardized).
* Pin the snapshot version in `env/pixi.toml`. Upstream hasn't tagged a release
  since 0.6.0, so the dev snapshot is the version that is actually
  maintained (Kokkos itself is built on it). Either pin the snapshot or vendor
  the headers.

**Where it helps GeoCal:**
* **Binding layer:** nanobind's `nb::ndarray` gives pointer, shape and
  strides, which map directly to `layout_stride` mdspan views, with no copy and
  no Blitz `MemoryBlock` tricks. It is a neutral array type between Python and
  C++, independent of Blitz.
* **New interfaces** (raster tiles, GDAL/VICAR buffers, FFT input/output,
  anything currently passing `T*` plus sizes) can take `mdspan` views instead
  of raw pointers or Blitz-specific types. That covers multi-dimensional
  `std::span` cases (§7.1 item 4).
* **Gradual decoupling from Blitz** without a rewrite: Blitz stays for
  arithmetic and `AutoDerivative` work, and mdspan becomes the type used at
  boundaries. Converting either way is zero-copy (pointer, extents, strides).

**Recommendation:** add `mdspan` (pinned snapshot) together with the nanobind
pilot, and use it first at the binding and IO boundaries. It is not needed
for anything before that.

### 3.4 GSL → Eigen + Boost.Math

GSL is required and GPL-3.0, while GeoCal is BSD-3. The usage turned out to
be **small and easy to map**. There are five areas, about 15 call sites
outside the `geocal_gsl_*` wrapper files (about 600 lines including tests):

| GSL use | Call sites | Replacement | Risk |
|---|---|---|---|
| 10-point Gauss–Legendre (`gsl_integration_glfixed`) | `refraction_msp.{h,cc}` | `boost::math::quadrature::gauss<double,10>` (header-only, Boost ≥1.66). Same nodes and weights, so results match to rounding. | Very low |
| LU solve / inverse (`gsl_linalg_LU_*`, `gsl_invert`) | `lsm_matcher.cc`, `rsm.cc` | Eigen `PartialPivLU` over `Eigen::Map`. Same algorithm. | Very low |
| Linear least squares (`gsl_fit`, built on `gsl_multifit_linear`, which uses SVD) | `quadratic_geometric_model.cc`, `rsm_low_order_polynomial.cc`, `rsm_rational_polynomial.cc`, `geocal_rpc.cc` (9 uses) | Eigen `BDCSVD`/`JacobiSVD` (or column-pivoted QR). Also return χ² and covariance if callers use them. | Low (only differs for nearly singular systems) |
| 1-D root, Brent (`gsl_root_fsolver_brent`) and `root_list` | `galileo_camera.cc`, `igc_rolling_shutter.cc`, `ipi.cc`, `rsm.cc`, `geocal_rpc.cc`, `glas_gfm_camera.cc` | `boost::math::tools::toms748_solve` (already used in the `AutoDerivative` overload in `geocal_gsl_root.cc`) | Low (different iterates, same root within tolerance) |
| N-D root (`gsl_multiroot_fsolver_hybrids`, `fdfsolver_hybridsj`) | `glas_gfm_camera.cc`, `rsm.cc`, `geocal_rpc.cc` | Eigen `unsupported/NonLinearOptimization` `HybridNonLinearSolver`. This is a port of MINPACK `hybrd`/`hybrj`, the same Powell-hybrid algorithm that GSL's `hybrids`/`hybridsj` implement. Alternatively, a ~50-line damped Newton using the Jacobian from `VFunctorWithDerivative`. | Medium-low (review test tolerances) |
| `gsl_check` / `<gsl/gsl_errno.h>` | `geocal_exception.h` (public header), `lsm_matcher.cc` | Remove. | Downstream code that used it gets fixed with the 2.0 migration. |

Things to watch:
* This adds **Eigen** as a new required header-only dependency (conda-forge
  `eigen`). Because this lands on 2.0 only, the closed gcc 11.2 system
  (`maintain_1.28`) doesn't need Eigen.
* `gsl_fit`, `gsl_root` and `gsl_invert` are exposed to Python
  (`geocal_gsl_fit.i`, `geocal_gsl_root.i`). **Decision: remove them from
  the API** (C++ and Python) as part of the 2.0 break, with no compatibility
  wrappers. The new functions get library-neutral names (for example
  `linear_least_squares`, `root`, `invert`). Any downstream code that used
  the old names is fixed as part of the 2.0 downstream migration; the
  downstream projects are maintained by the same person.
* `afids-carto`/VICAR may still link GSL, so removing it from GeoCal may not
  remove it from the AFIDS environment.
* Approach: do one row of the table per PR, top to bottom. For the solver
  rows, run old and new code side by side on the unit-test cases before
  deleting the GSL path.

### 3.5 FFTW vs pocketfft

How GeoCal uses FFTW: 2-D r2c/c2r transforms in `geocal_fftw.cc` (plans made
in constructors), and a 2-D complex transform in `phase_correlation_matcher.cc`.
**A new plan is created on every `match` call** (line ~234) on small square
search windows.

What pocketfft would gain:
* **Always available.** A single vendored header (`pocketfft_hdronly.hpp`,
  BSD-3), so there's no configure probe and no `#ifdef HAVE_FFTW`.
  `PhaseCorrelationMatcher` currently throws if GeoCal was built without FFTW.
* **License:** BSD instead of GPL (FFTW is otherwise sold under a commercial
  license from MIT).
* **Thread safety:** FFTW's planner (`fftw_plan_*`) is *not* thread-safe, and
  the matcher creates a plan per call. That is a hidden problem for anything
  using `ThreadPool` or other threads. pocketfft caches plans internally and
  is safe to call from several threads.
* Proven: numpy and `scipy.fft` both use pocketfft.

What we'd lose: raw speed. FFTW with SIMD and a measured plan can be
noticeably faster on large or power-of-two transforms. For phase-correlation
windows that are tens to a few hundred pixels across, the FFT is not the
bottleneck, and planning on every call probably costs more than the
difference in execution time.

**How it is used today:** `PhaseCorrelationMatcher` is not run from threads
now, but running it in threads would be useful. It is usually not a
bottleneck, but can be in some cases, for example **tie-point collection**.
So thread safety is the deciding argument.

| Option | Thread-safe? | Effort | Notes |
|---|---|---|---|
| Keep FFTW, call `fftw_make_planner_thread_safe()` once at start-up (FFTW ≥ 3.3.5) | Yes: planning is serialized behind a lock; executing a plan was already thread-safe | Tiny | Still plans on every `match` call, so threads would contend on the lock exactly in the case we care about. Still GPL and optional. |
| Keep FFTW, create the plan **once per matcher** (or per thread) and reuse it with `fftw_execute_dft` on new arrays | Yes, with the above | Small | Removes the per-call planning cost too. Still GPL and optional. |
| **pocketfft** (BSD, header-only, vendored) | Yes (thread-safe internal plan cache) | Small (3 files) | Always available, GPL-free; somewhat slower than FFTW on large or power-of-two sizes, which doesn't matter for these window sizes. |

**Recommendation:** go to **pocketfft** when threading the matcher becomes
a goal (for example, when tie-point collection is parallelized). It gives
thread safety, availability and licensing in one small change. If that
happens before the FFT work is scheduled, the two FFTW fixes in the table
are a quick interim measure. Either way, add a unit test that runs the
matcher in parallel with `ThreadPool` and compares against serial results.

### 3.6 libsdbx → Boost.Geometry R-tree

libsdbx (2008, about 68k vendored lines, installs its headers into
`$(includedir)/sdbx`) is used only by `LocationToFile`. That class already
hides the R-tree, and its own doc comment says the code could switch to
Boost's R-tree "if libsdbx ever becomes an issue". The Boost.Geometry R-tree
(`boost/geometry/index/rtree.hpp`, Boost ≥1.54) is header-only, maintained,
and part of a dependency we already require. It is present in the env.

Reasons to do it now:
* It removes 467 vendored files, `thirdparty/libsdbx/sdbx.am`, the header
  install, and the doxygen link.
* In the C++20 check (§7.3), libsdbx was the **only** part of the GeoCal
  tree that produced C++20 deprecation warnings (34 ×
  `-Wdeprecated-enum-enum-conversion` in `TreeItem.h`, plus
  `-Wignored-qualifiers`).
* `location_to_file.h` is an installed public header, so currently
  downstream builds also need the sdbx headers.

Implementation notes. The one subtle part is the **interval semantics**:
* `LocationToFile` uses half-open ranges `[x1, x2)`, and sdbx's `overlaps()`
  uses strict `<`, so tiles that share an edge do *not* overlap.
  Boost.Geometry's `intersects` treats touching boxes as intersecting. With
  integer coordinates, store a **closed box `[x1, x2-1] × [y1, y2-1]`** and
  query `[x, x+n-1]`. That gives exactly the old behaviour.
* Use `bgi::rtree<std::pair<box, std::size_t>, bgi::rstar<16>>`. `find`
  takes the first query hit; `find_region` collects all hits.
* Optional improvement: when files overlap, sdbx returns whichever one its
  tree walk reaches first. Sorting hits by insertion index makes the result
  deterministic ("first added wins"). `GdalMultiFile::...find_region(...,1,1)`
  depends on this choice, so record it in the test.
* `location_to_file_test.cc` already exists. Add an edge-adjacency test before
  making the switch.
* `LocationToFile` is not serialized (its owners rebuild it), so archives are
  not affected.

Effort: small (one header, one test, build-file and doc cleanup).

### 3.7 GDAL, HDF5, CSPICE, other vendored code

* **GDAL 3.11**: keep. Modernize call sites: use `GDALDatasetUniquePtr` (RAII)
  instead of raw handles, and the `std::string` versions of WKT export
  instead of `CPLFree` buffers.
* **HDF5 C++**: only 2 files; HighFive is optional.
* **CSPICE**: keep. Document that it is not thread-safe, next to `ThreadPool`.
* **Remove**: `boost_fix/`, `thirdparty/libraw` (disabled), `unused_code/`,
  after confirmation.

---

## 4. Python bindings: SWIG, pybind11, or nanobind

**Update (2026-11-07): nanobind is the plan for Phase 6.** The skeleton's pilot (S6) passed the tests recorded from
its last SWIG build, plus four that SWIG had failed. SWIG was removed there (S6.6), and everything was renamed away from SWIG (skeleton decision 23). Two review
passes then reworked the bindings into ordinary hand-written nanobind code and changed the converter to write that
style. The rest of this section is the reasoning as it stood before the pilot; Phase 6 (§5) has the design that came
out of it.

**Stay on SWIG for now** (until Phase 6):
* SWIG 4.4 works now (the env files pin `4.4.*`, matching the committed
  wrappers).
* **Keep committing the generated wrappers.** This is a deliberate
  trade-off so downstream users don't need SWIG. With nanobind the question
  disappears: the bindings are ordinary C++ files compiled by the normal
  build, so there is no generator for downstream users to install and no
  107 MB of generated code in git.
* After the `std::shared_ptr` change, pilot nanobind (which supports only
  `std::shared_ptr`) or pybind11 on a small closed group of classes,
  covering pickling, directors, and numpy arrays. **Update (2026-10-23):**
  the pilot is the skeleton repository's phase S6 (nanobind), on samples that
  copy geocal's patterns (Observer, multiple and virtual inheritance,
  directors, column-major arrays). geocal Phase 6 follows its result.

Reasons not to switch now: 230 `.i` files; 961 `%python_attribute`; 245
pickled classes; 9 directors; RTTI downcasting; Doxygen docstrings; and
downstream repos `%import` GeoCal's `.i` files, so they would all break at
once.

**Update (2.0 branch):** a nanobind move is part of the 2.0 API break
(considered when this was written; decided since, see the 2026-11-07 update above). Two points connect it to the rest of this plan:
* nanobind supports only `std::shared_ptr`, so the `std::shared_ptr`
  migration must come first (§5 Phase 5 → Phase 6).
* **SWIG constrains how much C++20 can appear in headers.** I tested SWIG
  4.3.1 (the env version); repeat the test after moving to 4.4, which may
  parse more. It parses `override`, `= default`, `noexcept`,
  concept *definitions*, constrained template parameters
  (`template<std::floating_point T>`), `std::span` parameters, attributes,
  and `auto` return types. It **fails** on defaulted `operator<=>`,
  `requires` clauses, `consteval`/`constinit`, and `using enum`. While SWIG
  is in use, those must stay out of SWIG-parsed headers (or sit behind
  `#ifndef SWIG`). nanobind does not parse headers at all (bindings are
  written in C++), so after a move to nanobind the headers can use any C++20
  feature. That was another argument for nanobind.
* **C++-only users don't need Python with nanobind either.** Keep today's
  split. Today `libgeocal` has no Python dependency: it only holds abstract
  hooks (`swig_type_mapper_base.h`: `is_python_director_check`,
  `do_swig_python_director_setup`, `to_python` returning `void*`), and the
  `Python.h` code lives in the wrapper library. With nanobind:
  * Bindings, trampoline classes (for Python subclasses) and pickling glue
    are compiled only into the extension module.
  * `libgeocal` keeps the abstract hooks; no nanobind types in its headers
    (`mdspan` is the neutral array type at the boundary, §3.3.1). The build
    enforces this, because nanobind headers need `Python.h`.
  * Linux extension modules don't link `libpython` at all; Python's symbols
    are resolved from the interpreter at import time.
  * The costs are the build and version pinning. nanobind officially supports
    CMake / scikit-build-core; under autotools its runtime is compiled from
    its single `src/nb_combined.cpp`, which was verified to work (§4 probe
    results below). The same nanobind version must be pinned across repos so
    downstream extension modules can share GeoCal types.
  * Packaging can get cleaner: a Python-free `libgeocal` package and a
    separate Python package, possibly one stable-ABI binary for Python ≥ 3.12.
* **`GenericObject` and `Printable` with nanobind: keep both.** Neither is
  needed for its original SWIG purpose, but both still have C++ roles.
  * *Downcasting:* nanobind returns the most-derived registered type for
    polymorphic classes on its own, so the 328 `%geocal_shared_ptr`
    registrations go away. **Correction (probes, 2026-10-23):** this only
    holds when the static type is in the class's *bound* base chain and is not
    a virtual base. Returning `shared_ptr<GenericObject>` (a virtual base)
    gives the right Python type and then segfaults on use, and declaring a
    virtual base as a nanobind base also crashes. So a small type mapper is
    still needed (see the next bullet).
  * *`GenericObject`* is still the polymorphic root for Boost.Serialization:
    `serialize_read_generic` / `serialize_write*` work on
    `shared_ptr<GenericObject>`, and the Python shelve code (`read_shelve` in
    `sqlite_shelf.py`, `misc.py`) uses that to read objects of unknown type.
    It is also used by `GEOCAL_GENERIC_BASE` (186 uses), `GenericObjectMap`
    and `weak_ptr_serialize_support`. It costs essentially nothing (empty
    apart from a virtual destructor). Change its doc comment from "for SWIG"
    to "serialization / heterogeneous-container root".
  * *`Printable<T>`* mostly serves C++: 275 `print(std::ostream&)`
    implementations, `operator<<` used on GeoCal objects in about 104 C++
    places, and the `desc()` hook that lets Python subclasses override
    printing (used in `igc_msp.py`). It is also how most classes get
    `GenericObject` (virtual inheritance), so removing it would mean adding
    `GenericObject` back wherever `GEOCAL_GENERIC_BASE` is used: churn with
    no gain. For 2.0, add a generic `std::formatter` (§7.1 item 1) and a
    nanobind helper that defines `__str__` from it; keep `desc()` or move that
    override into the trampoline classes. Replacing the CRTP template with a
    C++20 concept or deducing `this` (C++23) is possible later but low value.
* **nanobind probe results that affect geocal (2026-10-23).** Throwaway
  modules mimicking geocal's class patterns, nanobind 3.1.0. Full results and
  the test plan are in the skeleton plan
  (`<skeleton>/claude_notes/implementation_plan.md` §2.1; the skeleton path for
  each machine is in `CLAUDE.md` "Machine-Specific Notes"), which works all of this out on the small repository first.
  * *Autotools build works:* plain `g++` with nanobind's `src/nb_combined.cpp`,
    no CMake, no `libpython` link.
  * *Virtual inheritance stays.* geocal has real diamonds
    (`IgcRollingShutter : virtual ImageGroundConnection, virtual
    WithParameterNested`, `WithParameter : virtual GenericObject`), and
    framework has 169 virtual bases, so removing it isn't practical.
  * *The fix is a forked nanobind caster plus a small type mapper.* The rules
    library ships a fork of nanobind's 138-line `stl/shared_ptr.h` with two
    changes: return the most-derived address via `dynamic_cast<void*>`, and use
    `dynamic_pointer_cast` in its `enable_shared_from_this` path. A small
    type-mapper registry, rooted at `GenericObject`, covers parameters typed as
    `GenericObject` or as a secondary base. This is the same "own fork" model
    as today's `my_shared_ptr.i`.
  * *`weak_ptr`:* without `enable_shared_from_this`, nanobind makes a new
    control block for every Python→C++ conversion, so `weak_ptr`s expire
    immediately (for every class). Plan: `GenericObject :
    std::enable_shared_from_this<GenericObject>`. That conflicts with
    `Observer::this_obj = to_ptr(*this)` in `observer.h` (a non-owning self
    pointer would become `weak_this`).
    * *Rejected:* `weak_from_this()`. Observers register mostly in
      constructors (`GlasGfmCamera`, `IpiImageGroundConnection`,
      `OrbitDataImageGroundConnection`) and in serialization `load()`
      (`AirMspiOrbit`), where no `shared_ptr` owns them yet, so it would break.
    * *Fix:* keep the ownership-agnostic `this_obj` but build it, and
      `to_ptr`/`const_pointer` in `null_deleter.h`, with the aliasing
      `shared_ptr` constructor over a `shared_ptr<void>`, which leaves
      `weak_this` alone.
    * Tested: stack, member, `shared_ptr`-owned, Python-constructed and
      Python-subclass observers all work, and `shared_from_this()` returns the
      real owner.
    * No ownership requirement on Observers or Observables. Details: skeleton
      plan §2.4.
  * *Multiple inheritance:* nanobind allows one bound base per class. The 8
    geocal SWIG classes with two bases (`Camera`, `Orbit`, `IgcArray`,
    `IgcMultiplePass`, RSM classes, `TiePointCollection`, `MspiGimbal`) bind
    their primary base and get the other base's methods bound directly.
  * *Directors, Python object identity and exception propagation* worked as
    hoped.
  * Run the binding tests under AddressSanitizer: a wrong pointer adjustment
    returned the right answer by luck in one probe.
* **framework (to follow geocal later; surveyed 2026-10-23).** None of the
  geocal decisions need to change. Points to carry along:
  * The shared rules library ships CMake and automake flavors.
  * Arrays and `mdspan` must support column-major storage (framework has 831
    `ColumnMajor` uses); `layout_left` was tested OK.
  * The GSL replacement stays geocal-only: GSL is core in framework (563
    uses, including the `gsl_multifit_nlinear` solvers).
  * framework already uses Eigen.
  * It has 52 multiple-base SWIG classes and 47 directors, and must first move
    off `python<3.9`, `swig<4` and C++14. (*2026-11-07:* to python ≥ 3.13, the
    minimum of the nanobind rules, skeleton D14.)

---

## 5. Phased plan

All phases are on **`develop_2_0`**. Each phase leaves the tree buildable
with all C++ and Python tests passing. Phases 0–4 keep the public API mostly
unchanged; the exceptions are the C++23 requirement, the step 9 bug fixes
(for example no more `QuaternionCamera(other)` copies), and Phase 3 removing
`gsl_*` from the C++ and python API. Phases 5–6 are the main API break.

**How each phase runs** (what worked in the skeleton, §0.3):
* Before starting: record a baseline (test counts, warnings, lint findings,
  `script/api_snapshot.py` output, `make golden-check`), and add any bug
  reproductions as strict `xfail` tests, so the phase shows them going from
  failing to passing.
* A list of intended behavior changes. At the end, the API snapshot and the
  golden comparison may differ only by those, and each goes into
  `RELEASE_NOTES_2.0.md`.
* At least one failure-path test for each feature the phase converts.
* At the end: `script/check_all.sh` over the gcc 15, gcc 14,
  `--without-boost-serialization`, sanitizer and clang builds; then
  `make install` and `make installcheck`. Until Phase 1 has made geocal clean,
  `check_all.sh` compares warnings and lint findings with the recorded
  baseline and fails only on new ones (Phase 0 step 4). During Phases 5–6 it
  runs C++ only (`make geocal-check`, Phase 5).
* The steps are rehearsed in the skeleton first; copies from it are diffed file
  by file (skeleton decision 15).

### Phase 0: Baseline and safety net
1. **Create the maintenance branch:** `master` → `maintain_1.28`. It stays
   C++17, keeps the current dependencies, and gets bug fixes only; any
   feature it needs is implemented on 2.0 first and backported. Document in
   the README which branch serves which systems. **The step 9 bug fixes are
   backported to it** (decided 2026-11-07, §10), so it must exist by the first
   backport at the latest. Each backport is a re-port by hand, not a
   cherry-pick: `lib/` was reformatted with clang-format only on `develop_2_0`
   (for example, `blitz_array_serialize_support.cc` differs by 515 lines), and
   `maintain_1.28` is C++17 for gcc 11.2 (no `ends_with`, no format-string
   `Exception`). The step 9 table says what 1.28 gets for each row.
2. Do a clean build and run all tests in the pixi env; record the baseline.
   **First recreate the build env from the committed lock**, so the baseline
   matches what the env files describe. On frenchpeas (2026-11-07) the build
   env is older than `env/linux/pixi.lock`: it has conda-forge's
   `libblitz-1.0.2-h9c3ff4c_3`, while the env files ask for the afids build
   (`1.0.2.*`, build `*_1000`, locked as `libblitz-1.0.2-hb0f4dca_1000`).
   * Recreate it with `make -C env recreate-env`, from a fresh shell. That
     copies `env/linux/pixi.{toml,lock}` into the build directory (filling in
     the local afids channel, `../afids-conda-package/afids-conda-channel`)
     and runs `pixi install --frozen`. A plain `pixi install --frozen` in the
     build directory would reuse the build directory's own, older copy of the
     lock. **`recreate-env` deletes all of `geocal-build/build-pixi`**, the
     configured build tree included, so then configure again
     (`../build_pixi.sh`; the arguments are also at the top of the old
     `config.log`).
   * **Blocker on frenchpeas (2026-11-07):** the local afids channel has no
     `libblitz` package, and frenchpeas's `afids-conda-package` checkout has
     no `libblitz` recipe (its local `master` has no commits, and none of the
     fetched remote branches has the recipe). Build the afids `libblitz` there
     first, or `pixi install --frozen` fails. The env file's comment points to
     `afids-conda-package/libblitz`; that recipe is presumably on rifle (not
     checked).
   * Then check that `BZ_THREADSAFE` is defined in the new env's
     `blitz/gnu/bzconfig.h` (step 9), and that the build has the patched
     headers' fewer Blitz warnings (the `check_all.sh` warning baseline of
     step 4 is recorded after this).
3. **Add a minimum-compiler build:** a pixi environment pinned to the 2.0
   minimum (`gxx_linux-64 14.*`, §7.4), run regularly so features newer than
   the minimum are caught when they are added. An occasional osx build is
   optional (macOS is best-effort). (The skeleton's gcc 14 env needed
   `python = "3.13.*"` pinned: a fresh solve gave a python configure couldn't
   link against.)
3a. **Require C++23 with gcc 14 as the minimum** (decided 2026-11-07; §7.5,
   §10; moved here from Phase 1 by the plan review). It has to come before
   step 4a: the skeleton switched to C++20 in S1.1, before `277b657`, and that
   commit's rules use C++20 in code compiled into `libgeocal`
   (`Fname.ends_with(".gz")` in `serialize_function.cc`, `operator<=>` in
   `swig_type_mapper_base.h`), while geocal is still
   `AX_CXX_COMPILE_STDCXX([17], [ext], …)` (`config/m4/afids_common.m4`). It
   costs nothing: strict `-std=c++23` syntax-checks all of geocal's compile
   database (§7.3). The skeleton did the same in its decision 22, and its
   current rules library uses C++23 library features (`<spanstream>`,
   `std::ranges::to`, `fold_left`). Only use library features gcc 14 has: no
   range formatting (gcc 15), no `std::mdspan` (gcc 16). SWIG parses header
   syntax, not the `-std` flag, so the SWIG limits on headers (§4) still
   apply until Phase 6; repeat §4's SWIG header-syntax test with SWIG 4.4.
   * `config/m4` is a subtree shared with the other AFIDS repos
     (`autoconf-repo`), so put the requirement in GeoCal's own macros (the
     skeleton put `AX_CXX_COMPILE_STDCXX([23], [noext], [mandatory])` in its
     local `skeleton_common.m4`) or coordinate a change to `afids_common.m4`.
   * Then a full build, `make check`, `make install` and `make installcheck`,
     including the SWIG wrappers compiled as C++23, and the Msp code (rifle).
   * Expect libsdbx's `-Wdeprecated-enum-enum-conversion` in about 20 library
     and test files and 9 wrappers until Phase 2 removes libsdbx.
   * Check for the bootstrap warning `AC_PROG_AWK was expanded before it was
     required`; the skeleton fixed it with `AC_REQUIRE([AC_PROG_AWK])` at the
     top of its common macro.
4. Add `eigen` to `env/pixi.toml` (not `bear`, see §2.2; use the skeleton's
   `script/make_compile_commands.py` and `script/run_clang_tidy.sh`).
   `clang-tools` was added on 2026-11-05 and SWIG is pinned to `4.4.*`.
   `nanobind` and the pinned `mdspan` snapshot are added in Phase 6. Then the
   skeleton's checking tools (its second fix plan F2, first fix plan R0),
   adapted to geocal:
   * **`--enable-sanitizers[=address,undefined]`** in geocal's local m4 (the
     skeleton's is in `config/m4_local/skeleton_common.m4`): ASan and UBSan
     flags added after `AC_ENABLE_DEBUG`, a leak suppression for the accepted
     Boost truncated-archive leak (skeleton decision 20), and the python tests
     preloading both runtimes. Check that the flags really appear on the
     compile lines: the skeleton's earlier hand-configured ASan build turned
     out not to use them. On frenchpeas, configure it in an env with conda's
     compiler activation (skeleton `CLAUDE.md`). This replaces the
     hand-written recipe of the old step 4b.
   * **`-D_GLIBCXX_ASSERTIONS`** in `--enable-debug` builds (bounds checks in
     the standard containers; no ABI change). It doesn't depend on `NDEBUG`.
   * **A clang 22 build** with conda gcc's libstdc++ (`--gcc-install-dir`,
     `--sysroot`, as `run_clang_tidy.sh` passes). In the skeleton it found
     `-Wundefined-var-template` and a macro redefinition that gcc didn't
     report.
   * **`script/check_all.sh <build directories>`** (build, warnings, `make
     check`, `make golden-check` once step 5 has it, `format-check`, `lint`,
     one summary table). **It can't be zero-tolerance in geocal yet:**
     `make lint` has 2,016 findings for the 7 existing checks (fixed in
     Phase 1 item 5); the gcc 15 build has about 40 Blitz warnings (38
     `-Warray-bounds`, 2 `-Wmaybe-uninitialized`, §8.2); libsdbx warns after
     step 3a until Phase 2; and conda's `DEBUG_CXXFLAGS` include `-Wextra`, so
     `--enable-debug` builds bring back the 1,501 `-Wunused-parameter`. So
     record a baseline of warnings and lint findings, fail only on new ones,
     and switch each to zero-tolerance as Phase 1 items 3–5 make geocal clean.
     It also needs a C++-only mode (`make geocal-check`) for the
     `--without-boost-serialization` build and Phases 5–6. Break a test on
     purpose once and see it fail.
   * **`script/api_snapshot.py`**: every python class and function with its
     signature, docstring and properties. Run before and after each phase.
   * **Coverage** (gcovr; the system `lcov` 1.14 can't read gcc 15 data;
     recipe in skeleton plan §4a), C++ and python, especially of what
     Phases 4–6 convert (`swig_rules` typemaps, serialization, directors). The
     skeleton raised its line coverage from 52% to 82% (91.7% counted with
     gcovr's `--merge-lines`) in its phase ST, before converting anything;
     geocal should know its number and add tests where a phase will change
     untested code.
   * **pytest's temporary directory is in geocal's source tree** (correction
     2026-11-07): `python/tests/pytest.ini` has `addopts =
     --basetemp=./pytest_tmp`, so `cd python; pytest` uses `python/pytest_tmp`
     and installcheck uses `build-pixi/pytest_tmp`. pytest empties basetemp
     when it starts, so two runs from the same directory break each other,
     the bug the skeleton found. Pass `--basetemp=$(abs_builddir)/pytest_tmp`
     from `python.am`, as the skeleton's `python-check-build` does.
   * CI is deferred in the skeleton too (to be set up on the JPL runner, its
     second fix plan "Deferred: CI on the JPL runner"). geocal can use the same
     workflow on the same runner once the skeleton has one.
4a. **Copy the rules and m4 files from the skeleton** (decided), after step 3a.
   **Until Phase 6, copy from the skeleton's last SWIG state, commit
   `277b657`** ("Step S5", suggested tag `swig-final`), not from its current
   tree. The current `cxx_python_rules/` has no `.i` files, needs
   `std::shared_ptr` and C++23, and uses the new names. At `277b657` the
   directory is still `swig_rules/` with `SWIG_MAPPER_NAMESPACE`, so no renames
   are needed.
   * It brings the fixes from the skeleton's ST, S1, S2 and S3: array typemaps
     that crashed on bad input, the `std::optional`/`std::array` typemaps,
     Boost → std inside the rules, stale `weak_ptr` marks, the Boost #319
     workaround. (Checked by the plan review. It does **not** have the
     include guard, F1.2, or the write-side open check, R3.3; step 9 adds
     those.)
   * From `config/m4`: `ac_blitz.m4` (Blitz's `.pc` flags no longer go into
     every build), `ac_enable_debug.m4` and `ac_boost.m4`
     (`--without-boost-serialization`).
   * **That alone doesn't fix geocal's debug builds** (found by the plan
     review; checked in the skeleton's build directories, not fixed there
     yet). In an env with conda's compiler activation, which geocal's has,
     `AC_ENABLE_DEBUG` (geocal's and `277b657`'s) replaces `CXXFLAGS`/`CFLAGS`
     with conda's `DEBUG_*` flags but leaves `CPPFLAGS`, where the activation
     put `-DNDEBUG -D_FORTIFY_SOURCE=2 -O2`. So debug builds keep `-DNDEBUG`
     (it only switches off `assert()`s in library headers such as Boost's;
     geocal calls no `assert()` itself), and, since conda's `DEBUG_CXXFLAGS`
     have no `-DBZ_DEBUG`, they get **no Blitz bounds checking**. The skeleton
     added the fix to its third fix plan as **T6.11** (use `$DEBUG_CPPFLAGS` or
     strip `-DNDEBUG`; add `-DBZ_DEBUG` in both branches). Copy
     `ac_enable_debug.m4` after the skeleton has done it. Check: a debug
     compile line has `-DBZ_DEBUG` and no `-DNDEBUG`. The first debug build
     with Blitz bounds checks may find out-of-bounds accesses in geocal; fix
     them as part of this step.
   * As of 2026-11-07, geocal's `swig_rules/` differs from `277b657` in 41
     files and `config/m4` in 3 (geocal last synced on 2026-10-23); 18 of the
     41 differ only in whitespace, because the skeleton's copy is
     clang-formatted. Format geocal's copy first (or diff with `-w`), then
     diff file by file: geocal-only settings such as `swig_rules/__init__.py`'s
     `if True:` must survive. `277b657` is older than the skeleton's generated
     samples (T4), so its `.sample` files are diffed by hand too.
   * Regenerate the wrappers in a separate commit. `277b657`'s typemaps were
     only regenerated with SWIG 4.5 in the skeleton; geocal uses 4.4, so run
     the full python suite after regenerating.
   * The copy's `doxygen_single.cfg` drops `DOT_MULTI_TARGETS`;
     `config/doxygen.cfg` also sets it, and that file stays for the C++ API
     docs.
   * The skeleton's later fixes (its review passes) went into files that
     already used `std::shared_ptr` and nanobind, so they can't be copied into
     geocal while it still uses SWIG. Step 9 ports the bug fixes among them by
     hand, and Phase 6 copies the rest.
   * Don't use `git subtree pull` from the upstream repositories (now
     `cxx-python-rules` and `autoconf-repo`); they are left alone.
5. **Golden serialization tests**: existing `unit_test_data` archives and
   pickles of the main classes. Real downstream archives come mainly from
   **ECOSTRESS, where they are embedded in HDF5 files**. Extracting those
   is deferred until the serialization-related phases (4 and 5), and must
   be done before them. geocal has no golden archive generator or corpus yet,
   so this step adds both, and **`make golden-check`** (write the golden
   archives into a temporary directory and compare byte for byte; moved here
   from step 4). Include:
   * every Blitz element type and rank geocal serializes, including
     `blitz::Array<Time,1>` (a `PiecewiseLinear` archive);
   * **shelves holding python subclass objects** (SWIG directors), written
     now with the SWIG bindings, so Phase 6 can check that nanobind reads them
     (the skeleton's `unit_test_data/golden/director_python*.xml`);
   * **`.db` and `.json` shelves holding C++ objects**, written with the SWIG
     bindings. They store python pickles that name `geocal_swig.<module>`;
     Phase 6's compatibility shim (decided 2026-11-07) has to keep them
     loading, and these shelves are its test;
   * a **byte-identical rewrite test**: load each golden archive, write it
     again, compare (`golden_archive/rewrite_identical` in the skeleton). It
     caught a planned change that would have altered archives (pruning dead
     observers, skeleton F7.2).
6. Add a **`--without-boost-serialization` build** to the regular builds,
   after fixing the 12 `.cc` files that break it today (§3.2) by guarding
   them, the skeleton's S0.6 pattern. The rest of that pattern is already
   there or comes with step 4a: `-DSWIG_HAVE_BOOST_SERIALIZATION` is already
   conditional (`bindings/python/python.am`), and the
   `serialize_shared_ptr.h` guard and the `ac_boost.m4` change come with the
   copy. It covers only the C++ library and its tests, not the python
   extension (skeleton decision 19), so it is checked with
   `make geocal-check`: plain `make` compiles the committed wrappers too.
7. Optional: automake parallel tests (`parallel_test_execution_plan.md`).
8. ~~Add `.git-blame-ignore-revs` for the mass-edit commits.~~ Not done
   (maintainer, 2026-11-05: the 2.0 branch changes a lot anyway).
9. **Fix the bugs the skeleton found that geocal has today** (§0.2, §0.4).
   One commit each, each with the skeleton's test ported. **Write the fixes in
   C++17** (`Exception e; e << …; throw e;`, no `ends_with`), even though
   `develop_2_0` is C++23 by now: they are backported to `maintain_1.28`
   (decided 2026-11-07), and the format-string `Exception` only arrives in
   Phase 1 item 6. The skeleton's versions use both, so they are re-written,
   not copied.

   | Bug | Skeleton fix and test | geocal notes | `maintain_1.28` |
   |---|---|---|---|
   | Archive write errors are lost (a small archive to a full disk "succeeds"); a gzip write error hangs at 100% CPU; **a file that can't be opened** (nonexistent directory) "succeeds" too | Third fix plan T2: a sink in the Boost.Iostreams chain that throws on a short write and records the error (Boost swallows exceptions during `flush`/`close`); a `close()` that checks the record; **no** exception mask on the archive's stream (the XML archive writes its closing tag in its destructor); `/dev/full` tests run in subprocesses with a timeout. Plus the write-side open check from R3.3/F3.9 (`Trouble opening file … for writing: <reason>`), which T2 doesn't cover | **All three confirmed in geocal** (§0.2). geocal's class is `OstreamCompress`, not the skeleton's `OutputFile` (that came in R3.3, after S4), so T2 is re-implemented on geocal's structure. Add a nonexistent-directory test next to the `/dev/full` tests. The skeleton hasn't done T2 yet; **decided (2026-11-07): do T0–T2 in the skeleton first, then port** (§10). | Yes; matters most for production. |
   | Nested serialization loses `weak_ptr`s | F1.1: a per-thread stack of mark sets (`PtrSerializeScope`); `serialize_nested_test.py` and a two-thread C++ test | Same code in `weak_ptr_serialize_support.cc` (the commented-out "outermost lock" logic, an unused `ptr_serialize_depth`). Port by hand (the skeleton's file is from after S4). **No geocal class serializes a `weak_ptr`** (checked by the plan review), so this affects downstream code that does, not geocal itself, and the test needs a test-only class. The bottom set keeps geocal's three tests that write archives directly (`image_coordinate_test.cc`, `igc_rolling_shutter_test.cc`, `blitz_array_serialize_support_test.cc`) working. | Low priority. |
   | `serialize_shared_ptr.h` has no include guard | F1.2 | `#pragma once`; move the `shared_ptr_helper.hpp` include inside the serialization `#ifdef`; drop `<iostream>`. | Yes. |
   | Blitz serialization of column-major, reversed and strided arrays; loading into a column-major target | R1.1, then R3.2: the rank-generic `blitz_array_serialize_support.{h,cc}.sample` (one template instead of 37 typedefs and macro lines; archives byte-identical); tests for each case | Same code (`isStorageContiguous()` and `resize`), but **latent in geocal** (plan review): no `ColumnMajor`/`fortranArray` in `lib/`; the only nonzero bases (`reindexSelf` in `igc_map_projected.cc`, `CalcRaster`'s swap tiles) and reversed arrays (locals in `rsm_grid.cc`) aren't serialized, so the nonzero-base throw (skeleton D1) breaks nothing. geocal's list has types the sample lacks: add `blitz::Array<Time,1>` (`PiecewiseLinear`) and the float, unsigned, short and unsigned short types that have typedefs but no `BOOST_SERIALIZATION_SPLIT_FREE` line (the skeleton's ST bug 5). The sample's nonzero-base error uses the format constructor; write it with `<<`. Golden archives must stay identical (step 5). | The column-major, reversed and load fixes only, not the nonzero-base throw (a behavior change); low priority, since latent. |
   | Copying an `Observer`/`Observable` copies the self pointer | R1.2: delete copying (`Observable() = default;` is then needed); `static_assert` tests | The skeleton's trial build found one failure in geocal: `%copyctor QuaternionCamera` (`quaternion_camera.i`). Drop it, or give `QuaternionCamera` a copy constructor that starts with no observers. The trial used frenchpeas's compile database, which has no MSP: repeat it on rifle. | No: deleting copies removes python `QuaternionCamera(other)`. If 1.28 needs a fix, copies that start unregistered (the skeleton's option A). |
   | `"blah"` seed in `print_to_string` and `Exception` (outputs shorter than 4 characters come out wrong); the `Exception` copy constructor leaves the stream at position 0 | ST.7: a plain `std::ostringstream`; `s_ << E.s_.str()`; a test with a 1-character `print` | `printable.h`, `geocal_exception.h`, `lib/Msp/msp.h`, `lib/swig_quaternion.i`. Phase 1 item 6 rewrites `Exception` anyway, but this is a bug now. | Yes. |
   | `RayIntersect : public Printable<ImageGroundConnection>` (CRTP typo; C++ `operator<<` on it is undefined behavior) | Found by the plan review, the kind of mistake Phase 1 item 6's private `Printable` constructor turns into a compile error | `ray_intersect.h` line 11 → `Printable<RayIntersect>`. Python is unaffected (its `.i` doesn't expose `print_to_string`). | Yes. |
   | `make check` tests the installed library (§6.1) | ST.11 (skeleton decision 18): `-no-install -Wl,-rpath,$(abs_top_builddir)/.libs` on never-installed programs | `geocal_test_all` (`geocal.am`), the only `check_PROGRAMS`. Then drop the `LD_PRELOAD` workaround (§6.1, `resampler_threading_plan.md`). Check with `readelf -d`. The build-tree python tests still need `LD_PRELOAD` (Phase 6 item 7). | If its build has the same problem. |
   | `python-install` with `--force-reinstall` (leaves a stale copy when switching to editable; can uninstall the env's copy) | ST.7 (skeleton decision 17): a guarded uninstall from the prefix, then `pip install --ignore-installed` | `python/python.am`. Verify both switching directions as in `python_editable_install_plan.md`, and that the env's own install survives. | If it has the same rule. |
   | jsonpickle `DeprecationWarning` | `keys=True` | `sqlite_shelf.py` (lines 102, 165); check that old `.json` shelves still load. | Yes, if old shelves still load. |
   | `configure` accepts Boost 1.46 | F3.1 | `configure.ac` line 54 → 1.84 (was Phase 1 item 2). | Only up to what 1.28's code needs (about 1.66, for `asio::thread_pool`, §1), not 1.84. |
   | Blitz thread safety only assumed | T7.4: configure warns if `BZ_THREADSAFE` isn't defined | frenchpeas's current build env (conda-forge `libblitz-1.0.2-h9c3ff4c_3`) defines it; the env files now ask for the afids build (`*_1000`), which hasn't been checked. geocal's `ThreadPool` users copy Blitz arrays on worker threads, so the check matters more here than in the skeleton. | Optional. |

   Not in step 9:
   * the archive-load `chdir` (Phase 6, unless threads start reading archives
     sooner);
   * the python minimum (3.13, needed by the nanobind code, Phase 6);
   * **`None` for a `shared_ptr` argument**, which crashes python **today**
     (`ScaleImage(None, 2.0)` segfaults, exit 139, checked 2026-11-07). The
     skeleton's fix is on the nanobind side plus null checks in C++ (R1.4), so
     it belongs to Phase 6's audit. Add C++ null checks earlier where a crash is
     reported.

### Phase 1: Cleanup and hygiene
1. ~~Require C++23.~~ Moved to Phase 0 step 3a (plan review, 2026-11-07): the
   step 4a copy from `277b657` already needs C++20.
2. Delete `boost_fix/`, the disabled libraw, and `unused_code/`. (The Boost
   minimum moved to Phase 0 step 9.)
3. Show third-party headers with `-isystem` (Boost, GDAL; Blitz is done by the
   Phase 0 `ac_blitz.m4` copy) so GeoCal's own warnings are visible (§8.1).
   Then the skeleton's warning flags, **one at a time**, each only once geocal
   is clean for it: `-Wall -Wextra -Wpedantic -Wshadow -Wold-style-cast
   -Wsuggest-override -Wnon-virtual-dtor -Wconversion -Wsign-conversion`
   (skeleton R7.1), then `-Wduplicated-cond -Wduplicated-branches
   -Wlogical-op -Wcast-qual -Wformat=2 -Wdeprecated-copy-dtor` (F6.9). Left
   out there, with reasons: `-Wnull-dereference` (false positives in
   libstdc++'s `shared_ptr` in optimized builds), `-Wextra-semi` (macro
   style). clang 22 needs `-Wno-c2y-extensions` for the Boost.Test files.
   Add `--enable-werror` for `check_all.sh` and later CI. Decide what to do
   about geocal's 1501 `-Wunused-parameter` first (§8.1); conda's
   `DEBUG_CXXFLAGS` already include `-Wextra`, so debug builds show them
   today. As each flag becomes clean, `check_all.sh` stops using the Phase 0
   warning baseline for it.
4. Fix the real issues the compile check turned up (§8.2).
5. Mechanical idiom fixes with clang-tidy (§8.3), one directory per commit,
   with wrapper regeneration in separate commits. First clean geocal for the
   7 checks already in `.clang-tidy` (2,016 findings in 273 files as of
   2026-11-05). Then take the skeleton's added checks one at a time, each only
   after geocal is clean for it (the skeleton's `.clang-tidy` says so):
   `google-explicit-constructor`, `cppcoreguidelines-pro-type-member-init`
   (in the skeleton it found 13 uninitialized members where a review had seen
   4), `cppcoreguidelines-prefer-member-initializer`, `modernize-use-using`,
   `modernize-use-default-member-init`, `readability-container-contains`,
   `modernize-use-ranges`, `bugprone-*` without
   `bugprone-easily-swappable-parameters`, `modernize-concat-nested-namespaces`,
   `modernize-avoid-c-arrays`, `modernize-use-designated-initializers`,
   `readability-redundant-typename`, `readability-redundant-inline-specifier`,
   `performance-*` with `-performance-unnecessary-value-param` (below),
   `misc-use-anonymous-namespace`,
   `cppcoreguidelines-special-member-functions` (with the skeleton's two
   options). **Wait until Phase 6** for the checks that change signatures SWIG
   sees (`modernize-pass-by-value`, `performance-unnecessary-value-param`,
   §8.4). Check every `--fix` that touches a macro: in the skeleton, merging
   nested namespaces expanded `CXX_PYTHON_RULES_NAMESPACE` to `Skeleton`.
   C++ inside `.i` files (for example `lib/swig_quaternion.i`) is only seen
   in the generated wrappers, which lint skips, so clang-tidy can't fix it:
   check it by hand (skeleton S1.3). The two repositories can share one
   `.clang-tidy` again only after Phase 6: the skeleton's file has the two
   signature-changing checks and a `HeaderFilterRegex` naming
   `cxx_python_rules/`.
6. **`Exception` and `Printable`**, as the skeleton ended up after S1.4–S1.5
   and its review fixes (`lib/skeleton_exception.{h,cc}`, `lib/printable.h`):
   * Boost.Stacktrace kept as stored addresses (the Boost code in the `.cc`,
     `-ldl`), with `std::source_location` for the throw site, replacing the
     vendored backtrace (§3.2, §7.1 item 2). Resolve frames with `dladdr` and
     one `addr2line` run per module (S6 note: Boost's addr2line backend gave
     wrong file and line numbers for shared libraries, and took 11 s per trace
     from python). `binutils` in the env and a configure check.
   * A format-string constructor (`ExceptionFormat` with a `consteval`
     `std::format_string` and a defaulted `std::source_location`), constrained
     to at least one argument so `Exception("50%")` stays literal. Keep the
     doxygen `\cond` around the template constructor, or doxy2swig breaks the
     python constructor's docstring.
   * `operator<<` keeps the stream state between calls (framework relies on
     `e << std::setprecision(8) << ...`).
   * **Copies can't throw** (F5.1): message, location and trace in a
     `std::shared_ptr<const Data>`, copied on write by `operator<<`;
     `static_assert`s on nothrow copy. An exception class derived from
     `Exception` that adds its own `std::string` member loses the guarantee;
     say so in its comment.
   * `range_check`, `range_check_inclusive`, `range_min_check`,
     `range_max_check` become function templates with a defaulted
     `std::source_location` (the caller's file and line), `T` deduced from the
     value only (`std::type_identity_t` for the bounds), message as before
     (R1.8). geocal's macros of the same names go. Don't name a member
     `range_check` while the macro exists.
   * A generic `std::formatter` for `Printable<T>` (the `PrintableObject`
     concept), `static_cast` instead of the C-style casts, `Printable<T>`'s
     constructor private with `T` a friend
     (`bugprone-crtp-constructor-accessibility`).
   * A `TypeError : Exception` class for wrong-type arguments (the skeleton
     has one). The python translation comes in Phase 6.
7. **clang-format, adopted** (decided 2026-11-05, §8.5): `.clang-format`
   (the same file as swig-rules-skeleton's), `.clang-format-ignore`,
   `make format` / `make format-check`, and `make lint` (clang-tidy with
   `.clang-tidy`). The one-time reformat of `lib/` was done 2026-11-05 and is
   committed (`f5ea8926`, `62dc9874`); no `.git-blame-ignore-revs`
   (maintainer, 2026-11-05). New and changed C++ is formatted with
   `make format`.
8. **Configuration header with only project macros** (skeleton R1.3 and
   R2.11). *The namespace and serialization macros* (R1.3):
   `SWIG_MAPPER_NAMESPACE` and `SWIG_HAVE_BOOST_SERIALIZATION` are `-D` flags
   only in `bindings/python/python.am`, and `#define SWIG_MAPPER_NAMESPACE
   GeoCal` is repeated by hand in `vicar_lite_file.cc`, `vicar_file.cc`,
   `rsm_nitf.cc`, `boost_pcall.h`, `unit_test_support.h`,
   `geocal_shared_ptr.i` and `swig_quaternion.i`. Installed headers
   (`weak_ptr_serialize_support.h`, `serialize_function.h`) depend on them,
   so downstream code has to define them too. Define both in the new
   `geocal_config.h` template and delete the scattered defines. *The
   configuration header itself* (R2.11):
   the installed `geocal_config.h` becomes a small template
   (`AC_CONFIG_FILES`) defining only `GEOCAL_*` macros, and the autoheader
   output (`PACKAGE`, `HAVE_*`, ...) becomes a private build header. Then the
   `#undef PACKAGE_*` in `geocal_gdal.h` can go. Release builds get
   `BOOST_DISABLE_ASSERTS` on the command line, defined empty (a different
   definition warned in the skeleton). geocal has a second generated header,
   `geocal_external_config.h`; decide where its macros go.
9. **Class design idioms** from the skeleton's reviews (R2, F5), mostly through
   item 5's checks: `explicit` constructors; default member initializers;
   rule of zero (no `~X() override = default;` below the first virtual
   destructor); `#pragma once` in every header (skeleton D3; also prevents
   include guard clashes between geocal and framework headers); `mutable`
   instead of `const_cast`; `GenericObject`'s copy operations `protected` and
   no move operations (F5.2: a defaulted move assignment in a virtual base
   gives `-Wvirtual-move-assign`; the skeleton's trial over all 624 geocal
   files found nothing that slices, but that compile database has no MSP, so
   repeat it on rifle); `[[nodiscard]]` only where ignoring the
   result is always a mistake (the `serialize_read*` functions, for example).
   All of these are SWIG-safe.
10. **Thread safety written down** (F7.1): a "Thread safety:" line in the
   class comment, starting with the classes `ThreadPool` users touch (the
   Resampler work) and the shared Support classes. The skeleton's rule: a
   `const` function that changes `mutable` state without a lock is not safe
   to call concurrently, and the comment says so. Free-threaded python is a
   long-term goal there (D9), not required now.

### Phase 2: Small library swaps that are not in the API
1. **libsdbx → Boost.Geometry R-tree** (§3.6).
2. `filesystem`, `regex`, `format` (→ `std::format`), `lexical_cast`,
   `multi_array`, `date_time` (in `geocal_time`, → chrono calendar types) → `std`.
   The same swaps inside the rules library came with the Phase 0 step 4a copy
   (skeleton S2). `DirChange` stays as it is: it was never Boost, and restoring
   through the saved directory handle also works if the old directory was
   renamed meanwhile.
3. Remove the corresponding compiled Boost libraries from the build.

### Phase 3: GSL removal (§3.4)
Gauss–Legendre → LU → least squares → 1-D roots → N-D roots, one PR each,
with old and new results compared side by side. Then remove GSL from
`configure.ac`, `geocal.am` and `geocal_exception.h`. Remove `gsl_check`,
`gsl_fit`, `gsl_root` and `gsl_invert` from the C++ and Python API (no
compatibility wrappers), and fix any downstream callers as part of the 2.0
migration.

### Phase 4: Types that cross into serialization and SWIG
**Before starting:** collect the ECOSTRESS archives embedded in HDF5 files
for the golden tests (Phase 0 step 5).
`boost::optional` → `std::optional`, then `boost::array` → `std::array`,
each checked against the golden archives. The typemaps came with the Phase 0
step 4a copy (skeleton S3: one optional typemap macro for both types, the
`std::array` typemaps and `%python_attribute_std_array`). From the skeleton's
notes:
* the **Boost #319 workaround** in `serialize_shared_ptr.h` is needed as soon
  as geocal serializes a `std::optional` (it is in the `277b657` copy);
* `geocal_serialize_support.h` always includes
  `boost/serialization/array.hpp` (the `std::array` serializer), as the
  skeleton's `skeleton_serialize_support.h` does;
* the skeleton wrote golden archives with the `boost::` types **before**
  converting (S3.1), and checked the `std::` build loads them and writes the
  same bytes;
* a returned `std::array` is a numpy array in python, not an
  `Array_double_3` object: record it in `RELEASE_NOTES_2.0.md`.

### Phase 5: `std::shared_ptr` (GeoCal 2.0 API break)
As described in §3.2, using the skeleton's migration script,
`script/boost_to_std_ptr.py`: run it with `--dry-run` first, then fix what it
reports (`BOOST_SP_*`, `scoped_ptr`, `using boost::...`) and the standard
includes that Boost's headers used to pull in (the skeleton found one
`<vector>`). Not needed: `--swig` for the `.i` files, since the bindings stay
broken until Phase 6. Also in this phase:
* copy the skeleton's `serialize_shared_ptr.h` (same archive format: `px`,
  version 1, `track_never`) and `null_deleter.h`'s aliasing `to_ptr` and
  `const_pointer`, and change `Observer::this_obj` to match (§4 probe results);
* require the golden archives to load and a byte-identical rewrite of each
  (Phase 0 step 5);
* optional: observer lists as `std::vector` (skeleton D13; Boost writes a
  `std::list` and a `std::vector` byte for byte the same, checked).

**Decided:** the Python bindings may be broken from Phase 5 until Phase 6
(nanobind), as in the skeleton's S4. Phase 5 converts only C++ and
serialization; the SWIG layer (`my_shared_ptr.i` etc.) is not ported to
`std::shared_ptr`. During Phases 5–6:
* build with `--without-swig` **and test with `make geocal-check`**, not
  `make` or `make check`. `--without-swig` only stops the wrappers being
  regenerated (`configure.ac` sets `SWIG=/fake_so_fail`); `bindings/python/python.am`
  still compiles all 227 committed `*_wrap.cc` files, which stop compiling once
  the C++ uses `std::shared_ptr`, and `make check` depends on `all`. The
  skeleton used its `make skeleton-check` for the same reason; geocal already
  has `make geocal-check` (`geocal.am`). `check_all.sh` runs in its C++-only
  mode;
* acceptance is the C++ tests, the `--without-boost-serialization` build and
  the golden archives;
* record the Python test results just before Phase 5, as the Phase 6 target
  (with the Phase 0 step 5 director shelves already written).

**Keep the broken window short.** In the skeleton it lasted one day. geocal's
Phase 6 is much bigger (230 `.i` files), so start Phase 6 right after Phase 5,
and don't land unrelated python work on `develop_2_0` in between. The skeleton
did its consumer-pattern samples (S5) before S4 for the same reason: geocal's
equivalent is to have tests for its directors, observers, multiple-base classes
and arrays passing under SWIG before Phase 5 (the Phase 0 coverage work).

### Phase 6: nanobind
Follows the skeleton's S6 and the two review passes that reworked it. **Copy**
the skeleton's current `cxx_python_rules/` (after its third fix plan, if that
has been done by then; otherwise note which T steps are missing),
`config/m4/ac_nanobind.m4` and the python package files, replacing
`swig_rules/`. Drop `swig_rules/include` and the SWIG helpers (list in the
skeleton's S6.6 progress row). Take samples only from generated files (T4).

**Rename** (skeleton decision 23, "Notes for geocal" rename row): in geocal's own
files `SWIG_MAPPER_NAMESPACE` → `CXX_PYTHON_RULES_NAMESPACE`,
`SWIG_HAVE_BOOST_SERIALIZATION` → `CXX_PYTHON_RULES_HAVE_BOOST_SERIALIZATION`,
`@swigrules@` → `@cxxpythonrules@`, `pkgpythonswig*` → `pkgpythoncxx*`; the
python package `geocal_swig` → `geocal_cxx`, module `geocal_cxx._bindings`
(decided 2026-11-07; `geocal/__init__.py`, the mypy overrides, imports in
scripts). The skeleton's
perl substitutions are in its rename progress row; longest names first. Remove
the old `geocal_swig/` from `site-packages` before installing (a stale
`__init__.pyi` misleads mypy).

**Pickles and shelves** (corrected by the plan review): SWIG-era pickles break,
because they name the old module (a pickled `ImageCoordinate` refers to
`geocal_swig.image_coordinate` and `_new_from_serialization`). That includes
**`.db` and `.json` shelves of C++ objects**, since `sqlite_shelf.py` stores
`pickle.dumps(value)` (and jsonpickle stores the same reduce data). Only `.xml`
and `.bin` shelves, which are Boost archives, keep loading. The payload inside
the pickle is the same Boost binary blob either way, so a small compatibility
shim can keep old shelves readable: a `find_class` override when
`sqlite_shelf.py` unpickles, or a stub `geocal_swig` package mapping
`<module>._new_from_serialization` and `_new_from_init` onto the new support
module. **Decided (2026-11-07): the shim** (§10), not a conversion script.
Rehearse it in the skeleton first. Its golden shelves are only `.xml` and
`.bin` (Boost archives), and its rename (decision 23) accepted breaking
pickles, so first write `.db`/`.json` shelves of C++ objects from a scratch
build of its last SWIG commit `277b657` (module `skeleton_swig.<module>`),
then add the shim and a test there, then here. Test it with
the Phase 0 step 5 golden `.db`/`.json` shelves. Keep it until the maintainer
decides old shelves no longer need to load. The SWIG-era names
that are data stay: `SwigDirector_` in archives and `geocal_object`.
(`IostreamSwig` and `swig_shelve` are the skeleton's names; geocal's shelve
module is `sqlite_shelf.py`, and its only `IostreamSwigTest` is in an `.i`
file no `.am` builds.)

**Generate the bindings** with the skeleton's current `i_to_nanobind.py`
(`--namespace GeoCal --header-dir lib/... --prefix geocal_nb --out-dir ...
<all .i>`). It now writes the hand-written style (member pointers,
`nb::overload_cast`, `"name"_a` with the C++ parameter names, a `docs` helper
per class, lambdas only for director dispatch and conversions). **It adds no
`.none()` at all by default** (corrected by the plan review: `.none()` comes
only with `--allow-none`, which adds it to every `shared_ptr` argument, the
old crash-prone behavior). So the bindings need a **`None` audit**: grep the
`.i` files for `shared_ptr` parameters that default to null (32 of them, for
example `Ref = boost::shared_ptr<Refraction>()` in `orbit.i` and `ipi.i`),
and geocal's python and the downstream repositories for `None` passed to
C++; add `.none()` by hand where C++ accepts null, and a null check in C++
where it doesn't. Count this in the Phase 6 effort. (Under SWIG today `None`
crashes some calls: `ScaleImage(None, 2.0)` segfaults.) After the third fix
plan's T1.4 the generator also flushes stream arguments. Keep the pristine output, then hand-fix
the `HAND CONVERT` items (`%extend`, `%pythoncode`, `%typemap`). In the
skeleton the hand changes were 92 of 2,327 lines in 7 files. Rules for the
hand-written files: `cxx_python_rules/nanobind/README.md` "Writing bindings";
for classes python can subclass, `DirectorNotes.md`.

The design carried over from the skeleton:
* the forked `shared_ptr` caster (`gc_shared_ptr.h`, with a `static_assert`
  on the nanobind version) and the `GenericObject`-rooted type registry, with
  the key per project (F1.4, so geocal and framework can be imported together);
* `GenericObject : std::enable_shared_from_this<GenericObject>`;
* primary bound base only, with secondary-base methods bound directly
  (`isinstance(obj, SecondaryBase)` is `False`);
* trampolines for the 9 directors;
* `nb::new_` + `std::make_shared` constructors for plain `GenericObject`
  classes, **not** for director classes;
* `__reduce__` with the Boost binary blob for pickling, with a
  `multiprocessing.Pool` test (geocal uses it in production);
* the pinned `mdspan` reference implementation as the array type between
  `nb::ndarray` and Blitz (§3.3.1); a non-const `blitz::Array&` parameter
  rejects read-only and converted arrays;
* the `TypeError` translation (cleanup item below);
* docstrings from doxygen through `doxy2nanobind.py`, which writes a
  constant-data table and replaces it only when its contents change (on
  geocal's headers a `std::map` initializer took 3 min 24 s to compile; the
  table takes 0.6 s);
* `.pyi` stubs;
* `test_every_class_registered` (catches a missing `register_type`; check
  that it fails with one removed).

**Things found in the skeleton's S6 that apply here** (its "Notes for geocal"
nanobind row, plus later fixes):
1. Compile the binding files **without `-fvisibility=hidden`**, or director
   serialization breaks when reading archives of the base class.
2. `.none()` only on `shared_ptr` arguments C++ accepts as null, added by hand
   (the `None` audit above); check pointers in C++. A blanket `.none()` let
   python crash the interpreter with `None`.
3. No `nb::new_` for director classes.
4. A director class's binding calls the C++ implementation non-virtually for
   python subclasses, or `super()` runs the override twice. These lambdas
   can't be replaced by a helper template (skeleton review 2, §5.5).
5. A function that runs python callbacks on C++ worker threads (geocal's
   `ThreadPool` users) needs `nb::call_guard<nb::gil_scoped_release>()`, or it
   deadlocks. Under SWIG such a callback segfaulted.
6. nanobind crashes on a null docstring.
7. The python tests against the build tree need `LD_PRELOAD` of
   `.libs/libgeocal.so`, as the skeleton's `python-check-build` does: the
   extension modules are installed programs, so the Phase 0 step 9 rpath fix
   (never-installed programs only) doesn't cover them, and their RPATH would
   find the installed `libgeocal` first.
8. Remove an old `geocal_swig/` install directory before installing.
9. Overrides that take a C++ reference (observer `notify_update`) use
   `CXX_PYTHON_RULES_NB_OVERRIDE_REF`, or the python override gets a copy.
10. **Python 3.13 is the minimum** (`Py_IsFinalizing`); add the configure check
    to geocal's local m4 (not the shared `afids_python.m4`). geocal's envs are
    on 3.13.7.

**Also from the skeleton's review passes**, coming with the copy:
* the in-memory serialization functions are renamed (F4):
  `serialize_write_binary(Obj)` → `serialize_write_binary_string`,
  `serialize_read_binary(Data[, Base_dir])` →
  `serialize_read_binary_generic_string`; the python XML string functions use
  `bytes` (F1.3). Callers to check: `sqlite_shelf.py`, `mars_igc_test.py`,
  `rsm_grid_test.cc`, `boost_pcall.h`;
* the per-thread **archive base directory** instead of `chdir` during loads
  (R4.1): move each class that opens a file named in an archive to
  `resolve_archive_path(name)`, class by class (`AirMspiIgc`, the GDAL and
  VICAR raster images, DEMs, anything reading a file in `load()` or in a
  constructor called during a load; `spice_helper.cc` uses `DirChange` too).
  `read_shelve` keeps changing to the shelve's directory until all of them do.
  **This can't be gradual by itself** (plan review): the copied
  `serialize_function.cc` has no `DirChange`, so from the copy on, a direct
  `serialize_read_generic`/`serialize_read<T>` of an archive with relative file
  names breaks for every class not yet converted. Keep a temporary `DirChange`
  in geocal's copy of the read functions until the last class uses
  `resolve_archive_path` (as the skeleton's "option 2" did for `read_shelve`),
  or convert all of them in the same step as the copy. Also list the
  process-wide python `chdir`s (`local_target.py`, `vicar_interface.py`,
  `tempdir.py`) in the thread-safety notes;
* the python stream adapters let python errors out (T1), module statics set at
  import (T7.3), concepts on `serialize_read<T>` and `GenericObjectMap::get<T>`
  (T7.1; first grep geocal for uses with a type outside the `GenericObject`
  hierarchy);
* `from geocal import *` must still give the C++ classes. geocal has no
  `__all__`, so it does today; add a test (T5 found the skeleton's broken).

**Python API changes** for `RELEASE_NOTES_2.0.md`: start from the skeleton
plan's list (§10, "Python API changes (done in S6...)"): one extension module,
no `.this`, object identity kept across bases, `isinstance` with a secondary
base `False`, a `weak_ptr` to a python-only director object expires at once,
`Array_double_N` classes gone, SWIG pickles unreadable, exceptions
(`TypeError`, `cpp_exception`, `cpp_source`), keyword names now the C++
parameter names.

With SWIG gone, the remaining SWIG limits on headers go away (§4, §7.2), and
the signature-changing clang-tidy checks deferred in Phase 1 item 5 can be
taken. CMake is not needed for geocal: the skeleton's CMake reference build
is for framework.

**Cleanup items for this phase.** These were deferred until the new
bindings exist:

* **C++ type check failures should reach Python as `TypeError`** (deferred
  2026-10-25). On `develop_2_0`, the Python code now raises `TypeError`
  instead of `RuntimeError` when an argument has the wrong type (see
  `RELEASE_NOTES_2.0.md`). The C++ code can't do the same yet, because SWIG
  (`swig_rules/include/swig_exception.i`) turns every `std::exception` into
  `RuntimeError`.
  * *Known case:* `IgcMspImp::ground_coordinate_dem`
    (`lib/Msp/igc_msp.cc`, about line 294) throws `Exception` when the `Dem`
    isn't a `SimpleDem`. The Python `IgcMsp.ground_coordinate_dem`
    (`python/geocal/igc_msp.py`) makes the same check, and now raises
    `TypeError`.
  * *Why it waits:* don't add this to the SWIG rules. The Python bindings
    may be broken from Phase 5 until Phase 6, and the SWIG rules go away with
    nanobind. Do it once the nanobind bindings can turn a C++ exception into a
    Python `TypeError`.
  * *The skeleton has done this* (S6): `Skeleton::TypeError` in
    `skeleton_exception.h`, translated to python `TypeError` by
    `register_exceptions` in `cxx_python_rules_nb.h` (a python override
    returning the wrong type gives `TypeError` too). The class itself comes in
    Phase 1 item 6.
  * *Steps:*
    1. Add a GeoCal exception type for wrong-type arguments, for example
       `TypeError : Exception` in `geocal_exception.h`. It must be a GeoCal
       type, not nanobind's `nb::type_error`, because `libgeocal` stays free
       of nanobind and Python (§4). (Phase 1 item 6.)
    2. Register an exception translator in the binding module that maps it
       to Python `TypeError` (comes with the copied `cxx_python_rules_nb.h`).
    3. Throw the new type from `IgcMspImp::ground_coordinate_dem`.
    4. Change the Python `IgcMsp` code to match. If the C++ error now reaches
       Python as `TypeError`, the Python check duplicates it and can be
       removed. Add a test (it needs an MSP build).
    5. Look for other C++ type checks (typically a failed `dynamic_cast`
       followed by `throw Exception`) that should use the new type, and add
       the change to `RELEASE_NOTES_2.0.md`.

### Phase 7: Optional / later
* FFTW → pocketfft (§3.5). Move it earlier when threading
  `PhaseCorrelationMatcher` (for example in tie-point collection) becomes a goal.
* GDAL call-site RAII; HighFive.
* `RasterImage::read_ptr` and its ~14 overrides → a `layout_stride` `mdspan`
  virtual (`read_into`), with `read_ptr`/`write_ptr` kept as non-virtual
  wrappers so callers move over gradually. The worked pattern is the
  skeleton's `lib/raster_sample.{h,cc}` (S7.2), including the contiguity check
  and temporary for C-library backends (VICAR, GDAL), `read_with_pad` through
  `submdspan`, and `CalcRaster`'s writable `self.data`. Not wrapped by SWIG,
  so it can be done any time after Phase 1; it needs the `mdspan` package.
* Deducing `this` for `Printable<T>` (C++23), **in the skeleton first**
  (skeleton plan §11); it changes every class's base.
* Hardening flags (`-D_FORTIFY_SOURCE=3`, `-fstack-protector-strong`,
  `-D_GLIBCXX_ASSERTIONS` in release builds, or gcc's `-fhardened`): set aside
  in the skeleton (D17). conda's compiler activation already adds some; measure
  release-mode `_GLIBCXX_ASSERTIONS` on geocal's hot loops first.
* CI on the JPL runner, after the skeleton's (§0.1).
* Free-threaded python: a long-term goal in the skeleton (D9); its
  `cxx_python_rules/README.md` "Threads" section lists what is left.
* Reassess `std::mdspan` (gcc 16 minimum) and `std::stacktrace` (when the
  minimum gcc no longer needs `-lstdc++exp`).

### Review passes (added 2026-11-07)
A fresh code review, by someone (or a session) that didn't write the code,
then a fix plan, before moving on. The skeleton needed three after its phases
(§0.2), and the third still found bugs that lose data. For geocal:
1. **After Phase 2** (the C++ library modernized, SWIG still in use):
   `lib/` and `swig_rules/lib/`, for bugs and old idioms, as the skeleton's
   first pass did. Before the API break, so its fixes are cheap.
2. **After Phase 6** (the bindings): the binding files, the rules library,
   the python package and the build, as the skeleton's second pass did.
3. **A last pass focused on the boundaries**: I/O errors, the environment
   (minimum versions actually built), copied files, star imports, as the
   skeleton's third pass did.

What worked in the skeleton: read the code; a wide clang-tidy run, report only
(its check lists are in the appendices of its second and third reviews);
confirm each finding with a small program before calling it a bug; then a fix
plan with "Intended behavior changes", decisions for the maintainer, a baseline
step (strict-`xfail` reproductions, API snapshot, golden comparison), and a
final table of checks showing each problem gone. Have the fix plan reviewed
independently too (§0.3 item 7). Use `grep -r` in the checks: on a directory
without `-r`, GNU grep reports an error and finds nothing, which looks like a
pass. The skeleton's reviews were run at a high effort level.

---

## 6. Build system: autotools vs CMake

Unchanged: **not a prerequisite** for anything above. clang-tidy gets its
`compile_commands.json` from the autotools build with the skeleton's
`script/make_compile_commands.py` (`bear` doesn't run on this host, §2.2).
Revisit CMake if the binding tool changes, or if the sibling AFIDS repos
migrate together.

### 6.1 `make check` runs against the installed library (open, 2026-11-03; fix known 2026-11-05)

**Solved in the skeleton** (its ST.11, decision 18), with a fifth option not
listed below: a per-target rpath on every never-installed program linked to
the library, `-no-install -Wl,-rpath,$(abs_top_builddir)/.libs`. Automake puts
per-target `LDFLAGS` before `$(LDFLAGS)`, so `.libs` comes before conda's env
rpath. Option 4 alone did not fix it there (RUNPATH keeps the same order, and
`-no-install` programs have no libtool wrapper to set `LD_LIBRARY_PATH`). Not for
installed programs (the build directory would end up in their rpath). geocal
applies it in Phase 0 step 9. The original analysis follows.

Found during the Resampler work (`resampler_threading_plan.md`). In the pixi
build, `geocal_test_all` (and the other uninstalled programs) load the
**installed** `libgeocal` from the env, not the one just built in `.libs`:

* conda's compiler activation puts
  `-Wl,-rpath,$CONDA_PREFIX/lib -Wl,--disable-new-dtags` in `LDFLAGS`, so
  the env lib directory is in the RPATH before libtool's `.libs`
  (`readelf -d geocal_test_all`: RPATH
  `.../envs/default/lib:.../build-pixi/.libs`).
* `--disable-new-dtags` gives RPATH rather than RUNPATH, so
  `LD_LIBRARY_PATH` doesn't override it either.
* The prefix is the env itself (`--prefix=$CONDA_PREFIX`), so the env lib
  directory has an older `libgeocal.so.0` after the first `make install`.

Effect: `make check` silently tests the last installed library. With a
new symbol it fails (`undefined symbol: _ZN6GeoCal10ThreadPool7in_taskE`).
With a change to existing code it passes while testing the old code,
which is worse. Before the first install it works, because then there is no
`libgeocal` in the env.

Current workaround (used for the Resampler work):

    LD_PRELOAD=$PWD/.libs/libgeocal.so.0 ./geocal_test_all ...

(run in the build directory, under `pixi run` or `pixi shell`). It prints
harmless `sh: /lib64/libtinfo.so.6: no version information available`
lines, because the preload leaks into subshells the tests start.

Options to look at, none tried yet:
1. Install to a prefix separate from the env (e.g. `$CONDA_PREFIX/../geocal`
   or a directory in the build tree). The env lib directory then never has
   a libgeocal. Simple, but changes the editable install setup and
   `PATH`/`PYTHONPATH` for using the install.
2. Have `lib/test_all.sh` (the `TESTS` driver) run the test through
   `./libtool --mode=execute`, or set `LD_PRELOAD` itself. Libtool's
   wrapper sets `LD_LIBRARY_PATH`, which doesn't beat RPATH, so this
   probably needs the `LD_PRELOAD` (or option 3) anyway. Limited to the
   C++ tests; any other uninstalled program has the same problem.
3. Strip the env rpath from `LDFLAGS` for the build (conda allows
   overriding `LDFLAGS` in the env activation, or configure could filter
   `-Wl,-rpath,$prefix/lib`), so libtool's `.libs` rpath comes first.
   Installed programs then rely on libtool's install-time relink to get the
   prefix rpath, which libtool already does.
4. Link with `-Wl,--enable-new-dtags` (RUNPATH) so `LD_LIBRARY_PATH` from
   the libtool wrapper takes precedence. Smaller change than 3, but
   overrides a conda default for every program.

This affects every project built with `--prefix=$CONDA_PREFIX` in a conda
or pixi env (the downstream ones too), so the fix probably belongs in the
shared `config/m4` / skeleton, and should be tried in the skeleton first.
(*2026-11-07:* done there; the fix is in the `.am` files, not the shared m4,
and geocal applies it in §5 Phase 0 step 9.)

---

## 7. C++20 for GeoCal 2.0

**Update (2026-11-07): decided, C++23, in Phase 0 step 3a** (§7.5, §10). What follows about C++20 still holds; C++23 adds to it.

**Decision (revised):** the gcc 11.2 system is now served by the
`maintain_1.28` branch, so **`develop_2_0` should require C++20**, with the
stable conda-forge compiler minimums in §7.4. The first version of this plan
recommended staying on C++17. That was only because of the gcc 11.2 floor,
whose libstdc++ 11 lacks the most useful C++20 library features
(`std::format`, chrono calendar/parse). Without that floor, the switch costs
almost nothing (§7.3) and the benefits below are real.

### 7.1 What C++20 gives GeoCal, most useful first

Counts are from the current tree.

1. **`std::format`** (libstdc++ 13, libc++ 17). The biggest everyday win.
   * Replaces `boost::format` (62 uses / 16 files) and most of the 20
     `ostringstream` build-a-string helpers.
   * Error messages: there are **163** `Exception e; e << ... ; throw e;`
     blocks (53 files). Add a constructor taking a format string and
     arguments, so these become one line, for example
     `throw Exception("Line {} out of range [0, {})", ln, nline);`. Keep
     `operator<<` working for existing code.
   * Add one generic `std::formatter` for every `Printable<T>` (it calls the
     existing `print()`), so GeoCal objects can be formatted with `{}`.
   * Because the type checks happen at compile time, mismatched arguments
     become compiler errors instead of runtime exceptions from `boost::format`.
2. **`std::source_location`** (libstdc++ 11, libc++ 16). `Exception` can
   record file, line and function of the throw site automatically, through a
   defaulted constructor argument (when combined with variadic format
   arguments this needs a small helper struct, the usual trick). Together
   with Boost.Stacktrace this makes production error reports much more
   useful, without changing any of the existing throw sites.
3. **Concepts** (language). GeoCal has a lot of code that is generic over the
   scalar type `T` = `double` or `AutoDerivative<double>`: `TimeBase<T>`,
   the `quat_rot*<T>` helpers, coordinate conversions, the
   `*WithDerivative` paths. A `GeoCalScalar` concept documents that intent,
   gives readable error messages instead of pages of Blitz/Boost template
   errors, and lets overloads be selected cleanly instead of with
   `enable_if`. Also useful for `ThreadPool::add_task`. Moderate benefit;
   mostly for new code and when code is touched anyway. **SWIG note:** use the
   `template<GeoCalScalar T>` form, which SWIG parses; avoid `requires`
   clauses in SWIG-parsed headers (§4).
4. **`std::span`** (both libraries). There are 178 raw `.data()` uses, many
   paired with a separate length, for example buffers for GDAL `RasterIO`,
   VICAR RTL, FFTW, and raster tile copies. `std::span<double>` as the
   parameter type keeps pointer and length together, gives bounds checks in
   debug builds (`_GLIBCXX_ASSERTIONS`), and can be built from `std::vector`,
   `std::array`, or Blitz `data()`/`size()`. Good for internal interfaces;
   the public array type stays Blitz.
5. **`std::chrono` calendar types** (`year_month_day`, `hh_mm_ss`,
   `sys_days`). These replace the `boost::date_time` formatting and
   calendar arithmetic in `geocal_time.cc`, and `std::chrono::parse`
   (libstdc++ ≥14) replaces its parsing, which removes a compiled Boost
   library. Leave GeoCal's own leap-second / PGS / GPS time handling alone.
   It is validated, tied to SPICE / toolkit data, and the C++20
   `utc_clock`/`gps_clock` would bring a second source of leap-second data.
6. **Threading primitives**: `std::jthread` with `stop_token` (cancellation),
   `std::latch`, `std::barrier`, `std::counting_semaphore`. These would make a
   small `std`-only replacement for the `boost::asio::thread_pool` in
   `geocal_thread_pool.h` simple. Optional; Asio is fine to keep.
7. **Defaulted comparisons / `operator<=>`**: `bool operator==(const X&) const
   = default;` for simple value types (coordinates, `FrameCoordinate`,
   `ImageCoordinate`, …), and `<=>` could replace `boost::operators` in
   `Time`. Small (only about 14 hand-written `operator==`/`operator<` today).
   **SWIG cannot parse defaulted `<=>`**, so this has to wait for nanobind or
   use `#ifndef SWIG`.
8. **Small conveniences:** `std::numbers::pi` replaces the non-standard
   `M_PI` macro (9 uses); `starts_with`/`ends_with` replace
   `substr(0,…)`/`find(...)==0` in parsers (about 11); `contains()` on maps
   and sets; designated initializers for option structs; `std::ssize`
   (fewer signed/unsigned casts in loops); `[[likely]]`; template lambdas;
   `constexpr` `std::vector`/`std::string`.
9. **`<ranges>`**: `std::ranges::sort(v)` and similar algorithms, projections,
   `views::iota`. Useful but small, because most GeoCal loops are index
   loops over Blitz arrays. The C++23 additions (`views::enumerate`,
   `views::zip`) would help those loops more (§7.5).
10. **Not useful for GeoCal now:** modules (build tooling, SWIG and nanobind
    are not ready) and coroutines (no matching use case).

### 7.2 What it costs

* **Code changes needed to compile as C++20: essentially none** (measured,
  §7.3).
* **Downstream projects** that include GeoCal headers must also compile as
  C++20 once those headers use C++20 features. That fits in the 2.0 API
  break anyway.
* **While SWIG is in use**, some C++20 syntax must stay out of SWIG-parsed
  headers (`<=>`, `requires`, `consteval`/`constinit`, `using enum`; tested
  with SWIG 4.3.1, §4). Implementation (`.cc`) files have no such limit.
* **libc++ (macOS) is behind libstdc++** for some library features (§7.4).
  Because macOS is best-effort, that only means some features will need a
  fallback if macOS support is wanted at some point.
* The **gcc 11.2 system cannot build 2.0**. That is handled by `maintain_1.28`.

### 7.3 Is the code ready for C++20? (measured)

I syntax-checked (`-fsyntax-only -Wall -Wextra -Wdeprecated`) all 366
`GeoCalCore`, CartLab and Oco3 `.cc` files with the pixi gcc 15.1, Boost
1.84 and Blitz 1.0.2, once as `-std=gnu++17` and once as `-std=gnu++20`:

* **The same 364 files compile in both modes.** The 2 that don't
  (`gdal_multi_file*.cc`) fail in both because of a missing include path in
  my test harness, not because of the language level.
* **The warning output is identical** in both modes, because
  `-Wdeprecated` already turns on the C++20 deprecation warnings in C++17
  mode. There were no errors from C++20 rewritten comparison operators, the
  usual problem for code using `boost::operators` or expression templates.
* Almost all of the 118,947 warnings come from **third-party headers**
  (Blitz: about 10.8k `-Wdeprecated-copy` and 2.7k `-Wvolatile`; Boost.Asio:
  248 `-Wdeprecated`). The C++20-related warnings inside the GeoCal tree come
  only from **libsdbx**. GeoCal's own code has only 10 `-Wdeprecated-copy`
  warnings plus 1501 `-Wunused-parameter`.
* **C++23 too:** the same harness with `-std=gnu++23` gives the same
  result: the same 364 files compile, and the warning output is identical
  to gnu++20.
* **gcc 16.2, C++23:** same 364 files compile. gcc 16 adds 61
  `-Wunused-but-set-variable` warnings, 60 in Boost headers
  (`concept_check.hpp`, process `utf8.ipp`) and 1 in GeoCal
  (`pan_sharpen_test.cc:96`).
* Not covered: Msp and Mspi (their optional dependencies weren't set up),
  the SWIG wrapper `.cc` files, linking, running the tests, and clang/libc++.
  Phase 0 step 3a (full build and all tests as C++23) closes those gaps.
* **Repeated 2026-11-07, strict C++23** (`-std=c++23`, gcc 15.2,
  `-fsyntax-only`, by the plan review): all 398 library and test files and all
  226 wrappers in frenchpeas's compile database compile. The only warnings are
  the known libsdbx deprecations and one `-Wsign-compare`. Still not covered:
  Msp, gcc 14, linking and running the tests.

### 7.4 Proposed minimum compiler for 2.0

"Stable, readily available in conda-forge, nothing cutting-edge", with
macOS best-effort:

| | Minimum | Why this one | Development (current pixi locks) |
|---|---|---|---|
| Linux, gcc / libstdc++ | **gcc 14** | Completes the C++20 *library* (chrono `parse` and time zones, on top of `std::format` from gcc 13); also has nearly all C++23 language features if we move to C++23 later (§7.5). Released 2024, long available in conda-forge. | gcc 15.2 |
| macOS, clang / libc++ | best-effort | libc++ 21 (current osx lock) has `std::format`, `jthread`, `mdspan`, but no `chrono::parse` and no `std::stacktrace`. | clang 21 / libc++ 21 |
| Standard | **C++23** (`AX_CXX_COMPILE_STDCXX([23], [noext], [mandatory])`; decided 2026-11-07, was C++20) | `noext` (strict ISO): nothing in the skeleton or in geocal's compile database needed GNU extensions | |

Where a libstdc++-only feature is used (for example `chrono::parse`), keep
it in one place, so a fallback can be added if macOS support is ever
wanted.

### 7.5 C++23: worth considering? (measured, gcc 15.1)

**State of gcc.** The C++23 *language* is essentially complete in gcc 14/15.
The *library* is mostly there in gcc 15 and gcc 16 adds `<mdspan>`. I
tested with the pixi gcc 15.1 (`-std=c++23`, `__cplusplus == 202302`), and
with gcc 16.2 (conda-forge) for `mdspan`:

| C++23 feature | gcc 15.1 | First gcc | Useful to GeoCal? |
|---|---|---|---|
| `std::expected` | ✅ | 12 | **Moderate.** A non-throwing return type for code that fails often, for example `ImageGroundConnection` calls that throw `ImageGroundConnectionFailed`, or solvers throwing `ConvergenceFailure` inside loops. Callers can test the result instead of catching exceptions. Keep exceptions in the public API that Python sees. |
| `views::enumerate`, `views::zip`, `cartesian_product`, `ranges::to` | ✅ | 13 / 14 | **Moderate.** These are the views that actually help with GeoCal-style loops: `for (auto [i, x] : views::enumerate(v))`, zip over parallel arrays, and nested line/sample loops. Check performance before using them in hot raster loops. |
| `std::print` / `println` | ✅ | 14 | Small: GeoCal mostly prints through `operator<<` and `print()`; useful in tools and tests. |
| `std::flat_map` / `flat_set` | ✅ | 15 | Small to moderate: sorted-vector maps for read-mostly lookup tables, such as time-keyed orbit and time-table maps used with `lower_bound`. Faster to search than `std::map`. gcc 15 only. |
| `std::stacktrace` | ✅ with `-lstdc++exp` | 12 (exp) | Would replace Boost.Stacktrace, but `libstdc++exp` is a separate static library that gcc still calls experimental. Not for a shared library yet. |
| Deducing `this` | ✅ | 14 | Could simplify the `Printable<T>` CRTP base and const/non-const accessor pairs. **SWIG 4.3.1 cannot parse it** (tested), so only after nanobind. |
| Multidimensional `operator[]` (`a[i, j]`) | ✅ | 12 | Little: Blitz uses `operator()`. SWIG parses it (tested). |
| `std::mdspan` | ❌ missing in gcc 15.1; ✅ **in gcc 16.2** (tested, rank-3 and `layout_left`) | 16 | The most interesting one for GeoCal, but **it doesn't need C++23 or gcc 16**: the conda-forge `mdspan` reference implementation gives the same thing (plus `submdspan`/`mdarray`) in C++17/20 (§3.3.1). |
| `if consteval`, `[[assume]]`, `static operator()`, `std::unreachable`, `std::to_underlying`, `string::contains`, `std::move_only_function`, `std::out_ptr` | ✅ | 11–14 | Small conveniences. `out_ptr` is handy for C APIs (GDAL, SPICE) that hand back pointers via out-parameters; `move_only_function` suits thread-pool task queues. SWIG parses `if consteval`, `[[assume]]`, `static operator()` (tested). |
| `std::generator` | ✅ | 14 | Little: lazy tile iteration could use it, but nothing needs it. |

**Cost.** Moving from C++20 to C++23 costs no code changes (measured with gcc
15.1 and 16.2, §7.3). The real costs are:
1. The minimum compiler effectively becomes gcc 14 (gcc 15 for `flat_map`,
   gcc 16 for `std::mdspan`). Moving the pixi env to a newer gcc is possible,
   so this isn't a blocker.
2. **ABI caution:** GCC's documentation still describes its C++23 support as
   experimental, meaning the layout of new library types may still change
   between gcc releases. That matters for a shared library: keep C++23
   library types (`std::expected`, `flat_map`, …) **out of the public API**
   until gcc drops the experimental label, or downstream code built with a
   different gcc could break.
3. SWIG can't parse deducing `this` (in addition to the C++20 items in §4).

**Recommendation:** C++23 is **not compelling enough to make it the standard
today**. Most of the real benefit is in C++20, which delivers `format`,
`source_location`, concepts, `span`, and chrono. C++23 adds conveniences
(`expected`, enumerate/zip, `print`, `flat_map`). The most useful new piece
for GeoCal, `mdspan`, can be had in C++20 from the reference implementation,
so it's no longer a reason to move. The ABI is not declared stable either.
However, it would also cost almost nothing. Two reasonable choices:

* **(A, preferred) Require C++20 with gcc ≥ 14 now; move to C++23 later.**
  The switch is one line, because the code already compiles as C++23. A
  natural time is **when SWIG is gone** (nanobind, Phase 6): then deducing
  `this` and the other syntax SWIG can't parse become usable in headers, and
  that is when C++23 pays off most.
* **(B) Require C++23 with gcc ≥ 14 now**, with a written rule: C++23
  library types only in implementation files, not in public headers, until
  the ABI is stable. That gives `expected` and enumerate/zip internally right
  away. It's not wrong, but the gain over (A) is small, and it adds a rule
  everyone has to remember.

**Update (2026-11-07): (B), decided by the maintainer**, at Phase 0 step 3a (first planned for Phase 1; the plan review moved it earlier). The
skeleton moved to C++23 with gcc 14 as the minimum (its decision 22, S7), and
its rules library, which geocal copies in Phase 6, now uses C++23 library
features (`<spanstream>`, `std::ranges::to`, `fold_left`,
`views::cartesian_product`). So geocal needs C++23 by Phase 6 anyway, and
switching in Phase 1 means code written in between can use it too. The limits
the skeleton works within: only gcc 14 library features (no range formatting,
no `std::mdspan`); Kokkos `mdspan` indexes with `m[i, j]` only in C++23 mode,
and inside a macro such as `BOOST_CHECK_EQUAL` that needs extra parentheses;
deducing `this` and `std::expected` not used yet. Keep the ABI rule of (B) for
public headers. The SWIG syntax limits (§4) stay until Phase 6 either way.

---

## 8. General mechanical code cleanup

The goal is changes that are **mostly done by tools, easy to review, and
don't change the public API**. They are done on `develop_2_0` in C++23 mode.
Tooling: clang-tidy, with `compile_commands.json` from the skeleton's
`script/make_compile_commands.py` and the wrapper `script/run_clang_tidy.sh`.
The wrapper adds the conda sysroot and the Blitz config shim clang needs.
Run per directory with `-fix`, followed by a build and the full test suite.

### 8.1 Make warnings useful first
Today, 118,947 warnings bury the roughly 1,560 that come from GeoCal's own
code. (*Correction, 2026-11-07 plan review:* that count came from the
`-fsyntax-only` harness of §7.3, which used `-I` for everything. The real build
in the conda env already treats `$PREFIX/include` as a system directory: a
gcc 15 release build has about 40 warnings, 38 `-Warray-bounds` and 2
`-Wmaybe-uninitialized`, all from Blitz (§8.2). Debug builds add the 1,501
`-Wunused-parameter`, because conda's `DEBUG_CXXFLAGS` include `-Wextra`.) Add third-party include directories with `-isystem` instead of `-I`
(in the `ac_blitz.m4`, `ac_boost.m4`, `ac_gdal.m4`… flags, or with a
GeoCal-side rewrite of the flags). Then decide about `-Wunused-parameter`
(1501): either turn it off, or fix it only in code that is touched anyway.
Unnamed parameters in virtual overrides are normal.

### 8.2 Real issues the compile check found (fix by hand, tiny)
* ~~`lib/GeoCalCore/Support/dir_change.h`: the destructor throws, and the
  constructor leaks `dirhandle` if `chdir` fails.~~ **Done (2026-10-25)**:
  the destructor reports the error to `std::cerr` instead, and the
  constructor closes the handle. Other compiler warnings were fixed at the
  same time; the only ones left in a gcc 15 `-Wall` build are Blitz++
  `-Warray-bounds`/`-Wmaybe-uninitialized` false positives (rank-generic
  slice and reduce code), plus two in SWIG generated boilerplate.
* `MapInfo` has a user-declared copy constructor but an implicit copy
  assignment (`-Wdeprecated-copy`, seen through `srtm_dem.h` and
  `dem_map_info_offset.h`). Add `operator=` (or `= default` both).

### 8.3 Mechanical modernization (clang-tidy checks, with measured scope)

| Change | Scope | Tool / check | Notes |
|---|---|---|---|
| Add `override` to overriding virtuals | Hundreds (387 `virtual …;` declarations, only 31 `override` today) | `modernize-use-override` | Biggest benefit: the compiler catches signature mismatches. SWIG handles `override`. |
| `throw()` → `noexcept` | 15 | `modernize-use-noexcept` | |
| `NULL` → `nullptr` | 63 | `modernize-use-nullptr` | |
| Empty `~X() {}` / `X() {}` → `= default` | 307 / 185 | `modernize-use-equals-default` | SWIG handles `= default`. |
| `boost::noncopyable` → `= delete` | 19 | `modernize-use-equals-delete` + manual | |
| `boost::shared_ptr<X> p(new X(…))` / `p.reset(new X(…))` → `auto p = boost::make_shared<X>(…)` | 261 / 162 | `modernize-make-shared` with `MakeSmartPtrFunction: boost::make_shared` | One allocation instead of two. Fails for private constructors; clang-tidy skips those. Carries over unchanged to `std::` in Phase 5. |
| `scoped_ptr` → `std::unique_ptr` | 7 | manual | |
| Container loops → range-for | ~60 (`.size()` index loops and explicit-iterator loops) + 106 `BOOST_FOREACH` | `modernize-loop-convert`; `BOOST_FOREACH(decl, range)` → `for(decl : range)` with a simple script (macro arguments can't contain unprotected commas, so a regex is safe) | **Leave the ~1000 numeric index loops** (`for(int i = 0; i < number_line; ++i)`) alone: they index Blitz arrays or pixel positions, and range-for doesn't apply. |
| `auto` for iterators, `new`, and casts | 25 iterator declarations + the make_shared sites above | `modernize-use-auto` (default settings only cover iterators/new/casts) | See the Blitz warning below. |
| `<math.h>` etc. → `<cmath>` | 11 | `modernize-deprecated-headers` | |
| `typedef` → `using` | 134 | `modernize-use-using` | Optional: readability only. SWIG 4 handles aliases. |
| `M_PI` → `std::numbers::pi` (C++20) | 9 | manual / sed | Removes a non-standard macro. |
| `substr(0,n) == "..."` / `find("...") == 0` → `starts_with` (C++20) | ~11 | `modernize-use-starts-ends-with` (clang-tidy ≥18) | |
| `Exception e; e << …; throw e;` → format-string constructor (C++20) | 163 / 53 files | **Not mechanical**: convert opportunistically when touching code | Needs the new `Exception` constructor (§7.1 item 1) first. |
| C-style numeric casts → `static_cast` | 413 | — | **Skip**: too noisy for the benefit. |

### 8.4 Things *not* to do
* **No blanket `auto` on Blitz expressions.** `auto x = a + b;`
  (where `a` and `b` are `blitz::Array`) holds an *expression template* that
  refers to its operands, not an `Array`. It evaluates lazily and can dangle.
  The same goes for `auto` on results of `blitz::Range` slices that are
  expected to copy. Restrict `auto` to iterators, `make_shared`, and casts.
* **No `auto` return types or trailing return types in public headers.**
  SWIG parses those headers and can't deduce return types.
* No signature-changing checks (`modernize-pass-by-value`,
  `use-trailing-return-type`, `use-nodiscard`). They change what SWIG and
  downstream code see, causing churn for little gain.
* The codebase-wide reformat is done with clang-format (§5 Phase 1 item 7,
  §8.5), as its own commit.

### 8.5 clang-format: measured impact (2026-10-23, clang-format 21.1.8)

Measured on `lib/` (652 files, 106k lines, generated wrappers and
`thirdparty/` excluded):

| Configuration | Files changed | Lines changed | Non-whitespace lines changed |
|---|---|---|---|
| Whitespace cleanup only (tabs → spaces, trailing whitespace) | — | 13,495 | 0 |
| Stock `LLVM` | 650 | 48,603 | 17,340 |
| Stock `GNU` | 650 | 82,768 | 33,969 |
| **Tuned to geocal** (skeleton `claude_notes/clang_format_trial.clang-format`) | 646 | **38,102 (36%)** | **15,400 (14%)** |

* *The tuned config keeps:* 2-space indent, 80 columns, `if(`/`for(`
  without a space, `X& y`, `template<class T>`, function braces on their own
  line, `public:` at column 0, no namespace indentation, includes not
  reordered, and comments not reflowed (the `//---` Doxygen blocks stay as
  they are).
* *What it can't keep* (these three make up most of the visible change):
  1. The "function name, newline, `(` arguments" layout: 1,046 lines in 262
     files. clang-format always writes `name(` and breaks inside the
     parentheses.
  2. `{ body }` on the line after a short signature: 608 lines. It is either
     joined onto the signature line or expanded to three lines.
  3. The serialization chains (`ar & GEOCAL_NVP_(x)` with each
     `& GEOCAL_NVP` on its own line): 595 continuation lines in 126 files.
     These get packed together, and some configurations even misparse
     `ar & X` as a reference declaration (`ar& X`). Use
     `// clang-format off/on` around them, or accept the packing.
* `ColumnLimit: 0` ("keep my line breaks") is worse: it joins lines (one
  300-character serialization line). The `Macros:` option made the
  serialization chains worse too.
* The rest is normalization of existing inconsistencies: 8.5k lines with
  tabs, 5.9k with trailing whitespace, default arguments written `x=1` vs
  `x = 1`, `Archive & ar` vs `Archive& ar`.
* **Assessment:** the result looks like geocal (same indentation, brace and
  spacing conventions), but it is a large one-time diff. The function
  signature layout changes visibly everywhere.
* **Options:**
  * (a) one-time reformat on `develop_2_0` with a `.git-blame-ignore-revs`
    entry, then `clang-format` as a routine tool (like `ruff format`);
  * (b) format only changed lines (`git clang-format`), so the style converges
    slowly and the code is mixed for a long time;
  * (c) whitespace cleanup only.
* **Recommendation (superseded):** (a), timed with the 2.0 break: after the
  clang-tidy passes, before the `std::shared_ptr` mass edit. Try the config on
  the skeleton first (S1) and review a few geocal files formatted with it
  before deciding. `.i` files are not formatted by clang-format.
* *Earlier decision (2026-11-05), superseded the same day:* no clang-format,
  formatting cleanup by AI instead (skeleton plan §5a).
* **Decided (maintainer, 2026-11-05, final): clang-format**, for geocal and
  the skeleton, with the **same `.clang-format`** in both. The configuration
  was found with `clang-format-auto-infer`, which searches for the options
  that change the existing code least (procedure: skeleton plan §5b). On
  geocal's `lib/` it changes 36,539 of 109k lines (13,925 ignoring
  whitespace, and 14,044 lines are tab/trailing-space cleanup alone). What no
  option keeps: `//---` doxygen blocks inside classes are indented, `/****//**`
  banners become `/****/ /**` (doxygen output unchanged), aligned parameter
  lists are repacked, `ar & X` becomes `ar& X`. Checked 2026-11-05 on
  frenchpeas: full rebuild with no new warnings, all wrappers regenerated
  (one docstring changed, for the better: `TimeTable`), C++ 623/623, Python
  218 passed, 118 skipped. The maintainer's reasoning: like `ruff format`,
  there are things one would do differently, but a standard tool plus all
  code conforming to it counts for a lot.
* Each check is its own commit, and so is each wrapper regeneration. (No
  `.git-blame-ignore-revs`: maintainer, 2026-11-05, the branch changes a lot
  anyway.)

---

## 9. Risks and how to handle them

| Risk | Mitigation |
|---|---|
| Accidentally using features newer than the minimum compiler | Phase 0 minimum-compiler (gcc 14) build, run regularly. |
| Fixes needed on both `maintain_1.28` and `develop_2_0` | Keep 1.x to bug fixes only; fix on `maintain_1.28` first and merge or cherry-pick forward while the code bases are still close. Features go the other way: 2.0 first, then backport. |
| `--without-boost-serialization` rots again | A regular C++-only build with the option (Phase 0 step 6); drop the option if it becomes a burden. |
| Threaded `PhaseCorrelationMatcher` hitting FFTW's non-thread-safe planner | pocketfft, or the FFTW interim fixes in §3.5, before parallelizing tie-point collection; parallel-vs-serial unit test. |
| C++20/23 syntax in headers breaks SWIG | Keep `<=>`, `requires`, `consteval`/`constinit`, `using enum` and deducing `this` out of SWIG-parsed headers until nanobind (§4); repeat the SWIG header-syntax test with SWIG 4.4 at the C++23 switch. |
| Old archives or pickles can no longer be read | Golden archive tests; keep NVP names and layout. |
| Downstream breakage from `std::shared_ptr` | GeoCal 2.0, migration script, convert reference downstream repos first. |
| Numeric differences after replacing GSL solvers | One solver family at a time; side-by-side comparison; explicit tolerance review. |
| Changed tile selection when files overlap after the R-tree swap | Edge-adjacency and overlap tests before the switch; deterministic ordering. |
| clang-tidy edits breaking SWIG parsing | Only SWIG-compatible checks (§8.4); regenerate wrappers and run the Python tests after each check. |
| Huge diffs from regenerated wrappers | Separate commits for wrapper regeneration (the wrappers stay in git by design until a nanobind move removes them). |
| Old bugs carried along unexamined by the mechanical phases (§0.3) | Fix the bugs the skeleton found first (Phase 0 step 9); review passes after Phases 2 and 6 and at the end (§5). |
| Archive writes that fail silently, or hang on `.gz`, in production now | Phase 0 step 9 (port of the skeleton's T2), with `/dev/full` tests. |
| A check that silently doesn't run (the skeleton's ASan build without ASan; `make check` on the installed library) | When adding a check, break something on purpose once and see it fail; check the flags in the build log. |
| Copies of the rules library or samples drifting between the skeleton and geocal | Copy from a named skeleton commit (`277b657` until Phase 6), diff file by file, take samples only from generated ones. |
| A long window with broken python bindings between Phases 5 and 6 | Record the python results and director shelves before Phase 5; start Phase 6 right after; no unrelated python work in between. |
| Tests only on the happy path (most of the skeleton's late bugs) | A failure-path test for each converted feature; coverage measured in Phase 0. |

---

## 10. Decisions and open questions

**Decided (maintainer answers):**
* Maintenance branch `maintain_1.28`: bug fixes only; any feature needed
  there is implemented on 2.0 first and backported.
* macOS: best-effort. Boost: 2.0 requires ≥ 1.84. Newer gcc (up to the
  current conda-forge 16.2) is acceptable if it matters.
* `gsl_check`, `gsl_fit`, `gsl_root`, `gsl_invert`: removed from the API in
  2.0; downstream is fixed as part of the migration.
* SWIG 4.4: the old problem is fixed; 4.4 can be used.
* Generated SWIG wrappers stay committed (deliberate trade-off so downstream
  doesn't need SWIG); nanobind would remove the issue.
* `--without-boost-serialization`: keep while it's cheap; it's currently
  broken in 12 files (§3.2). Fix, and add a regular build to keep it working.
* `PhaseCorrelationMatcher` is not threaded today, but threading it (for
  example in tie-point collection) is desirable, so FFT thread safety matters (§3.5).
* **Python API changes are accepted** for 2.0, as part of the backward-compatibility
  break (decided in the skeleton discussion, 2026-10-23).
* **No `boost::shared_ptr` support** after the conversion; no pointer-type
  parameterization of `swig_rules`.
* **Observers:** no ownership requirement; keep the ownership-agnostic
  `this_obj`, built with the aliasing constructor (§4).
* **`weak_ptr` to an object owned only by Python:** plain classes get
  `nb::new_` + `std::make_shared` constructors; for Python subclasses,
  document that the `weak_ptr` is only valid while C++ also owns the object.
  geocal has no such `weak_ptr` members outside the Observer machinery.
* **Every step is rehearsed in the skeleton first** (skeleton plan §0).
* **Commit policy in the skeleton:** stage only, the maintainer commits
  (geocal: never commit, per `CLAUDE.md`).
* **Shared rules come from the skeleton:** copy `swig_rules/` and changed
  `config/m4/` files from the skeleton (`swig-rules-skeleton`, now
  `cxx-python-skeleton`) into geocal (§5 Phase 0 step 4a). The upstream
  `swig-rules-repo` (now `cxx-python-rules`) and `autoconf-repo` are left
  alone; merging back may be looked at later.
* **Python bindings may be broken from Phase 5 until Phase 6** (§5 Phase 5).
* **Type check failures raise `TypeError`** in 2.0 (decided 2026-10-25). The
  Python code was changed on `develop_2_0`. The C++ side, starting with
  `IgcMspImp::ground_coordinate_dem` in `igc_msp.cc`, is deferred to a
  Phase 6 cleanup item, once the nanobind bindings can produce a `TypeError`.

**Decided (maintainer, 2026-11-07),** from the skeleton review:
* **C++23 with gcc 14 as the minimum, at the start of the work** (§5 Phase 0 step 3a, moved there from Phase 1 by the plan review; §7.5),
  as in the skeleton (its decision 22).
* **The skeleton's third fix plan goes first, at least T0–T2**, before geocal's
  Phase 0 (plus T6.11, the debug flags, before geocal's step 4a copies
  `ac_enable_debug.m4`). T2 is then ported into geocal's `swig_rules/lib/serialize_function.cc`
  in Phase 0 step 9; the rest of the third fix plan comes with geocal's
  Phase 6 copy.
* **The Phase 0 step 9 bug fixes are backported to `maintain_1.28`** (the
  archive write fix matters most for production). When to cut that branch is
  still open (item 2 below).
* **Old `.db`/`.json` shelves keep loading after Phase 6 through a
  compatibility shim**, not a conversion script (§5 Phase 6, "Pickles and
  shelves"). They store pickles that name `geocal_swig.<module>`; the shim (a
  `find_class` override where `sqlite_shelf.py` unpickles, or a stub
  `geocal_swig` package) maps those names onto the new support module. The
  payload, a Boost binary archive, is unchanged. Rehearsed in the skeleton
  first.
* **The python module after nanobind is `geocal_cxx._bindings`**, with
  `geocal` still the package users import (§5 Phase 6), as the skeleton's
  `skeleton_cxx._bindings` (its decision 23).

**Settled in the skeleton, carried over here (2026-11-07)** (these were
decided by the maintainer for the skeleton, which rehearses geocal; say so if
any should differ for geocal):
* **Phase 6 is nanobind.** The pilot worked and SWIG is gone there (§4).
* **Until Phase 6, rules come from the skeleton's last SWIG commit,
  `277b657`** (§5 Phase 0 step 4a); from Phase 6, from its current
  `cxx_python_rules/`.
* Blitz arrays with a nonzero base: saving throws (skeleton D1). Copying
  observers: deleted (D2). `#pragma once` in every header (D3).
* The in-memory serialization functions are renamed and the old names dropped
  (D8); the python XML string functions use `bytes` (D6).
* Free-threaded python is a long-term goal, not required (D9); thread safety
  is written down per class.
* CI deferred until it can be set up on the JPL runner (D11).
* Python 3.13 is the minimum from Phase 6 (D14; geocal's envs are 3.13.7).

**Still open:**
1. **Downstream archive inventory.** ECOSTRESS is probably the only
   producer of persisted archives, and they are embedded in HDF5 files.
   Revisit when the serialization work starts (before Phases 4–5).
2. **When to cut `maintain_1.28`.** At the latest before the first Phase 0
   step 9 backport (decided item above).
3. ~~**clang-format** adoption (§8.5)~~ Decided 2026-11-05: clang-format
   adopted, same configuration as the skeleton (§8.5, skeleton plan §5b).
4. ~~**Final Python module name** after nanobind.~~ Decided 2026-11-07:
   `geocal_cxx._bindings` (above).
5. ~~**`make check` tests the installed library** in conda/pixi builds
   installed into the env (§6.1). Pick a fix, try it in the skeleton.~~
   Fixed in the skeleton (ST.11, decision 18); geocal applies it in Phase 0
   step 9.
6. ~~**C++23 instead of C++20 in Phase 1.**~~ Decided 2026-11-07: yes (above),
   and moved to Phase 0 step 3a.
7. ~~**Do the skeleton's third fix plan before geocal's Phase 0?**~~ Decided
   2026-11-07: yes, at least T0–T2 (above).
8. ~~**Backport the Phase 0 step 9 bug fixes to `maintain_1.28`?**~~ Decided
   2026-11-07: yes (above). The step 9 table says what 1.28 gets per row
   (not the behavior-changing parts: deleted observer copies, the
   nonzero-base throw, Boost 1.84).
9. ~~**Old `.db`/`.json` shelves of C++ objects after Phase 6.**~~ Decided
   2026-11-07: a compatibility shim (above).
