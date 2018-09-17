from geocal_swig import *
from geocal.image_ground_connection import *
from geocal.tie_point_collect_picmtch import *
from test_support import *
import subprocess


@require_vicarb
@require_vicar_gdalplugin
@require_vicar
def test_tp_picmtch(isolated_dir):
    demin = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-2.img", demin)
    igc_coll = IgcArray([igc1, igc2])
    surfimg_fname1 = stereo_unit_test_data + "10MAY21-1_projected.tif"
    surfimg_fname2 = stereo_unit_test_data + "10MAY21-2_projected.tif"
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

# This used mars data. Use while we get this initially working, will
# probably go away.
@require_vicarb
@require_vicar_gdalplugin
@require_vicar
def test_mars_tp_picmtch(isolated_dir):
    bdir = "/home/smyth/Local/MiplMarsTest/run/"
    img1 = bdir + "ctx1_full_6m.img"
    img2 = bdir + "hrsc2_sub_corr_12m.img"
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    igc1 = MapInfoImageGroundConnection(VicarLiteRasterImage(img1), dem)
    igc_coll = IgcArray([igc1,])
    tpcollect = TiePointCollectPicmtch(igc_coll, [img1, img2],
                                       ref_image_fname = img2,
                                       log_file = "tpcol.log",
                                       quiet = False)
    tpcol = tpcollect.tie_point_grid(42, 42)
    print(tpcol)
    
    
    


