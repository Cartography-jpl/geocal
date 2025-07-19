from test_support import *
from geocal_swig import GdalRasterImage, MapInfo, Geodetic360Converter

# Various tests for handling dateline.

def test_gdal_cross_dateline(isolated_dir):
    mi = MapInfo(Geodetic360Converter(), 179, 30, 182, 29, 100, 100)
    img = GdalRasterImage("test.tif", "gtiff", mi, 1, GdalRasterImage.Byte)
    mi2 = MapInfo(Geodetic360Converter(), 200, 30, 202, 29, 100, 100)
    img = GdalRasterImage("test2.tif", "gtiff", mi2, 1, GdalRasterImage.Byte)
    img = None
    img = GdalRasterImage("test.tif")
    img2 = GdalRasterImage("test2.tif")
    print(img.is_geodetic_360)
    print(img.map_info)
    print(img2.is_geodetic_360)
    print(img2.map_info)
    img.change_to_geodetic360()
    img2.change_to_geodetic360()
    print(img.is_geodetic_360)
    print(img.map_info)
    print(img2.is_geodetic_360)
    print(img2.map_info)
