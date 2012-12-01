from geocal import *
from functools import partial
from itertools import chain
import multiprocessing
import time

def _interest_point_grid_raw_wrap(self, img, mask, number_grid_line, 
                                  number_grid_sample, border, tstart, i):
    '''Wrapper around _interest_point_grid_raw that can be pickled. 
    We can\'t directly use pool.map on _interest_point_grid_raw because
    python can\`t pickle a instance function'''
    i
    res = []
    t2 = time.time()
    #print "Starting ", i, " - ", multiprocessing.current_process().name, \
    #    " Time : ", time.time() - tstart
    for j in range(number_grid_sample):
        if(mask is None):
            r = self._interest_point_grid_raw(img, i, j,
                          number_grid_line, number_grid_sample, border)
        else:
            r = self._interest_point_grid_raw(img, mask, i, j,
                          number_grid_line, number_grid_sample, border)
        if(r is not None):
            res.append(r)
    #print "Done with  ", i, " - ", multiprocessing.current_process().name, \
    #    " Time : ", time.time() - tstart, " Delta t: ", time.time() - t2
    return res
    
# Add some useful functions to FeatureDetector
def interest_point_grid(self, img, number_grid_line, number_grid_sample,
                        border=0, mask=None, pool = None):
    '''This is used to generate a set of interest points. We divide
    the Image into the given number of grid line and samples, and in
    each grid point find the point with the interest point with the
    greatest weight.

    You can optionally specify a border to exclude in the search, this
    is useful when the points will be used for image matching and we
    want to exclude points too close to the edge where the image matching
    will fail.

    You can optionally supply a Mask, this will be used to exclude points
    based on for example a land/water mask or a cloud mask. If this is not
    supplied, we look at every point.

    You can optionally specify a multiprocessing.Pool if you want to 
    collect these points in parallel.
    '''
    res = []
    index_list = range(number_grid_line)
    tstart = time.time()
    func = partial(_interest_point_grid_raw_wrap, self, img, mask,
                   number_grid_line, number_grid_sample, border, tstart)
    if(pool):
        res = pool.map(func, index_list)
    else:
        res = map(func, index_list)
    # This next command flattens the list
    return list(chain.from_iterable(res))

setattr(FeatureDetector, "interest_point_grid", interest_point_grid)
