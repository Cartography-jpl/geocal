# GeoCal 2.0 Release Notes

GeoCal 2.0 is developed on the `develop_2_0` branch. This release
deliberately allows API breaking changes, so code using GeoCal may need
to be updated. Breaking changes are listed first.

This file was started partway through 2.0 development. Earlier changes
on `develop_2_0` (e.g., the C++17 requirement, the directory
reorganization, moving `CombineOrbit` from ecostress-level1) still need
to be added.

## API breaking changes

### Type check failures raise `TypeError` instead of `RuntimeError`

Python code that rejects an argument because it has the wrong type now
raises `TypeError`. `TypeError` and `RuntimeError` both derive directly
from `Exception`; neither is a subclass of the other, so
`except RuntimeError:` no longer catches these errors.

Affected:

- `RsmImageSegmentHook` (`geocal_nitf_rsm`), when writing a NITF file
  with an `Rsm` that contains an element or `RsmMultiSection` section of
  an unrecognized type.
- `GlasGfm.create` / `NitfImageSegment.create_glas_gfm` (`glas_gfm`),
  when given an unsupported `ImageGroundConnection` type.
- `IgcMsp.ground_coordinate_dem` (`igc_msp`), when given a `Dem` that
  isn't a `SimpleDem`.

Errors coming from the C++ library through SWIG are still
`RuntimeError`. In particular, the C++ `IgcMspImp::ground_coordinate_dem`
has the same `SimpleDem` check, and still raises `RuntimeError`.

Code catching `RuntimeError` to handle these cases should catch
`TypeError` instead. Note that the generic "this point failed" handlers
in GeoCal (e.g., tie point collection and simultaneous bundle adjustment)
used to silently skip a point when given the wrong `Dem` type for
`IgcMsp`. That error now propagates instead.

### Default arguments are no longer shared between objects

These constructors used a single shared mutable default, so modifying
one object's value changed every other object created with the default.
Each object now gets its own:

- `GlasGfmCoreSet(group=...)`: `sensor_error_parameter_group`
- `GlasGfmGroup(adjustable_parameter=...)`: `adjustable_parameter`

The defaults are now `None`, meaning "create a new empty one".
`VicarInterface.build_command(extra_text=...)` also now defaults to
`None` rather than `[]`, with the same behavior.

### `test_support` removed

The old `python/test_support/test_support.py` module was removed. Test
fixtures and `require_*` skip markers now live in `python/tests/fixtures/`.

## Other changes

- Added `--enable-python-editable-mode` to configure (and
  `PYTHON_EDITABLE_MODE=t` for make) to install the pure python code as an
  editable pip install.
- Importing `geocal` no longer leaves OpenCV's library directory on
  `LD_LIBRARY_PATH`. This prepending broke system programs run through a
  shell, e.g., `subprocess.run(..., shell=True)`, on systems where the
  conda libraries didn't match the system ones.
- `IbisFile.column` now raises `RuntimeError` for an unrecognized column
  type. Before, it tried to raise a string, which itself fails with a
  `TypeError`.
- `VicarInterface` now closes the VICAR log file after the run.
- Python code updated for ruff 0.17 (f-strings, sorted imports, file
  handles closed with `with`, removed Python 2 compatibility code).
  `bin/findepend.py` was updated from Python 2.
- Development environment updated to ruff 0.17.2 and mypy 2.4.0, and
  type stubs added for scipy, tabulate and seaborn (`scipy-stubs` is
  matched to the scipy version).
- Type checking support:
  - `geocal_swig` now installs a generated `__init__.pyi`, so type checkers
    (mypy, IDEs) see every SWIG class and its attributes. SWIG method
    signatures are still untyped (`Any`), except in modules that have a
    hand-written stub (currently `image_coordinate`).
  - `geocal` has an `__init__.pyi` listing its modules, so
    `from geocal import X` is understood by type checkers.
  - `geocal` passes mypy cleanly. Several wrong type hints were corrected;
    e.g. `RayIntersect2.ray_intersect` and `RayIntersect3.ray_intersect`
    are now documented as returning `TiePoint | None`.
- `safe_matplotlib_import` works again. An earlier cleanup had removed its
  test import of `matplotlib.pyplot`, so it never fell back to the `agg`
  backend. The fallback also used `matplotlib.use(warn=...)`, which
  matplotlib no longer accepts.
