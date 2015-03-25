from geocal_swig import *
from igc_collection_extension import *
from tie_point import *
from ray_intersect import *
from feature_detector_extension import *
import math
import itertools
import multiprocessing
from multiprocessing import Pool
import time
import logging
import cv2

class TiePointWrap(object):
    '''Wrapper around tp_collect.tie_point that can be pickled. We can\'t
    directly use pool.map on tp_collect.tie_point because python can\'t 
    pickle a instance function'''
    def __init__(self, tp_collect):
        self.tp_collect = tp_collect
    def __call__(self, ic):
        log = logging.getLogger("geocal-python.tie_point_collect")
        log.info("Processing point (%f, %f)" % (ic.line, ic.sample) )
        for h in logging.getLogger("geocal-python").handlers:
            h.flush()
        try:
            tp = self.tp_collect.tie_point(ic)
            if(tp is None):
                log.info("Got 0 matches")
            else:
                log.info("Got %d matches" % tp.number_image_location)
            return tp
        except RuntimeError:
        # We may try to find points that don't actually intersect
        # the ground (e.g., we are at a steep angle and above
        # the surface). In that case, just skip this point and
        # go to the next one
            log.info("Got 0 matches")
            pass
        for h in logging.getLogger("geoca1-python").handlers:
            h.flush()
        return None

class TiePointCollect(object):
    '''Given a IgcCollection, collect tiepoints by image matching.'''
    def __init__(self, igc_collection, map_info = None, base_image_index = 0,
                 max_ground_covariance = 20 * 20,
                 start_image_index = 0,
                 end_image_index = None,
                 avg_level = 0, use_intersection = False,
                 grid_spacing = 1):
        '''This sets up for doing tie point collection. A IgcCollection
        needs to be supplied.

        You can optionally specify avg_level to use. If supplied, we
        use a PyramidImageMatcher on top of the CcorrLsmMatcher, useful for
        difficult to match imagery.

        You can optionally pass in the grid spacing to use if we are
        projecting to the surface. This has a significant effect on
        how long this process takes. If the underlying DEM is coarse, 
        there is no reason to do the ImageGroundConnection calculation 
        at every point.
        
        There is a trade off between getting the largest coverage (by
        taking a union of all the igc on the surface) and the
        strongest points (by looking at places seen by all the
        cameras. You may want to do both - so generate a set of GCPs
        using the largest coverage first and then the intersection
        second.  which one is used is controlled by the option
        "use_intersection"
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
        igc1 = self.igc_collection.image_ground_connection(i)
        for j in range(self.igc_collection.number_image):
            igc2 = self.igc_collection.image_ground_connection(j)
            if(map_info is None):
                self.itoim[j] = IgcImageToImageMatch(igc1, igc2,
                                                     image_matcher)
            else:
                self.itoim[j] = SurfaceImageToImageMatch(igc1, igc2, 
                             map_info, image_matcher, grid_spacing)

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
        log = logging.getLogger("geocal-python.tie_point_collect")
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
                 avg_level = 0, use_intersection = False,
                 grid_spacing = 1):
        '''This sets up for doing a tie point collection with a reference
        image. A IgcCollection and reference image needs to be supplied

        You can optionally specify avg_level to use. If supplied, we
        use a PyramidImageMatcher on top of the CcorrLsmMatcher, useful for
        difficult to match imagery.

        You can optionally pass in the grid spacing to use if we are
        projecting to the surface. This has a significant effect on
        how long this process takes. If the underlying DEM is coarse, 
        there is no reason to do the ImageGroundConnection calculation 
        at every point.

        There is a trade off between getting the largest coverage (by
        taking a union of all the igc on the surface} and the
        strongest points (by looking at places seen by all the
        cameras). You may want to do both - so generate a set of GCPs
        using the largest coverage first and then the intersection
        second.
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
            if(use_intersection):
                mi = mi.intersection(igc.cover(mi))
            else:
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
                                                     mi, image_matcher,
                                                     grid_spacing)

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
        tstart = time.time()
        log = logging.getLogger("geocal-python.tie_point_collect")
        log.info("Starting interest point")
        log.info("Time: %f" % (time.time() - tstart))
        fd = ForstnerFeatureDetector()
        iplist = fd.interest_point_grid(self.sub_ref_image, num_y, num_x, 
                                        border, pool = pool)
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

