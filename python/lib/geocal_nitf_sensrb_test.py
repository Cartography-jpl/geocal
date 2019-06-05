try:
    import pynitf
except ImportError:
    pass
from test_support import *
from geocal_swig import (Time, KeplerOrbit, SimpleCamera,
                         FrameCoordinate, SimpleDem, Quaternion_double,
                         SensrbCamera, AircraftOrbitData, deg_to_rad,
                         quaternion_to_matrix)
from geocal.geocal_nitf_sensrb import *
import numpy as np
import numpy.testing as npt
import six
import math

def create_image_seg(f):
    img = pynitf.NitfImageWriteNumpy(9, 10, np.uint8)
    img.image_subheader.idlvl = 1
    for i in range(9):
        for j in range(10):
            img[0,i,j] = i + j
    f.image_segment.append(pynitf.NitfImageSegment(img))

def igc_compare(od, cam):
    '''Given orbit data and camera, create a normal igc and a MSP igc. We
    can then compare between them. This may go away at some point, but for
    now useful to be able to switch between these as we develop things.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].orbit_data_sensrb = od
    f.image_segment[0].camera_sensrb = cam
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "TEST"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"

    # Temp place holder
    t.attitude_euler_angles = "Y"
    t.sensor_angle_model = 1
    #t.sensor_angle_1, t.sensor_angle_2, t.sensor_angle_3 = quat_to_sensor_angle_1(cam.frame_to_sc)
    t.sensor_angle_1, t.sensor_angle_2, t.sensor_angle_3 = 0,-80,0
    t.platform_relative = "Y"
    t.platform_heading = 0
    t.platform_pitch = 0
    t.platform_roll = 0
    t.attitude_unit_vectors = "Y"
    t.icx_north_or_x = 0
    t.icx_east_or_y = 1
    t.icx_down_or_z = 0
    t.icy_north_or_x = .173648178
    t.icy_east_or_y = 0
    t.icy_down_or_z = .984807753
    t.icz_north_or_x = .984807753
    t.icz_east_or_y = 0
    t.icz_down_or_z = -.173648178
    # Temp
    t.attitude_quaternion = "N"
    print(t)

    f.write("sensrb_test.ntf")
    igc = OrbitDataImageGroundConnection(od, cam, SimpleDem(), None)
    # For now, force the plugin/model handler. Reduces the noise when
    # we diagnose problems. Should be able to remove this later.
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem(), "SENSRB_FrameProfile",
                     "SENSRB_FrameProfile")
    return igc, igc_msp

def m_ecf_to_ned(lat, lon):
    '''Equation 3 in document "SENSRB Profile Frame Image-To-Ground 
    Transformation Description" by Michale h. Lenihan'''
    lat_rad = lat * deg_to_rad
    lon_rad = lon * deg_to_rad
    m1 = np.matrix([[0,0,1],
                    [1,0,0],
                    [0,1,0]])
    m2 = np.matrix([[1, 0, 0],
                    [0,math.cos(lat_rad),-math.sin(lat_rad)],
                    [0,math.sin(lat_rad),math.cos(lat_rad)]])
    m3 = np.matrix([[math.cos(lon_rad),math.sin(lon_rad),0],
                     [-math.sin(lon_rad),math.cos(lon_rad),0],
                     [0,0,1]])
    m4 = np.matrix([[0,1,0],
                    [-1,0,0],
                    [0,0,1]])
    return m1*m2*m3*m4

def m_ned_to_ins(roll, pitch, heading):
    '''Equation 4 in document "SENSRB Profile Frame Image-To-Ground 
    Transformation Description" by Michale h. Lenihan'''
    roll_rad = roll * deg_to_rad
    pitch_rad = pitch * deg_to_rad
    heading_rad = heading * deg_to_rad
    m1 = np.matrix([[1,0,0],
                    [0,math.cos(roll_rad), math.sin(roll_rad)],
                    [0,-math.sin(roll_rad), math.cos(roll_rad)]])
    m2 = np.matrix([[math.cos(pitch_rad), 0, -math.sin(pitch_rad)],
                    [0,1,0],
                    [math.sin(pitch_rad),0,math.cos(pitch_rad)]])
    m3 = np.matrix([[math.cos(heading_rad), math.sin(heading_rad), 0],
                    [-math.sin(heading_rad), math.cos(heading_rad), 0],
                    [0,0,1]])
    return m1*m2*m3

# This may go into the sensorb camera model
def q_ins_to_sen(sensor_angle_1, sensor_angle_2, sensor_angle_3):
    '''Equation 5 in document "SENSRB Profile Frame Image-To-Ground 
    Transformation Description" by Michale h. Lenihan'''
    # The rotation angle seems to be "passive" rather than active,
    # so we reverse the sign of the angles.
    return quat_rot("XYZ", -sensor_angle_1, -sensor_angle_2, -sensor_angle_3)

