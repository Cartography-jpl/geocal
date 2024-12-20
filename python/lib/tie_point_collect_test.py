from geocal_swig import *
from .tie_point_collect import *
from .image_ground_connection import *
from .sqlite_shelf import *
import multiprocessing
from multiprocessing import Pool
import pickle
from .safe_matplotlib_import import *
import matplotlib.pyplot as plt
from test_support import *

demin = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img", demin)
igc2 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-2.img", demin)
igc3 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-3.img", demin)
igc_coll = IgcArray([igc1, igc2, igc3])
gaoi = VicarLiteRasterImage(stereo_unit_test_data + "aoi.img").map_info
ref_img = VicarLiteRasterImage(stereo_unit_test_data + "nevada_doq_aoi.img")

tp_collect = TiePointCollect(igc_coll)

def test_tp():
    ic = igc1.image_coordinate(demin.surface_point(gaoi.ground_coordinate(550, 550)))
    ic.line = round(ic.line)
    ic.sample= round(ic.sample)
    tp,diag = tp_collect.tie_point(ic)
    assert distance(tp.ground_location, Geodetic(36.7731575234,
                    -116.116743146, 1276.36490196)) < 0.1

@require_serialize    
def test_tie_point_grid():
    pool = Pool()
    tpcol, diag = tp_collect.tie_point_grid(10, 10, aoi = gaoi, dem = demin,
                                      pool = pool)
    print(tpcol)
    diag.print_report()
    assert len(tpcol) == 95

@require_serialize    
def test_pickle():
    t = pickle.dumps(tp_collect)

@skip    
def test_show_image():
    tp,diag = tp_collect.tie_point(ImageCoordinate(500, 500))
    tp.display(igc_coll)
    plt.show()

# Data is way too big to check into source, so we put it here. This means
# we can only tests that depend on this on pistol. We may fold this into
# the afids data area at some point.
# This takes about 2 minutes to run. This isn't that long, but is a bit too
# long for a unit test. So normally skip this.
@skip
@require_serialize
@require_geocal_test_data
def test_fm():
    '''Test tiepoint generation using feature matching.'''
    if(not have_cv2):
        raise SkipTest
    igccol = read_shelve(geocal_test_igc)
    ref_image = VicarLiteRasterImage(geocal_test_data + "ref.img")
    tp_collect = TiePointCollectFM(igccol, ref_image = ref_image,
                                   max_ground_covariance = 200 ** 2)
    # Parallel doesn't work yet.
    pool = None
    tpcol = tp_collect.tie_point_list(pool = pool)
    print(tpcol)
