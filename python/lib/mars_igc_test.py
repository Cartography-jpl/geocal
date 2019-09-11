from test_support import *
from geocal.mars_igc import *

@require_spice
@require_mars_spice
def test_igc_mro_context(mars_test_data):
    igc = igc_mro_context(mars_test_data + "P16_007388_2049_XI_24N020W.IMG")
    print(igc)

@require_spice
@require_mars_spice
def test_igc_mex_hrsc(mars_test_data):
    igc = igc_mex_hrsc(mars_test_data + "h1326_0000_nd2.img")
    print(igc)

@require_spice
@require_mars_spice
def test_igc_mro_hirise(mars_test_data):
    igc = igc_mro_hirise(mars_test_data + "esp_025012_1745_red2.norm.cub")
    print(igc)
    
@require_spice
@require_mars_spice
def test_hirise_camera(mars_test_data):
    '''This looks specifically at the camera, comparing it to results that
    we have from the ISIS program campt to verify out camera model.'''
    cam,tt,orb = igc_mro_hirise(mars_test_data + "esp_025012_1745_red2.norm.cub")

    # This is the pixel 0,0 in campt (0.5,0.5 is ISIS coordinates
    fc = cam.frame_coordinate(ScLookVector(-0.0080898394215051,
                                            0.0060382951197886,
                                            0.99994904544691),0)
    print(fc)
    d = cam.sc_look_vector(fc,0).direction
    print(d[0],d[1],d[2])
    print(cam.frame_coordinate(cam.sc_look_vector(fc,0),0))
    # This is the pixel 0,2048 in campt (0.5,248.5 is ISIS coordinates
    fc = cam.frame_coordinate(ScLookVector(-0.0081013647757587,
                                           0.0039895794354517,
                                           0.99995922474104),0)
    print(fc)
    d = cam.sc_look_vector(fc,0).direction
    print(d[0],d[1],d[2])
    print(cam.frame_coordinate(cam.sc_look_vector(fc,0),0))
        
