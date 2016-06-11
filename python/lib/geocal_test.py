from geocal_swig import *
from numpy.testing import assert_almost_equal
import datetime

# Unit tests for geocal python code

# Test time. Note that we check for pickling in the pickle_test.py
def test_time():
    t1 = Time.parse_time("2000-01-01T12:00:00Z")
    assert_almost_equal (t1.j2000, 64.18392726778984)
    t1 = Time.time_j2000(0.0)
    t2 = Time.time_j2000(100.0)
    assert_almost_equal(t2 - t1, 100.0)
    assert_almost_equal(t2 - (t1 + 100.0), 0)
    assert_almost_equal(t2 - (100 + t1), 0)
    assert_almost_equal((t2 - 100) - t1, 0)
    assert t1 == t1
    assert t1 < t2
    assert t1 <= t2
    assert t2 > t1
    assert t2 >= t1
    assert t2 <= t2

def test_eci_to_ecr():
    g = Geodetic(10, 20, 100)
    ecr = Ecr(g)
    t = Time.time_j2000(100.0)
    eci = ecr.convert_to_ci(t)
    assert distance(eci.convert_to_cf(t), g) < 0.01

    
    
