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
import six

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
		       1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 3000,
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
		       0,0,0,0,0,0,0, 3000,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2),
                       "VIS")
    f.image_segment[0].camera_sensrb = cam
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "TEST"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"
    t.generation_date = 20000101
    t.generation_time = 0
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
    t.sensor_angle_model = 1
    t.sensor_angle_1 = 0
    t.sensor_angle_2 = 0
    t.sensor_angle_3 = 0
    t.platform_relative = "Y"
    t.platform_heading = 0
    t.platform_pitch = 0
    t.platform_roll = 0
    t.row_fov = 16.93658629935865
    t.col_fov = 8.514793599982974
    # Temp
    t.attitude_quaternion = "N"
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


@require_msp    
@require_pynitf
def test_sensrb_sample_msp(isolated_dir):
    '''Use the sample SENSRB TRE make sure MSP can read it. This might go away
    once we have our own version of writing SENSRB, but for now we'll use
    this to have something clean to work with.

    This comes from the
    http://www.gwg.nga.mil/ntb/baseline/software/demo.html and is the file
    SB_Seattle_WithUncertainties.txt. This doesn't have all the sections needed
    by the MSP library, so we have to add a few. Also, the MSP library seems
    to require Geocentric coordinates rather than lat/lon so we need to convert
    that.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    fh = six.BytesIO(b"SENSRB01804YACESHY                                                   Super Hornet                                             Airborne  1WGS84GHAESIDEG1997040500000000000000199704050000000000000000------------------YVIS                 00001024000010240000000200000002000003.5----------------NNN000000000000----------------47.58288058-122.328413900000000360000000000000000000000000Y1-000000017-000000180000000000N----------------------------NNY000000100-0000005000000000004Image Center             001000005120000051247.5951277-0122.3316513.106--------Image Footprint          004000010240000000147.5941277-0122.3326513.106--------000000010000000147.5961277-0122.3326513.106--------000000010000102447.5961277-0122.3306513.106--------000010240000102447.5941277-0122.3306513.106--------Ground Area              004000008760000021247.5944277-0122.3316513.106--------000005180000050147.5951277-0122.3321513.106--------000007750000078947.5958277-0122.3316513.106--------000004940000051247.5951277-0122.3311513.106--------Ground Points            003000008000000011547.5948277-0122.3318513.106--------000004000000050347.5954277-0122.3318513.106--------000008000000096247.5954277-0122.3313513.106--------000001911e2.1     -----------000000001011f2.1     -----------000000001011e2.2     -----------000000002011f2.2     -----------000000002011e2.3     -----------000000001011f2.3     -----------000000001011e2.4     -----------000000002011f2.4     -----------000000002011e1.1     -----------000000000511f1.1     -----------000000001011e1.1     11f1.1     000000000011e3.1     -----------000000001011f3.1     -----------000000001011e3.2     -----------000000001011f3.2     -----------000000001011e3.3     -----------000000001011f3.3     -----------000000001011e3.4     -----------000000002011f3.4     -----------0000000020000")
    t = pynitf.TreSENSRB()
    t.read_from_file(fh)

    # Should go through and figure out what we actually need to modify
    t.calibrated = "Y"
    t.sensor_calibration_data = "Y"
    t.calibration_unit = "px"
    t.principal_point_offset_x = 0
    t.principal_point_offset_y = 0
    t.image_formation_data = "Y"
    t.method = "Single Frame"
    t.mode = "004"
    t.row_count = 1024
    t.column_count = 1024
    t.row_set = 1024
    t.column_set = 1024
    t.row_rate = 0.001
    t.column_rate = 0.001
    t.first_pixel_row = 1
    t.first_pixel_column = 1
    t.transform_params = 0
    # Convert to ECR coordinates, MSP doesn't work with lat/lon
    pt = Ecr(Geodetic(t.latitude_or_x, t.longitude_or_y, t.altitude_or_z))
    t.geodetic_type = "C"
    t.latitude_or_x, t.longitude_or_y, t.altitude_or_z = pt.position
    
    f.image_segment[0].tre_list.append(t)
    f.write("sensrb_test.ntf")
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem(), "SENSRB_FrameProfile",
                     "SENSRB_FrameProfile")
    print(Geodetic(igc_msp.ground_coordinate(ImageCoordinate(10,10))))
            
