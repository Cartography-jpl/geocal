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
    cam = lro_wac_camera(band=3,mode="COLOR")
    # Some intermediate calculations we got from isis_camera.cc directly
    # running ISIS. We don't need these results any more, but it is useful
    # to leave this is place as an illustration of how we can diagnose stuff
    # like this
    print("Distortion calculation")
    print("----------------------")
    print("ISIS focal plane was (0.00595126, 3.17637)")
    print(cam.focal_plane_to_dcs(0, 0.00595126, 3.17637))
    print("Result from ISIS: [0.00675484, 3.60527, 5.99839]")
    print()
    print("Focal plane calculation")
    print("-----------------------")
    print("ISIS coordinate (13.5,703.5) (note 0.5 difference from our convention)")
    print(cam.fc_to_focal_plane(FrameCoordinate(13,703),0))
    print("Result from ISIS: (0.543071, 3.17634)")
    print()
    print("Overall calculation")
    print("-------------------")
    print("ISIS coordinate (13.5,703.5) (note 0.5 difference from our convention)")
    print(cam.sc_look_vector(FrameCoordinate(13,703),0))
    print("Result from ISIS: [0.088002, 0.51471, 0.852836]")
    
    # Add check for other bands, include UV. Add checks for other modes
    # like BW
    
