from geocal import *
from igc_collection import *
from tie_point import *
from ray_intersect import *
import math
import itertools
import multiprocessing
from multiprocessing import Pool
from functools import partial
import time

def _tie_point_wrap(tp_collect, ic):
    '''Wrapper around tp_collect.tie_point that can be pickled. We can\'t
    directly use pool.map on tp_collect.tie_point because python can\'t 
    pickle a instance function'''
    try:
        return tp_collect.tie_point(ic)
    except RuntimeError:
        # We may try to find points that don't actually intersect
        # the ground (e.g., we are at a steep angle and above
        # the surface). In that case, just skip this point and
        # go to the next one
        pass
    return None

class TiePointCollect:
    '''Given a IgcCollection, collect tiepoints by image matching.'''
    def __init__(self, igc_collection, map_info = None, base_image_index = 0,
                 max_ground_covariance = 20 * 20,
                 start_image_index = 0,
                 end_image_index = None):
        '''This sets up for doing tie point collection. A IgcCollection
        needs to be supplied.'''
        image_matcher = CcorrLsmMatcher()
        self.start_image_index = start_image_index
        if(end_image_index is None):
            self.end_image_index = igc_collection.number_image
        else:
            self.end_image_index = end_image_index
        self.map_info = map_info
        self.base_image_index = base_image_index
        self.max_ground_covariance = max_ground_covariance
        self.igc_collection = igc_collection
        self.ri = RayIntersect2(self.igc_collection,
                                max_ground_covariance = max_ground_covariance)
        self.itoim = [None]*self.igc_collection.number_image 
        i = self.base_image_index
        img1 = self.igc_collection.image_ground_connection(i)
        for j in range(self.igc_collection.number_image):
            img2 = self.igc_collection.image_ground_connection(j)
            if(map_info is None):
                self.itoim[j] = IgcImageToImageMatch(
                    self.igc_collection.image_ground_connection(i),
                    self.igc_collection.image_ground_connection(j),
                    image_matcher)
            else:
                self.itoim[j] = SurfaceImageToImageMatch(img1, img2, 
                                              map_info, image_matcher)

    def __getstate__(self):
        return {"igc_collection": self.igc_collection,
                "map_info" : self.map_info,
                "base_image_index" : self.base_image_index,
                "max_ground_covariance" : self.max_ground_covariance,
                "start_image_index" : self.start_image_index,
                "end_image_index" : self.end_image_index,
                }

    def __setstate__(self, dict):
        self.__init__(dict["igc_collection"], map_info = dict["map_info"],
                      base_image_index = dict["base_image_index"],
                      max_ground_covariance = dict["max_ground_covariance"],
                      start_image_index = dict["start_image_index"],
                      end_image_index = dict["end_image_index"],
                      )

    @property
    def number_image(self):
        return self.igc_collection.number_image

    def tie_point_grid(self, num_x, num_y, aoi = None, dem = None, 
                       border = 100, pool = None):
        '''Return a grid of tie points. Note that we may be missing results
        for particular grid points if matching isn\'t successful.
        
        There are two common ways to collect tie points. One is to simply
        search through the full images supplied with the 
        ImageGroundConnection. The other is to restrict this to an area 
        of interest (AOI). You can optionally supply a MapInfo to define
        a AOI, along with a Dem to use. If present, we find tiepoints 
        distributed on that. If not, then we find relative to the first 
        image in the image list'''
        tstart = time.time()
        if(aoi):
            nline = aoi.number_y_pixel
            nsamp = aoi.number_x_pixel
            step_line = max(int(math.floor((nline - 2 * border) / num_y)), 1)
            step_samp = max(int(math.floor((nsamp - 2 * border) / num_x)), 1)
            iplist = []
            for ln in range(border, nline - border, step_line):
                for smp in range(border, nsamp - border, step_samp):
                    gp = aoi.ground_coordinate(smp, ln)
                    gp = dem.surface_point(gp)
                    ic1 = self.igc_collection.image_coordinate(
                       self.base_image_index, gp)
                    ic1.line = round(ic1.line)
                    ic1.sample = round(ic1.sample)
                    iplist.append(ic1)
        else:
            fd = ForstnerFeatureDetector()
            #print "Starting interest point"
            #print "Time: ", time.time() - tstart
            iplist = fd.interest_point_grid(
                self.igc_collection.image(self.base_image_index),
                num_y, num_x, border, pool = pool)
        #print "Done with interest point"
        #print "Time: ", time.time() - tstart
        #print "Starting matching"
        func = partial(_tie_point_wrap, self)
        if(pool):
            res = pool.map(func, iplist, 
               len(iplist) / multiprocessing.cpu_count())
        else:
            res = map(func, iplist)
        #print "Done with matching"
        #print "Time: ", time.time() - tstart
        return filter(lambda i : i is not None, res)

    def tie_point(self, ic1):
        '''Return a tie point that is roughly at the given location in the
        first image. Note that we try matching all the other images to the
        first image, i.e., we don\'t chain by map 2 to 3 and 3 to 4. 
        If we aren't successful at matching, this returns None.'''
        tp = TiePoint(self.number_image)
        tp.image_location[self.base_image_index] = ic1, 0.05, 0.05
        for i in range(self.start_image_index, self.end_image_index):
            if(i != self.base_image_index):
                ic2, lsigma, ssigma, success, diagnostic = \
                    self.itoim[i].match(ic1)
                if(success):
                    tp.image_location[i] = ic2, lsigma, ssigma
        return self.ri.ray_intersect(tp)
