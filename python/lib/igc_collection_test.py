from nose.tools import *
from geocal import *
from image_ground_connection import *
from igc_collection import *
from nose.plugins.skip import Skip, SkipTest

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_connection_igc():
    '''Note that this also tests the inheritance in python of a C++ class
    ImageGroundConnection, and passing this back to C++ (the generate_rpc
    command)'''
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    igc = ImageGroundConnectionIgc(igc_coll, 2)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert distance(gp, igc.ground_coordinate(ic)) < 0.01
    ic = igc3.image_coordinate(gp)
    assert_almost_equal(igc.image_coordinate(gp).line, ic.line, 4)
    assert_almost_equal(igc.image_coordinate(gp).sample, ic.sample, 4)
    rpc = Rpc.generate_rpc(igc, -100, 100)
    assert_almost_equal(rpc.image_coordinate(gp).line, ic.line, 4)
    assert_almost_equal(rpc.image_coordinate(gp).sample, ic.sample, 4)

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
    igc1.rpc.fit_line_numerator[0] = True
    igc1.rpc.fit_sample_numerator[0] = True
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc2.rpc.fit_line_numerator[0] = True
    igc2.rpc.fit_sample_numerator[0] = True
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc3.rpc.fit_line_numerator[0] = True
    igc3.rpc.fit_sample_numerator[0] = True
    igc_coll = IgcArray([igc1, igc2, igc3])
    assert igc_coll.number_image == 3
    assert_almost_equal(igc_coll.parameter[0], igc1.parameter[0], 10)
    assert_almost_equal(igc_coll.parameter[3], igc2.parameter[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_ecr(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_ecr(Ecr(gp))).max() < 1e-6
    igc_coll.parameter = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter[0], 1, 10)
    assert_almost_equal(igc2.parameter[1], 4, 10)