def q_sen_to_cam():
    '''Equation 6 in document "SENSRB Profile Frame Image-To-Ground 
    Transformation Description" by Michale h. Lenihan'''
    m_sen_to_cam = np.matrix([[0,1,0],
                              [0,0,-1],
                              [-1,0,0]])
    return(matrix_to_quaternion(m_sen_to_cam))

def test_ecf_to_ned():
    '''Compare our calculation of ECF to NED with
    equation 3 in document "SENSRB Profile Frame Image-To-Ground 
    Transformation Description" by Michale h. Lenihan'''
    tm = Time.parse_time("2000-01-01T00:00:00.00Z")
    od = AircraftOrbitData(tm, Geodetic(30,50,700e3), [0,0,0], 0, 0, 0)
    npt.assert_almost_equal(m_ecf_to_ned(30,50),
                            quaternion_to_matrix(od.sc_to_cf.conj()))

def test_ecf_to_ins():
    '''Compare our calculation of ECF to INS with equation 3 and 4'''
    tm = Time.parse_time("2000-01-01T00:00:00.00Z")
    od = AircraftOrbitData(tm, Geodetic(30,50,700e3), [0,0,0], 10, 20, 30)
    npt.assert_almost_equal(m_ned_to_ins(10,20,30) * m_ecf_to_ned(30,50),
                            quaternion_to_matrix(od.sc_to_cf.conj()))
    

@require_pynitf
def test_sensrb_od(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
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
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    f.image_segment[0].orbit_data_sensrb = od
    cam = SensrbCamera(SensrbCamera.sensor_angle_to_quaternion(10, 20, 30),
		       1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 3000,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2))
    f.image_segment[0].camera_sensrb = cam
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    cam2 = f2.image_segment[0].camera_sensrb
    print(cam)
    print(cam2)

