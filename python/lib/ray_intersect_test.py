from nose.tools import *
from igc_collection import *
from ray_intersect import *
from image_ground_connection import *
from tie_point import *

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

def test_ray_intersect():
    initial_height = 1291
    dem = SimpleDem(initial_height)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", dem)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", dem)
    igc_coll = IgcArray([igc1, igc2])
    ri = RayIntersect2(igc_coll)
    ic1 = ImageCoordinate(550, 550)
    ic2 = ImageCoordinate(553.659, 553.99)
    p, dist = ri.two_ray_intersect(ic1, ic2)
    assert_almost_equal(dist, 0.359094, 4)
    assert distance(p, Geodetic(36.7754303197, -116.118981291, 1290.64)) < 0.01

def test_tp():
    demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
    igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
    igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
    igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
    igc_coll = IgcArray([igc1, igc2, igc3])
    ri = RayIntersect2(igc_coll)
    tp = TiePoint(3)
    tp.image_location[0] = ImageCoordinate(944, 916), 0.1, 0.1
    tp.image_location[1] = ImageCoordinate(975.65, 934.365), 0.1, 0.1
    tp.image_location[2] = ImageCoordinate(1007.83, 950.128), 0.1, 0.1
    ri.ray_intersect(tp)
    assert distance(tp.ground_location, Geodetic(36.7731550801,
                                                 -116.116742412, 1276.05)) < 1
    
