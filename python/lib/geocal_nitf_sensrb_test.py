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
import io
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
    can then compare between them.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    f.image_segment[0].orbit_data_and_camera(od, cam)
    t = f.image_segment[0].find_exactly_one_tre("SENSRB")
    t.sensor = "TEST"
    t.platform = "fake_platform"
    t.operation_domain = "Spaceborne"
    t.generation_date = 20000101
    t.generation_time = 0
    # Temp
    #t.principal_point_offset_x = 1.8e-5
    #t.principal_point_offset_x = 21e-6 * 1e3
    f.write("sensrb_test.ntf")
    # MSP doesn't do aberration of light correction, so to match
    # it we need to fake a 0 velocity to directly compare
    od2 = AircraftOrbitData(od.time, od.position_cf, [0,0,0],
                            od.roll, od.pitch, od.heading)
    igc = OrbitDataImageGroundConnection(od2, cam, SimpleDem(), None)
    igc_msp = IgcMsp("sensrb_test.ntf", SimpleDem())
    return igc, igc_msp

@require_pynitf
def test_sensrb_cam_od(isolated_dir):
    '''Create a file, and write out a SENSRB, then make sure we can read it.'''
    f = pynitf.NitfFile()
    create_image_seg(f)
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    cam = SensrbCamera(SensrbCamera.sensor_angle_to_quaternion(10, 70, 20),
		       0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 3000,
		       2048, 1024, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(2048/2, 1024/2))
    f.image_segment[0].orbit_data_and_camera(od, cam)
    f.write("sensrb_test.ntf")
    f2 = pynitf.NitfFile("sensrb_test.ntf")
    od2 = f2.image_segment[0].orbit_data_sensrb
    cam2 = f2.image_segment[0].camera_sensrb
    fc = FrameCoordinate(0,752)
    dem = SimpleDem()
    assert(distance(od.surface_intersect(cam, fc, dem),
                    od2.surface_intersect(cam2, fc, dem)) < 1.0)


@require_msp    
@require_pynitf
def test_sensrb_msp(isolated_dir):
    '''Compare SENSRB computionation with MSP library'''
    t = Time.parse_time("1998-06-30T10:51:28.32Z")
    korb = KeplerOrbit(t, t + 100.0)
    od = korb.orbit_data(t + 5)
    od_ac = AircraftOrbitData(od)
    # Add some roll and pitch to make sure this gets accounted for correctly.
    od = AircraftOrbitData(od_ac.time, od_ac.position_cf, [0,0,0], 10, 20,
                           od_ac.heading)
    
    nline = 2048
    nsamp = 1024
    cam = SensrbCamera(SensrbCamera.sensor_angle_to_quaternion(10, 70, 20),
		       0,0,0,0,0,0,0, 3000,
		       nline, nsamp, 18e-6, 21e-6,
		       123.8e-3, FrameCoordinate(nline/2, nsamp/2))
    igc, igc_msp = igc_compare(od, cam)
    # Compare also with igc we directly read from pynitf. Should be
    # the same as igc, up to round off, but we want to check that.
    f = pynitf.NitfFile("sensrb_test.ntf")
    # Write out tre as string
    fh2 = io.BytesIO()
    t = f.image_segment[0].find_one_tre("SENSRB")
    t.write_to_file(fh2)
    print(fh2.getvalue())
    print(f.image_segment[0].attitude_quaternion_sensrb)
    # MSP doesn't do aberration of light correction, so to match
    # it we need to fake a 0 velocity to directly compare
    od2 = f.image_segment[0].orbit_data_sensrb
    od2 = AircraftOrbitData(od2.time, od2.position_cf, [0,0,0],
                            od2.roll, od2.pitch, od2.heading)
    igc2 = OrbitDataImageGroundConnection(od2,
                                          f.image_segment[0].camera_sensrb,
                                          SimpleDem(), None)
    h = 0
    if(True):
        ic = ImageCoordinate(0,0)
        p1 = igc_msp.ground_coordinate_approx_height(ic, h)
        p2 = igc.ground_coordinate_approx_height(ic, h)
        #p3 = igc2.ground_coordinate_approx_height(ic, h)
        print(f.image_segment[0].find_exactly_one_tre("SENSRB"))
        print(Geodetic(od.position_cf))
        print(Geodetic(p2))
        print(Geodetic(p1))
        print(geocal_swig.distance(p1, p2))
        #print(geocal_swig.distance(p1, p3))
        print(igc.resolution_meter())
        print(igc.image_coordinate(p1))
        #print(igc2.image_coordinate(p1))
    if True:
        for ln in np.linspace(0, 2048, 10):
            for smp in np.linspace(0, 1024, 10):
                ic = ImageCoordinate(ln,smp)
                p1 = igc_msp.ground_coordinate_approx_height(ic, h)
                p2 = igc.ground_coordinate_approx_height(ic, h)
                p3 = igc2.ground_coordinate_approx_height(ic, h)
                # Pixel size is about 100m, so difference here is small
                assert(geocal_swig.distance(p1, p2) < 5.0)
                assert(geocal_swig.distance(p2, p3) < 5.0)


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
    # MSP doesn't include aberration of light coorection, so we fake a
    # zero velocity so we are comparing things more directly
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
