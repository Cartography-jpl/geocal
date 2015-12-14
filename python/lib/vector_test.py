from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from geocal_swig import *

def convert_vector_test():
    '''Historically passing a sequence to something expected at std::vector
    didn't work correctly. We have a fix for this now in common.i, but leave
    a test in place in case we go to a new version of SWIG that breaks this.
    '''
    ulc_x = 50
    ulc_y = 60
    x_pixel_res = 0.25
    y_pixel_res = -0.50
    number_x_pixel = 100
    number_y_pixel = 200
    mi = MapInfo(GeodeticConverter(),
                 ulc_x, ulc_y, 
                 ulc_x + x_pixel_res * number_x_pixel, 
                 ulc_y + y_pixel_res * number_y_pixel, 
                 number_x_pixel, number_y_pixel)
    gp = [mi.ground_coordinate(10.1, 20.1),
          mi.ground_coordinate(10 + 28.9, 20 + 38.9)]
    sub = mi.cover(gp)
    assert_almost_equal(sub.lrc_x, ulc_x + x_pixel_res * 40)
    assert_almost_equal(sub.lrc_y, ulc_y + y_pixel_res * 60)
    assert sub.number_x_pixel == 30
    assert sub.number_y_pixel == 40
    assert_almost_equal(sub.ulc_x, ulc_x + 10 * x_pixel_res)
    assert_almost_equal(sub.ulc_y, ulc_y + 20 * y_pixel_res)
