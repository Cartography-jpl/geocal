from builtins import range
from geocal.raster_image_extension import *
from test_support import *
import geocal.safe_matplotlib_import
import matplotlib.pyplot as plt

img = VicarLiteRasterImage(unit_test_data + "vicar.img")
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

@skip    
def test_display():
    img = VicarLiteRasterImage(test_data + "Stereo/10MAY21-1.img")
    img.display(ImageCoordinate(300, 300), 20)
    plt.show()

@skip    
def test_display2():
    img = VicarLiteRasterImage(test_data + "Stereo/10MAY21-1.img")
    img.display(ImageCoordinate(100, 50), 500)
    plt.show()
