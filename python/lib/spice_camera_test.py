from test_support import *
from .spice_camera import *
from .sqlite_shelf import write_shelve, read_shelve

@require_spice
@require_mars_spice
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
@require_mars_spice
def test_ctx1_camera(mars_kernel):
    print(ctx_camera())

@require_spice
def test_wac_camera():
    klist = SpiceKernelList(["$lro/kernels/ik/lro_lroc_v18.ti",
                             "$lro/kernels/iak/lro_instrumentAddendum_v04.ti"])
    klist.load_kernel()
    gcam = read_shelve("gcam.xml")
    cam = lro_wac_camera(band=3,mode="COLOR")
    print(cam.frame_coordinate(gcam.sc_look_vector(FrameCoordinate(7,704/2),0),0))
    print(cam.full_camera.focal_length)
    print(cam.full_camera.focal_plane_to_dcs(0, 0.543071, 3.17634))
#    cam = SimpleCamera(0,0,0,6e-3,9.0e-6,9.0e-6,1024,1024)
#    print(cam.frame_coordinate(gcam.sc_look_vector(FrameCoordinate(7,704/2),0),0))
    
    
    
