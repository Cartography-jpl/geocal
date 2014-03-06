from geocal_swig import *
from igc_collection import *
from tie_point import *
from ray_intersect import *
from feature_detector_extension import *
import math
import itertools
import multiprocessing
from multiprocessing import Pool
import time
import logging

class TiePointWrap(object):
    '''Wrapper around tp_collect.tie_point that can be pickled. We can\'t
    directly use pool.map on tp_collect.tie_point because python can\'t 
    pickle a instance function'''
    def __init__(self, tp_collect):
        self.tp_collect = tp_collect
    def __call__(self, ic):
        try:
            return self.tp_collect.tie_point(ic)
        except RuntimeError:
        # We may try to find points that don't actually intersect
        # the ground (e.g., we are at a steep angle and above
        # the surface). In that case, just skip this point and
        # go to the next one
            pass
        return None

class TiePointCollect(object):
    '''Given a IgcCollection, collect tiepoints by image matching.'''
    def __init__(self, igc_collection, map_info = None, base_image_index = 0,
                 max_ground_covariance = 20 * 20,
                 start_image_index = 0,
                 end_image_index = None,
                 avg_level = 0):
        '''This sets up for doing tie point collection. A IgcCollection
        needs to be supplied.

        You can optionally specify avg_level to use. If supplied, we
        use a PyramidImageMatcher on top of the CcorrLsmMatcher, useful for
        difficult to match imagery.
        '''
        image_matcher = CcorrLsmMatcher()
        if(avg_level > 0):
            image_matcher = PyramidImageMatcher(image_matcher, avg_level)
        self.start_image_index = start_image_index
        if(end_image_index is None):
            self.end_image_index = igc_collection.number_image
        else:
            self.end_image_index = end_image_index
        self.avg_level = avg_level
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
                "avg_level" : self.avg_level,
                }

    def __setstate__(self, dict):
        self.__init__(dict["igc_collection"], map_info = dict["map_info"],
                      base_image_index = dict["base_image_index"],
                      max_ground_covariance = dict["max_ground_covariance"],
                      start_image_index = dict["start_image_index"],
                      end_image_index = dict["end_image_index"],
                      avg_level = dict["avg_level"],
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
        log = logging.getLogger("afids-python.tie_point_collect")
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
            log.info("Starting interest point")
            log.info("Time: %f" % (time.time() - tstart))
            iplist = fd.interest_point_grid(
                self.igc_collection.image(self.base_image_index),
                num_y, num_x, border, pool = pool)
        log.info("Done with interest point")
        log.info("Time: %f" % (time.time() - tstart))
        log.info("Starting matching")
        func = TiePointWrap(self)
        if(pool):
            res = pool.map(func, iplist, 
               len(iplist) / multiprocessing.cpu_count())
        else:
            res = map(func, iplist)
        log.info("Done with matching")
        log.info("Time: %f" % (time.time() - tstart))
        return TiePointCollection(filter(lambda i : i is not None, res))

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

class GcpTiePointCollect(object):
    '''Given a IgcCollection and a reference image, collect GCPs by 
    image matching.'''
    def __init__(self, ref_image, dem, igc_collection,
                 avg_level = 0):
        '''This sets up for doing a tie point collection with a reference
        image. A IgcCollection and reference image needs to be supplied

        You can optionally specify avg_level to use. If supplied, we
        use a PyramidImageMatcher on top of the CcorrLsmMatcher, useful for
        difficult to match imagery.
        '''
        image_matcher = CcorrLsmMatcher()
        if(avg_level > 0):
            image_matcher = PyramidImageMatcher(image_matcher, avg_level)
        self.avg_level = avg_level
        self.igc_collection = igc_collection
        self.dem = dem
        self.ref_image = ref_image
        # Find an area that covers all the ground projected images
        mi = self.igc_collection.image_ground_connection(0).\
            cover(ref_image.map_info)
        for i in range(igc_collection.number_image):
            igc = igc_collection.image_ground_connection(i)
            mi = mi.map_union(igc.cover(mi))
        # Subset ref image to cover as much of that as possible
        mi = ref_image.map_info.intersection(mi)
        vg = Vector_GroundCoordinate()
        vg.push_back(mi.ground_coordinate(0, 0))
        vg.push_back(mi.ground_coordinate(mi.number_x_pixel, mi.number_y_pixel))
        self.sub_ref_image = SubRasterImage(ref_image, vg)
        self.ref_igc = MapInfoImageGroundConnection(self.sub_ref_image, 
                       dem, "Reference Image")
        self.itoim = [None]*self.igc_collection.number_image 
        for j in range(self.igc_collection.number_image):
            igc2 = self.igc_collection.image_ground_connection(j)
            self.itoim[j] = SurfaceImageToImageMatch(self.ref_igc, igc2,
                                                     mi, image_matcher)

    def __getstate__(self):
        return {"ref_image" : self.ref_image,
                "dem" : self.dem,
                "igc_collection" : self.igc_collection,
                "avg_level" : self.avg_level,
                }

    def __setstate__(self, dict):
        self.__init__(dict["ref_image"], dict["dem"], dict["igc_collection"],
                      avg_level = dict["avg_level"])

    @property
    def number_image(self):
        return self.igc_collection.number_image

    def tie_point_grid(self, num_x, num_y, border = 100, pool = None):
        fd = ForstnerFeatureDetector()
        iplist = fd.interest_point_grid(self.sub_ref_image, num_y, num_x, 
                                        border, pool = pool)
        func = TiePointWrap(self)
        if(pool):
            res = pool.map(func, iplist, 
               len(iplist) / multiprocessing.cpu_count())
        else:
            res = map(func, iplist)
        return TiePointCollection(
            filter(lambda i : i is not None and i.number_image_location > 0,
                   res))

    def tie_point(self, ic1):
        '''Return a tie point that is roughly at the given location in the
        reference image. Note that we try matching all the other images to the
        first image, i.e., we don\'t chain by map 2 to 3 and 3 to 4. 
        If we aren't successful at matching, this returns None.'''
        tp = TiePoint(self.number_image)
        tp.is_gcp = True
        tp.ground_location = \
            Ecr(self.sub_ref_image.ground_coordinate(ic1, self.dem))
        for i in range(self.number_image):
            ic2, lsigma, ssigma, success, diagnostic = \
                self.itoim[i].match(ic1)
            if(success):
                tp.image_location[i] = ic2, lsigma, ssigma
        return tp


                                                     
