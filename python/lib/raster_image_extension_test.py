from builtins import range
from geocal_swig import *
from geocal.raster_image_extension import *
import numpy as np
from nose.plugins.skip import Skip, SkipTest
import geocal.safe_matplotlib_import
import matplotlib.pyplot as plt

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"
img = VicarLiteRasterImage(test_data + "vicar.img")
d = img.read(0, 0, 10, 10)

def test_past_edge():
    t = img.read_with_pad(-1, -2, 10, 9)
    for i in range(10):
        for j in range(9):
            if(i - 1 < 0):
                assert t[i, j] == 0
            elif(j - 2 < 0):
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i - 1, j - 2]

def test_past_edge2():
    t = img.read_with_pad(2, 3, 10, 9)
    for i in range(10):
        for j in range(9):
            if(i + 2 >= 10):
                assert t[i, j] == 0
            elif(j + 3 >= 10):
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i + 2, j + 3]

def test_past_edge3():
    t = img.read_with_pad(-1, -2, 15, 15)
    for i in range(10):
        for j in range(9):
            if(i - 1 < 0 or i - 1 > 10):
                assert t[i, j] == 0
            elif(j - 2 < 0 or j - 2 > 10):
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i - 1, j - 2]

def test_no_image():
    t = img.read_with_pad(15, 15, 10, 10)
    assert (t == np.zeros((10, 10), dtype = np.int)).all()

def test_display():
    raise SkipTest
    img = VicarLiteRasterImage(test_data + "Stereo/10MAY21-1.img")
    img.display(ImageCoordinate(300, 300), 20)
    plt.show()

def test_display2():
    raise SkipTest
    img = VicarLiteRasterImage(test_data + "Stereo/10MAY21-1.img")
    img.display(ImageCoordinate(100, 50), 500)
    plt.show()
