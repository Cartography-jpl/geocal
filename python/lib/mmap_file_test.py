from mmap_file import *
from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
import cPickle

def test_read_write():
    # Basic read and write test.
    # Nothing special about this data, it is just a reasonable size for
    # stepping through
        
    # Dummy MapInfo
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        VicarRasterImage
    except NameError:
        raise SkipTest
    ulc_x = 50
    ulc_y = 60
    x_pixel_res = 0.25
    y_pixel_res = -0.50
    number_x_pixel = 100
    number_y_pixel = 200
    mi = MapInfo(GeodeticConverter(), ulc_x, ulc_y, 
                 ulc_x + x_pixel_res * number_x_pixel, 
                 ulc_y + y_pixel_res * number_y_pixel, 
                 number_x_pixel, number_y_pixel)
    f = mmap_file("mmap_file_test.img", mi, dtype=np.float64)
    assert f.shape[0] == 200
    assert f.shape[1] == 100
    for i in range(200):
        for j in range(100):
            f[i,j] = (i * 200 + j * 100) / 4.0
    f = None
    f2 = mmap_file("mmap_file_test.img")
    assert f2.shape[0] == 200
    assert f2.shape[1] == 100
    for i in range(200):
        for j in range(100):
            assert_almost_equal(f2[i,j], (i * 200 + j * 100) / 4.0)
    t = cPickle.dumps(f2, cPickle.HIGHEST_PROTOCOL)
    # Make sure we aren't just dumping all the data
    assert len(t) < 200
    f3 = cPickle.loads(t)
    assert f3.shape[0] == 200
    assert f3.shape[1] == 100
    for i in range(200):
        for j in range(100):
            assert_almost_equal(f3[i,j], (i * 200 + j * 100) / 4.0)
