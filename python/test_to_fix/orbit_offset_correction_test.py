from builtins import range
from test_support import *

try:
    # Depending on the options used when building, this class might
    # not be available. If not, then just skip this test.
    orb_uncorr = HdfOrbit_EciTod_TimeAcs(unit_test_data + "sample_orbit.h5")
except NameError:
    orb_uncorr = None

require_orb_uncorr = pytest.mark.skipif(not orb_uncorr,
                                   reason = "Need to have HDF orbit to run.")

cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                       3375, 3648,
                       1.0 / 2500000,
                       1.0 / 2500000,
                       1.0,
                       FrameCoordinate(1688.0, 1824.5),
                       QuaternionCamera.LINE_IS_Y)

@require_orb_uncorr
def test_time():
    t = Time.time_acs(215077459.472);
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(orb_uncorr.orbit_data(t),
                                         cam, SimpleDem(), img, "Image 1")
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    for i in range(100000):
        t = igc.image_coordinate(gp)

@require_orb_uncorr
def test_time_rolling():
    '''Test that uses a rolling shutter. This hits the orbit data much more
    than a OrbitDataImageGroundConnection, so we'll check the timing for 
    this.'''
    #orb = OrbitOffsetCorrection(orb_uncorr)
    orb = orb_uncorr
    t = Time.time_acs(215077459.472);
    tspace = 1e-3
    tt = RollingShutterConstantTimeTable(t, t + cam.number_line(0) * tspace,
                                         tspace)
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = IgcRollingShutter(orb, tt, cam, SimpleDem(), img, 
                            IgcRollingShutter.ROLL_LINE_DIRECTION, "Image 1")
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    for i in range(1000):
        t = igc.image_coordinate(gp)
    
@require_orb_uncorr
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

@require_orb_uncorr
def test_orbit_offset_pos():
    t2 = Time.time_acs(215077459.472)
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_position_time_point(t1)
    orb.insert_attitude_time_point(t1)
    orb.insert_attitude_time_point(t2)
    orb.insert_attitude_time_point(t3)
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

@require_orb_uncorr
def test_insert_attitude_time_point():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_position_time_point(t1)
    orb.parameter = [0, 0, 0]
    orb.insert_attitude_time_point(t1)
    assert (orb.parameter == [0, 0, 0, 0, 0, 0]).all()
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600]
    orb.insert_attitude_time_point(t3)
    assert_almost_equal(orb.parameter,
                           [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600,
                            0, 0, 0])
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, -10, -20, -30]
    orb.insert_attitude_time_point(t2)
    assert_almost_equal(orb.parameter,
                           [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, 0, 0, 0,
                            -10, -20, -30])
    orb.parameter = [0, 0, 0, 50 * 3600, 20 * 3600, 30 * 3600, 20, 40, 
                     60, -10, -20, -30]

@require_orb_uncorr
def test_frame_coordinate():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_position_time_point(t1)
    orb.insert_attitude_time_point(t1)
    orb.insert_attitude_time_point(t2)
    orb.insert_attitude_time_point(t3)
    orb.parameter = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30]
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(orb.orbit_data(t1 + 5), 
                                         cam, SimpleDem(), img)
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    i0 = orb.frame_coordinate(t1 + 5, gp, cam)
    assert_almost_equal(ic.line, i0.line, 4)
    assert_almost_equal(ic.sample, i0.sample, 4)

@require_orb_uncorr
def test_observer():
    '''Check that we properly notify other objects when parameter changes'''
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_position_time_point(t1)
    orb.insert_attitude_time_point(t1)
    orb.insert_attitude_time_point(t2)
    orb.insert_attitude_time_point(t3)
    orb.parameter = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30]
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    igc = OrbitDataImageGroundConnection(orb, t1 + 5, cam, SimpleDem(), img)
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    i0 = orb.frame_coordinate(t1 + 5, gp, cam)
    assert_almost_equal(ic.line, i0.line, 4)
    assert_almost_equal(ic.sample, i0.sample, 4)
    orb.parameter = [100, 200, 300, 60, 30, 40, 20, 40, 60, -10, -20, -30]
    gp = igc.ground_coordinate(ic)
    i0 = orb.frame_coordinate(t1 + 5, gp, cam)
    assert_almost_equal(ic.line, i0.line, 4)
    assert_almost_equal(ic.sample, i0.sample, 4)
    
@require_orb_uncorr
def test_frame_coordinate_with_der():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_position_time_point(t1)
    orb.insert_attitude_time_point(t1)
    orb.insert_attitude_time_point(t2)
    orb.insert_attitude_time_point(t3)
    orb.parameter = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30]
    img = MemoryRasterImage(cam.number_line(0), cam.number_sample(0))
    # We have 20 parameters, 8 in the camera 12 in orbit
    cam_jac = np.zeros((8, 20))
    orb_jac = np.zeros((12, 20))
    for i in range(cam_jac.shape[0]):
        cam_jac[i,i] = 1
    for i in range(orb_jac.shape[0]):
        orb_jac[i, i + cam_jac.shape[0]] = 1
    cam.parameter_with_derivative = ArrayAd_double_1(cam.parameter, cam_jac)
    orb.parameter_with_derivative = ArrayAd_double_1(orb.parameter, orb_jac)
    igc = OrbitDataImageGroundConnection(orb.orbit_data(t1 + 5), 
                                         cam, SimpleDem(), img)
    ic = ImageCoordinate(100, 200)
    gp = igc.ground_coordinate(ic)
    ic0 = orb.frame_coordinate_with_derivative(TimeWithDerivative(t1 + 5), gp, cam)
    assert_almost_equal(ic.line, ic0.line.value, 4)
    assert_almost_equal(ic.sample, ic0.sample.value, 4)
    # Do camera differences first. This checks correct propagation
    # through the orbit.
    jac = np.zeros((2, 20))
    jac[0,:] = ic0.line.gradient
    jac[1,:] = ic0.sample.gradient
    jac_fd = np.zeros((2, 20))
    eps = [0.00001, 0.00001, 0.00001, 1e-9, 1e-9, 0.001, 0.1, 0.1]
    p0 = cam.parameter.copy()
    for i in range(len(eps)):
        pdelta = np.zeros((8,))
        pdelta[i] = eps[i]
        cam.parameter = p0 + pdelta
        ic = orb.frame_coordinate(t1 + 5, gp, cam)
        jac_fd[0, i] = (ic.line - ic0.line.value) / eps[i]
        jac_fd[1, i] = (ic.sample - ic0.sample.value) / eps[i]
    cam.parameter = p0
    eps = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 ,1]
    p0 = orb.parameter.copy()
    for i in range(len(eps)):
        pdelta = np.zeros((12,))
        pdelta[i] = eps[i]
        orb.parameter = p0 + pdelta
        ic = orb.frame_coordinate(t1 + 5, gp, cam)
        jac_fd[0, 8 + i] = (ic.line - ic0.line.value) / eps[i]
        jac_fd[1, 8 + i] = (ic.sample - ic0.sample.value) / eps[i]
    # Finite difference and real jacobian won't be the same, something
    # like 1% would be a good value. So we check the scaled difference, 
    # being careful not to divide by zero
    scl = jac.copy()
    scl[jac != 0] = 1 / jac[jac != 0]
    assert abs((jac - jac_fd) * scl).max() < 1e-2

