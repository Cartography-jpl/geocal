from test_support import *
from .isis_support import pds_to_isis
from .isis_to_igc import *
from .isis_igc import IsisIgc
from geocal_swig import ImageCoordinate
from .sqlite_shelf import write_shelve

def check_igc(fname):
    '''Compare a IGC with the same ISIS calculation. This tends to be
    common for different instruments, so collect the generic part of
    what we check here.'''
    igc = isis_to_igc(fname)
    igc_isis = IsisIgc(fname)
    # During development, can be useful have things turned on an off, so
    # the tests below are all in conditionals.  Just flip True to False to
    # turn off
    # Check every 100th line time
    if True:
        for ln in range(0,igc.number_line,100):
            ic = ImageCoordinate(ln,igc.number_sample/2)
            if ln % 1000 == 0:
                print(f"Checking line {ln}")
            assert_almost_equal(igc.pixel_time(ic).j2000,
                                igc_isis.pixel_time(ic).j2000)

    if True:
        cam = igc.ipi.camera
        gcam =igc_isis.glas_cam_model(cam.focal_length)
        write_shelve("cam.xml", cam)
        write_shelve("gcam.xml", gcam)
        # This shows we agree pretty well between the 2 cameras.
        t = gcam.compare_camera(cam,0)
        print(f"Camera compare {t}")
        # GLAS doesn't really capture the line direction, so ignore this
        # in our comparison
        #assert t[0] < 1e-2
        assert t[1] < 1e-2
    # Note this fails, and as it turns out for a good reason. IsisIgc
    # incorrectly uses LT+S in its position calculation (see description
    # in IsisIgc). We get a different value, but we are actually right
    # and ISIS is wrong. We can turn this on if we manually change our
    # Igc to be wrong in the same way - useful to track down other
    # differences between the models
    if False:
        for ln in range(0,igc.number_line,1000):
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                assert distance(igc.cf_look_vector_pos(ic),
                                igc_isis.cf_look_vector_pos(ic)) < 10
                ic2 = igc.image_coordinate(igc_isis.ground_coordinate(ic))
                assert abs(ic.line - ic2.line) < 0.1
                assert abs(ic.sample - ic2.sample) < 0.1
    if True:
        igc,rsm = isis_to_igc(fname, glas_gfm=True, rsm=True)
        print(igc)
        print(rsm)
        
@long_test
@require_isis
def test_ctx_to_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                    "ctx.cub")
        fname = "ctx.cub"
    check_igc(fname)
    # Verify that we calculate nearly the same boresight at SPICE.
    igc = isis_to_igc(fname)
    ic = ImageCoordinate(200, igc.ipi.camera.principal_point(0).sample)
    pts = SpiceHelper.boresight_and_footprint(igc.pixel_time(ic),
                                              PlanetConstant.MARS_NAIF_CODE,
                                              "MRO", "MRO_CTX", "CN+S")
    print("Distance spice boresight and IGC: ",
          distance(pts[0], igc.ground_coordinate(ic)))
    assert distance(pts[0], igc.ground_coordinate(ic)) < 1.0
    print(ic)
    print(igc.image_coordinate(pts[0]))
    
@long_test
@require_isis
def test_hirise_to_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/hirise.cub"
    else:
        f1 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_0.img"
        f2 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_1.img"
        pds_to_isis(f1, "hirise.cub", pds_fname2=f2)
        fname = "hirise.cub"
    check_igc(fname)
        

    
