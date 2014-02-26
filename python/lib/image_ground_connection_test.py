from nose.tools import *
from image_ground_connection import *
import cPickle

test_data = os.path.dirname(__file__) + "/../../unit_test_data/Stereo/"
dem = SimpleDem()

def test_rpc_image_ground_pickle():
    img = VicarLiteRasterImage(test_data + "10MAY21-1.img")
    ig = RpcImageGroundConnection(img.rpc, dem, img)
    t = cPickle.dumps(ig.rpc)
    t = cPickle.dumps(ig.dem)
    t = cPickle.dumps(ig.title)
    t = cPickle.dumps(img)
    t = cPickle.dumps(ig.image)
    t = cPickle.dumps(ig)
    ig2 = cPickle.loads(t)

def test_vicar_image_ground_connection():
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", dem)
    t = [False] * 20
    t[0] = True
    igc1.rpc.fit_line_numerator = t
    igc1.rpc.fit_sample_numerator = t
    t = cPickle.dumps(igc1)
    igc2 = cPickle.loads(t)
    assert_almost_equal(igc2.parameter[0], 0.003954957)
    assert_almost_equal(igc2.parameter[1], 0.0007519057)
    assert_almost_equal(igc2.rpc.line_numerator[0], 0.003954957)
    assert_almost_equal(igc2.rpc.sample_numerator[0], 0.0007519057)
    igc2.parameter = [1, 2]
    assert_almost_equal(igc2.parameter[0], 1)
    assert_almost_equal(igc2.parameter[1], 2)
    assert_almost_equal(igc2.rpc.line_numerator[0], 1)
    assert_almost_equal(igc2.rpc.sample_numerator[0], 2)

def test_gdal_image_ground_connection():
    igc1 = GdalImageGroundConnection(test_data + "10MAY21-1.img", dem)
    t = cPickle.dumps(igc1)
    igc2 = cPickle.loads(t)

def test_view_angle():
    igc1 = GdalImageGroundConnection(test_data + "10MAY21-1.img", dem)
    ic = ImageCoordinate(igc1.image.number_line / 2.0,
                         igc1.image.number_sample / 2.0)
    zen, azm = igc1.view_angle(ic)
    assert_almost_equal(zen, 33.2911, 2)
    assert_almost_equal(azm, 7.2390, 2)

def test_footprint_geometry():
    igc1 = GdalImageGroundConnection(test_data + "10MAY21-1.img", dem)
    g = igc1.footprint_geometry()
    assert g.IsValid()


    

