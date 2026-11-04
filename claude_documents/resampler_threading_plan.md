# Speeding up Resampler (resampler.cc)

Status: plan approved pending go ahead, nothing implemented yet
(2026-11-03). Open questions resolved; see "Decisions" and
"Implementation order" at the end.

## Baseline timings (rifle, 40 cores)

Synthetic case: 5000x5000 raw data, nsub = 2 (10000x10000 subpixels), a
rotated (20 degree) geodetic swath, 10000x10000 output MapInfo. Times are for
the installed libgeocal (-O2).

| Operation                                   | Time    |
|---------------------------------------------|---------|
| `Resampler(blitz x, blitz y, ...)`          | 1.39 s  |
|   coordinate_to_index (blitz expression)    | 0.50 s  |
|   rint/cast into data_index                 | 0.35 s  |
|   min/max loop                              | 0.22 s  |
|   subtract miny/minx                        | 0.19 s  |
|   fill value loop                           | 0.14 s  |
| `Resampler(RasterImage x, RasterImage y)`   | 2.57 s  |
|   2x MagnifyBilinear + read_double          | 1.2 s   |
| `determine_range`                           | 1.18 s  |
| `resample_field(RasterImage)`               | 1.17 s  |
|   MagnifyReplicate + read_double            | 0.57 s  |
|   scatter loop + final where                | 0.6 s   |
| `empty_resample()`                          | ~0 (returns on first hit; worst case ~0.1 s) |
| `map_values`                                | not timed; per pixel virtual calls + shared_ptr allocation |

Every step is memory bandwidth bound streaming over arrays of about 800 MB.
The rotated swath doesn't change any of the costs.

## Changes

### 1. resample_field

* New overload taking `const blitz::Array<double,2>& Data` at raw
  (unmagnified) resolution. Look up `Data(i/nsub, j/nsub)` directly instead
  of using MagnifyReplicate. That gives the same values with no 800 MB
  temporary. The RasterImage version becomes `read_double` + a call to the
  new overload. `empty_resample(Data, thresh)` gets the same treatment.
* Parallel scatter. Output cells receive contributions from many input
  subpixels, so tasks can't write to res/cnt directly.
  - Step A (parallel over ~1024x1024 input tiles): each task finds the
    bounding box of the output cells its tile touches and accumulates sum/cnt
    in private buffers (about 0.3x the output size in total). For
    Use_smallest_ic it also keeps the smallest input linear index i*ncol+j
    per cell.
  - Step B (parallel over output row bands): merge the tile buffers in a
    fixed order, then apply Scale_data/Fill_value/Negative_to_zero. Taking
    the smallest linear index reproduces the serial "first hit" rule
    exactly. The fixed order makes results deterministic; sums can differ
    from the serial code by about 1e-15 relative.
  - Run serially when number_thread()==1 or the problem is small (the tiled
    version is 0.7x serial speed on one thread).
* Drop the fill value `else` branch. Any cell with cnt==0 is overwritten by
  Fill_value in the final `where`, so that branch has no effect.
* Bug fix: `resample_field(Fname, Data, Scale, File_type, Neg, Fill,
  Thresh)` passes Thresh as the `bool Use_smallest_ic` argument. Note the fix
  in RELEASE_NOTES_2.0.md.

Prototype measurements (scatter only, exact match to serial):
1 thread 0.7x, 4 threads 2.1x, 10 threads 3.5x, 20 threads 3.7x.
Use_smallest_ic: 3.1x at 10 threads.

Expected: 1.17 s -> 0.2-0.3 s (4-5x).

### 2. Constructor / init

* Fuse the five blitz passes into two explicit loops, parallel over row
  blocks:
  - Pass 1: coordinate_to_index (the scalar inline version in map_info.h)
    + lrint into data_index + per task min/max of valid points.
    Reduce the min/max serially (one entry per task).
  - Pass 2: subtract miny/minx and set fill points to -9999.
