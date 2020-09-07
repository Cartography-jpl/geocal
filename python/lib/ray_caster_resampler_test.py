from geocal_swig import *
from geocal.sqlite_shelf import *
from geocal.mmap_file import *
from geocal.ray_caster_resampler_extension import *
from test_support import *
from multiprocessing import Pool

def test_ray_caster_resampler(isolated_dir):
    '''Because of the nature of these tests, we can't really do this well
    at the unit level. We really need a fairly large amount of data along
    with a bit of time to run. So this test was created for the initial
    development, but we don't normally run.

    This could be dusted off and modified to work on any issues that arise.
    
    This requires an existing igc as input, we originally used one of the
    mars test cases.'''
    # This blob is the head of rsm_project 
    igc = read_shelve("/bigdata/smyth/GlasTest/run_glas/ctx1_full_igc.xml")
    mibase = GdalRasterImage("/bigdata/smyth/MiplMarsTest/run/ctx1_full_6m.img").map_info
    dem = read_shelve("/bigdata/smyth/GlasTest/run_glas/dem.xml")
    igc.dem = dem
    resolution = 6.0
    resbase = mibase.resolution_meter
    miscale = mibase.scale(resolution / resbase,
                           resolution / resbase)
    miscale = igc.cover(miscale)
    mp = IgcMapProjected(miscale, igc, 1, -1, False)
    mi = mp.map_info
    mp = None
    pool = Pool(10)
    ray_caster_project("resample.img", igc, mi, pool=pool)

    
