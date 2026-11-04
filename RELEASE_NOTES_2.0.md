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

### `Resampler` handling of missing coordinates

- The `Mark_missing` argument of the constructors (default -1000) is
  replaced by `Fill_value_threshold` (default: no threshold). A NaN X or Y
  coordinate is always missing. If you pass `Fill_value_threshold`, a point
  is also missing if *either* X or Y is <= the threshold. The old default
  of -1000 meant projected coordinates (e.g., meters) below -1000 were
  treated as fill, which silently dropped part of the data. Code that
  relied on the default to remove fill values like -9999 needs to either
  replace the fill with NaN or pass `Fill_value_threshold`.
- Before, the range of the coverage MapInfo left out points where either
  coordinate was fill, but the points were only marked as unused if both
  were fill. A point with only one fill coordinate could put data in the
  wrong place.
- The `RasterImage` constructor applies the threshold to the coordinates
  *before* interpolating them to the subpixels, so fill values only affect
  the subpixels next to them (the result is NaN, so those subpixels are
  missing). Before, the fill values were interpolated with the good values,
  which gave garbage coordinates near the edge of the fill. The array
  constructor takes coordinates that are already interpolated, so if you
  interpolate them yourself, replace the fill with NaN first.
- With `Exactly_match_mi`, missing points are now marked as unused. Before,
  they weren't marked at all, and were only dropped if their (garbage)
  index happened to fall outside the MapInfo.
- The constructors now throw an exception if all the points are missing.
  Before, they silently created a 1 x 1 MapInfo (or, with
  `Exactly_match_mi`, a resampler with no data). Valid points that just
  don't fall in the MapInfo are still fine, use `empty_resample()` to
  check for that.
- The constructors and `determine_range` now throw an exception if the
  coordinates cross a discontinuity of the MapInfo's projection, rather than
  silently giving wrong results (e.g., data smeared across the whole map):
  - For a `Geodetic` (or OGR geographic, e.g., EPSG:4326) MapInfo, data
    crossing the date line. The message says to use a `Geodetic360`
    MapInfo instead. For a `Geodetic360` MapInfo, data crossing the prime
    meridian (use `Geodetic`).
  - For a geographic MapInfo, data covering a pole. The message says to
    use a polar projection, such as polar stereographic.
  - For a projected MapInfo, a step between neighboring coordinates more
    than `Resampler.max_step_ratio()` (default 50) times the median step.
    This catches a projection cut or a pole (e.g., the Albers cut opposite
    the central meridian, or Mercator near a pole). The ratio can be
    changed with `Resampler.set_max_step_ratio`; 0 turns the check off.

  Unmarked fill values in the coordinates (e.g., -1e20) also trigger these
  checks, and the message mentions this. Code that worked before may now
  throw; in those cases the results were wrong.
- `determine_range` has a new last argument `Fill_value_threshold`, the
  same as the constructors.

### `Resampler` RasterImage constructor coordinates are centered on the pixels

`resample_field` copies each raw data pixel to all `Num_sub_pixel` x
`Num_sub_pixel` of its subpixels, so the coordinates of those subpixels
should be centered on that raw pixel. The `Resampler` constructor taking
`RasterImage` used `MagnifyBilinear`, which is corner aligned (subpixel k
at raw position k / nsub). That put the data (nsub - 1) / (2 nsub) raw
pixels from where it belongs (1/4 pixel for nsub 2, 1/3 for nsub 3), and
the last raw line and sample were never used. The constructor now
interpolates centered on the raw pixels (subpixel k at raw position
(k + 0.5) / nsub - 0.5), extrapolating linearly at the edges. The
subpixel grid is now `number_line * nsub` x `number_sample * nsub`,
rather than `(number_line - 1) * nsub` x `(number_sample - 1) * nsub`.
This changes the output, which is now correct.

The array constructor uses whatever interpolation the caller did. Note
that `scipy.ndimage.zoom(order=1)`, which ECOSTRESS uses, has the same kind
of problem (it stretches rather than shifts: up to (nsub - 1) / (2 nsub)
raw pixels off at the edges of the swath, and correct in the middle). That
is unchanged, and the constructor documentation describes it.

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
- Python lists can now be passed where the C++ takes a `std::vector` of
  `Time`, `ImageCoordinate`, `InterestPoint` or `AutoDerivative<double>`,
  e.g., `PiecewiseLinear([t0, t1, t2], ...)`, `MeasuredTimeTable([t0, t1])`
  or `Vector_Time([t0, t1])`. Before, these raised a `TypeError` and you had
  to build a `Vector_Time` (etc.) with `push_back`. Iterating over these
  vectors now returns the actual objects. Before, it returned untyped SWIG
  pointers (and leaked memory). `Vector_Time` can now be pickled.
