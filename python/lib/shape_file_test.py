from nose.tools import *
from shape_file import *
import os

test_data = os.path.dirname(__file__) + "/../unit_test_data/"

def test_read():
    '''Test basic reading'''
    sf = ShapeFile(test_data + "FRST_POIof090514.shp")
    assert sf.file_name, test_data + "FRST_POIof090514.shp"
    assert sf.keys() == ["FRST_POIof090514"]
    assert sf["FRST_POIof090514"].name == "FRST_POIof090514"

def test_write():
    '''Test basic writing'''
    with ShapeFile("test_file.shp", "w") as f:
        pass
    f = ShapeFile("test_file.shp")
    assert f.file_name == "test_file.shp"

