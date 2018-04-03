from geocal_swig import *
from geocal.image_ground_connection import *
from geocal.tie_point_collect_picmtch import *
from test_support import *
import subprocess

demin = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img", demin)
igc2 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-2.img", demin)
igc_coll = IgcArray([igc1, igc2])
surfimg_fname1 = stereo_unit_test_data + "10MAY21-1_projected.tif"
surfimg_fname2 = stereo_unit_test_data + "10MAY21-2_projected.tif"

@require_vicarb
@require_vicar_gdalplugin
@require_vicar
def test_tp_picmtch(isolated_dir):
    subprocess.check_call(["gdal_translate", "-q",
                           "-of", "VICAR",
                           surfimg_fname1, "img1.img"])
    subprocess.check_call(["gdal_translate", "-q",
                           "-of", "VICAR",
                           surfimg_fname2, "img2.img"])
    tpcollect = TiePointCollectPicmtch(igc_coll, ["img1.img", "img2.img"],
                                   ref_image_fname = "img2.img",
                                   log_file = "tpcol.log")
    tpcol = tpcollect.tie_point_grid(10, 10)
    print(tpcol)
    
    


