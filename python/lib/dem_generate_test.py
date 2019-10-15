from test_support import *
from geocal.dem_generate import *
from geocal.image_ground_connection import *
from geocal_swig import VicarLiteRasterImage
import multiprocessing
from multiprocessing import Pool
import pickle

demin = VicarLiteDem(stereo_unit_test_data + "nevada_elv_aoi.img", True)
igc1 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-1.img",
                                  demin)
igc2 = VicarImageGroundConnection(stereo_unit_test_data + "10MAY21-2.img",
                                  demin)
# We are working with raw WV2 data which hasn't been corrected yet for 
# misregistration. We'll eventually code and test an SBA, but for the 
# purpose of these tests just correct the RPC to coregister the data.
igc2.rpc.line_offset -= -0.949821385786
igc2.rpc.sample_offset -= -0.97417620076
gaoi = VicarLiteRasterImage(stereo_unit_test_data + "aoi.img")
gaoi = SubRasterImage(gaoi, 0, 0, 100, 100)
dgen = DemGenerate(igc1, igc2, gaoi.map_info)
simg1 = GdalRasterImage(stereo_unit_test_data + "10MAY21-1_projected.tif")
simg2 = GdalRasterImage(stereo_unit_test_data + "10MAY21-2_projected.tif")
dgen_surf = DemGenerate(igc1, igc2, gaoi.map_info, surface_image1 = simg1,
                        surface_image2 = simg2)

def test_range_img1():
    lstart, sstart, lend, send = dgen.range_image1()
    assert lstart == 498
    assert lend == 581
    assert sstart == 500
    assert send == 577

@require_serialize    
def test_height_grid():
    pool = Pool()
#    pool = None
    print(dgen.height_grid(pool = pool))
#    dgen.plot_res()

@require_serialize    
def test_height_grid_surf():
    pool = Pool()
#    pool = None
    print(dgen_surf.height_grid(pool = pool))
#    dgen.plot_res()

@require_serialize    
def test_pickle():
    t = pickle.dumps(dgen)
    dgen2 = pickle.loads(t)
    r = dgen2.surface_point(550, 550, 551, 551, False)
    assert_almost_equal(r[0][0], 36.77545375201947, 3)
    assert_almost_equal(r[0][1], -116.11898159763552, 3)
    assert_almost_equal(r[0][2], 1294.49, 1)

# Don't normally run this
@skip    
def test_plot():
    pool = Pool()
    dgen.height_grid(pool = pool)
    dgen.plot_res(gaoi)