class TiePointCollectFM(object):
    '''Given a IgcCollection, collect tiepoints by feature matching. This is
    very similar to TiePointCollect, except we use feature matching.

    Note that unlike TiePointCollect we don't have the concept of a 'base_image' here,
    instead we match all the images at once.'''
    def __init__(self, igc_collection, max_ground_covariance = 20 * 20,
                 ref_image = None,
                 number_feature = 500, number_octave_levels = 4,
                 number_ref_feature = 1000):
        self.raster_image = [igc_collection.image(i) for i in range(igc_collection.number_image)]
        self.ri = RayIntersect2(igc_collection,
                                max_ground_covariance = max_ground_covariance)
        self.max_ground_covariance = max_ground_covariance
        self.sift = cv2.SIFT(number_feature, number_octave_levels)
        self.sift_ref = cv2.SIFT(number_ref_feature, number_octave_levels)
        if(ref_image is not None):
            mi = ref_image.map_info
            mi_ref = None
            for i in range(igc_collection.number_image):
                mi_t = igc_collection.image_ground_connection(i).cover(mi)
                if(mi_ref is None):
                    mi_ref = mi_t
                else:
                    mi_ref = mi_ref.map_union(mi_t)
            self.ref_image = SubRasterImage(ref_image, mi_ref)
        else:
            self.ref_image = None
        self.bf = cv2.BFMatcher()

    def detect_and_compute(self, ind, log):
        '''Detect keypoints and compute descriptor the given raster image.'''
        log.info("Detecting features for image %d " % ind)
        d = self.raster_image[ind].read_all_byte_scale()
        kp, des = self.sift.detectAndCompute(d, None)
        return [kp, des]

    def detect_and_compute_ref(self, log):
        '''Detect keypoints and compute descriptor in the reference image.'''
        log.info("Detecting features for reference image")
        d = self.ref_image.read_all_byte_scale()
        kp, des = self.sift_ref.detectAndCompute(d, None)
        return [kp, des]

    def match_feature(self, kp_and_desc, ind1, ind2):
        '''Match features between two images'''
        kp1, des1 = kp_and_desc[ind1]
        kp2, des2 = kp_and_desc[ind2]
        matches = self.bf.knnMatch(des1, des2, k=2)
        # This test is from the SIFT paper. If selects matches only if they
        # have a single unique match that is significantly better than the
        # next match.
        good = []
        for m,n in matches:
            if m.distance < 0.75*n.distance:
                good.append(m)
        good = sorted(good, key = lambda x:x.distance)
        res = []
        for g in good:
            ic1 = ImageCoordinate(kp1[g.queryIdx].pt[1], kp1[g.queryIdx].pt[0])
            ic2 = ImageCoordinate(kp2[g.trainIdx].pt[1], kp2[g.trainIdx].pt[0])
            res.append([ic1, ic2, g.queryIdx])
        return res

    def match_feature_ref(self, kp_and_desc, kp_and_desc_ref, ind1):
        '''Match features between two images'''
        kp1, des1 = kp_and_desc[ind1]
        kp2, des2 = kp_and_desc_ref
        matches = self.bf.knnMatch(des1, des2, k=2)
        # This test is from the SIFT paper. If selects matches only if they
        # have a single unique match that is significantly better than the
        # next match.
        good = []
        for m,n in matches:
            if m.distance < 0.75*n.distance:
                good.append(m)
        good = sorted(good, key = lambda x:x.distance)
        res = []
        for g in good:
            ic1 = ImageCoordinate(kp1[g.queryIdx].pt[1], kp1[g.queryIdx].pt[0])
            ic2 = ImageCoordinate(kp2[g.trainIdx].pt[1], kp2[g.trainIdx].pt[0])
            res.append([ic1, ic2, g.queryIdx])
        return res

    def tp_list(self, kp_and_desc, kp_and_desc_ref, ind):
        '''Generate list of tie points, starting with the given image.'''
        res = {}
        for i in range(ind + 1, len(self.raster_image)):
            good = self.match_feature(kp_and_desc, ind, i)
            for ic1, ic2, idx in good:
                if idx in res:
                    tp = res[idx]
                else:
                    tp = TiePoint(len(self.raster_image))
                    tp.image_location[ind] = (ic1, 0.5, 0.5)
                tp.image_location[i] = (ic2, 0.5, 0.5)
                res[idx] = tp
        if(kp_and_desc_ref is not None):
            good = self.match_feature_ref(kp_and_desc, kp_and_desc_ref, ind)
            for ic1, ic2, idx in good:
                if idx in res:
                    tp = res[idx]
                else:
                    tp = TiePoint(len(self.raster_image))
                    tp.image_location[ind] = (ic1, 0.5, 0.5)
                tp.is_gcp = True
                tp.ground_location = Ecr(self.ref_image.ground_coordinate(ic2))
                res[idx] = tp
            
        return res

    def tie_point_list(self, pool = None):
        '''Generate a TiePointCollect.'''
        res = TiePointCollection()
        tstart = time.time()
        log = logging.getLogger("geocal-python.tie_point_collect")
        log.info("Starting feature detection")
        kp_and_desc = [self.detect_and_compute(i, log)
                       for i in range(len(self.raster_image))]
        if(self.ref_image):
            kp_and_desc_ref = self.detect_and_compute_ref(log)
        else:
            kp_and_desc_ref = None
        log.info("Done with feature detection")
        log.info("Time: %f" % (time.time() - tstart))
        log.info("Starting feature matching")
        for i in range(len(self.raster_image) - 1):
            tpl = self.tp_list(kp_and_desc, kp_and_desc_ref, i)
            for tp in tpl.itervalues():
                if(not tp.is_gcp):
                    tp2 = self.ri.ray_intersect(tp)
                    if(tp2 is not None):
                        res.append(tp2)
                else:
                    res.append(tp)
        log.info("Done with feature matching")
        log.info("Time: %f" % (time.time() - tstart))
        return res
                                                     
