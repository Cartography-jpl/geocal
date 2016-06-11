from geocal.image_ground_connection import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_igc_write():
    if(not VicarFile.vicar_available()):
        raise SkipTest
    dem = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", dem)
    gaoi = VicarLiteRasterImage(test_data + "aoi.img")
    mproj = IgcMapProjected(gaoi.map_info, igc1)
    out = VicarRasterImage("igc_test.out", gaoi.map_info, "HALF")
    copy_raster(mproj, out)

    
