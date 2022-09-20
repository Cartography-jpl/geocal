from test_support import *
from .isis_support import pds_to_isis
from .isis_to_igc import *
from .isis_igc import IsisIgc
from .glas_gfm import *
from geocal_swig import ImageCoordinate, SubCamera, VicarRasterImage, copy_raster
from .sqlite_shelf import write_shelve
import pandas as pd
import pynitf

def check_igc(fname, check_time=False, check_camera=False,check_isis=False,
              check_spice=False, check_glas_rsm=True, check_glas=False,
              check_create_nitf=False,
              band = None):
    '''Compare a IGC with the same ISIS calculation. This tends to be
    common for different instruments, so collect the generic part of
    what we check here.'''
    keyword = {}
    if(band):
        keyword["band"]=band
    igc = isis_to_igc(fname, **keyword)
    igc_isis = IsisIgc(fname)
    # Check every 100th line time
    if check_time:
        for ln in range(0,igc.number_line,100):
            ic = ImageCoordinate(ln,igc.number_sample/2)
            if ln % 1000 == 0:
                print(f"Checking line {ln}")
            assert_almost_equal(igc.pixel_time(ic).j2000,
                                igc_isis.pixel_time(ic).j2000,
                                decimal=6)

    if check_camera:
        cam = igc.ipi.camera
        if(isinstance(cam, SubCamera)):
            focal_length = cam.full_camera.focal_length
        else:
            focal_length = cam.focal_length
        if(cam.number_line(0) == 1):
            gcam =igc_isis.glas_cam_model(focal_length)
        else:
            gcam =igc_isis.gfm_cam_model(focal_length, cam.number_line(0))
        write_shelve("cam.xml", cam)
        write_shelve("gcam.xml", gcam)
        # This shows we agree pretty well between the 2 cameras.
        t = gcam.compare_camera(cam,0)
        print(f"Camera compare {t}")
        # GLAS doesn't really capture the line direction, so ignore this
        # in our comparison. But check for GFM
        if(gcam.number_line(0) != 1):
            assert t[0] < 1e-2
        assert t[1] < 1e-2
    if check_isis:
        igc_match_isis = isis_to_igc(fname, match_isis=True, **keyword)
        dlist = []
        for ln in range(0,igc.number_line,1000):
            if(ln > 0):
                print(f"Looking at line {ln}. max dist: {max(dlist)}")
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                #print(ic)
                # Skip for now, doesn't work the WAC and NAC, which
                # is probably an issue. We'll want to come back to
                # this
                #assert distance(igc_match_isis.cf_look_vector_pos(ic),
                #                igc_isis.cf_look_vector_pos(ic)) < 10
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
        igc_spice = isis_to_igc(fname, spice_igc=True, **keyword)
        igc_match_isis = isis_to_igc(fname, match_isis=True, **keyword)
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
    if check_glas_rsm or check_glas:
        if(check_glas_rsm):
            igc_glas,rsm = isis_to_igc(fname, glas_gfm=True, rsm=True, **keyword)
            print(rsm)
        else:
            igc_glas = isis_to_igc(fname, glas_gfm=True, **keyword)
        pxdist = []
        res = igc.resolution_meter()
        for ln in range(0,igc.number_line,1000):
            if(ln > 0 and len(pxdist) > 0):
                print(f"Looking at line {ln}. {max(pxdist)}")
            for smp in range (0,igc.number_sample,100):
                ic = ImageCoordinate(ln, smp)
                # We may have some locations seen multiple times on the
                # ground. So look at distance we get on the ground
                # from round tripping
                gc = igc_glas.ground_coordinate(ic)
                ic2 = igc.image_coordinate(gc)
                gc2 = igc_glas.ground_coordinate(ic2)
                pxdist.append(distance(gc, gc2)/res)
                gc = igc.ground_coordinate(ic)
                ic2 = igc_glas.image_coordinate(gc)
                gc2 = igc.ground_coordinate(ic2)
                pxdist.append(distance(gc, gc2)/res)
                if(pxdist[-2] > 1 or pxdist[-1] > 1):
                    print(ic)
        print("GLAS diff")
        print(pd.DataFrame(pxdist).describe())
    if(check_create_nitf):
        igc_glas = isis_to_igc(fname, glas_gfm=True, **keyword)
        f = pynitf.NitfFile()
        img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8, idlvl=2)
        for i in range(9):
            for j in range(10):
                img[0, i,j] = i * 10 + j
        f.image_segment.append(pynitf.NitfImageSegment(img))
        f.image_segment[0].create_glas_gfm(igc_glas)
        f.write("gfm_test.ntf")
        f2 = NitfFile("gfm_test.ntf")
        print(f2.image_segment[0].glas_gfm)
        
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

@long_test
@require_isis
def test_lunar_wac_to_igc(isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/wac.cub"
    else:
        f = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549036CC.IMG"
        pds_to_isis(f, "wac.cub")
        fname = "wac.cub"
    if False:
        igc = isis_to_igc(fname, band=3)
        write_shelve("igc.xml", igc)
    # Try writing raster data
    if False:
        igc = isis_to_igc(fname, band=3, radiance_scale=None)
        f = VicarRasterImage(f"wac.img", "DOUB",
                             igc.number_line, igc.number_sample)
        copy_raster(igc.image, f, True, -1, igc.number_sample)
        return
    check_igc(fname, band=3, check_glas_rsm=False, check_time=False,
              check_camera=False, check_isis=True, check_spice=False,
              check_glas = True, check_create_nitf = True)

@long_test
@require_isis
def test_lunar_nac_to_igc(isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        lfname = "/home/smyth/Local/geocal-repo/python/lnac.cub"
        rfname = "/home/smyth/Local/geocal-repo/python/rnac.cub"
    else:
        lcam_nac_fname = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549139LE.IMG"
        rcam_nac_fname = "/raid28/tllogan/Moon_Luna_Data/WAC/mixed_WAC_NAC_edr_cdr/M1124549139RE.IMG"
        pds_to_isis(lcam_nac_fname, "lnac.cub")
        pds_to_isis(rcam_nac_fname, "rnac.cub")
        lfname = "lnac.cub"
        rfname = "rnac.cub"
    for fname in (lfname, rfname):
        check_igc(fname, check_glas_rsm=False, check_time=False,
                  check_camera=False, check_isis=True, check_spice=False,
                  check_glas = False, check_create_nitf = False)

@long_test
@require_isis
def test_lunar_nac_to_igc2(isolated_dir):
    '''Second NAC test were the sampling factor is 1 instead of 2'''
    if True:
        # While developing, skip import by using hard coded path
        lfname = "/home/smyth/Local/geocal-repo/python/lnac2.cub"
    else:
        lcam_nac_fname = "/raid28/tllogan/Moon_Luna_Data/NAC_pilot_PDS/M1192065066LE.IMG"
        pds_to_isis(lcam_nac_fname, "lnac2.cub")
        lfname = "lnac2.cub"
    for fname in (lfname, ):
        check_igc(fname, check_glas_rsm=False, check_time=False,
                  check_camera=False, check_isis=True, check_spice=False,
                  check_glas = False, check_create_nitf = False)
        
    
    
