from nose.tools import *
from geocal_swig import *
from image_ground_connection import *
from igc_collection_extension import *
from nose.plugins.skip import Skip, SkipTest
import scipy
import numpy.testing as npt

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"

def test_connection_igc():
    '''Note that this also tests the inheritance in python of a C++ class
    ImageGroundConnection, and passing this back to C++ (the generate_rpc
    command)'''
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    igc = igc_coll.image_ground_connection(2)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert distance(gp, igc.ground_coordinate(ic)) < 0.01
    ic = igc3.image_coordinate(gp)
    assert_almost_equal(igc.image_coordinate(gp).line, ic.line, 2)
    assert_almost_equal(igc.image_coordinate(gp).sample, ic.sample, 2)
    rpc = Rpc.generate_rpc(igc, -100, 100)
    assert_almost_equal(rpc.image_coordinate(gp).line, ic.line, 2)
    assert_almost_equal(rpc.image_coordinate(gp).sample, ic.sample, 2)

def test_time():
    raise SkipTest
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    ic = ImageCoordinate(100, 200)
    gp = igc_coll.ground_coordinate(2, ic)
    for i in range(100000):
        t = igc_coll.image_coordinate(2, gp)

def test_igc_array():
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc2.rpc.fit_line_numerator = t
    igc2.rpc.fit_sample_numerator = t
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc3.rpc.fit_line_numerator = t
    igc3.rpc.fit_sample_numerator = t
    igc_coll = IgcArray([igc1, igc2, igc3])
    assert igc_coll.number_image == 3
    assert_almost_equal(igc_coll.parameter_subset[0], 
                        igc1.parameter_subset[0], 10)
    assert_almost_equal(igc_coll.parameter_subset[3], 
                        igc2.parameter_subset[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter_subset = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter_subset[0], 1, 10)
    assert_almost_equal(igc2.parameter_subset[1], 4, 10)
    if(False):
        '''This is just to compare with the Jacobian from 
        test_igc_array_with_igc_collection below. We manually ran this 
        and compared the results'''
        jac = scipy.sparse.lil_matrix((len(igc_coll.parameter_subset), len(igc_coll.parameter_subset)))
        igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
        print jac

# We don't support this anymore. I don't think we actually need this, but
# leave the test in place in case we need to come back to this.
def test_igc_array_with_igc_collection():
    '''Test a IgcArray when one of the entries is a IgcCollection.'''
    raise SkipTest
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc2.rpc.fit_line_numerator = t
    igc2.rpc.fit_sample_numerator = t
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc3.rpc.fit_line_numerator = t
    igc3.rpc.fit_sample_numerator = t
    igc_coll_sub = IgcArray([igc2, igc3])
    igc_coll = IgcArray([igc1, igc_coll_sub])
    assert igc_coll.number_image == 3
    assert_almost_equal(igc_coll.parameter_subset[0], 
                        igc1.parameter_subset[0], 10)
    assert_almost_equal(igc_coll.parameter_subset[3], 
                        igc2.parameter_subset[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter_subset = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter_subset[0], 1, 10)
    assert_almost_equal(igc2.parameter_subset[1], 4, 10)
    assert igc_coll.title(0) == "10MAY21-1.img"
    assert igc_coll.title(1) == "10MAY21-2.img"
    assert igc_coll.title(2) == "10MAY21-3.img"
    assert os.path.basename(igc_coll.image(0).file.file_name) == \
        "10MAY21-1.img"
    assert os.path.basename(igc_coll.image(1).file.file_name) == \
        "10MAY21-2.img"
    assert os.path.basename(igc_coll.image(2).file.file_name) == \
        "10MAY21-3.img"
    assert os.path.basename(igc_coll.image_ground_connection(0).
                            image.file.file_name) == \
                            "10MAY21-1.img"
    assert os.path.basename(igc_coll.image_ground_connection(1).
                            image.file.file_name) == \
                            "10MAY21-2.img"
    assert os.path.basename(igc_coll.image_ground_connection(2).
                            image.file.file_name) == \
                            "10MAY21-3.img"
    jac = scipy.sparse.lil_matrix((len(igc_coll.parameter_subset), 
                                   len(igc_coll.parameter_subset)))
    igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
    # We got the result we expect from running the igc_array test above
    assert_almost_equal(jac[0, 4], 15387.7492279, 2)
    assert_almost_equal(jac[1, 5], 17633.137014, 2)

def test_igc_collection_rolling_shutter():
    '''Test IgcCollectionRollingShutter.'''
    tmin = Time.parse_time("1998-06-30T10:51:28.32Z");
    dem = SimpleDem()
    cam = QuaternionCamera(quat_rot("zyx", 0.1, 0.2, 0.3),
                           3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
                           1.0, FrameCoordinate(1688.0, 1824.5),
                           QuaternionCamera.LINE_IS_Y)
    orb = KeplerOrbit(tmin, tmin + 1000)
    igccol = IgcCollectionRollingShutter(orb, cam, dem)
    for i in range(10):
        t = tmin + 20 * i
        tspace = 1e-3;
        tt = RollingShutterConstantTimeTable(t, 
           t + cam.number_line(0) * tspace, tspace);
        title = "Image %d" % (i+1)
        igccol.add_image(None, tt, title)
    ic = ImageCoordinate(cam.number_line(0) / 2, cam.number_sample(0) / 2)
    igccol.determine_orbit_to_match(ic, 4)
    gp = igccol.ground_coordinate(4, ic)
    for i in range(10):
        assert abs(igccol.image_coordinate(i, gp).line - ic.line) < 0.1
        assert abs(igccol.image_coordinate(i, gp).sample - ic.sample) < 0.1

