from geocal.image_ground_connection import *
from geocal.shape_file import have_shape_file
import pickle
from test_support import *

dem = SimpleDem()

@require_serialize
def test_rpc_image_ground_pickle():
    img = VicarLiteRasterImage(stereo_unit_test_data + "10MAY21-1.img")
    ig = RpcImageGroundConnection(img.rpc, dem, img)
    t = pickle.dumps(ig.rpc)
    t = pickle.dumps(ig.dem)
    t = pickle.dumps(ig.title)
    t = pickle.dumps(img)
    t = pickle.dumps(ig.image)
    t = pickle.dumps(ig)
    ig2 = pickle.loads(t)

@require_serialize    
def test_vicar_image_ground_connection():
    igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img",
                                      dem)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    t = pickle.dumps(igc1)
    igc2 = pickle.loads(t)
    assert_almost_equal(igc2.parameter_subset[0], 0.003954957)
    assert_almost_equal(igc2.parameter_subset[1], 0.0007519057)
    assert_almost_equal(igc2.rpc.line_numerator[0], 0.003954957)
    assert_almost_equal(igc2.rpc.sample_numerator[0], 0.0007519057)
    igc2.parameter_subset = [1, 2]
    assert_almost_equal(igc2.parameter_subset[0], 1)
    assert_almost_equal(igc2.parameter_subset[1], 2)
    assert_almost_equal(igc2.rpc.line_numerator[0], 1)
    assert_almost_equal(igc2.rpc.sample_numerator[0], 2)

@require_serialize    
def test_gdal_image_ground_connection():
    igc1 = GdalImageGroundConnection(stereo_unit_test_data + "10MAY21-1.tif",
                                     dem)
    t = pickle.dumps(igc1)
    igc2 = pickle.loads(t)

def test_view_angle():
    igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img",
                                      dem)
    ic = ImageCoordinate(igc1.image.number_line / 2.0,
                         igc1.image.number_sample / 2.0)
    zen, azm = igc1.view_angle(ic)
    assert_almost_equal(zen, 33.2911, 2)
    assert_almost_equal(azm, 7.2390, 2)

def test_footprint_geometry():
    igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img",
                                      dem)
    # Depends on options used in build, we might not have this available
    if(not have_shape_file):
        raise SkipTest
    g = igc1.footprint_geometry()
    assert g.IsValid()


    