* Fixes worth making while we are in there:
  - The fill loop uses `&&` (both X and Y are fill) where the min/max loop
    uses `||` semantics (excludes a point if either is fill). Decided:
    use `||` in both.
  - The fill points are shifted by -miny/-minx before being set to -9999,
    so this is harmless, but in Exactly_match_mi mode fill points are never
    marked at all. They are dropped only if their (garbage) index happens
    to fall outside the MapInfo.
  - If there are no valid points at all, `Mi.subset(0,0,1,1)` is silently
    used. Decided: throw (verified ECOSTRESS doesn't rely on it).
* RasterImage constructor: MagnifyBilinear is 1.2 s of the 2.57 s. Replace
  it with a direct bilinear interpolation into the X/Y arrays inside pass 1
  (read the raw arrays once, interpolate on the fly), which also avoids the
  two 800 MB interpolated arrays. Need care to exactly match MagnifyBilinear
  at the edges (it extrapolates using the last row/col). Unit test compares
  against MagnifyBilinear.

Prototype: fused init 0.87 s on 1 thread (vs 1.39 s), 0.23 s on 4,
0.11 s on 10, 0.07 s on 20.

Expected: blitz ctor 1.39 s -> ~0.1 s (~12x at 10 threads); RasterImage
ctor 2.57 s -> ~0.2 s.

### 3. determine_range

Same structure: one fused parallel loop computing the index per point and a
per task (lstart, lend, sstart, send), then a serial reduce. Use the scalar
coordinate_to_index instead of building two 800 MB index arrays.

Prototype: 0.98 s on 1 thread, 0.27 s on 4, 0.12 s on 10, 0.07 s on 20.

Expected: 1.18 s -> ~0.12 s (~10x). Note that the edge rounding to nsub
multiples (lend/send clamped to rows/cols) is kept as is.

### 4. empty_resample()

Usually returns almost immediately. Worst case (no data, ~0.1 s) is too
small to be worth threading. Leave alone, apart from removing
MagnifyReplicate from the Data overload.

### 5. map_values (optional)

Per output pixel this calls mi.ground_coordinate (virtual,
allocates a shared_ptr) + lat_lon_height. Easy to parallelize over rows,
but only if the CoordinateConverter and Dem are thread safe. GeodeticConverter
is, but OgrCoordinate (OGRCoordinateTransformation) and Dem with tile caches
are not in general. Leave out unless someone is actually seeing this as a
bottleneck.

### 6. Lat/lon constructor with a swath range (replaces maia build_resampler)

`maia_misc_tools/resample_pace.py` (`/home/smyth/Local/maia-misc-tools`)
builds a Resampler in python with `build_resampler(lat, lon, nsubpixel, w, mi)`:

1. Convert lat/lon (degrees) to map x/y with
   `OgrWrapper.lat_lon_coordinate_convert`.
2. `determine_range(x, y, mi, 1)` on the coarse grid, pad by 1 pixel on
   each side, clamp to the swath: lrange_f/srange_f.
3. `subpixel_swath_extrapolate` on the coarse subset: center aligned
   bilinear interpolation (subpixel k is at raw position
   -0.5 + (k+0.5)/nsub), linearly extrapolated at the edges
   (scipy RegularGridInterpolator, fill_value=None).
4. `determine_range(x_ex, y_ex, mi, nsub)` on the subpixel grid, trim to
   `slice(ls, le + nsub)`, `slice(ss, se + nsub)`.
5. `Resampler(x_ex[trim], y_ex[trim], mi, nsub, True)`, and return the
   resampler plus lrange_f/srange_f. Every field read afterwards
   (write_oci_fields -> geometry_grid, resample_swath, coverage_mask,
   write_spexone_fields) is cut with `isel(scans=lrange, pixels=srange)`.

Timing on the real OCI granule (PACE_OCI.20240306T201910, 1709x1272 swath,
nsub = 15, subset 547x803 -> 8205x12045 subpixels):

| Step                                   | Time    |
|----------------------------------------|---------|
| ogr convert                            | 0.28 s  |
| determine_range, coarse                | 0.02 s  |
| subpixel_swath_extrapolate (scipy)     | 20.60 s |
| determine_range, fine                  | 1.13 s  |
| Resampler ctor (incl numpy -> blitz)   | 0.81 s  |
| total                                  | 22.8 s  |
| one resample_swath call after that     | 1.63 s  |

**Bug in build_resampler found while checking this.** Step 4 trims
`ls` subpixel rows off the top (ls = 15 = one raw scan for this granule), but
the returned lrange_f is not moved down to match. Data row 0 is then
paired with coordinates for the next scan. Measured by resampling the swath
latitude itself and comparing it to the grid latitude: mean error
-1.17 km (rms 1.21 km) with the current lrange_f, -0.000 km (rms 0.29 km)
with lrange_f shifted by one scan. The same thing happens for columns when
ss > 0. The new constructor computes the combined range itself, which
fixes this. (Separate fix for maia-misc-tools in the meantime:
`lrange = slice(lrange_f.start + ls // nsub, lrange_f.start + (le + nsub) // nsub)`,
the same for srange.)

**New factory, create_from_lat_lon** (signature under "Named factory" below;
Boundary is the padding in raw pixels, the "-1/+1" above)

It always matches Mi exactly (Exactly_match_mi = true, which is what maia
uses). Steps, all in C++:

1. Coarse lat/lon -> map x/y with Mi's CoordinateConverter. For
   OgrCoordinateConverter (dynamic_cast) use the bulk OGR Transform, like
   lat_lon_coordinate_convert. Otherwise call convert_to_coordinate per
   point (GeodeticConverter etc.). Serial: OGRCoordinateTransformation isn't
   thread safe, and this is only 0.28 s for the full OCI swath. NaN lat/lon
   are missing points.
2. Coarse range (determine_range logic, nsub = 1), pad by Boundary,
   clamp. Throw a clear exception if nothing falls in Mi.
3. Parallel pass over the subpixel grid of the coarse subset: bilinear
   interpolate x/y on the fly (center aligned, clamped cell index, which is
   the same as linear extrapolation at the edges), coordinate_to_index,
   and per task bounding box of the in-range subpixels. Nothing big is
   stored. The bounding box is rounded to nsub multiples as in
   determine_range.
4. Allocate data_index for the trimmed region only and fill it in a
   second parallel pass (same interpolation). A subpixel whose stencil
   touches a missing coarse point is -9999.
5. Store the swath range: line_start = coarse_start + ls/nsub,
   number_line = (le + nsub - ls)/nsub, and the same for samples. Check
   data_index rows == number_line * nsub.

Prototype of the fused interpolate + index + range pass at the OCI size
(8205x12045 subpixels, /tmp/rsproto/interp.cc): 2.21 s on 1 thread,
0.60 s on 4, 0.27 s on 10, 0.16 s on 20. Step 4 costs about the same
again. Expected total for the OCI resampler: ~0.9 s at 10 threads vs 22.8 s
now (~25x). Memory: no x_ex/y_ex (2x0.8 GB), trimmed copies (2x0.8 GB)
or xindex/yindex temporaries.

The interpolation must agree with subpixel_swath_extrapolate to ~1e-6 m
(so maia output doesn't change apart from the range bug fix). Check this
with a python test in maia-misc-tools against the scipy version, in addition
to the C++ unit test below.

Note on the existing RasterImage constructor: MagnifyBilinear is corner
aligned (output size (N-1)*nsub, subpixel k at raw position k/nsub) while
the data is replicated with MagnifyReplicate (subpixel k is raw pixel
floor(k/nsub)). Coordinates and data are offset by about half a raw pixel,
and the last raw row/column gets no coordinates. This is the existing
ECOSTRESS behavior. Leave it alone, but note it in the class documentation;
the new constructor doesn't have the problem.

**Swath range attributes**

    int swath_line_start() const;     // default 0
    int swath_number_line() const;    // default std::numeric_limits<int>::max()
    int swath_sample_start() const;   // default 0
    int swath_number_sample() const;  // default std::numeric_limits<int>::max()
    bool has_swath_range() const;

The existing constructors keep the default (all the data). In python,
add `%pythoncode` properties `line_range` and `sample_range` returning
`slice(start, start + number)`. With the defaults this is
`slice(0, 2147483647)`, which selects the full array in numpy, xarray
`isel` and h5py, so callers can always cut with them, range or not:

    geo["solar_zenith"].isel(scans=r.line_range, pixels=r.sample_range)

(write_spexone_fields needs `c0 + start`, so it uses swath_sample_start
directly.) start + number overflows int for the defaults, so compute the
slice in python, or use None for the stop when number is max int.

**Documentation** (class comment, resample_field, empty_resample and the new
constructor): when the Resampler has a swath range, Data passed to
resample_field/empty_resample must already be subset to
`[swath_line_start, swath_line_start + swath_number_line)` x
`[swath_sample_start, ...)` of the full swath, with a python example. We
subset rather than passing the full field so that only the part of each
field we use is read from the file (the OCI subset is 547x803 of
1709x1272).

**Size check**: resample_field currently only checks that the magnified data
is at least as large as data_index. A full swath passed by mistake is
larger, so it passes the check and silently gives a shifted result. When
has_swath_range(), require Data to be exactly swath_number_line x
swath_number_sample, and throw an Exception that names the expected
range. Otherwise keep the existing check.

**Serialization**: add the four fields, `GEOCAL_CLASS_VERSION(Resampler, 1)`,
and give version 0 archives the defaults, so existing pickles (e.g. the
maia resampler.pickle) still load.

**Named factory (decided)**: the new constructor would have the same first
four argument types as the existing blitz constructor (X/Y interpolated,
MapInfo, int), so as an overload it would be ambiguous in C++ and in SWIG.
Use a static factory instead, which also says clearly what it does:

    static boost::shared_ptr<Resampler> create_from_lat_lon
    (const blitz::Array<double, 2>& Latitude,
     const blitz::Array<double, 2>& Longitude,
     const MapInfo& Mi, int Num_sub_pixel, int Boundary = 1);

It uses a private constructor (or the private default constructor, then
fills in the fields). Add it to resampler.i as a static method, along with
the attributes.

**maia-misc-tools follow up** (not part of the geocal change):
build_resampler becomes one call; write_oci_fields/write_spexone_fields/
coverage_mask take the ranges from the resampler instead of separate
lrange/srange arguments; resample_swath can pass the numpy array straight
to the new blitz resample_field overload (no MemoryRasterImageFloat copy).

### 7. Date line (and prime meridian) check

Resampling to lat/lon grids is used in other applications (the maia
example is Albers and isn't affected). With a Geodetic MapInfo
(longitude in [-180, 180]) a swath crossing the date line goes wrong
silently. Tested with the installed geocal: a 200x200 swath at 0.01 deg
running from 179.005 to -179.005:

* Coverage MapInfo (Exactly_match_mi false): the MapInfo becomes 36000
  pixels wide (-180 to 180) for 200 pixels of data.
* Exactly_match_mi: the data on the other side of the line goes to the
  wrong place, or is dropped if it falls outside the MapInfo.
* Interpolated coordinates: callers interpolate before we see the data
  (scipy zoom, MagnifyBilinear), so the subpixels between the two sides
  get made up longitudes anywhere between -180 and 180 (max step
  of 270-360 deg between subpixels one raw pixel apart). These scatter data
  across unrelated tiles far from the swath.

We don't try to handle this, we throw. The caller switches to a
Geodetic360 MapInfo (Geodetic360Converter, longitude in [0, 360)), where the
date line is just 180.

**Detection**: when the MapInfo is geographic, compare each coordinate with
the one a raw pixel away (`nsub` subpixels) in both line and sample
directions, and throw if `|dx| > 180` degrees. Skip pairs where either
point is missing (Mark_missing / NaN). A real swath step is far below 180
degrees (0.01 deg here, a few degrees for a very coarse grid). Checked
against interpolated grids (spline order 2 and bilinear, nsub 1, 2, 15, 21):
it caught every crossing and gave no false positives on a swath next to
the line but not crossing it. Using a step of nsub rather than adjacent
subpixels matters: adjacent subpixels of interpolated junk can differ by
only 180/nsub.

"Geographic MapInfo" means:
* GeodeticConverter: date line check, message says to use Geodetic360.
* Geodetic360Converter: the same problem at the prime meridian (0/360).
  The check is free to apply, so do it, with the message saying to use
  Geodetic.
* OgrCoordinateConverter where `ogr().ogr().IsGeographic()` (e.g.
  EPSG:4326): date line check, message says to use Geodetic360.
* Projected systems (Albers, UTM, polar stereographic): no check.

Exception message, e.g.:

    Resampler: the coordinates cross the -180/180 longitude line (a jump of
    359.98 degrees between line 1200, sample 99 and line 1200, sample 100),
    and the MapInfo is Geodetic. Use a Geodetic360 MapInfo for data that
    crosses the date line.

Swaths over a pole can't be handled by either Geodetic or Geodetic360
(longitude covers all values). The check catches that too, since the swath
crosses both lines. For a geographic MapInfo we can tell this apart and give
the right message, see the next section.

**Where**: a shared helper
`check_longitude_wrap(X, Mi, Step, Mark_missing)` (private static) called from:
* `init` (all existing constructors): Step = nsub, on the interpolated
  coordinates, done in the fused parallel pass 1 (per task "first failure"
  recorded, thrown after the pass, so we don't throw from inside a task).
* `determine_range`: Step = Num_sub_pixel, also fused.
* `create_from_lat_lon`: on the coarse converted grid (Step = 1, exact,
  cheap) before interpolating. With a Geodetic360 MapInfo the factory does
  the conversion itself through the converter, so it gets 0-360
  longitudes and works across the date line without the caller doing
  anything.
* RasterImage constructor: on the raw X raster before MagnifyBilinear
  (Step = 1).

Cost: only for a geographic MapInfo. Whether to check is decided once per
call from the converter type, and the pass is a template on a
`bool Check_wrap` parameter, so the non geographic version is compiled
without the check: no extra reads and no per pixel branch. Projected
MapInfo (Albers, UTM, ...) have no cost at all. For geographic MapInfo, the
sample direction neighbor is already in cache, but the line direction
comparison reads a second row stream, so pass 1 might be ~10-20% slower.
This is an estimate; measure it during implementation.

**Behavior change**: the existing constructors and determine_range will now
throw on input that used to give (wrong) results silently. Note this in
RELEASE_NOTES_2.0.md.

**Unit tests** (resampler_test.cc, fixture: 200x200 swath at 0.01 deg,
lon from 179.005 across the line, lat from 10.005):

18. `date_line_geodetic_throws`: GeodeticConverter MapInfo,
    BOOST_CHECK_THROW(..., Exception) for the blitz constructor with
    Exactly_match_mi false and true, determine_range, and
    create_from_lat_lon. Also catch one and check what() mentions
    Geodetic360, so the user gets the useful message, not some other
    exception (e.g. a range_check from Geodetic).
19. `date_line_interpolated_throws`: the same swath, bilinear
    interpolated to nsub = 2 and 15 (done in the test), blitz constructor
    throws. This is the realistic case.
20. `date_line_geodetic360_works`: same swath with longitude in [0, 360),
    Geodetic360Converter MapInfo matching the swath exactly. No throw;
    the coverage MapInfo is 200 pixels wide (not 36000); resampling a field
    of ones fills all 200x200 cells. create_from_lat_lon with the original
    [-180, 180) lat/lon and a Geodetic360 MapInfo also works.
21. `near_date_line_no_throw`: a swath ending at 179.99 (not crossing)
    with a Geodetic MapInfo doesn't throw. Guards against false positives.
22. `prime_meridian_geodetic360_throws`: swath crossing 0 with a
    Geodetic360 MapInfo throws, message mentions Geodetic.
23. `date_line_ogr_geographic_throws`: EPSG:4326 OgrCoordinateConverter
    MapInfo throws; an Albers MapInfo with the same swath does not.
24. `date_line_missing_points`: fill (-9999) points next to valid ones
    don't trigger the check (a jump from -9999 isn't a crossing).

### 8. Pole and projection cut check (all map projections)

The date line problem is one case of a general one. Every projection
except azimuthal and transverse ones has a cut (a line where the map
coordinates jump) and/or singular poles:

* Geodetic / cylindrical (Mercator, EASE-Grid 2.0 global EPSG:6933, ...):
  cut at lon_0 + 180, the poles are lines (or at infinity for Mercator).
* Conic (Albers, Lambert conformal conic): cut at lon_0 + 180 (61.4 E for
  the LA Albers), and the far pole is a line.
* Azimuthal (polar stereographic, LAEA polar): only singular at the
  opposite pole.
* Transverse (UTM): no problem at the pole, but very large distortion far
  from the central meridian.

Tested with pyproj on synthetic 300x300, 1 km swaths (`/tmp/rsproto/pole.py`),
counting raw neighbor steps that are more than K times the median step
of the swath:

| Swath                     | Projection              | Steps flagged (K=20/50/100) |
|---------------------------|-------------------------|-----------------------------|
| OCI (real)                | LA Albers, UTM 11N      | 0 / 0 / 0                   |
| 35N, 61.4E (Albers cut)   | LA Albers               | 300 / 300 / 300             |
| 35N, 61.4E                | UTM 41N                 | 0 / 0 / 0                   |
| over north pole           | LA Albers               | 464 / 270 / 240             |
| over north pole           | EASE 6933, Mercator     | ~370 / ~185 / ~160          |
| over north pole           | polar stereo 3413, LAEA 3574, UTM | 0 / 0 / 0         |
| 88N, not over the pole    | LA Albers               | 23 / 23 / 23                |
| 88N, not over the pole    | EASE 6933               | 0 / 0 / 0                   |
| over south pole           | polar stereo south 3031 | 0 / 0 / 0                   |
| over south pole           | polar stereo north 3413 | 4492 / 1788 / 904           |

The results are the same on interpolated grids (nsub 2 and 15) when the
check uses every nsub'th subpixel (the raw grid points). The OCI swath has
a max step / median step of 6.9 (pixel growth to the edge of the scan), so
K = 50 has a big margin. The 88N Albers case is a real failure (the swath
reaches the far side of the Albers pole line), not a false positive.

So yes, this applies to all the projections, and we check them all:

**Check A, geographic MapInfo (Geodetic, Geodetic360, OGR IsGeographic)**:
the longitude jump check from section 7. If a cell of the raw grid
contains a pole (the longitude change summed around the 4 corners of the
cell, each wrapped to [-180, 180], is +/-360 rather than 0), the message
says the swath covers the pole and to use a polar projection (e.g. polar
stereographic). Otherwise it says to switch Geodetic <-> Geodetic360.
A grid point exactly on the pole has an arbitrary longitude and breaks the
winding sum (a south pole swath with a point exactly at -90 gave 0 pole
cells), so also count any point with |lat| > 90 - 1e-6 as covering the
pole. With that, the test found the pole in all the north/south pole
swaths, and none in the 88N one (`/tmp/rsproto/wind.py`).
The winding test needs lat/lon, so it's done in create_from_lat_lon,
where we have them. For the other constructors (x/y only, which for a
geographic MapInfo are lon/lat) it's the same computation on X.

**Check B, any MapInfo, including projected**: a raw neighbor step more
than K = 50 times the median raw step of the swath means we cross a cut
or pole of the projection. This is on the map coordinates only, so it
works for every constructor and determine_range. The median is computed
from a fixed size strided sample (see Cost below) to keep it cheap. The message says
what it found and suggests a polar projection if the data is at high
latitude (we know this in create_from_lat_lon; for the other constructors
we can convert the two points back with the converter, only when we are
about to throw).

Message, e.g.:

    Resampler: the coordinates jump between line 150, sample 149 and
    line 150, sample 150 (step 4.2e6 m, 8400 times the typical step of
    500 m). The data crosses a discontinuity of the map projection
    (the cut at longitude 61.4 or a pole). Data near the poles needs a
    polar projection, such as polar stereographic.

Why K times the median and not a fixed distance: the swath step varies from
meters (subpixels) to many km (coarse grids). A ratio adapts to the data.
Duplicated scans (zero steps) don't matter, since we only flag large steps.
Missing points are skipped, as in section 7.

Limits:
* A swath that reaches into the pole region without a jump in the map
  coordinates (e.g. EASE 6933 at 88N) isn't caught. It's distorted, not
  wrong, so that's fine.
* K = 50 is from these tests; if some instrument has a legitimately
  large step range (e.g. very wide scan angles), it might false positive.
  Make K a static member `Resampler::max_step_ratio` with a documented
  default, so it can be changed (or set to 0 to turn off the check)
  without a code change.

Cost: check B applies to every MapInfo, so unlike section 7 it isn't
free for projected systems. Measured with a C++ prototype
(`/tmp/rsproto/checkb.cc`), timed as its own pass next to the fused
interpolate + index pass (pass 1 of the constructor). The subpixel count is
about 1e8 in every case:

| Raw grid      | nsub | Check B, 1 thread | Check B, 10 threads |
|---------------|------|-------------------|---------------------|
| 547x803 (OCI) | 15   | 0.003 s (0.2%)    | 0.002 s (1.2%)      |
| 5000x5000     | 2    | 0.061 s (3.6%)    | 0.010 s (5.5%)      |
| 10000x10000   | 1    | 0.233 s (14%)     | 0.032 s (17%)       |

Percentages are of the fused pass (1.65 s on 1 thread, 0.19 s on 10). The
constructor is about two such passes, so as a fraction of the constructor
it's about half that: ~0.5% for OCI, ~3% for nsub = 2, ~8% for nsub = 1.
As a fraction of a full run (constructor + several resample_field calls)
it's smaller again.

Implementation details that matter for the cost:
* Median from a fixed size sample (strided over the raw grid, about 64K
  pairs) with nth_element, not every 16th row. The every 16th row version
  was serial and took 0.25 s for 1e8 raw points, more than the fused
  pass itself at 10 threads.
* Compare squared distances against (K * median)^2, no hypot/sqrt.
* Scan the raw grid in parallel (row blocks, a count or first failure per
  task). For nsub = 1 it can be fused into pass 1 instead of a separate
  scan, since the raw and subpixel grids are the same there. That should
  remove most of the 8%, because the coordinates are already being read.

Behavior change: RELEASE_NOTES_2.0.md, like section 7.

**Unit tests (resampler_test.cc)**:

25. `pole_geodetic_throws`: swath over the north pole, Geodetic MapInfo,
    throws with the polar projection message (not the Geodetic360 one).
    Same for Geodetic360.
26. `pole_projected_throws`: the same swath with a polar crossing
    projection (Albers via OgrWrapper WKT, and EPSG:3857 Mercator) throws.
27. `pole_polar_stereographic_works`: same swath, EPSG:3413 polar
    stereographic MapInfo, no throw, and resampling a field of ones fills
    the covered cells.
28. `projection_cut_throws`: a swath at 35N straddling the Albers cut
    (lon_0 + 180) throws; the same swath in UTM doesn't.
29. `wrong_pole_throws`: a south pole swath with north polar
    stereographic throws; with south polar stereographic (EPSG:3031)
    it doesn't.
30. `no_false_positive_wide_scan`: a swath whose step grows 10x across
    the scan (like OCI) doesn't throw.
31. `max_step_ratio`: setting it to 0 turns check B off (test 28 doesn't
    throw); restore the default afterwards.

All EPSG tests need the proj database; skip them like other OGR
tests do if it's not available.

### 9. SWIG / python changes (all sections in one place)

resampler.i (keep in step with resampler.h):

* New `resample_field(const blitz::Array<double, 2>& Data, ...,
  int Number_task = -1)` overload next to the RasterImage one. SWIG
  dispatches on the first argument: a numpy array matches the blitz
  typemap, a RasterImage matches the shared_ptr typemap, a str matches the
  file name version. Check in a python test that all three dispatch
  correctly, in particular that a MemoryRasterImageFloat doesn't get picked
  up by the blitz typecheck and that a numpy float32 array is accepted
  (maia passes float64, but other callers may not).
* New `empty_resample(const blitz::Array<double, 2>& Data, double
  Fill_value_threshold)` overload, same as above.
* `Number_task = -1` argument added to the end of the existing
  constructors and resample_field, matching the C++ defaults, so existing
  positional calls don't change. Not for determine_range (see "Additional
  items": its OUTPUT arguments make this awkward).
* `static boost::shared_ptr<Resampler> create_from_lat_lon(...)` with
  `Boundary = 1` default.
* `%python_attribute` for swath_line_start, swath_number_line,
  swath_sample_start, swath_number_sample, has_swath_range.
* `%pythoncode` properties `line_range` / `sample_range` returning a
  python slice (stop None when the number is max int).
* max_step_ratio: expose as static `max_step_ratio()` /
  `set_max_step_ratio(double)` functions rather than a static data member;
  static members go through `cvar` in SWIG, which is awkward. (Or a
  `%pythoncode` class property calling them.)
* Fix the doc of the `Fname` resample_field overload along with the C++
  argument bug fix (section 1).
* `%pickle_serialization()` is already there; with GEOCAL_CLASS_VERSION 1
  pickles get the range fields, and old pickles load (python test below).

geocal_thread_pool.i: no interface change. The guards are internal, and
add_task isn't exposed to python anyway. The new exceptions are
GeoCal::Exception, so python sees them as RuntimeError through
swig_exception.i (`%exception`). Nothing needed for that.

Python typing: `geocal_swig/__init__.pyi` is generated by make, so new
methods show up automatically (untyped). No hand written resampler.pyi;
not worth it for one class. Python code in maia-misc-tools that uses the
new attributes needs nothing extra.

Exceptions in python: the date line / pole / size check / ThreadPool
errors are all GeoCal::Exception -> RuntimeError. GeoCal 2.0 prefers
more specific types where they make sense (e.g. TypeError); RuntimeError
is fine for these.

Python tests (new `python/tests/unit/resampler_test.py`, there is none
now; needs make install):

* `test_resample_field_dispatch`: numpy float64, numpy float32,
  MemoryRasterImage and MemoryRasterImageFloat all give the same result.
* `test_create_from_lat_lon`: factory works, ranges are ints,
  `line_range`/`sample_range` are slices that cut a numpy array, and an
  xarray `isel` gives the same cut.
* `test_default_range`: the existing constructor gives line_range that
  selects the whole array.
* `test_wrong_size_raises`: full swath passed to a resampler with a range
  raises RuntimeError with the expected range in the message.
* `test_date_line_raises`: RuntimeError from python, message mentions
  Geodetic360.
* `test_pickle`: round trip a small Resampler with a range; loading a
  version 0 pickle (made with the current code and checked into
  tests/unit/test_data, small) gives the default range.
* `test_number_thread`: ThreadPool.thread_pool().number_thread = 1 and 10
  give the same result; restore afterwards.

RELEASE_NOTES_2.0.md: new factory and overloads, swath range attributes,
the Fname resample_field argument fix, and the new exceptions (date line,
pole/projection cut, size check, ThreadPool misuse) as user visible
behavior changes.

## Threading mechanism: ThreadPool vs OpenMP

Prototype timings for init/determine_range with std::thread tasks and
`#pragma omp parallel for` were identical (within 3%) at 1, 4, 10 and 20
threads. For bandwidth bound loops the mechanism doesn't matter.

Points:

* OpenMP is shorter: `#pragma omp parallel for reduction(min:...)` covers
  the init and determine_range loops in a couple of lines. The scatter needs
  the private buffers + merge either way, so there isn't much difference
  there.
* OpenMP isn't actually enabled in the geocal build. geocal.am uses
  `$(OPENMP_CFLAGS)`, but configure.ac never calls AC_OPENMP, so the
  variable is empty, and there are no `#pragma omp` in lib (libgomp shows up
  in ldd only through libmuparser). Using OpenMP means adding AC_OPENMP and
  checking it works with the conda compilers and blitz thread safety. When
  it isn't available the pragmas silently do nothing, so code is serial
  rather than broken.
* Two thread controls: ThreadPool::number_thread() is already the user
  facing knob (SWIG wrapped, used by local_median and
  linear_gradient_bad_pixel_detection). OpenMP adds OMP_NUM_THREADS, a
  second knob that defaults to all cores (40 on rifle). That is bad for
  multiprocessing pools in python (40 processes x 40 threads).
* Fork safety: ThreadPool starts threads lazily and joins them in
  wait_tasks_finish, so there are no idle threads when python forks.
  libgomp keeps its worker threads alive after a parallel region, and a
  forked child that uses OpenMP can hang. This is a known GNU OpenMP
  problem and relevant since ECOSTRESS uses multiprocessing pools.
* Exceptions: today ThreadPool tasks that throw call std::terminate, the
  same as OpenMP. The guard below fixes this for ThreadPool (exceptions are
  rethrown in the caller), which OpenMP can't easily do.

Recommendation: use ThreadPool, same pattern as local_median, with a
Number_task argument (default -1 meaning about 4 x number_thread()). It's
consistent with the existing code, has one thread knob, and doesn't hit the
fork problem. Factor out a small helper (e.g. in geocal_thread_pool.h)

    template<class F> void parallel_row_blocks(int Nrow, int Number_task, F f);

that splits [0, Nrow) into Number_task blocks, calls f(task_index, start,
end), runs serially when number_thread()==1 or Number_task==1, and waits.
local_median can then be switched to it as well.

### Guarding against misuse of the global ThreadPool

ThreadPool is a single global pool, and wait_tasks_finish waits on *all*
tasks. How the current code behaves:

* A task throws: boost::asio calls std::terminate and the process dumps
  core (tested). This is the real crash today.
* Nested use (a task calls add_task/wait_tasks_finish): the inner
  wait_tasks_finish takes and joins the shared pool from one of its own
  worker threads. It happened to finish in a test, but depending on timing
  it can hang or crash.
* Two threads using the pool at once: each wait_tasks_finish can join the
  other's tasks, or return before its own tasks are done.

From python the third case doesn't happen: the SWIG wrappers are built
without -threads, so the GIL is held for every C++ call. C++ callers can
still hit it.

We don't need to support any of these, just give a clean Exception. Changes
to ThreadPool (prototyped in /tmp/rsproto/guard.cc with the same
boost::asio pool; all three cases gave the expected exceptions and the pool
worked normally afterwards):

1. Exceptions in tasks: the lambda in add_task wraps the call in
   try/catch(...) and saves the first std::exception_ptr (under the existing
   mutex). wait_tasks_finish rethrows it after the join, so the caller gets
   the original exception.
2. Nested use: a `thread_local bool in_task` is set while a task runs.
   add_task, wait_tasks_finish and number_thread(V) throw
   `Exception("ThreadPool used from inside a ThreadPool task, which isn't supported")`
   when it is set. The exception is caught by (1) in the outer task and
   rethrown from the outer wait_tasks_finish, so it reaches the original
   caller.
3. Concurrent use: the first add_task records `std::this_thread::get_id()`
   as the owner. add_task/wait_tasks_finish/number_thread from any other
   thread throws `Exception("ThreadPool is already being used by another thread")`.
   wait_tasks_finish clears the owner.

Remaining gap: if a caller throws between add_task and wait_tasks_finish
(e.g. partway through its task loop), the owner is never cleared and the
pool stays locked to that thread. `parallel_row_blocks` handles this by
always calling wait_tasks_finish, using a scope guard (and swallowing a
second exception from the wait if one is already propagating). Make
parallel_row_blocks the normal way to use the pool; switch local_median and
linear_gradient_bad_pixel_detection to it.

Cost: one mutex lock per add_task, which is nothing next to a task
of ~1M pixels.

## Overall expected improvement (10 threads)

| Operation                    | Now     | Expected |
|------------------------------|---------|----------|
| Resampler(blitz)             | 1.39 s  | ~0.1 s   |
| Resampler(RasterImage)       | 2.57 s  | ~0.2 s   |
| determine_range              | 1.18 s  | ~0.12 s  |
| resample_field(RasterImage)  | 1.17 s  | ~0.25 s  |
| maia build_resampler (OCI)   | 22.8 s  | ~0.9 s (new lat/lon constructor) |

The date line / pole checks add ~0.5% (OCI) to ~8% (nsub = 1) to the
constructors; see section 8.

A typical tiled run (determine_range + ctor + several resample_field calls)
goes from a few seconds per tile to a few tenths. Memory also drops by about
2.4 GB per call (no MagnifyReplicate / MagnifyBilinear / xindex+yindex
temporaries). Numbers are from synthetic data and prototypes, not the final
code.

## Unit tests (resampler_test.cc)

Fixture: ~200x300 raw grid, nsub=2, rotated lon/lat with a few fill
(-9999) points, GeodeticConverter MapInfo. A reference implementation of
the current serial algorithm (with explicit replication) lives in the test
file.

1. `array_matches_reference`: new blitz overload == reference (1e-12).
2. `raster_matches_array`: MemoryRasterImage overload == blitz overload.
3. `thread_consistency`: number_thread 1/4/10 and Number_task 1/7/100 give
   identical results; restore number_thread afterwards.
4. `use_smallest_ic`: exact match to reference for several task counts.
5. `fill_values`: cells hit only by fill data get Fill_value; mixed cells
   average only the valid data.
6. `scale_and_negative_to_zero`.
7. `size_check`: data that is too small throws.
8. `exactly_match_mi`: points outside the MapInfo are ignored.
9. `ctor_matches_reference`: data_index/map_info from the new init match
   a reference copy of the old init (blitz and RasterImage constructors;
   RasterImage version against MagnifyBilinear), for several thread counts.
10. `determine_range_matches_reference`: same, plus the edge cases (no
    overlap gives lstart > lend, full overlap).
11. `timing`: large case serial vs parallel, returns early unless enabled
    by hand.
12. `lat_lon_constructor`: synthetic rotated lat/lon swath much larger than
    a GeodeticConverter MapInfo. Compare data_index, map_info and the
    swath range against a reference built the old way in the test (convert,
    coarse determine_range, explicit center aligned extrapolating bilinear
    interpolation, fine determine_range, blitz constructor with
    Exactly_match_mi), with the range shift done correctly. Same result for
    number_thread 1/4/10.
13. `lat_lon_resample_consistency`: resample the swath latitude through the
    new resampler and check it reproduces the grid latitude to a fraction of
    a pixel (this is the check that found the build_resampler shift).
14. `swath_range_defaults`: the existing constructors give 0 / max int and
    has_swath_range() is false.
15. `swath_range_size_check`: with a range, passing the full swath or a
    wrong size throws; the subset size works.
16. `lat_lon_edge_cases`: swath entirely outside Mi throws; NaN lat/lon
    points never produce data; swath touching the edge of the swath array
    (clamping of Boundary).
17. `serialization`: a small Resampler with a range round trips through
    serialize_write_string/serialize_read_string, and a version 0 archive
    loads with the default range.

ThreadPool tests (geocal_thread_pool_test.cc):

* `task_exception`: a task throws, wait_tasks_finish rethrows it, and the
  pool still works afterwards.
* `nested_use`: a task calls add_task, and the outer wait_tasks_finish
  throws the nested use error.
* `concurrent_use`: a second std::thread calls add_task while the first has
  tasks pending, and gets the error.
* `helper_cleanup`: parallel_row_blocks with a function that throws leaves
  the pool usable from another thread.

## Additional items from the review

* **Missing value convention.** The existing code marks missing
  coordinates with `Mark_missing` (<= -1000), maia uses NaN for missing
  data values, and the new factory treats NaN lat/lon as missing. Make all
  the passes treat both NaN and <= Mark_missing as missing coordinates
  (NaN fails every comparison, so today a NaN coordinate goes through
  lrint as INT_MIN and is silently dropped). The factory has no
  Mark_missing argument; it uses NaN and the same default (-1000).
* **Exactly_match_mi and the coverage MapInfo for the factory.** The
  factory always matches Mi exactly (what maia needs). If coverage based
  MapInfo is ever wanted from lat/lon, it can be added later; not in scope.
* **Number_task default.** -1 means 4 x number_thread(). Small problems
  (less than ~1M subpixels) run serially regardless, so the unit tests'
  small fixtures must pass an explicit Number_task (or lower the cutoff
  through a test hook) to actually exercise the parallel path. The cutoff
  is a static constant, `Resampler::min_parallel_size`.
* **Number_task in SWIG for determine_range.** It's a static with
  int& OUTPUT arguments; Number_task has to go before the OUTPUT ones in the
  C++ signature for the SWIG typemap ordering to work, which changes the C++
  argument order. Alternative: leave determine_range without Number_task
  (always default). Recommend the latter; it's simpler and nobody needs to
  tune it.
* **Memory of the scatter buffers.** Step A of resample_field allocates
  about 0.3x the output size in private buffers (measured for a rotated
  swath). A swath with very non-compact tiles (e.g. a strongly curved swath)
  can have bounding boxes much bigger than the touched area. Cap the
  per tile buffer at the output size and fall back to the serial scatter
  if the total would exceed 2x the output size. Log nothing, just do it.
* **Thread safety of MapInfo / CoordinateConverter in the passes.** The
  parallel passes only use the inline affine coordinate_to_index (pure
  arithmetic on param), never the converter. The converter is only used
  in the serial step 1 of the factory and when building an exception
  message. Note this in a comment so nobody "simplifies" it to call
  mi.coordinate() inside a task.
* **Existing callers to rerun.** ECOSTRESS (ecostress-level1) has its own
  copy of the class today, so it isn't affected until it switches to the
  geocal one; when it does, its tests are the real check. OCO-3 and other geocal users: grep for Resampler
  before the release. The || vs && fill change in section 2 is the one
  thing that could change ECOSTRESS output, so make it a separate,
  clearly labeled step.
* **Documentation.** Doxygen comments on all new functions (they become
  python help()), the class comment updated with: the swath range and
  subsetting, the date line / pole behavior and what MapInfo to use, the
  threading (uses the global ThreadPool, not reentrant), and the
  MagnifyBilinear/MagnifyReplicate half pixel note.

## Decisions (2026-11-03)

1. **Fill test in init: use `||`.** A point is missing if either X or Y
   is fill, in both the min/max and the marking. If this changes ECOSTRESS
   output, ECOSTRESS was wrong. (In practice it shouldn't change anything:
   both ECOSTRESS callers set x and y to -1e20 together whenever lat or lon
   is fill, see below.)
2. **No valid points in init: throw.** Verified against ecostress-level1
   (27fabc7, 2026-10-29). ECOSTRESS currently uses its own copy,
   `Ecostress::Resampler` (lib/ecostress_resampler.cc, init identical to
   geocal's), but its callers are the ones that would move to the geocal
   class:
   * `L2ctGenerate.process_tile` (l2ct_generate.py:534): calls
     `determine_range` first and returns "Tile is empty, skipping" when
     `lend < lstart or send < sstart`, before building the Resampler.
     It then builds with Exactly_match_mi=True on the trimmed range, so
     there are always valid points. The later `empty_resample(data_in)`
     check is for tiles whose data is all fill, which is unaffected.
   * `L1cgGenerate.run` (l1cg_generate.py:238): one Resampler for the
     whole scene with a coverage MapInfo. Only empty if every lat/lon in the
     scene is fill, which isn't a scene we'd process; throwing is right
     there too.
   So no caller relies on the silent 1x1 MapInfo. The exception message
   says no valid coordinates were found (and, with Exactly_match_mi, none
   fall in the MapInfo is *not* an error; empty_resample() handles that).
   Note: only the "no valid (non fill) coordinates at all" case throws.
3. **Exactly_match_mi: mark fill points -9999 too.**
4. **Check B ratio: K = 50 default, configurable** through
   `Resampler::max_step_ratio()` / `set_max_step_ratio()` (0 turns check
   B off). No further work on choosing K.
5. **maia-misc-tools: out of scope.** It will switch to the C++ factory
   separately. The build_resampler range bug is documented above for
   reference only.

Related ECOSTRESS notes found while checking:
* ECOSTRESS skips scenes that cross the date line (l1b_geo_process.py:437,
  l1b_proj.py:107), so the new date line check won't fire in normal
  ECOSTRESS processing. L1CG uses the cib01 MapInfo, which is
  GeodeticConverter, so a date line scene that got through would now throw
  instead of producing a 36000+ pixel wide MapInfo.
* Both callers mark fill coordinates as -1e20 before interpolating
  (scipy zoom order 1, or utm_coor). Bilinear interpolation between -1e20
  and a valid value gives at least ~1e20/nsub in magnitude (still huge and
  negative; with Fill_value_threshold set, e.g. -1e19, they are missing), and
  the checks skip them.
* L2CT uses UTM tiles, so only check B applies there.

## Working notes for the implementation

* **Running the C++ tests before `make install`.** In the pixi build,
  `geocal_test_all` loads the installed `libgeocal` from the env, not the
  one in `.libs` (the env lib directory comes first in its RPATH, see
  `cpp_modernization_and_library_plan.md` §6.1). So `make check` and
  `./geocal_test_all` test the old library, or fail with
  `undefined symbol` for anything new. Run the tests from the build
  directory (`/ldata/smyth/geocal-build/build-pixi`) as

      make -j 20 geocal_test_all
      pixi run env LD_PRELOAD=$PWD/.libs/libgeocal.so.0 ./geocal_test_all --run_test=resampler

  and ignore the `sh: /lib64/libtinfo.so.6: no version information
  available` lines (the preload leaks into subshells). After `make install`
  the plain command works again, until the next change.
* The build directory uses this source tree (`../../geocal-repo` resolves to
  `/ssdata/smyth/geocal-repo`), configured with
  `--enable-maintainer-mode --enable-python-editable-mode`.

## Progress

* **Step 1 done (2026-11-03), staged:** ThreadPool guards,
  `parallel_row_blocks`, `array_local_median` switched to it, 5 new
  ThreadPool tests, release note. Full C++ suite passes (583 tests, with the
  `LD_PRELOAD` workaround); the ThreadPool suite passed 200 repeated runs.
  Python tests not run yet (need `make install`), do that after step 2.
  Small addition to the plan: `number_thread(V)` also requires V >= 1.

* **Step 2 done (2026-11-03), staged:** `resample_field` and
  `empty_resample` blitz overloads, parallel tiled scatter + merge,
  `Number_task` argument, `min_parallel_size` (1e6 subpixels), Fname
  argument fix, SWIG updated and wrappers regenerated. Test results:
  * 10 new resampler tests, all against a reference copy of the old
    algorithm in the test file. Checked that they catch bugs: two
    deliberate errors in the merge gave 21 failures.
  * Full C++ suite, `make install`, Python tests (250 passed, 77
    skipped), and `make mypy` all pass.
  * Timing test (5000x5000 raw, nsub 2, 10 threads): 0.97 s single
    task, 0.19 s default (5.2x). Run it with
    `GEOCAL_RESAMPLER_TIMING=1 ./geocal_test_all --run_test=resampler/timing`.
  Found and fixed along the way:
  * resampler_test.cc was never in implementation.am, so the existing
    (disabled) test was never even compiled. Added.
    `orbit_array_test.cc` and `rsm_rational_polynomial_test.cc` are also not
    in the build; not looked at, left alone.
  * An anonymous namespace in resampler.cc makes doxy2swig produce a
    `%feature` line that SWIG can't parse. Helper types go in the class
    (private) instead.
  * `data_index` renamed `data_index_` (serialized name unchanged, using
    GEOCAL_NVP_), with a const `data_index()` accessor (C++ only), used
    by the test reference implementation.
  * The `else` fill branch was dropped as planned. The reference
    implementation in the test keeps it, and the results match, which
    confirms it had no effect.
* Python dispatch checked by hand (`/tmp/rsproto/pycheck.py`): numpy
  float64, numpy float32, MemoryRasterImage and MemoryRasterImageFloat all
  go to the right overload and give the same result. The proper python
  test file comes in step 6.
* Note for later steps: `cp` is aliased to `cp -i` on rifle; use
  `/bin/cp -f` in scripts.

* **Step 3a done (2026-11-03), staged:** speed part of step 3, no
  behavior change.
  * `init` is a template taking a coordinate function, used by both
    constructors. Two parallel passes: (1) index + rint + per task
    min/max, marking both-fill points with INT_MIN in data_index; (2)
    subtract the offset, fill points -> -9999. The fill marking in pass 1
    means pass 2 doesn't recompute the coordinates (this was 1.46 s ->
    1.02 s single threaded for the array constructor).
  * RasterImage constructor interpolates on the fly, exactly matching
    MagnifyBilinear (same formula and order of operations; the test checks
    the data_index is identical for nsub 1, 2, 3). No 2 x 800 MB
    interpolated arrays.
  * determine_range: one parallel pass, per task ranges reduced at the
    end.
  * `default_number_task` shared by resample_field, init and
    determine_range. determine_range has no Number_task argument (as
    decided).
  * Constructors have a `Number_task = -1` last argument (C++ and SWIG).
  * Tests: `ctor_matches_reference` (1/4/10 threads, exact/coverage,
    with/without fill, 4 task counts: data_index and MapInfo identical
    to a copy of the old init), `raster_ctor_matches_magnify_bilinear`,
    `determine_range_matches_reference` (partial, full, no overlap;
    nsub 1, 2, 3). 14 resampler tests, full C++ suite, Python tests (250
    passed, 77 skipped) and mypy pass.
  * Timing (5000x5000 raw, nsub 2, 10 threads): array constructor 1.02 s
    -> 0.12 s, determine_range 1.11 s -> 0.13 s, RasterImage constructor
    (python, 5000x5000 coordinates) 2.39 s -> 0.36 s.
  * One small side effect: a NaN coordinate (both x and y) with a coverage
    MapInfo now gets -9999 instead of an overflowed INT_MIN - miny. It was
    never used either way.
* **Step 3b done (2026-11-03), staged:** the decided init behavior
  fixes. A point is missing if either X or Y is <= Mark_missing or NaN
  (`!(x > Mark_missing && y > Mark_missing)`, which is false for NaN);
  missing points are -9999 with Exactly_match_mi too, and don't get
  coordinate_to_index at all; all points missing throws "no valid
  coordinates". Release note under API breaking changes.
  * The test reference init has the same fixes (commented as the 2.0
    fixes); `coor_with_fill` adds NaN points and an only-X NaN block.
  * New tests `missing_points` (every missing point is -9999, missing raw
    pixels set to 1e10 never show up in the result) and `no_valid_points`
    (fill and NaN, both MapInfo modes; valid but outside Mi with
    Exactly_match_mi is fine).
  * Checked the tests catch the old behavior: putting back `&&`-style
    marking and no throw gave 990 failures in 3 tests.
  * 16 resampler tests, full C++ suite, Python (250 passed, 77 skipped),
    mypy pass.
* **Step 3b revised (2026-11-03, committed as "Fix for fill value data,
  now treat as nan"):** `Mark_missing` (default -1000) replaced by
  `Fill_value_threshold`, default `numeric_limits<double>::lowest()` (no
  threshold). The -1000 default broke projected coordinates below -1000
  (the SW quadrant of an Albers grid). NaN is always missing; with a
  threshold, a point is missing if `!(x > T && y > T)`. The RasterImage
  constructor turns threshold points into NaN *before* interpolating
  (zero weight terms skipped, so a NaN only contaminates its neighbors).
  The array constructor's callers should put NaN in before their own
  bilinear zoom. Supersedes decision 1's `Mark_missing` wording; the `||`
  logic is unchanged. Checked usage: ECOSTRESS uses -1e20 (its own copy of
  Resampler), `l1b_proj.py` zooms with -9999 still in place (an existing
  bug NaN fixes), maia passes only valid coordinates, oco3 doesn't use
  Resampler. Test `raster_ctor_fill` covers fill with and without the
  threshold; skipping the zero weight terms is checked (removing it gave 69
  failures).
* **Step 4 done (2026-11-03), staged:** date line, pole and projection
  cut checks (A and B), as a `ResamplerCoordinateCheck` class in
  resampler.cc (named, not an anonymous namespace, because of doxy2swig).
  * Type from the MapInfo's converter: GeodeticConverter,
    Geodetic360Converter, or an OGR geographic system with traditional
    GIS axis order are geographic; everything else is projected. An OGR
    geographic system with authority (lat first) order is skipped.
  * Geographic: throw on a longitude jump > 180 deg between raw points
    (`nsub` apart in the subpixel grid), with a Geodetic360 / Geodetic
    message. Pole: winding sum of longitude steps around each raw cell,
    or |lat| > 90 - 1e-6; message suggests polar stereographic.
  * Projected: median raw step from a ~32768 point sample
    (nth_element), throw on a step > `max_step_ratio()` (default 50) x
    the median. `set_max_step_ratio(0)` turns it off.
  * Missing points are skipped. Each task checks its own rows
    (`check_rows`: branchless scan, then rescan only if something was found
    to locate it), and the exception is thrown after the parallel pass
    from the first task with a problem, so the result doesn't depend on
    the number of tasks. Every message also mentions unmarked fill values.
  * Used in init (both constructors) and determine_range.
    determine_range got a trailing `Fill_value_threshold` argument (C++
    and SWIG), and skips the threshold test when there isn't one, and does
    the MapInfo range check in double (same result as the std::round
    check, and NaN fails it).
  * Tests that used GeodeticConverter as a stand in for a projected
    MapInfo now use `OgrCoordinateConverter::utm_converter(11)`; with
    Geodetic they cross the date line. `raster_ctor_fill` without a
    threshold now expects the exception.
  * 13 new tests (date_line_geodetic_throws,
    date_line_interpolated_throws, date_line_geodetic360_works,
    near_date_line_no_throw, prime_meridian_geodetic360_throws,
    date_line_ogr_geographic_throws, date_line_missing_points,
    pole_geodetic_throws, pole_projected_throws (Albers and EPSG:3857),
    pole_polar_stereographic_works (EPSG:3413), projection_cut_throws,
    wrong_pole_throws (EPSG:3031 data on a 3413 grid),
    no_false_positive_wide_scan). Disabling the checks gave 21 failures.
    31 resampler tests, full C++ suite, Python (250 passed, 77 skipped),
    mypy pass.
  * Cost (array constructor, 10000x10000 subpixels, 10 threads): nsub 1
    0.128 -> 0.197 s (+54%, more than the ~8% estimated, since every point
    is a raw point and the check is a second read of the coordinates),
    nsub 2 0.128 -> 0.147 s, nsub 15 0.127 -> 0.133 s. Folding the check
    into the main pass (row cache, per task state) was tried and was
    slower. A raw row cache could cut the nsub 1 cost if it matters.
    determine_range: 1.11 s -> 0.76 s single threaded and 0.088 s with 10
    threads, including the check (the has_threshold and double range check
    changes more than pay for it).

## Implementation order

Each step builds and passes `make check` (and installcheck where python
is involved) before the next. Stop after each step for review if wanted.

1. ThreadPool guards + parallel_row_blocks helper + tests; switch
   local_median / linear_gradient_bad_pixel_detection to the helper.
2. resample_field: blitz overload, no MagnifyReplicate, parallel scatter,
   Fname argument bug fix, empty_resample overload. Reference tests
   written first, against the current code, so they pin the old behavior.
3. init / determine_range / RasterImage constructor: fused parallel
   passes, reference tests first. Then, as a separate clearly labeled
   change, the decided init fixes (|| fill test, -9999 marking with
   Exactly_match_mi, throw on no valid points).
4. Date line and pole checks (A and B) + tests.
5. create_from_lat_lon, swath range attributes, size check,
   serialization version 1 + tests.
6. SWIG and python tests, RELEASE_NOTES_2.0.md, documentation.
7. Benchmarks on rifle with the final code, update the tables here.
