import numpy as np
import pytest

from geocal import ImageCoordinate, VicarLiteRasterImage


@pytest.fixture(scope="function")
def img(unit_test_data):
    return VicarLiteRasterImage(str(unit_test_data / "vicar.img"))


@pytest.fixture(scope="function")
def d(img):
    return img.read(0, 0, 10, 10)


def test_past_edge(img, d):
    t = img.read_with_pad(-1, -2, 10, 9)
    for i in range(10):
        for j in range(9):
            if i - 1 < 0 or j - 2 < 0:
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i - 1, j - 2]


def test_past_edge2(img, d):
    t = img.read_with_pad(2, 3, 10, 9)
    for i in range(10):
        for j in range(9):
            if i + 2 >= 10 or j + 3 >= 10:
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i + 2, j + 3]


def test_past_edge3(img, d):
    t = img.read_with_pad(-1, -2, 15, 15)
    for i in range(10):
        for j in range(9):
            if i - 1 < 0 or i - 1 > 10 or j - 2 < 0 or j - 2 > 10:
                assert t[i, j] == 0
            else:
                assert t[i, j] == d[i - 1, j - 2]


def test_no_image(img):
    t = img.read_with_pad(15, 15, 10, 10)
    assert (t == np.zeros((10, 10), dtype=int)).all()


@pytest.mark.skip(reason="Interactive display (plt.show), only for manual debugging")
def test_display(stereo_unit_test_data):
    import matplotlib.pyplot as plt

    img = VicarLiteRasterImage(str(stereo_unit_test_data / "10MAY21-1.img"))
    img.display(ImageCoordinate(300, 300), 20)
    plt.show()


@pytest.mark.skip(reason="Interactive display (plt.show), only for manual debugging")
def test_display2(stereo_unit_test_data):
    import matplotlib.pyplot as plt

    img = VicarLiteRasterImage(str(stereo_unit_test_data / "10MAY21-1.img"))
    img.display(ImageCoordinate(100, 50), 500)
    plt.show()
