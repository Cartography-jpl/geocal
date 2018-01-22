from test_support import *
from geocal.spice_camera import *
from geocal.sqlite_shelf import write_shelve

@require_spice
def test_hrsc_camera(mars_kernel):
    print(hrsc_camera())

    # Old code, here in case we want to compare
    if(False):
        focal_length = 175
        pitch = 7e-3
        cam = QuaternionCamera(Quaternion_double(1,0,0,0), 1,
                               5176,
                               pitch, pitch, focal_length, 
                               FrameCoordinate(0 + (-0.007000 / pitch),
                                               5176/2 + 0.0295 / pitch),
                               QuaternionCamera.LINE_IS_Y,
                               QuaternionCamera.INCREASE_IS_NEGATIVE,
                               QuaternionCamera.INCREASE_IS_NEGATIVE)
        print(cam)
    
@require_spice
def test_ctx1_camera(mars_kernel):
    print(ctx_camera())

    
    