- `PiecewiseLinear` no longer has a `FunctionType` attribute. This was
  an unused variable of an anonymous enum type, not the type of the
  enum. The enum is now named `PiecewiseLinear::FunctionType`; the values
  `PiecewiseLinear.LINEAR`, `CONSTANT` and `LINEAR_TO_ZERO` are unchanged.
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
  - An exception thrown by a task crashed the process (`std::terminate`).
    It is now caught and rethrown by `wait_tasks_finish`.
  - Using the pool from inside one of its own tasks, or from a second
    thread while the first still has tasks it hasn't waited for, could
    hang or give wrong results. Neither is supported, and both now throw
    an exception. Changing `number_thread` while tasks are pending also
    throws, and `number_thread` must be at least 1.
  - New C++ helper `parallel_row_blocks`, which divides rows into blocks
    for the pool and always waits for the tasks, even if one throws.
    `array_local_median` now uses it.
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
- `Resampler` changes:
  - `resample_field` and `empty_resample` can take the data as a numpy
    array (at the original resolution, not magnified by the number of
    subpixels), so you no longer need to copy it into a
    `MemoryRasterImageFloat`. The `RasterImage` versions still work.
  - `resample_field` is multithreaded, using the `ThreadPool`
    (`ThreadPool.thread_pool().number_thread` sets the number of
    threads). It has a new last argument `Number_task` (default -1 uses
    4 x the number of threads, and does small data in a single thread). It
    is about 5x faster for a 10000 x 10000 subpixel resample with 10
    threads (0.97 s -> 0.19 s), and no longer creates a magnified copy of
    the data. Results are the same apart from floating point round off in
    the averages (about 1e-15 relative).
  - The constructors and `determine_range` are also multithreaded, and the
    constructors have a new last argument `Number_task`, the same as
    `resample_field`. For 10000 x 10000 subpixels with 10 threads: the
    constructor from arrays 1.4 s -> 0.17 s, `determine_range`
    1.1 s -> 0.09 s (these include the new projection checks described
    above). The `RasterImage` constructor interpolates the
    coordinates as it goes rather than creating the full size
    interpolated arrays with `MagnifyBilinear`, so it uses much less
    memory (2.6 s -> 0.45 s for 5000 x 5000 coordinates, nsub 2,
    including the projection checks). Apart
    from the missing coordinate changes and the `RasterImage` constructor
    interpolation fix above, the results are identical to before.
  - Bug fix: `resample_field(Fname, Data, ...)`, which writes a VICAR file,
    passed `Fill_value_threshold` as the `Use_smallest_ic` argument, so the
    threshold was ignored, and any nonzero threshold turned on
    `Use_smallest_ic`.
  - The C++ unit tests for `Resampler` were never built (the test file
    wasn't in `implementation.am`). They are now, with tests comparing
    against the old algorithm.
- New `Resampler.create_from_lat_lon(Latitude, Longitude, Mi,
  Num_sub_pixel, Boundary=1, Fill_value_threshold, Number_task)`. This
  builds a resampler directly from swath latitude and longitude, all in
  C++: it converts to the coordinates of Mi, finds the part of the swath
  that falls in Mi (padded by `Boundary` raw pixels), interpolates
  centered on the raw pixels (the same as
  `scipy.interpolate.RegularGridInterpolator`, linear with extrapolation)
  and trims to the part that is used. It always matches Mi exactly. This
  replaces `build_resampler` in maia-misc-tools: for the PACE OCI granule
  used there it gives identical output, and takes 0.66 s rather than
  20.7 s.
  - The part of the swath used is given by the new attributes
    `swath_line_start`, `swath_number_line`, `swath_sample_start`,
    `swath_number_sample` and `has_swath_range` (and in python
    `line_range` and `sample_range` slices). Data passed to
    `resample_field` and `empty_resample` must be subset to this range,
    e.g., `r.resample_field(d[r.line_range, r.sample_range])`, and a
    different size throws an exception. Resamplers from the other
    constructors cover the whole swath, and `line_range`/`sample_range`
    select all the data, so you can always subset with them.
  - Serialization (and pickling) includes the swath range. Older
    serialized `Resampler` objects still load, with the full swath.
  - `Resampler.data_index` (the output line and sample of each subpixel,
    -9999 for missing coordinates) is now available in python, mostly
    for testing.
  - New python unit tests in `tests/unit/resampler_test.py`.
- `safe_matplotlib_import` works again. An earlier cleanup had removed its
  test import of `matplotlib.pyplot`, so it never fell back to the `agg`
  backend. The fallback also used `matplotlib.use(warn=...)`, which
  matplotlib no longer accepts.
