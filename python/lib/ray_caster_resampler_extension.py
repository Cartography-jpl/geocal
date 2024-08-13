from geocal_swig import *
from .mmap_file import *
import numpy as np
from tempfile import NamedTemporaryFile
from functools import partial
import logging

def _ray_caster_project_subset(igc, number_line_framelet,
                               nline_process, nintegration_step,
                               map_info, scratch_file_name,
                               nsub_line, nsub_sample,
                               start_line):
    nline = min(nline_process, igc.number_line - start_line)
    rcast = IgcRayCaster(igc, number_line_framelet, start_line,
                         nline, nintegration_step,
                         map_info.resolution_meter)
    rcast.number_sub_line = nsub_line
    rcast.number_sub_sample = nsub_sample
    rcast_data = np.memmap(scratch_file_name, mode="r+",
                           shape = (igc.number_line, rcast.shape(1),
                                    rcast.shape(2),
                                    rcast.shape(3), rcast.shape(4), 2),
                           dtype = np.int32)
    log = logging.getLogger("geocal-python.ray_caster_resampler")
    log.info(f"Processing {start_line} to {start_line+nline}")
    rsamp = RayCasterResampler(rcast, map_info)
    rsamp.ray_cast_step(rcast_data)
    
    
def ray_caster_project(out_fname, igc, map_info, dtype=np.int16,
                       mmap_scratch=False, pool = None,
                       nline_process=100,
                       nsub_line=-1,nsub_sample=-1):
    '''This puts together everything to generate map projected data.
    We write the output to the given file, as a VICAR file of given
    dtype.

    This uses scratch data which can either be kept all in memory, or
    for larger images we can optionally use temporary files for the 
    scratch data. This is controlled by mmap_scratch, if True we use
    scratch file otherwise we just use memory.

    Note for parallel processing, the mmap_scratch will be used.
    '''
    # Common special case it to not have an integration time. In that case,
    # no reason to include multiple integration times
    if(igc.ipi.camera.integration_time(igc.ipi.band) == 0):
        nintegration_step = 1
    else:
        nintegration_step = 2
    number_line_framelet = igc.ipi.camera.number_line(igc.ipi.band)
    # Adjust nline_process to be a even multiple of number_line_framelet,
    # required by ray caster
    nline_process = int(round(nline_process / number_line_framelet)) * number_line_framelet
    rcast = IgcRayCaster(igc, number_line_framelet, 0, -1, nintegration_step,
                         map_info.resolution_meter)
    if(nsub_line > 0):
        rcast.number_sub_line = nsub_line
    if(nsub_sample > 0):
        rcast.number_sub_sample = nsub_sample
    log = logging.getLogger("geocal-python.ray_caster_resampler")        
    log.info(f"Number subpixel {rcast.number_sub_line} x {rcast.number_sub_sample}")
    with NamedTemporaryFile(dir=os.path.dirname(out_fname)) as fh:
        if(mmap_scratch or pool is not None):
            rcast_data = np.memmap(fh.name, mode="r+",
                                   shape = (igc.number_line, rcast.shape(1),
                                   rcast.shape(2),
                                   rcast.shape(3), rcast.shape(4), 2),
                                   dtype = np.int32)
        else:
            rcast_data = np.empty((igc.number_line, rcast.shape(1),
                                   rcast.shape(2),
                                   rcast.shape(3), rcast.shape(4), 2),
                                  dtype = np.int32)
        rsamp = RayCasterResampler(rcast, map_info)
        if(pool is None):
            rsamp.ray_cast_step(rcast_data)
        else:
            f = partial(_ray_caster_project_subset,
                        igc, number_line_framelet,
                        nline_process, nintegration_step,
                        map_info, fh.name, rcast.number_sub_line,
                        rcast.number_sub_sample)
            pool.map(f, range(0, igc.number_line, nline_process))
        scratch_count = np.zeros((map_info.number_y_pixel, map_info.number_x_pixel))
        log.info("Starting final rad step")
        if(dtype == np.float32 or dtype == np.float64):
            res = np.zeros((map_info.number_y_pixel,
                            map_info.number_x_pixel), dtype=np.float64)
            rsamp.final_rad_step_double(igc.image, rcast_data, res, scratch_count)
        else:
            res = np.zeros((map_info.number_y_pixel,
                            map_info.number_x_pixel), dtype=np.int32)
            rsamp.final_rad_step(igc.image, rcast_data, res, scratch_count)
    out = mmap_file(out_fname, map_info, dtype=dtype)
    out[:,:] = res[:,:]

__all__ = ["ray_caster_project",]
