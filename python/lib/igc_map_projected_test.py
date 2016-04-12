from nose.tools import *
from geocal.image_ground_connection import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_igc_write():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        VicarRasterImage
    except NameError:
        raise SkipTest
    dem = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", dem)
    gaoi = VicarLiteRasterImage(test_data + "aoi.img")
    mproj = IgcMapProjected(gaoi.map_info, igc1)
    out = VicarRasterImage("igc_test.out", gaoi.map_info, "HALF")
    copy_raster(mproj, out)

    
