# This contains support for doing parallel processing.

from sqlite_shelf import write_shelve
from functools import partial
import numpy as np
from multiprocessing import Pool

def parallel_process_tile(img_in, lstart, sstart, nline, nsamp, out, 
                          verbose = False):
    if(verbose):
        print "Processing (%d, %d) to (%d, %d)" % \
            (lstart, sstart, lstart + nline, sstart + nsamp)
    if(out.dtype == np.float32 or out.dtype == np.float64):
        out[lstart:(lstart + nline), sstart:(sstart + nsamp)] = \
            img_in.read_double(lstart, sstart, nline, nsamp)
    else:
        out[lstart:(lstart + nline), sstart:(sstart + nsamp)] = \
            img_in.read(lstart, sstart, nline, nsamp)

def do_parallel_process(p):
    p()
    return True

def parallel_process_image(img_in, out, process_nline, process_nsamp, 
                           number_process, shelve_name = None, 
                           verbose = False):
    '''This processes an input image, writing to a mmap_file output file. We
    process the data by tiles of the given number of lines and samples, using
    the given number of processes to run. You can optionally specify a more
    verbose version where we write out each tile as we process it.

    Instead of actually processing, you can supply a database name. We then
    store data suitable for use by shelve_job_run. In this case, we return
    the number of jobs stored, so you can use this to then actually process
    the jobs.'''
    process_list =  [ ] 
    job_index = 0
    for lstart in range(0, img_in.number_line, process_nline):
        tile_nline = process_nline
        if(lstart + tile_nline > img_in.number_line):
            tile_nline = img_in.number_line - lstart
        for sstart in range(0, img_in.number_sample, process_nsamp):
            tile_nsamp = process_nsamp
            if(sstart + tile_nsamp > img_in.number_sample):
                tile_nsamp = img_in.number_sample - sstart
            job = partial(parallel_process_tile, img_in, lstart, sstart,
                          tile_nline, tile_nsamp, out, verbose = verbose)
            if(shelve_name is not None):
                write_shelve(shelve_name + ":job_%d" % job_index, job)
            job_index += 1
            process_list.append(job)
    if(shelve_name is None):
        if(number_process ==1):
            res = map(do_parallel_process, process_list)
        else:
            pool = Pool(number_process)
            res = pool.map(do_parallel_process, process_list)
    return job_index
