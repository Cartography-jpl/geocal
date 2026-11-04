import pickle

import numpy as np
import pytest
import scipy.interpolate

from geocal import (
    Geodetic360Converter,
    GeodeticConverter,
    MapInfo,
    MemoryRasterImageFloat,
    OgrCoordinateConverter,
    Resampler,
)

# Most of the testing is done in the C++ unit tests (resampler_test.cc).
# This checks the python interface: which overloads get called, the
# swath range slices, pickling and exceptions.


@pytest.fixture
def utm_swath() -> tuple[MapInfo, np.ndarray, np.ndarray]:
    """A UTM MapInfo of 200 x 200 pixels of 100 m, and a rotated
    300 x 300 swath at 250 m that extends well past it."""
    conv = OgrCoordinateConverter.utm_converter(11)
    mi = MapInfo(conv, 380000.0, 3800000.0, 400000.0, 3780000.0, 200, 200)
    n = 300
    th = np.radians(20)
    jj, ii = np.meshgrid(np.arange(n), np.arange(n))
    u = (jj - n / 2 + 0.3) * 250
    v = (ii - n / 2 + 0.1) * 250
    x = 390000 + u * np.cos(th) - v * np.sin(th)
    y = 3790000 - u * np.sin(th) - v * np.cos(th)
    lat = np.empty((n, n))
    lon = np.empty((n, n))
    for i in range(n):
        for j in range(n):
            gc = conv.convert_from_coordinate(x[i, j], y[i, j])
            lat[i, j] = gc.latitude
            lon[i, j] = gc.longitude
    return mi, lat, lon


@pytest.fixture
def geodetic_swath() -> tuple[MapInfo, np.ndarray, np.ndarray]:
    """A geodetic MapInfo, and 100 x 120 coordinates that cover it, already
    interpolated to 2 subpixels."""
    mi = MapInfo(GeodeticConverter(), 10.0, 11.0, 11.0, 10.0, 200, 200)
    nsub = 2
    jj, ii = np.meshgrid(np.arange(120 * nsub), np.arange(100 * nsub))
    x = 10.05 + (jj + 0.5) / nsub * 0.0075
    y = 10.95 - (ii + 0.5) / nsub * 0.0075
    return mi, x, y


def swath_data(nl: int, ns: int) -> np.ndarray:
    ii, jj = np.meshgrid(np.arange(nl), np.arange(ns), indexing="ij")
    return ((ii * 37 + jj * 101) % 997 + 0.25).astype(float)


