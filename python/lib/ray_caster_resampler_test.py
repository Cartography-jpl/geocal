from geocal_swig import *
from geocal.sqlite_shelf import *
from geocal.mmap_file import *
from test_support import *

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
    # Common special case it to not have an integration time. In that case,
    # no reason to include multiple integration times
    if(igc.ipi.camera.integration_time(igc.ipi.band) == 0):
        nintegration_step = 1
    else:
        nintegration_step = 2
    rcast = IgcRayCaster(igc, 0, -1, nintegration_step, mi.resolution_meter)
    rcast_data = np.empty((igc.number_line, rcast.shape(1), rcast.shape(2),
                           rcast.shape(3), rcast.shape(4), 2), dtype = np.int32)
    rsamp = RayCasterResampler(rcast, mi)
    rsamp.ray_cast_step(rcast_data)
    res = np.zeros((mi.number_y_pixel, mi.number_x_pixel), dtype=np.int32)
    scratch_count = np.zeros_like(res)
    rsamp.final_rad_step(igc.image, rcast_data, res, scratch_count)
    out = mmap_file("resample.img", mi, dtype=np.int16)
    out[:,:] = res[:,:]

    
