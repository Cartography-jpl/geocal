from nose.tools import *
import cPickle
from geocal import *
from igc_offset_correction import *

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

orb_uncorr = HdfOrbit_EciTod_TimeAcs(test_data + "../sample_orbit.h5")
cam = QuaternionCamera(Quaternion_double(1,0,0,0),
                       3375, 3648,
                       1.0 / 2500000,
                       1.0 / 2500000,
                       1.0,
                       FrameCoordinate(1688.0, 1824.5),
                       1.0, 1.0, QuaternionCamera.LINE_IS_Y)
demin = SimpleDem()
img1 = VicarLiteRasterImage(test_data + "10MAY21-1.img")
img2 = VicarLiteRasterImage(test_data + "10MAY21-2.img")
img3 = VicarLiteRasterImage(test_data + "10MAY21-3.img")

def test_igc():
    t2 = Time.time_acs(215077459.472);
    t1 = t2 - 10
    t3 = t2 + 10
    img1.time = t1 + 5
    img1.title = "Image 1"
    img2.time = t2
    img2.title = "Image 2"
    img3.time = t2 + 5
    img3.title = "Image 3"
    igc_coll = IgcOffsetCorrection([img1, img2, img3], cam, demin,
                                   orb_uncorr, time_point = [t1, t2, t3])
    igc_coll.parameter_subset = [100, 200, 300, 50, 20, 30, 20, 40, 60, -10, -20, -30]
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
    assert igc_coll2.image_title(1) == igc_coll.image_title(1)
    assert_almost_equal(igc_coll2.image_coordinate(0, gp).line, ic.line, 4)
    assert_almost_equal(igc_coll2.image_coordinate(0, gp).sample, ic.sample, 3)


