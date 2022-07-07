from test_support import *
from .isis_support import pds_to_isis
from .isis_igc import *
from .isis_to_igc import isis_to_igc
from geocal_swig import (ImageCoordinate, Planetocentric, MolaDemFile, GdalDem,
                         NoVelocityAberration)
from .sqlite_shelf import write_shelve

@long_test
@require_isis
def test_ctx_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/ctx.cub"
    else:
        pds_to_isis(mars_test_data + "P16_007388_2049_XI_24N020W.IMG",
                    "ctx.cub")
        fname = "ctx.cub"
    igc = IsisIgc(fname)
    ic = ImageCoordinate(1000,10)
    gc1 = igc.ground_coordinate(ic)
    print(Planetocentric(gc1))
    igc2 = isis_to_igc(fname)
    dem = MolaDemFile("/raid26/marsdem/meg128/megr44n270hb.lbl")
    dem2 = GdalDem("/bigdata/isisdata/base/dems/molaMarsPlanetaryRadius0005.cub")
    gc2 = igc2.ground_coordinate_dem(ic,dem)
    print(Planetocentric(gc2))
    print(Planetocentric(igc2.ground_coordinate_dem(ic,dem2)))
    igc2.dem = dem
    print(igc2.image_coordinate(gc2))
    print(igc2.image_coordinate(gc1))
    print(igc.image_coordinate(gc2))
    print(igc.image_coordinate(gc1))
    print(igc.pixel_time(ic))
    print(igc2.pixel_time(ic))
    if False:
        gcam =igc.glas_cam_model(focal_length=11994.9988e-3)
        write_shelve("gcam.xml", gcam)
        cam2 = igc2.ipi.camera
        # This shows we agree pretty well between the 2 cameras.
        print(gcam.compare_camera(cam2,0))
    print(igc2.cf_look_vector_pos(ic))
    print(igc.cf_look_vector_pos(ic))
    print(distance(igc2.cf_look_vector_pos(ic),igc.cf_look_vector_pos(ic)))
    print(igc2.cf_look_vector_lv(ic))
    print(igc.cf_look_vector_lv(ic))
    # The positions are off. If we replace the position in igc2 with
    # what ISIS has, we get almost the same location on the ground. So
    # the difference seems to be just in the satellite position.
    p1 = igc.cf_look_vector_pos(ic)
    p2 = igc2.cf_look_vector_pos(ic)
    lv1 = igc.cf_look_vector_lv(ic)
    lv2 = igc2.cf_look_vector_lv(ic)
    d = PlanetSimpleDem(gc1.height_reference_surface, igc2.naif_code)
    print(distance(d.intersect(p1,lv1,0.1),gc1))
    gc2 = igc2.ground_coordinate_dem(ic,d)
    print(distance(d.intersect(p2,lv2,0.1),gc2))
    print(distance(d.intersect(p1,lv2,0.1),gc1))
    
    
@long_test
@require_isis
def test_hirise_isis_igc(mars_test_data, isolated_dir):
    if True:
        # While developing, skip import by using hard coded path
        fname = "/home/smyth/Local/geocal-repo/python/hirise.cub"
    else:
        f1 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_0.img"
        f2 = "/raid26/tllogan/mars_map/hirise_gale/PDS_Image_Files/esp_025012_1745_red0_1.img"
        pds_to_isis(f1, "hirise.cub", pds_fname2=f2)
        fname = "hirise.cub"
    igc = IsisIgc(fname)
    ic = ImageCoordinate(1000,10)
    gc1 = igc.ground_coordinate(ic)
    print(Planetocentric(gc1))
    igc2 = isis_to_igc(fname)
    dem = MolaDemFile("/raid26/marsdem/meg128/megr44n270hb.lbl")
    dem2 = GdalDem("/bigdata/isisdata/base/dems/molaMarsPlanetaryRadius0005.cub")
    if False:
        gcam =igc.glas_cam_model()
        write_shelve("gcam.xml", gcam)
        cam2 = igc2.ipi.camera
        # This shows we agree pretty well between the 2 cameras.
        print(gcam.compare_camera(cam2,0))
    gc2 = igc2.ground_coordinate_dem(ic,dem)
    print(Planetocentric(gc2))
    print(Planetocentric(igc2.ground_coordinate_dem(ic,dem2)))
    igc2.dem = dem
    print(igc2.image_coordinate(gc2))
    print(igc2.image_coordinate(gc1))
    print(igc.image_coordinate(gc2))
    print(igc.image_coordinate(gc1))
    print(igc.pixel_time(ic))
    print(igc2.pixel_time(ic))
    print(igc2.cf_look_vector_pos(ic))
    print(igc.cf_look_vector_pos(ic))
    print(distance(igc2.cf_look_vector_pos(ic),igc.cf_look_vector_pos(ic)))
    print(igc2.cf_look_vector_lv(ic))
    print(igc.cf_look_vector_lv(ic))
    # The positions are off. If we replace the position in igc2 with
    # what ISIS has, we get almost the same location on the ground. So
    # the difference seems to be just in the satellite position.
    p1 = igc.cf_look_vector_pos(ic)
    p2 = igc2.cf_look_vector_pos(ic)
    lv1 = igc.cf_look_vector_lv(ic)
    lv2 = igc2.cf_look_vector_lv(ic)
    d = PlanetSimpleDem(gc1.height_reference_surface, igc2.naif_code)
    print(distance(d.intersect(p1,lv1,0.1),gc1))
    gc2 = igc2.ground_coordinate_dem(ic,d)
    print(distance(d.intersect(p2,lv2,0.1),gc2))
    print(distance(d.intersect(p1,lv2,0.1),gc1))
    
    
    
        

    
