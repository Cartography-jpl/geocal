from nose.tools import *
from geocal import *
from orbit_offset_correction import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../../unit_test_data/"

orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "sample_orbit.h5")
cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                       3375, 3648,
                       1.0 / 2500000,
                       1.0 / 2500000,
                       1.0,
                       FrameCoordinate(1688.0, 1824.5),
                       1.0, 1.0, QuaternionCamera.LINE_IS_Y)

def test_time():
    raise SkipTest
    t = Time.time_acs(215077459.472);
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(orb_uncorr.orbit_data(t),
                                         cam, SimpleDem(), img, "Image 1")
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    for i in range(100000):
        t = igc.image_coordinate(gp)
    
def test_orbit_offset_unchanged():
    '''Test orbit where we just forward everything. This makes sure that
    passing through python and C++ works correctly'''
    orb = OrbitOffsetCorrection(orb_uncorr)
    t = Time.time_acs(215077459.472);
    pt = orb.reference_surface_intersect_approximate(t, cam, 
                                        FrameCoordinate(3375, 3648))
    pt2 = orb_uncorr.reference_surface_intersect_approximate(t, cam, 
                                        FrameCoordinate(3375, 3648))
    assert distance(pt, pt2) < 0.01

def test_orbit_offset_pos():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.time_point = [t1, t2, t3]
    assert len(orb.parameter) == 3 + 3 * 3
    orb.parameter = [100, 200, 300, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    assert_almost_equal(orb.parameter[0], 100, 4)
    assert_almost_equal(orb.parameter[1], 200, 4)
    assert_almost_equal(orb.parameter[2], 300, 4)
    pdiff = np.zeros(3)
    for i in range(3):
        pdiff[i] = orb.position_ci(t1).position[i] - \
            orb_uncorr.position_ci(t1).position[i]
    assert_almost_equal(pdiff[0], 100, 4)
    assert_almost_equal(pdiff[1], 200, 4)
    assert_almost_equal(pdiff[2], 300, 4)

def test_orbit_quaternion_correction():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr, time_point = [t1, t2, t3])
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, 20, 40, 
                     60, -10, -20, -30]
    q = orb.quaternion_correction(t1 + 5)
    assert_almost_equal(q.R_component_1, 0.959964, 4)
    assert_almost_equal(q.R_component_2, 0.13533, 4)
    assert_almost_equal(q.R_component_3, 0.0834714, 4)
    assert_almost_equal(q.R_component_4, 0.230623, 4)
    # Make sure we can call for each time
    orb.quaternion_correction(t1)
    orb.quaternion_correction(t2)
    orb.quaternion_correction(t3)

def test_insert_time_point():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.parameter = [0, 0, 0]
    orb.insert_time_point(t1)
    assert (orb.parameter == [0, 0, 0, 0, 0, 0]).all()
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600]
    orb.insert_time_point(t3)
    assert (orb.parameter == [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600,
                              0, 0, 0]).all()
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, -10, -20, -30]
    orb.insert_time_point(t2)
    assert (orb.parameter == [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, 0, 0, 0,
                              -10, -20, -30]).all()
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, 20, 40, 
                     60, -10, -20, -30]
    q = orb.quaternion_correction(t1 + 5)
    assert_almost_equal(q.R_component_1, 0.959964, 4)
    assert_almost_equal(q.R_component_2, 0.13533, 4)
    assert_almost_equal(q.R_component_3, 0.0834714, 4)
    assert_almost_equal(q.R_component_4, 0.230623, 4)
    # Make sure we can call for each time
    orb.quaternion_correction(t1)
    orb.quaternion_correction(t2)
    orb.quaternion_correction(t3)

def test_image_coordinate():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr, time_point = [t1, t2, t3])
    orb.parameter = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30]
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(orb.orbit_data(t1 + 5), 
                                         cam, SimpleDem(), img)
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    i0 = orb.image_coordinate(t1 + 5, gp, cam)


