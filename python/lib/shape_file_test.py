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
    assert sf["FRST_POIof090514"].field_list == ["GM_LAYER", "GM_TYPE", "NAME", "LAYER"]
    assert len(sf["FRST_POIof090514"]) == 11
    assert sf["FRST_POIof090514"][3]["NAME"] == "Point_4"

def test_footprint_poi_intersect():
    '''Test finding footprints that match a particular POI set.'''
    sh = ShapeFile(test_data + "poi.shp")
    fp = ShapeFile(test_data + "footprint.shp")
    ly = fp["footprint"]
    assert len(ly), 16830
    ly.set_spatial_filter(sh["poi"].geometry_collection())
    ly.set_attribute_filter("Camera = 1")
    assert len(ly), 30

def test_write():
    '''Test basic writing'''
    with ShapeFile("test_file.shp", "w") as f:
        pass
    f = ShapeFile("test_file.shp")
    assert f.file_name == "test_file.shp"

