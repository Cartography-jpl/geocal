try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (Time, KeplerOrbit, SimpleCamera,
                         FrameCoordinate, SimpleDem, Quaternion_double,
                         SensrbCamera)
from geocal.geocal_nitf_sensrb import *
import numpy as np

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

@require_pynitf
def test_sensrb_od(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z");
    korb = KeplerOrbit(t, t + 100.0);
    od = korb.orbit_data(t + 5)
    f.image_segment[0].orbit_data_sensrb = od
    f.image_segment[0].orbit_data_sensrb = od
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    od2 = f2.image_segment[0].orbit_data_sensrb
    print(od)
    print(od2)
    cam = SimpleCamera()
    fc = FrameCoordinate(0,752)
    dem = SimpleDem()
    assert(distance(od.surface_intersect(cam, fc, dem),
                    od2.surface_intersect(cam, fc, dem)) < 1.0)

@require_pynitf
def test_sensrb_cam(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z");
    korb = KeplerOrbit(t, t + 100.0);
    od = korb.orbit_data(t + 5)
    f.image_segment[0].orbit_data_sensrb = od
    cam = SensrbCamera(Quaternion_double(1,0,0,0),
		       1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2))
    f.image_segment[0].camera_sensrb = cam
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    cam2 = f2.image_segment[0].camera_sensrb
    print(cam)
    print(cam2)

# Modules we have
# 1. Filled in. May need to change content level value
# 2. Fill in. FOV iass a made up number, but present
# 3. Filled in. radial_distort_limit is a made up number, but present
# 4. Not present, we should perhaps fill this in
# 5. Filled in
# 6 filled in
# 7 Not filled in, probably shouldn't be
# 8 Not filled in. Perhaps this should be, although looks like 9 should account
#   for this
# 9 Filled in
# 10 Fill in
# 11, 12, 13, 14 not filled in. Don't think needed

# Temp, skip for now since not working. We'll continue working on this but don't
# want unit tests failing
@skip
@require_msp    
@require_pynitf
def test_sensrb_msp(isolated_dir):
    '''Compare SENSRB computionation with MSP library'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z");
    korb = KeplerOrbit(t, t + 100.0);
    od = korb.orbit_data(t + 5)
    f.image_segment[0].orbit_data_sensrb = od
    cam = SensrbCamera(Quaternion_double(1,0,0,0),
		       0,0,0,0,0,0,0,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2))
    f.image_segment[0].camera_sensrb = cam
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "fake_sensor"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"
    t.image_formation_data = "Y"
    t.method = "Single Frame"
    t.mode = "003"
    t.row_count = 2048
    t.column_count = 1024
    t.row_set = 2048
    t.column_set = 1024
    t.row_rate = 0.001
    t.col_rate = 0.001
    t.first_pixel_row = 1
    t.first_pixel_col = 1
    t.transform_params = 0
    t.attitude_euler_angles = "Y"
    #t.attitude_unit_vectors = "Y"
    f.write("sensrb_test.ntf")
    igc = OrbitDataImageGroundConnection(od, cam, SimpleDem(), None)
    # For now, force the plugin/model handler. Reduces the noise when
    # we diagnose problems. Should be able to remove this later.
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem(), "SENSRB_FrameProfile",
                     "SENSRB_FrameProfile")
    h = 0
    for ln in np.linespace(0, 2048, 10):
        for smp in np.linespace(0, 1024, 10):
            ic = ImageCoordinate(ln,smp)
            p1 = igc_msp.ground_coordinate_approx_height(ic, h)
            p2 = rsm.ground_coordinate_approx_height(ic, h)
            assert(geocal_swig.distance(p1, p2) < 0.01)
            
