from nose.tools import *
from geocal_swig import *
from image_ground_connection import *
from igc_collection import *
from nose.plugins.skip import Skip, SkipTest
import scipy

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
    igc = ImageGroundConnectionIgc(igc_coll, 2)
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
    assert_almost_equal(igc_coll.parameter[0], igc1.parameter[0], 10)
    assert_almost_equal(igc_coll.parameter[3], igc2.parameter[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter[0], 1, 10)
    assert_almost_equal(igc2.parameter[1], 4, 10)
    if(False):
        '''This is just to compare with the Jacobian from 
        test_igc_array_with_igc_collection below. We manually ran this 
        and compared the results'''
        jac = scipy.sparse.lil_matrix((len(igc_coll.parameter), len(igc_coll.parameter)))
        igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
        print jac

def test_igc_array_with_igc_collection():
    '''Test a IgcArray when one of the entries is a IgcCollection.'''
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
    assert_almost_equal(igc_coll.parameter[0], igc1.parameter[0], 10)
    assert_almost_equal(igc_coll.parameter[3], igc2.parameter[1], 10)
    ic = ImageCoordinate(100, 200)
    gp = igc3.ground_coordinate(ic)
    assert abs(igc_coll.image_coordinate(2, gp).line - ic.line) < 0.1
    assert abs(igc_coll.image_coordinate(2, gp).sample - ic.sample) < 0.1
    assert abs(igc_coll.image_coordinate_jac_cf(2, Ecr(gp)) - 
               igc3.image_coordinate_jac_cf(Ecr(gp))).max() < 1e-6
    igc_coll.parameter = [1,2,3,4,5,6]
    assert_almost_equal(igc1.parameter[0], 1, 10)
    assert_almost_equal(igc2.parameter[1], 4, 10)
    assert igc_coll.image_title(0) == "10MAY21-1.img"
    assert igc_coll.image_title(1) == "10MAY21-2.img"
    assert igc_coll.image_title(2) == "10MAY21-3.img"
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
    jac = scipy.sparse.lil_matrix((len(igc_coll.parameter), len(igc_coll.parameter)))
    igc_coll.image_coordinate_jac_parm(2, gp, jac, 0, 0)
    # We got the result we expect from running the igc_array test above
    assert_almost_equal(jac[0, 4], 15387.7492279, 2)
    assert_almost_equal(jac[1, 5], 17633.137014, 2)
    assert igc_coll.parameter_subset_mask == \
        [True, True, True, True, True, True]
    assert igc_coll.igc[1].parameter_subset_mask == [ True, True, True, True]
    igc_coll.parameter_subset_mask = [False, False, True, True, True, True]
    assert igc_coll.parameter_subset_mask == \
        [False, False, True, True, True, True]
    assert igc_coll.igc[1].parameter_subset_mask == [ True, True, True, True]
    igc_coll.parameter_subset_mask = [False, False, True, False, True, True]
    assert igc_coll.parameter_subset_mask == \
        [False, False, True, False, True, True]
    assert igc_coll.igc[1].parameter_subset_mask == [ True, False, True, True]
 

    
