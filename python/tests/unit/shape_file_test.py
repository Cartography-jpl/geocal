try:
    from geocal import ShapeFile, ShapeLayer
    from osgeo import ogr
except ImportError:
    # Let this fail, it just means we can't run the unit test
    pass
import shutil
import pytest


@pytest.mark.skip(
    reason="Test data FRST_POIof090514.shp isn't in unit_test_data, and test needs updating"
)
def test_read(unit_test_data):
    """Test basic reading"""
    try:
        ShapeFile
    except NameError:
        pytest.skip("ShapeFile not available")
    sf = ShapeFile(str(unit_test_data / "FRST_POIof090514.shp"))
    assert sf.file_name, "FRST_POIof090514.shp"
    assert list(sf.keys()) == ["FRST_POIof090514"]
    assert sf["FRST_POIof090514"].name == "FRST_POIof090514"
    assert sf["FRST_POIof090514"].field_list == ["GM_LAYER", "GM_TYPE", "NAME", "LAYER"]
    assert len(sf["FRST_POIof090514"]) == 11
    assert sf["FRST_POIof090514"][3]["NAME"] == "Point_4"


@pytest.mark.skip(
    reason="Needs poi.shp/footprint.shp data that isn't available, and test needs updating"
)
def test_footprint_poi_intersect(unit_test_data):
    """Test finding footprints that match a particular POI set."""
    try:
        ShapeFile
    except NameError:
        pytest.skip("ShapeFile not available")
    sh = ShapeFile(str(unit_test_data / "poi.shp"))
    fp = ShapeFile(str(unit_test_data / "footprint.shp"))
    ly = fp["footprint"]
    assert len(ly), 16830
    ly.set_spatial_filter(sh["poi"].geometry_collection())
    ly.set_attribute_filter("Camera = 1")
    assert len(ly), 30


@pytest.mark.skip(
    reason="Not currently working, needs to be updated for current ShapeFile/ogr"
)
def test_write():
    """Test basic writing"""
    try:
        ShapeFile
    except NameError:
        pytest.skip("ShapeFile not available")
    shutil.rmtree("test_dir", True)
    with ShapeFile("test_dir", "w") as f:
        lay = f.add_layer(
            "test",
            ogr.wkbPolygon,
            [["File", ogr.OFTString, 100], ["Row", ogr.OFTInteger]],
        )
        lay.add_feature(
            {
                "File": "f1.txt",
                "Row": 10,
                "Geometry": ShapeLayer.polygon_2d([(1, 1), (2, 1), (2, 2), (1, 2)]),
            }
        )
        lay.add_feature(
            {
                "File": "f2.txt",
                "Row": 11,
                "Geometry": ShapeLayer.polygon_2d([(1, 1), (3, 1), (3, 3), (1, 3)]),
            }
        )
        lay = f.add_layer(
            "test2",
            ogr.wkbPoint,
            [["File2", ogr.OFTString, 100], ["Row2", ogr.OFTInteger]],
        )
        lay.add_feature(
            {
                "File2": "f3.txt",
                "Row2": 12,
                "Geometry": ShapeLayer.point_2d(1, 1),
                "Style": "BRUSH(fc:#0000FF)",
            }
        )
        lay.add_feature(
            {"File2": "f4.txt", "Row2": 13, "Geometry": ShapeLayer.point_2d(2, 2)}
        )
    f = ShapeFile("test_dir")
    assert f.file_name == "test_dir"
    assert f.layers["test2"] is not None
    assert f.layers["test"] is not None
