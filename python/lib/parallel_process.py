# This contains support for doing parallel processing.

from functools import partial
import numpy as np
from multiprocessing import Pool

def parallel_process_tile(img_in, lstart, sstart, nline, nsamp, out):
    if(out.dtype == np.float32 or out.dtype == np.float64):
        out[lstart:(lstart + nline), sstart:(sstart + nsamp)] = \
            img_in.read_double(lstart, sstart, nline, nsamp)
    else:
        out[lstart:(lstart + nline), sstart:(sstart + nsamp)] = \
            img_in.read(lstart, sstart, nline, nsamp)

def do_parallel_process(p):
    p()
    return True

def parallel_process_image(img_in, out, number_process):
    process_nline = img_in.number_line / number_process
    process_list =  [ ] 
    for lstart in range(0, img_in.number_line, process_nline):
        tile_nline = process_nline
        if(lstart + tile_nline > img_in.number_line):
            tile_nline = img_in.number_line - lstart
        job = partial(parallel_process_tile, img_in, lstart, 0,
                      tile_nline, img_in.number_sample, out)
        process_list.append(job)
    if(number_process ==1):
        res = map(do_parallel_process, process_list)
    else:
        pool = Pool(number_process)
        res = pool.map(do_parallel_process, process_list)
