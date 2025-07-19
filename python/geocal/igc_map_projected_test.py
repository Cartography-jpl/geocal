from .image_ground_connection import *
from test_support import *

@require_vicar
def test_igc_write(isolated_dir):
    dem = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img",
                                      dem)
    gaoi = VicarLiteRasterImage(stereo_unit_test_data + "aoi.img")
    mproj = IgcMapProjected(gaoi.map_info, igc1)
    out = VicarRasterImage("igc_test.out", gaoi.map_info, "HALF")
    copy_raster(mproj, out)

    