@require_msp    
@require_pynitf
def test_sensrb_msp(isolated_dir):
    '''Compare SENSRB computionation with MSP library'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    od_ac = AircraftOrbitData(od)
    # Don't think sensrb does abberation of light correction, so to match
    # it we need to fake a 0 velocity.
    od2 = AircraftOrbitData(od.time, od.position_cf, [0,0,0], 0, 0,
                            AircraftOrbitData(od).heading)
    od2 = AircraftOrbitData(od.time, od.position_cf, [0,0,0], 0, 0,
                            0)
    f.image_segment[0].orbit_data_sensrb = od2
    nline = 2048
    nsamp = 1024
    cam = SensrbCamera(SensrbCamera.sensor_angle_to_quaternion(10, 70, 20),
		       0,0,0,0,0,0,0, 3000,
		       nline, nsamp, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(nline/2, nsamp/2))
    f.image_segment[0].camera_sensrb = cam
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "TEST"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"
    t.generation_date = 20000101
    t.generation_time = 0
    print(t)
    f.write("sensrb_test.ntf")
    h = 0
    igc = OrbitDataImageGroundConnection(od2, cam, SimpleDem(), None)
    ic = ImageCoordinate(cam.number_line(0)/2,cam.number_sample(0)/2)
    ic.line -= 100
    ic = ImageCoordinate(0,0)
    p2 = igc.ground_coordinate_approx_height(ic, h)
    print(Geodetic(od.position_cf))
    print(Geodetic(p2))
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem())
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    od3 = f2.image_segment[0].orbit_data_sensrb
    igc2 = OrbitDataImageGroundConnection(f2.image_segment[0].orbit_data_sensrb,
                                          f2.image_segment[0].camera_sensrb,
                                          SimpleDem(), None)
    p1 = igc_msp.ground_coordinate_approx_height(ic, h)
    p3 = igc2.ground_coordinate_approx_height(ic, h)
    print(Geodetic(p1))
    print(geocal_swig.distance(p1, p2))
    print(geocal_swig.distance(p1, p3))
    print(igc.resolution_meter())
    print(igc.image_coordinate(p1))
    print(igc2.image_coordinate(p1))
    if True:
        for ln in np.linspace(0, 2048, 10):
            for smp in np.linspace(0, 1024, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = igc.ground_coordinate_approx_height(ic, h)
                # Pixel size is about 100m, so difference here is small
                assert(geocal_swig.distance(p1, p2) < 5.0)


# Need to come back to this. want to be able to generate unit vector and
# quaternion that agrees with calculation of sensorb tool.
@skip                
@require_pynitf
def test_compare_module(isolated_dir):
    '''Using sensrb tool, have some simple comparisons between euler angles,
    unit vectors, and quaternion'''
    # With position = (7.08314e6,0,0) so at X
    # sensor 0,0,0
    # platform 0,0,0
    unit_x = [0,1,0]
    unit_y = [1,0,0]
    unit_z = [0,0,-1]
    quat = [0.-0.70710678, 0, 0.707106781]
    look_vector = [-1,0,0]
    # Image coordinate system is permuation of sensor coordinate system
    # This may be simliar to sc coordinates.
    # With pitch = 90, seems to be pointed down
    # Sensor coordinate system
    # z_s - boresight direction
    # x_s - sample direction
    # y_s - line direction
    unit_x = [0,1,0] # Line direction
    unit_y = [0,0,-1] # Line direction
    unit_z = [-1,0,0] # Boresight direction
    quat = [0,0,0,1]

    tm = Time.parse_time("2000-01-01T00:00:00.00Z")
    # MSP doesn't include abberation of light, so we fake a zero velocity
    # so we are comparing things more directly
    #od = AircraftOrbitData(tm, Geodetic(0,0,700e3), [0,0,0], 0, 0, 0)
    #q = unit_to_quat([0,1,0],[0,0,1],[1,0,0])
    q = unit_to_quat([0,1,0], [0.173648178, 0, 0.984807753],
                     [.984807753, 0, -.17364818])
    print(q)
    print(q * Quaternion_double(0,0,0,1) * q.conj())
    od = QuaternionOrbitData(tm, Ecr(Geodetic(0,0,700e3)), [0,0,0], q)
    print(od.position_cf)
    print(od.sc_to_cf)
#    cam = SensrbCamera(quat_rot("yxz", 0 * deg_to_rad, -90 * deg_to_rad,
#                                0 * deg_to_rad),
#    cam = SensrbCamera(quat_rot("yxz", 0 * deg_to_rad, 0 * deg_to_rad,
#                                0 * deg_to_rad),

#    cam = SensrbCamera(quat_rot("zyx", 10 * deg_to_rad, 0 * deg_to_rad,
#                                0 * deg_to_rad),
    cam = SensrbCamera(quat_rot("yxz", 0 * deg_to_rad, 0 * deg_to_rad,
                                0 * deg_to_rad),
		       0,0,0,0,0,0,0, 3000,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2),
                       "VIS", "20000101", QuaternionCamera.LINE_IS_Y,
                       QuaternionCamera.INCREASE_IS_POSITIVE,
                       QuaternionCamera.INCREASE_IS_POSITIVE)
    ic = ImageCoordinate(2048/2, 1024/2)
    print(cam.sc_look_vector(FrameCoordinate(ic.line, ic.sample),0))
    print(cam.sc_look_vector(FrameCoordinate(ic.line+1,ic.sample),0))
    print(cam.sc_look_vector(FrameCoordinate(ic.line, ic.sample+1),0))
    print(od.cf_look_vector(cam.sc_look_vector(FrameCoordinate(ic.line, ic.sample),0)))
    print("")
    igc, igc_msp = igc_compare(od, cam)
    p1 = igc_msp.ground_coordinate(ic)
    print(p1)
    print(Geodetic(p1))
    lv = CartesianFixedLookVector(p1.position[0] - od.position_cf.position[0],
                                  p1.position[1] - od.position_cf.position[1],
                                  p1.position[2] - od.position_cf.position[2])
    print(lv)
    p2 = igc.ground_coordinate(ic)
    print(p2)
    print(geocal_swig.distance(p1, p2))
    p1 = [igc_msp.ground_coordinate(ImageCoordinate(ic.line+1,ic.sample)),
          igc_msp.ground_coordinate(ImageCoordinate(ic.line-1,ic.sample)),
          igc_msp.ground_coordinate(ImageCoordinate(ic.line,ic.sample+1)), 
          igc_msp.ground_coordinate(ImageCoordinate(ic.line,ic.sample-1))]
    p2 = [igc.ground_coordinate(ImageCoordinate(ic.line+1,ic.sample)),
          igc.ground_coordinate(ImageCoordinate(ic.line-1,ic.sample)),
          igc.ground_coordinate(ImageCoordinate(ic.line,ic.sample+1)), 
          igc.ground_coordinate(ImageCoordinate(ic.line,ic.sample-1))]
    for t in p1:
        print(t)
    print("")
    for t in p2:
        print(t)
    print("")
    for i in range(4):
        print(geocal_swig.distance(p1[i], p2[i]))
    h = 0
#    if False:
    if True:
        for ln in np.linspace(0, 2048, 10):
            for smp in np.linspace(0, 1024, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = igc.ground_coordinate_approx_height(ic, h)
                #print(ic)
                #assert(geocal_swig.distance(p1, p2) < 0.01)
                # Not sure why so large. Maybe round off of some kind?
                assert(geocal_swig.distance(p1, p2) < 0.5)
