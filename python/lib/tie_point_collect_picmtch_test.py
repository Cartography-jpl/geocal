from geocal_swig import *
from geocal.image_ground_connection import *
from geocal.tie_point_collect_picmtch import *
from geocal.warp_image import *
from geocal.sqlite_shelf import *
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

# This uses mars data. Use while we get this initially working, will
# probably go away. We depend on a specific run, including the ibis file
# output. So this is something where the data will likely disappear. But
# leave this code in place so we have a record of the testing we did.
@skip
@require_vicarb
@require_vicar_gdalplugin
@require_vicar
def test_mars_tp_picmtch(isolated_dir):
    bdir = "/home/smyth/Local/MiplMarsTest/run/"
    img1 = bdir + "ctx1_full_6m.img"
    img2 = bdir + "hrsc2_sub_corr_12m.img"
    img2_res = bdir + "hrsc2_sub_corr_6m.img"
    dem = PlanetSimpleDem(PlanetConstant.MARS_NAIF_CODE)
    igc1 = MapInfoImageGroundConnection(VicarLiteRasterImage(img1), dem)
    igc_coll = IgcArray([igc1,])
    tpcollect = TiePointCollectPicmtch(igc_coll, [img1, img2],
                                       ref_image_fname = img2,
                                       log_file = "tpcol.log",
                                       quiet = False)
    tpcol = tpcollect.tie_point_grid(42, 42)
    write_shelve("tpcol.xml", tpcol)
    original_ibis = IbisFile(bdir + "test1_grid")
    assert len(tpcol) == original_ibis.shape[0]
    ref_res = VicarLiteRasterImage(img2_res)
    for i in range(original_ibis.shape[0]):
        ic = VicarImageCoordinate(tpcol[i].image_coordinate(0))
        assert ic.line == pytest.approx(original_ibis[i, 13], abs=0.01)
        assert ic.sample == pytest.approx(original_ibis[i, 14], abs=0.01)
        ic2 = VicarImageCoordinate(ref_res.coordinate(tpcol[i].ground_location))
        assert ic2.line == pytest.approx(original_ibis[i, 5], abs=0.01)
        assert ic2.sample == pytest.approx(original_ibis[i, 6], abs=0.01)
    print(tpcol)
    warp_image(img1, "ctx1_full_corr_6m.img", img2_res, tpcol,
               nah=1000, nav=1000,quiet=False)
    subprocess.run([bdir + "../mygdalcompare.py", "ctx1_full_corr_6m.img",
                    bdir + "ctx1_full_corr_6m.img"])

# This was a particular test case we added search for. Turns out this
# doesn't actually work for this case because the image is too small for
# a large search. But go ahead and save the test case here for a record.
@skip    
def test_mars_search(isolated_dir):
    dem = read_shelve("/home/smyth/Local/MarsRsmProblem/dem_gale.xml")
    img = VicarLiteRasterImage("/home/smyth/Local/MarsRsmProblem/hirise_gale_red0.img")
    reference_image = "/raid26/tllogan/mars_map/hirise_gale/5degfix/ESP_025012_1745_RED.hlf"
    rsm = img.rsm
    igc = RsmImageGroundConnection(rsm, dem, img, "RSM Image")
    igccol = IgcArray([igc,])
    proj_fname = "/home/smyth/Local/MarsRsmProblem/hirise_gale_red0_proj.img"
    tpcollect = TiePointCollectPicmtch(igccol, [proj_fname,],
                            ref_image_fname=reference_image,
                            fftsize=256, magnify=6,
                            magmin=3, toler=0.5,
                            redo=50,
                            search=700,
                            ffthalf=2, seed=562, autofit=20,
                            thr_res=10.0, quiet = False)
    tpcol = tpcollect.tie_point_grid(42, 42)
    #tpcol = tpcollect.tie_point_grid(10, 10)
    print(tpcol)

    
