import pickle

import numpy.testing as npt
import pytest
from geocal_swig.auto_derivative import vector_auto_derivative

from geocal import (
    AutoDerivativeDouble,
    GeodeticConverter,
    ImageCoordinate,
    InterestPoint,
    MapInfo,
    MeasuredTimeTable,
    PiecewiseLinear,
    Time,
    Vector_ImageCoordinate,
    Vector_InterestPoint,
    Vector_Time,
)


def test_convert_vector():
    """Historically passing a sequence to something expected at std::vector
    didn't work correctly. We have a fix for this now in common.i, but leave
    a test in place in case we go to a new version of SWIG that breaks this.
    """
    ulc_x = 50
    ulc_y = 60
    x_pixel_res = 0.25
    y_pixel_res = -0.50
    number_x_pixel = 100
    number_y_pixel = 200
    mi = MapInfo(
        GeodeticConverter(),
        ulc_x,
        ulc_y,
        ulc_x + x_pixel_res * number_x_pixel,
        ulc_y + y_pixel_res * number_y_pixel,
        number_x_pixel,
        number_y_pixel,
    )
    gp = [mi.ground_coordinate(10.1, 20.1), mi.ground_coordinate(10 + 28.9, 20 + 38.9)]
    sub = mi.cover(gp)
    npt.assert_almost_equal(sub.lrc_x, ulc_x + x_pixel_res * 40)
    npt.assert_almost_equal(sub.lrc_y, ulc_y + y_pixel_res * 60)
    assert sub.number_x_pixel == 30
    assert sub.number_y_pixel == 40
    npt.assert_almost_equal(sub.ulc_x, ulc_x + 10 * x_pixel_res)
    npt.assert_almost_equal(sub.ulc_y, ulc_y + 20 * y_pixel_res)


def test_convert_value_vector():
    """Types like Time are wrapped as shared_ptr in python, but used by
    value in a std::vector (e.g., std::vector<Time>). The default SWIG
    conversion doesn't handle this, we have %shared_ptr_value_traits in
    geocal_common.i to fix this."""
    t0 = Time.parse_time("2003-01-01T11:11:00Z")
    v = Vector_Time([t0, t0 + 1])
    assert v.size() == 2
    # Iterating used to return untyped (and leaked) "GeoCal::Time *"
    assert [str(t) for t in v] == [str(t0), str(t0 + 1)]
    assert [str(t) for t in pickle.loads(pickle.dumps(v))] == [str(t0), str(t0 + 1)]
    with pytest.raises(TypeError):
        Vector_Time([t0, 1.0])
    ic = Vector_ImageCoordinate([ImageCoordinate(1, 2), ImageCoordinate(3, 4)])
    assert [(i.line, i.sample) for i in ic] == [(1, 2), (3, 4)]
    assert Vector_InterestPoint([InterestPoint()]).size() == 1
    ad = vector_auto_derivative([AutoDerivativeDouble(1.0), AutoDerivativeDouble(2.0)])
    assert [a.value for a in ad] == [1.0, 2.0]


def test_time_list_argument():
    """Functions taking a std::vector<Time> should accept a list of Time"""
    t0 = Time.parse_time("2003-01-01T11:11:00Z")
    p = PiecewiseLinear(
        [t0, t0 + 10, t0 + 20], [PiecewiseLinear.LINEAR, PiecewiseLinear.CONSTANT]
    )
    p.parameter = [1.0, 3.0]
    assert p.value(t0 + 5) == pytest.approx(2.0)
    assert p.value(t0 + 15) == pytest.approx(3.0)
    tt = MeasuredTimeTable([t0, t0 + 1])
    assert tt.max_time - tt.min_time > 1
