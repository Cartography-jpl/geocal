from nose.tools import *
import cPickle
from geocal_swig import *
from igc_offset_correction import *
from nose.plugins.skip import Skip, SkipTest
import numpy.testing as npt

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                       3375, 3648,
                       1.0 / 2500000,
                       1.0 / 2500000,
                       1.0,
                       FrameCoordinate(1688.0, 1824.5),
                       QuaternionCamera.LINE_IS_Y)
cam.fit_focal_length = False
for i in range(cam.number_band):
    cam.fit_principal_point_line(False, i)
    cam.fit_principal_point_sample(False, i)
demin = SimpleDem()
img1 = VicarLiteRasterImage(test_data + "10MAY21-1.img")
img2 = VicarLiteRasterImage(test_data + "10MAY21-2.img")
img3 = VicarLiteRasterImage(test_data + "10MAY21-3.img")

def test_igc():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        HdfOrbit_EciTod_TimeAcs
    except NameError:
        raise SkipTest

    orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "../sample_orbit.h5")
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    img1.time = t1 + 5
    img1.title = "Image 1"
    img2.time = t2
    img2.title = "Image 2"
    img3.time = t2 + 5
    img3.title = "Image 3"
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_time_point(t1)
    orb.insert_time_point(t2)
    orb.insert_time_point(t3)
    igc_coll = IgcOffsetCorrection([img1, img2, img3], cam, demin, orb)
    igc_coll.parameter_subset = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30, cam.euler[0], cam.euler[1], cam.euler[2], cam.line_pitch, cam.sample_pitch]
    npt.assert_almost_equal(igc_coll.parameter_subset, [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30, cam.euler[0], cam.euler[1], cam.euler[2], cam.line_pitch, cam.sample_pitch])
    assert igc_coll.number_image == 3
    igc1 = OrbitDataImageGroundConnection(igc_coll.orbit.orbit_data(img1.time), 
                                          cam, demin, img1, img1.title, 
                                          igc_coll.refraction)
    ic = ImageCoordinate(100, 200)
    gp = igc1.ground_coordinate(ic)
    assert distance(gp, igc_coll.ground_coordinate(0, ic)) < 0.01
    assert_almost_equal(igc_coll.image_coordinate(0, gp).line, ic.line, 4)
    assert_almost_equal(igc_coll.image_coordinate(0, gp).sample, ic.sample, 3)
    t = cPickle.dumps(igc_coll, cPickle.HIGHEST_PROTOCOL)
    igc_coll2 = cPickle.loads(t)
    assert igc_coll2.title(1) == igc_coll.title(1)
    assert_almost_equal(igc_coll2.image_coordinate(0, gp).line, ic.line, 4)
    assert_almost_equal(igc_coll2.image_coordinate(0, gp).sample, ic.sample, 3)

def test_igc_jac():
    try:
        # Depending on the options used when building, this class might
        # not be available. If not, then just skip this test.
        HdfOrbit_EciTod_TimeAcs
    except NameError:
        raise SkipTest

    orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "../sample_orbit.h5")
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    img1.time = t1 + 5
    img1.title = "Image 1"
    img2.time = t2
    img2.title = "Image 2"
    img3.time = t2 + 5
    img3.title = "Image 3"
    orb = OrbitOffsetCorrection(orb_uncorr)
    orb.insert_time_point(t1)
    orb.insert_time_point(t2)
    orb.insert_time_point(t3)

    igc_coll = IgcOffsetCorrection([img1, img2, img3], cam, demin, orb)
    igc_coll.parameter_subset = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30, cam.euler[0], cam.euler[1], cam.euler[2], cam.line_pitch, cam.sample_pitch]

    gp = igc_coll.ground_coordinate(0, ImageCoordinate(100, 100))
    jac = np.zeros((2, len(igc_coll.parameter_subset)))
    igc_coll.image_coordinate_jac_parm(0, gp, jac, 0, 0)
    ic0 = igc_coll.image_coordinate(0, gp)
    eps = [1, 1, 1, 10, 10, 10, 10, 10, 10, 10, 10, 10, 0.0001 * deg_to_rad,
           0.0001 * deg_to_rad, 0.0001 * deg_to_rad, 0.01 * cam.line_pitch,
           0.01 * cam.sample_pitch]
    p0 = np.array(igc_coll.parameter_subset)
    jac_fd = np.zeros((2, len(igc_coll.parameter_subset)))
    for i in range(len(igc_coll.parameter_subset)):
        p = p0.copy()
        p[i] += eps[i]
        igc_coll.parameter_subset = p
        ic = igc_coll.image_coordinate(0, gp)
        jac_fd[0,i] = (ic.line - ic0.line) / eps[i]
        jac_fd[1,i] = (ic.sample - ic0.sample) / eps[i]
        igc_coll.parameter_subset = p0
    # Finite difference and real jacobian won't be the same, something
    # like 1% would be a good value. So we check the scaled difference, 
    # being careful not to divide by zero
    scl = jac.copy()
    scl[jac != 0] = 1 / jac[jac != 0]
    assert abs((jac - jac_fd) * scl).max() < 1e-2
        

