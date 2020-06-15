from map_info_extension import *
from misc import cib01_mapinfo
from test_support import *

def test_rotated_map(igc_half_meter_pushbroom):
    igc = igc_half_meter_pushbroom
    mi = cib01_mapinfo(0.5)
    mi = mi.rotated_map(igc)
    gc1 = igc.ground_coordinate(ImageCoordinate(0, igc.number_sample / 2))
    gc2 = igc.ground_coordinate(ImageCoordinate(igc.number_line - 1,
                                                igc.number_sample / 2))
    x1, y1 = mi.coordinate(gc1)
    x2, y2 = mi.coordinate(gc2)
    assert x1 == pytest.approx(x2)
    assert mi.resolution_meter == pytest.approx(0.5, abs=1e-2)

