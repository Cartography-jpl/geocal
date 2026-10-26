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
- `ThreadPool` fixes. These affected anything run with more than one
  task, in particular `array_local_median` and
  `linear_gradient_bad_pixel_detection` (and the python
  `LinearGradientBadPixelDetection`) with `Number_task > 1`, which gave
  wrong results:
  - Tasks referred to the arguments of `add_task` after it returned
    (dangling references), so tasks could process the wrong data.
  - The pool couldn't be used again after `wait_tasks_finish`, so a second
    use in the same process silently ran nothing.
  - The pool's 10 threads were started when `libgeocal` was loaded, even if
    never used. This made every python process using geocal
    multi-threaded, which matters when multiprocessing forks. Threads are
    now only started when the pool is used.
- Development environment pins SWIG to 4.4, matching the committed
  wrappers (it was pinned to `<4.4.0`, so regenerated wrappers used an
  older SWIG than the rest).
- Python test warnings cleaned up; `pytest.ini` now uses `--strict-markers`.
  A misspelled `pytest.mark.skp` meant `test_generate_tie_point_collection`
  ran wherever the geocal test data was available, overwriting the
  simulated test data file.
- Bug fixes found while adding type hints (`mypy --disallow-untyped-defs`):
  - Writing or reading a GLAS/GFM covariance with a separate error
    covariance for each post (a 3-D `errcov_c2`) didn't work: the shape
    check always failed, the write used the wrong field and never set
    `common_posts_cov`, and the cov wrapper looked up the matrix size with
    the wrong index.
  - `DesCSEPHB` difference checking compared `min_time` with itself, so a
    difference in start time was never reported.
  - `TiePointCollectPicmtch` (and its helper) failed with the default
    `log_file=None`.
  - `SpiceIgc.image_coordinate` raised `TypeError` instead of
    `RuntimeError("ImageGroundConnectionFailed")` when it couldn't find a
    point.
  - `OutTempLocalTarget.error_exists` raised `TypeError`.
  - `ShapeFeature.__len__` took an extra argument, so `len()` failed.
  - The `isis_to_igc` HiRISE handler passed a float sample spacing where an
    integer is expected.
  - Log handler flushing in tie point collection used a misspelled logger
    name (`geoca1-python`).
- Small behavior changes from the same work:
  - The metadata attribute that `GlasGfm.igc` adds to the igc is now
    spelled `dynamic_range` (was `dyanmic_range`).
  - `GlasGfm.igc` raises a `RuntimeError` naming the missing DES if the
    CSSFAB, CSEPHB or CSATTB DES is missing, rather than failing later with
    an `AttributeError` on `None`. Similarly for other required inputs
    (e.g., `tie_point_grid` with an `aoi` but no `dem`, `pleides_rpc` with a
    missing XML element, `SpiceKernelByTime` with `.bsp`/`.bc` files but no
    `kernel_file`).
  - `GlasGfm.create` is now a `staticmethod` (it was always called that way).
  - `GlasGfmSpdcfComposite` raises an error for an unrecognized SPDCF
    family, rather than storing `None`.
  - Uses the non-deprecated `cv2.SIFT.create` instead of
    `cv2.xfeatures2d.SIFT_create`.
- `safe_matplotlib_import` works again. An earlier cleanup had removed its
  test import of `matplotlib.pyplot`, so it never fell back to the `agg`
  backend. The fallback also used `matplotlib.use(warn=...)`, which
  matplotlib no longer accepts.
