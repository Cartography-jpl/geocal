from nose.tools import *
from cubic_spline import *

def test_spline():
    '''Test CubicSpline'''
    cs = CubicSpline()
    cs.add_knot(10)
    cs.add_knot(15)
    cs.add_knot(20)
    cs.parameter = [10, 20, 5, 30]
    print cs.value(10)
    print cs.value(15)
    print cs.value(20)
