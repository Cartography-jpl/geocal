from nose.tools import *
from geocal import *
from tie_point_collect import *
from image_ground_connection import *
import multiprocessing
from multiprocessing import Pool
from nose.plugins.skip import Skip, SkipTest
import cPickle
import safe_matplotlib_import
import matplotlib.pyplot as plt

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"
demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
igc3 = VicarImageGroundConnection(test_data + "10MAY21-3.img", demin)
igc_coll = IgcArray([igc1, igc2, igc3])
gaoi = VicarLiteRasterImage(test_data + "aoi.img").map_info
ref_img = VicarLiteRasterImage(test_data + "nevada_doq_aoi.img")

tp_collect = TiePointCollect(igc_coll)
gtp_collect = GcpTiePointCollect(ref_img, demin, igc_coll)

def test_tp():
    ic = igc1.image_coordinate(demin.surface_point(gaoi.ground_coordinate(550, 550)))
    ic.line = round(ic.line)
    ic.sample= round(ic.sample)
    tp = tp_collect.tie_point(ic)
    assert distance(tp.ground_location, Geodetic(36.7731567549,
                    -116.116742172, 1276.29944154)) < 0.01

def test_tie_point_grid():
    pool = Pool()
    tpcol = tp_collect.tie_point_grid(10, 10, aoi = gaoi, dem = demin,
                                      pool = pool)
    assert len(tpcol) == 95

def test_gp_point_grid():
    pool = Pool()
    tpcol = gtp_collect.tie_point_grid(10, 10, pool = pool)
    assert len(tpcol) == 53

def test_pickle():
    t = cPickle.dumps(tp_collect)
    t = cPickle.dumps(gtp_collect)

def test_show_image():
    raise SkipTest
    tp = tp_collect.tie_point(ImageCoordinate(500, 500))
    tp.display(igc_coll)
    plt.show()

def test_show_ref_image():
    raise SkipTest
    tp = gtp_collect.tie_point_grid(10, 10)
    tp[0].display(igc_coll, ref_image = gtp_collect.sub_ref_image)
    plt.show()


