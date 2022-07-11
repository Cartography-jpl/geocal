from test_support import *
from .isis_support import pds_to_isis
from .isis_to_igc import *
from .isis_igc import IsisIgc
from geocal_swig import ImageCoordinate, SubCamera
from .sqlite_shelf import write_shelve
import pandas as pd

def check_igc(fname, check_time=False, check_camera=False,check_isis=False,
              check_spice=False, check_glas_rsm=True):
    '''Compare a IGC with the same ISIS calculation. This tends to be
    common for different instruments, so collect the generic part of
    what we check here.'''
    igc = isis_to_igc(fname)
    igc_isis = IsisIgc(fname)
    # Check every 100th line time
    if check_time:
        for ln in range(0,igc.number_line,100):
            ic = ImageCoordinate(ln,igc.number_sample/2)
            if ln % 1000 == 0:
                print(f"Checking line {ln}")
            assert_almost_equal(igc.pixel_time(ic).j2000,
                                igc_isis.pixel_time(ic).j2000)

    if check_camera:
        cam = igc.ipi.camera
        if(isinstance(cam, SubCamera)):
            focal_length = cam.full_camera.focal_length
        else:
            focal_length = cam.focal_length
        gcam =igc_isis.glas_cam_model(focal_length)
        write_shelve("cam.xml", cam)
        write_shelve("gcam.xml", gcam)
        # This shows we agree pretty well between the 2 cameras.
        t = gcam.compare_camera(cam,0)
        print(f"Camera compare {t}")
        # GLAS doesn't really capture the line direction, so ignore this
        # in our comparison
        #assert t[0] < 1e-2
        assert t[1] < 1e-2
    if check_isis:
        igc_match_isis = isis_to_igc(fname, match_isis=True)
        dlist = []
        for ln in range(0,igc.number_line,1000):
            if(ln > 0):
                print(f"Looking at line {ln}. max dist: {max(dlist)}")
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                assert distance(igc_match_isis.cf_look_vector_pos(ic),
                                igc_isis.cf_look_vector_pos(ic)) < 10
                gc = igc_isis.ground_coordinate(ic)
                # We don't currently have no aberration working with
                # sc_look_vector in orbit, so IPI can't have aberration turned
                # off. So look at distance instead
                #ic2 = igc_match_isis.image_coordinate(gc)
                gc2 = igc_match_isis.ground_coordinate_approx_height(ic, gc.height_reference_surface)
                dlist.append(distance(gc,gc2))
        print("ISIS diff")
        print(pd.DataFrame(dlist).describe())
    if check_spice:
        igc_spice = isis_to_igc(fname, spice_igc=True)
        igc_match_isis = isis_to_igc(fname, match_isis=True)
        d = []
        for ln in range(0,igc.number_line,1000):
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                gc = igc_spice.ground_coordinate(ic)
                gc2 = igc_match_isis.ground_coordinate(ic)
                d.append(distance(gc,gc2))
                ic2 = igc.image_coordinate(gc)
                assert abs(ic.line - ic2.line) < 0.1
                assert abs(ic.sample - ic2.sample) < 0.1
        print("Difference ISIS vs SPICE")
        print(pd.DataFrame(d).describe())
    if check_glas_rsm:
        igc_glas,rsm = isis_to_igc(fname, glas_gfm=True, rsm=True)
        print(igc_glas)
        print(rsm)
        lnlist = []
        smplist = []
        for ln in range(0,igc.number_line,1000):
            if(ln > 0):
                print(f"Looking at line {ln}. ({max(lnlist)},{max(smplist)})")
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                gc = igc_glas.ground_coordinate(ic)
                ic2 = igc.image_coordinate(gc)
                lnlist.append(abs(ic.line - ic2.line))
                smplist.append(abs(ic.sample - ic2.sample))
        print("GLAS diff")
        print(pd.DataFrame(lnlist).describe())
        print(pd.DataFrame(smplist).describe())

    # TODO Spice comparison 
        
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

@long_test
@require_isis
def test_ctx_sample_first_to_igc(mars_test_data, isolated_dir):
    '''A case Tom had where the sample_first isn't 0'''
    fname = "/home/smyth/Local/geocal-repo/python/tom_problem.cub"
    check_igc(fname)
    
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
        
    