def test_array_and_raster_image(geodetic_swath) -> None:
    """resample_field and empty_resample take a numpy array (float64 or
    float32) or a RasterImage, and give the same result."""
    mi, x, y = geodetic_swath
    r = Resampler(x, y, mi, 2)
    assert not r.has_swath_range
    d = swath_data(x.shape[0] // 2, x.shape[1] // 2)
    res = r.resample_field(d, 1.0, False, -9999.0)
    # Without Exactly_match_mi, the output MapInfo covers the data
    assert res.shape == (r.map_info.number_y_pixel, r.map_info.number_x_pixel)
    assert (res > -9999).sum() > 0.5 * res.size
    res32 = r.resample_field(d.astype(np.float32), 1.0, False, -9999.0)
    np.testing.assert_allclose(res32, res, rtol=1e-6)
    img = MemoryRasterImageFloat(d.shape[0], d.shape[1])
    img.write(0, 0, d)
    np.testing.assert_allclose(r.resample_field(img, 1.0, False, -9999.0), res)
    assert not r.empty_resample(d)
    assert not r.empty_resample(img)
    assert r.empty_resample(np.full(d.shape, -9999.0), -999.0)
    # Number_task shouldn't change the result, apart from round off.
    np.testing.assert_allclose(
        r.resample_field(d, 1.0, False, -9999.0, False, -1e300, 7), res, rtol=1e-12
    )


def test_missing_and_threshold(geodetic_swath) -> None:
    """NaN coordinates are always missing, and a threshold marks fill."""
    mi, x, y = geodetic_swath
    d = swath_data(x.shape[0] // 2, x.shape[1] // 2)
    x2 = x.copy()
    x2[:20, :] = np.nan
    r = Resampler(x2, y, mi, 2, True)
    assert (r.data_index[:20, :, :] == -9999).all()
    x2[:20, :] = -9999.0
    r2 = Resampler(x2, y, mi, 2, True, -999.0)
    assert (r2.data_index == r.data_index).all()
    np.testing.assert_allclose(r2.resample_field(d), r.resample_field(d))
    with pytest.raises(RuntimeError, match="no valid coordinates"):
        Resampler(np.full(x.shape, np.nan), y, mi, 2)


def test_determine_range(geodetic_swath) -> None:
    mi, x, y = geodetic_swath
    ls, le, ss, se = Resampler.determine_range(x, y, mi, 2)
    # le and se are rounded up to a multiple of the number of subpixels,
    # so they can be the size of the array.
    assert 0 <= ls <= le <= x.shape[0]
    assert 0 <= ss <= se <= x.shape[1]
    x2 = x.copy()
    x2[:, :40] = -9999.0
    _, _, ss2, _ = Resampler.determine_range(x2, y, mi, 2, -999.0)
    assert ss2 >= 40


def test_date_line() -> None:
    """Crossing the date line with a Geodetic MapInfo is an error, and
    works with Geodetic360."""
    jj, ii = np.meshgrid(np.arange(200), np.arange(200))
    lon360 = 179.005 + jj * 0.01
    lon = np.where(lon360 >= 180, lon360 - 360, lon360)
    lat = 10.005 + ii * 0.01
    mi = MapInfo(GeodeticConverter(), 178.0, 13.0, 180.0, 9.0, 200, 400)
    with pytest.raises(RuntimeError, match="Use a Geodetic360 MapInfo"):
        Resampler(lon, lat, mi, 1)
    mi360 = MapInfo(Geodetic360Converter(), 179.0, 12.0, 181.0, 10.0, 200, 200)
    r = Resampler(lon360, lat, mi360, 1, True)
    assert (r.resample_field(np.ones(lat.shape), 1.0, False, -9999.0) == 1).all()


def test_max_step_ratio(utm_swath) -> None:
    """A projected MapInfo throws for a large jump in the coordinates
    (here a fill value that isn't marked missing). Setting max_step_ratio
    to 0 turns the check off."""
    mi, lat, lon = utm_swath
    w = mi.coordinate_converter.ogr
    xy = w.lat_lon_coordinate_convert(lat.ravel(), lon.ravel())
    x = xy[:, 0].reshape(lat.shape).copy()
    y = xy[:, 1].reshape(lat.shape).copy()
    x[150, 150] = -9999.0
    y[150, 150] = -9999.0
    with pytest.raises(RuntimeError, match="set_max_step_ratio"):
        Resampler(x, y, mi, 1)
    sv = Resampler.max_step_ratio()
    assert sv == 50
    try:
        Resampler.set_max_step_ratio(0)
        assert Resampler.max_step_ratio() == 0
        Resampler(x, y, mi, 1)
    finally:
        Resampler.set_max_step_ratio(sv)
    # Marking the fill as missing is the right fix.
    Resampler(x, y, mi, 1, False, -1000.0)


def test_create_from_lat_lon(utm_swath) -> None:
    mi, lat, lon = utm_swath
    r = Resampler.create_from_lat_lon(lat, lon, mi, 3)
    assert r.has_swath_range
    assert r.number_sub_pixel == 3
    assert r.line_range == slice(
        r.swath_line_start, r.swath_line_start + r.swath_number_line
    )
    assert r.sample_range == slice(
        r.swath_sample_start, r.swath_sample_start + r.swath_number_sample
    )
    # Only uses part of the swath
    assert r.swath_number_line < lat.shape[0] // 2
    assert r.swath_number_sample < lat.shape[1] // 2
    d = swath_data(*lat.shape)
    res = r.resample_field(d[r.line_range, r.sample_range], 1.0, False, -9999.0)
    assert (res > -9999).sum() > 0.9 * res.size
    assert not r.empty_resample(d[r.line_range, r.sample_range])
    # The full swath (or anything else that isn't the subset) is an error
    with pytest.raises(RuntimeError, match="line_range and sample_range"):
        r.resample_field(d)
    with pytest.raises(RuntimeError, match="line_range and sample_range"):
        r.empty_resample(d)
    # The resampled latitude should match the grid latitude, with no bias.
    rlat = r.resample_field(lat[r.line_range, r.sample_range], 1.0, False, -9999.0)
    err = []
    for i in range(0, rlat.shape[0], 7):
        for j in range(0, rlat.shape[1], 7):
            if rlat[i, j] > -9999:
                err.append(rlat[i, j] - mi.ground_coordinate(j, i).latitude)
    assert abs(np.mean(err) * 111e3) < 10
    with pytest.raises(RuntimeError, match="none of the swath falls in the MapInfo"):
        far = MapInfo(
            mi.coordinate_converter, 480000.0, 3800000.0, 500000.0, 3780000.0, 200, 200
        )
        Resampler.create_from_lat_lon(lat, lon, far, 3)


def test_create_from_lat_lon_matches_scipy(utm_swath) -> None:
    """create_from_lat_lon should give the same result as building the
    Resampler in python with scipy, the way maia-misc-tools
    build_resampler does it (with the swath range bug in that code
    fixed)."""
    mi, lat, lon = utm_swath
    nsub = 5
    r = Resampler.create_from_lat_lon(lat, lon, mi, nsub)
    w = mi.coordinate_converter.ogr
    xy = w.lat_lon_coordinate_convert(lat.ravel(), lon.ravel())
    x = xy[:, 0].reshape(lat.shape)
    y = xy[:, 1].reshape(lat.shape)
    ls_f, le_f, ss_f, se_f = Resampler.determine_range(x, y, mi, 1)
    lr_f = slice(max(ls_f - 1, 0), min(le_f + 1, lat.shape[0] - 1) + 1)
    sr_f = slice(max(ss_f - 1, 0), min(se_f + 1, lat.shape[1] - 1) + 1)
    h = lr_f.stop - lr_f.start
    wd = sr_f.stop - sr_f.start
    yn = np.linspace(-0.5 + 1 / (2 * nsub), h - 0.5 - 1 / (2 * nsub), h * nsub)
    xn = np.linspace(-0.5 + 1 / (2 * nsub), wd - 0.5 - 1 / (2 * nsub), wd * nsub)
    xg, yg = np.meshgrid(xn, yn)
    pts = np.stack([yg.ravel(), xg.ravel()], axis=-1)

    def interp(a: np.ndarray) -> np.ndarray:
        f = scipy.interpolate.RegularGridInterpolator(
            (np.arange(h), np.arange(wd)),
            a[lr_f, sr_f],
            method="linear",
            bounds_error=False,
            fill_value=None,
        )
        return f(pts).reshape(h * nsub, wd * nsub)

    x_ex = interp(x)
    y_ex = interp(y)
    ls, le, ss, se = Resampler.determine_range(x_ex, y_ex, mi, nsub)
    rpy = Resampler(
        x_ex[ls : le + nsub, ss : se + nsub],
        y_ex[ls : le + nsub, ss : se + nsub],
        mi,
        nsub,
        True,
    )
    lr = slice(lr_f.start + ls // nsub, lr_f.start + (le + nsub) // nsub)
    sr = slice(sr_f.start + ss // nsub, sr_f.start + (se + nsub) // nsub)
    assert r.line_range.start == lr.start
    assert r.sample_range.start == sr.start
    d = swath_data(*lat.shape)
    res = r.resample_field(d[r.line_range, r.sample_range], 1.0, False, -9999.0)
    respy = rpy.resample_field(d[lr, sr], 1.0, False, -9999.0)
    # Floating point round off can very occasionally move a subpixel to
    # the neighboring output pixel.
    assert (np.abs(res - respy) > 1e-8).sum() <= res.size / 2000


def test_full_swath_slices(geodetic_swath) -> None:
    """Without a swath range, line_range and sample_range select all the
    data, so code can always subset with them."""
    mi, x, y = geodetic_swath
    r = Resampler(x, y, mi, 2)
    d = swath_data(x.shape[0] // 2, x.shape[1] // 2)
    assert d[r.line_range, r.sample_range].shape == d.shape
    assert r.swath_line_start == 0
    assert r.swath_number_line == 2**31 - 1
    np.testing.assert_allclose(
        r.resample_field(d[r.line_range, r.sample_range]), r.resample_field(d)
    )


def test_pickle(utm_swath) -> None:
    mi, lat, lon = utm_swath
    r = Resampler.create_from_lat_lon(lat, lon, mi, 2)
    r2 = pickle.loads(pickle.dumps(r))
    assert r2.line_range == r.line_range
    assert r2.sample_range == r.sample_range
    assert (r2.data_index == r.data_index).all()
    d = swath_data(*lat.shape)[r.line_range, r.sample_range]
    np.testing.assert_allclose(r2.resample_field(d), r.resample_field(d))
