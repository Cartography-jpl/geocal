try:
    import pynitf
except ImportError:
    pass
from test_support import *
from .mars_igc import *

@require_spice
@require_mars_spice
def test_igc_mro_context(mars_test_data):
    igc = igc_mro_context(mars_test_data + "P16_007388_2049_XI_24N020W.IMG")
    print(igc)

@require_spice
@require_mars_spice
@require_pynitf
def test_glas_mro_context(isolated_dir, mars_test_data):
    igc_r = igc_mro_context(mars_test_data + "P16_007388_2049_XI_24N020W.IMG")
    igc_g = igc_mro_context_to_glas(igc_r)
    f = pynitf.NitfFile()
    img = pynitf.NitfImageWriteNumpy(1,1,np.uint8)
    img[0,0] = 0
    f.image_segment.append(pynitf.NitfImageSegment(img))
    f.image_segment[0].create_glas_gfm(igc_g)
    f.write("glas_test.ntf")
    f2 = NitfFile("glas_test.ntf")
    print(f2)
    with open("glas_test.bin", "wb") as fh:
        fh.write(serialize_write_binary(igc_g))
    with open("glas_test.bin", "rb") as fh:
        igc_gr = serialize_read_binary(fh.read())
    print(igc_gr)
    
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
    d = igc.ipi.camera.sc_look_vector(FrameCoordinate(0,0),0).direction
    print([d[0], d[1], d[2]])
    d = igc.cf_look_vector_lv(ImageCoordinate(100,0)).direction
    print([d[0], d[1], d[2]])
    print(igc.ground_coordinate(ImageCoordinate(100,0)).convert_to_cf())
    print(igc.dem)
    print(Planetocentric(igc.ground_coordinate(ImageCoordinate(100,0))))
    
@require_spice
@require_mars_spice
def test_hirise_camera(mars_test_data):
    '''This looks specifically at the camera, comparing it to results that
    we have from the ISIS program campt to verify out camera model.'''
    igc = igc_mro_hirise(mars_test_data + "esp_025012_1745_red2.norm.cub")
    cam = igc.ipi.camera
    # This is the pixel 0,0 from campt (0.5,0.5 is ISIS coordinates).
    fc = cam.frame_coordinate(ScLookVector(-0.0080898394215051,
                                            0.0060382951197886,
                                            0.99994904544691),0)
    assert(abs(fc.line) < 1e-3)
    assert(abs(fc.sample) < 1e-3)
    d = cam.sc_look_vector(fc,0).direction
    assert_almost_equal(d[0], -0.0080898394215051)
    assert_almost_equal(d[1], 0.0060382951197886)
    assert_almost_equal(d[2], 0.99994904544691)
    fc2 = cam.frame_coordinate(cam.sc_look_vector(fc,0),0)
    assert(abs(fc2.line) < 1e-3)
    assert(abs(fc2.sample) < 1e-3)
    
    # This is the pixel 0,2048 in campt (0.5,248.5 is ISIS coordinates
    fc = cam.frame_coordinate(ScLookVector(-0.0081013647757587,
                                           0.0039895794354517,
                                           0.99995922474104),0)
    assert(abs(fc.line) < 1e-3)
    assert(abs(fc.sample - 2048) < 1e-3)
    fc2 = cam.frame_coordinate(cam.sc_look_vector(fc,0),0)
    assert(abs(fc2.line) < 1e-3)
    assert(abs(fc2.sample - 2048) < 1e-3)
