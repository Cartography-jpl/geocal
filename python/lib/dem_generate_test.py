from nose.tools import *
from nose.plugins.skip import Skip, SkipTest
from geocal import *
from dem_generate import *
from image_ground_connection import *
import multiprocessing
from multiprocessing import Pool
import cPickle

test_data = os.path.dirname(__file__) + "/../unit_test_data/Stereo/"

demin = VicarLiteDem(test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(test_data + "10MAY21-1.img", demin)
igc2 = VicarImageGroundConnection(test_data + "10MAY21-2.img", demin)
# We are working with raw WV2 data which hasn't been corrected yet for 
# misregistration. We'll eventually code and test an SBA, but for the 
# purpose of these tests just correct the RPC to coregister the data.
igc2.rpc.line_offset -= -0.949821385786
igc2.rpc.sample_offset -= -0.97417620076
gaoi = VicarLiteRasterImage(test_data + "aoi.img")
gaoi = SubRasterImage(gaoi, 0, 0, 100, 100)
dgen = DemGenerate(igc1, igc2, gaoi.map_info)
simg1 = GdalRasterImage(test_data + "10MAY21-1_projected.tif")
simg2 = GdalRasterImage(test_data + "10MAY21-2_projected.tif")
dgen_surf = DemGenerate(igc1, igc2, gaoi.map_info, surface_image1 = simg1,
                        surface_image2 = simg2)

def test_range_img1():
    lstart, sstart, lend, send = dgen.range_image1()
    assert lstart == 498
    assert lend == 581
    assert sstart == 500
    assert send == 577

def test_height_grid():
    pool = Pool()
#    pool = None
    print dgen.height_grid(pool)
#    dgen.plot_res()

def test_height_grid_surf():
    pool = Pool()
#    pool = None
    print dgen_surf.height_grid(pool)
#    dgen.plot_res()

def test_pickle():
    t = cPickle.dumps(dgen)
    dgen2 = cPickle.loads(t)
    r = dgen2.surface_point(550, 550, 551, 551)
    assert_almost_equal(r[0][0], 36.77545375201947, 3)
    assert_almost_equal(r[0][1], -116.11898159763552, 3)
    assert_almost_equal(r[0][2], 1294.49, 1)

def test_plot():
    # Don't normally run this
    raise SkipTest
    pool = Pool()
    dgen.height_grid(pool)
    dgen.plot_res(gaoi)
