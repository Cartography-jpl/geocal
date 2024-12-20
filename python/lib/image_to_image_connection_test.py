from .image_to_image_connection import *
import pickle
from test_support import *

@require_serialize
def test_rpc_image_ground_connection():
    '''Basic test of Rpc'''
    # A sample RPC. Nothing special about this, these are just reasonable
    # values. Note that this gets created in pickle_test.py if you need to
    # regenerate this for some reason
    with open(unit_test_data + "rpc_example.pkl", "rb") as f:
        rpc1 = pickle.load(f)
    with open(unit_test_data + "rpc_example.pkl", "rb") as f:
        rpc2 = pickle.load(f)
    rpc2.line_offset += 2
    rpc2.sample_offset += 3
    dem = SimpleDem()
    img = MemoryRasterImage(1000,1000)
    c = ImageToImageConnection(RpcImageGroundConnection(rpc1, dem, img),
                               RpcImageGroundConnection(rpc2, dem, img))
    ic1 = ImageCoordinate(10, 20)
    ic2 = ImageCoordinate(ic1.line + 2, ic1.sample + 3)
    assert_almost_equal(c.image_coordinate2(ic1).line, ic2.line, 1)
    assert_almost_equal(c.image_coordinate2(ic1).sample, ic2.sample, 1)
    

