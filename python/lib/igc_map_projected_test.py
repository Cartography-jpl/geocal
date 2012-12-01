from nose.tools import *
from image_ground_connection import *

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_igc_write():
    dem = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", dem)
    gaoi = VicarLiteRasterImage(test_data + "aoi.img")
    mproj = IgcMapProjected(gaoi.map_info, igc1)
    out = VicarRasterImage("igc_test.out", gaoi.map_info, "HALF")
    mproj.write_image(out)

    
