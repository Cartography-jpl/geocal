from __future__ import division
from __future__ import absolute_import
from builtins import map
from builtins import range
from past.utils import old_div
from builtins import object
from geocal_swig import *
from .igc_collection_extension import *
from .tie_point_extension import *
from .ray_intersect import *
from .feature_detector_extension import *
from .misc import *
import math
import itertools
import multiprocessing
from multiprocessing import Pool
import time
import logging
# Might not have cv2 available
try:
    import cv2
    have_cv2 = True
except ImportError:
    have_cv2 = False

class TiePointWrap(object):
    '''Wrapper around tp_collect.tie_point that can be pickled. We can\'t
    directly use pool.map on tp_collect.tie_point because python can\'t 
    pickle a instance function'''
    def __init__(self, tp_collect):
        self.tp_collect = tp_collect
    def __call__(self, ic):
        log = logging.getLogger("geocal-python.tie_point_collect")
        log.debug("Processing point (%f, %f)" % (ic.line, ic.sample) )
        for h in logging.getLogger("geocal-python").handlers:
            h.flush()
        try:
            tp = self.tp_collect.tie_point(ic)
            if(tp is None):
                log.debug("Got 0 matches")
            else:
                log.debug("Got %d matches" % tp.number_image_location)
            return tp
        except RuntimeError:
        # We may try to find points that don't actually intersect
        # the ground (e.g., we are at a steep angle and above
        # the surface). In that case, just skip this point and
        # go to the next one
            log.debug("Got 0 matches")
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
                 grid_spacing = 1,
                 image_matcher = None,
                 gcp_image_matcher = None,
                 ref_image = None,
                 ref_dem = None,
                 surface_image = None,
                 scale_factor = None):
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

        You can optionally pass in a scale factor. This is applied to
        the raster data before image matching. This is useful for
        example AirMSPI data where the image is a reflectance value
        between 0.0 and 1.0. Since the image matchers expect integer
        data, you want get any results unless you scale to a different
        range.

        You can pass a list of surface projected images to match with.
        These should be the projection of the igc_collection, and can
        be used as an alternative to doing the projection on the fly.
        
        There is a trade off between getting the largest coverage (by
        taking a union of all the igc on the surface) and the
        strongest points (by looking at places seen by all the
        cameras. You may want to do both - so generate a set of GCPs
        using the largest coverage first and then the intersection
        second.  which one is used is controlled by the option
        "use_intersection"

        Can supply a reference image. If present, we use this as ground
        truth and generate GCPs if we can. 
        '''
        if(image_matcher is None):
            self.image_matcher = CcorrLsmMatcher()
            if(avg_level > 0):
                self.image_matcher = PyramidImageMatcher(self.image_matcher, avg_level)
        else:
            self.image_matcher = image_matcher
        if(gcp_image_matcher is not None):
            self.gcp_image_matcher = gcp_image_matcher
        else:
            self.gcp_image_matcher = self.image_matcher
        self.start_image_index = start_image_index
        if(end_image_index is None):
            self.end_image_index = igc_collection.number_image
        else:
            self.end_image_index = end_image_index
        self.ref_image = ref_image
        self.ref_dem = ref_dem
        self.avg_level = avg_level
        self.scale_factor = scale_factor
        self.map_info = map_info
        self.base_image_index = base_image_index
        self.max_ground_covariance = max_ground_covariance
        self.igc_collection = igc_collection
        self.surface_image = surface_image
        if(self.igc_collection.number_image > 1):
            self.ri = RayIntersect3(self.igc_collection,
                                  max_ground_covariance = max_ground_covariance)
        if(self.ref_image is not None):
            self.itoim = [None]*(self.igc_collection.number_image + 1)
        else:
            self.itoim = [None]*self.igc_collection.number_image 
        i = self.base_image_index
        igc1 = self.igc_collection.image_ground_connection(i)
        if(scale_factor is not None):
            igc1 = ScaleImageGroundConnection(igc1, scale_factor)
        for j in range(self.igc_collection.number_image):
            igc2 = self.igc_collection.image_ground_connection(j)
            if(scale_factor is not None):
                igc2 = ScaleImageGroundConnection(igc2, scale_factor)
            if(map_info is None):
                if(surface_image is None):
                    self.itoim[j] = IgcImageToImageMatch(igc1, igc2,
                                                         self.image_matcher)
                else:
                    self.itoim[j] = \
                        SurfaceImageToImageMatch(igc1, surface_image[i],
                                                 igc2, surface_image[j],
                                                 self.image_matcher)
            else:
                self.itoim[j] = SurfaceImageToImageMatch(igc1, igc2, 
                              map_info, self.image_matcher, grid_spacing)
        if(self.ref_image is not None):
            dem = self.ref_dem
            if(dem is None):
                dem = igc1.dem
            self.ref_igc = MapInfoImageGroundConnection(self.ref_image, 
                dem, "Reference Image")
            j = self.igc_collection.number_image
            if(surface_image is not None):
                self.itoim[j] = SurfaceImageToImageMatch(igc1, surface_image[i],
                        self.ref_igc, self.ref_image, self.gcp_image_matcher)
            else:
                mi = map_info
                if(mi is None):
                    mi = igc1.coverage(ref_image.map_info)
                self.itoim[j] = SurfaceImageToImageMatch(igc1, ref_igc,
                                 mi, self.gcp_image_matcher, grid_spacing)

    def __getstate__(self):
        return {"igc_collection": self.igc_collection,
                "map_info" : self.map_info,
                "base_image_index" : self.base_image_index,
                "max_ground_covariance" : self.max_ground_covariance,
                "start_image_index" : self.start_image_index,
                "end_image_index" : self.end_image_index,
                "avg_level" : self.avg_level,
                "image_matcher" : self.image_matcher,
                "gcp_image_matcher" : self.gcp_image_matcher,
                "ref_image" : self.ref_image,
                "ref_dem" : self.ref_dem,
                "surface_image" : self.surface_image,
                "scale_factor": self.scale_factor
                }

    def __setstate__(self, dict):
        self.__init__(dict["igc_collection"], map_info = dict["map_info"],
                      base_image_index = dict["base_image_index"],
                      max_ground_covariance = dict["max_ground_covariance"],
                      start_image_index = dict["start_image_index"],
                      end_image_index = dict["end_image_index"],
                      image_matcher = dict["image_matcher"],
                      gcp_image_matcher = dict["gcp_image_matcher"],
                      ref_image = dict["ref_image"],
                      ref_dem = dict["ref_dem"],
                      avg_level = dict["avg_level"],
                      surface_image = dict["surface_image"],
                      scale_factor = dict["scale_factor"]
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
            step_line = max(int(math.floor(old_div((nline - 2 * border), num_y))), 1)
            step_samp = max(int(math.floor(old_div((nsamp - 2 * border), num_x))), 1)
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
            img = self.igc_collection.image(self.base_image_index)
            if(self.scale_factor):
                img = ScaleImage(img, self.scale_factor)
            iplist = fd.interest_point_grid(img,
                num_y, num_x, border, pool = pool)
        log.info("Done with interest point")
        log.info("Time: %f" % (time.time() - tstart))
        log.info("Starting matching")
        func = TiePointWrap(self)
        if(pool):
            res = pool.map(func, iplist, 
               old_div(len(iplist), multiprocessing.cpu_count()))
        else:
            res = list(map(func, iplist))
        log.info("Done with matching")
        log.info("Time: %f" % (time.time() - tstart))
        res2 = TiePointCollection()
        res2.extend([i for i in res if i is not None])
        log.info("Total number tp: %d" % len(res2))
        log.info("Number GCPs:     %d" % res2.number_gcp)
        return res2

    def tie_point(self, ic1):
        '''Return a tie point that is roughly at the given location in the
        first image. Note that we try matching all the other images to the
        first image, i.e., we don\'t chain by map 2 to 3 and 3 to 4. 
        If we aren't successful at matching, this returns None.'''
        tp = TiePoint(self.number_image)
        tp.image_coordinate(self.base_image_index, ic1, 0.05, 0.05)
        for i in range(self.start_image_index, self.end_image_index):
            if(i != self.base_image_index):
                ic2, lsigma, ssigma, success, diagnostic = \
                    self.itoim[i].match(ic1)
                if(success):
                    tp.image_coordinate(i, ic2, lsigma, ssigma)
        if(self.ref_image is not None):
            i = self.igc_collection.number_image
            ic2, lsigma, ssigma, success, diagnostic = \
                self.itoim[i].match(ic1)
            if(success):
                tp.ground_location = Ecr(self.ref_igc.ground_coordinate(ic2))
                tp.is_gcp = True
                return tp
        return self.ri.ray_intersect(tp)

class TiePointCollectFM(object):
    '''Given a IgcCollection, collect tiepoints by feature matching. This is
    very similar to TiePointCollect, except we use feature matching.

    Note that unlike TiePointCollect we don't have the concept of a
    'base_image' here, instead we match all the images at once.

    Note also that feature matching can have much larger blunders than
    we typically see for image matching (since it can match any 2 features
    in an image). You will want to do some kind of outlier rejection after
    using this class to determine tiepoints (e.g., outlier_reject_ransac).

    I have not had much success with using this class to generate tie points, 
    the accuracy of the feature matching tends to be pretty poor. We will 
    leave this is place for now, perhaps this will be of some use in the 
    future. 
    '''
    def __init__(self, igc_collection, max_ground_covariance = 20 * 20,
                 ref_image = None,
                 number_feature = 500, number_octave_levels = 4,
                 number_ref_feature = 1000):
        if(not have_cv2):
            raise RuntimeError("This class requires the openCV python library cv2, which is not available.")
        self.raster_image = [igc_collection.image(i) for i in range(igc_collection.number_image)]
        if(self.igc_collection.number_image > 1):
            self.ri = RayIntersect3(igc_collection,
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
                    tp.image_coordinate(ind, ic1, 0.5, 0.5)
                tp.image_coordinate(i, ic2, 0.5, 0.5)
                res[idx] = tp
        if(kp_and_desc_ref is not None):
            good = self.match_feature_ref(kp_and_desc, kp_and_desc_ref, ind)
            for ic1, ic2, idx in good:
                if idx in res:
                    tp = res[idx]
                else:
                    tp = TiePoint(len(self.raster_image))
                    tp.image_coordinate(ind, ic1, 0.5, 0.5)
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
        for i in range(len(self.raster_image)):
            tpl = self.tp_list(kp_and_desc, kp_and_desc_ref, i)
            for tp in tpl.values():
                if(not tp.is_gcp):
                    tp2 = self.ri.ray_intersect(tp)
                    if(tp2 is not None):
                        res.append(tp2)
                else:
                    res.append(tp)
        log.info("Done with feature matching")
        log.info("Time: %f" % (time.time() - tstart))
        return res
                                                     
def _point_list(ind1, ind2_or_ref_image, tpcol):
    '''Internal function, gives list of points between 2 images.'''
    pt1 = []
    pt2 = []
    ind = []
    for i, tp in enumerate(tpcol):
        ic1 = tp.image_coordinate(ind1)
        if(isinstance(ind2_or_ref_image,RasterImage)):
            if(tp.is_gcp):
                ic2 = ind2_or_ref_image.coordinate(tp.ground_location)
            else:
                ic2 = None
        else:
            ic2 = tp.image_location[ind2_or_ref_image]
        if(ic1 is not None and
           ic2 is not None):
            pt1.append([ic1.line, ic1.sample])
            pt2.append([ic2.line, ic2.sample])
            ind.append(i)
    return [np.array(pt1), np.array(pt2), np.array(ind)]

def _point_list_surface_proj(ind1, ind2_or_ref_image, tpcol, 
                             igc1, igc2, ref_image):
    '''Internal function, gives list of points between 2 images. This version
    uses the supplied igc and reference image to first project to the surface,
    and then collect points in the reference image space. This is useful for 
    images where the differences with a frame camera are too great (e.g., 
    aircraft data).'''
    pt1 = []
    pt2 = []
    ind = []
    for i, tp in enumerate(tpcol):
        if(tp.image_coordinate(ind1) is not None):
            ic1 = ref_image.coordinate(igc1.ground_coordinate(tp.image_coordinate(ind1)))
        else:
            ic1 = None
        if(isinstance(ind2_or_ref_image,RasterImage)):
            if(tp.is_gcp):
                ic2 = ref_image.coordinate(tp.ground_location)
            else:
                ic2 = None
        else:
            if(tp.image_coordinate(ind2_or_ref_image) is not None):
                ic2 = ref_image.coordinate(igc2.ground_coordinate(tp.image_coordiante(ind2_or_ref_image)))
            else:
                ic1 = None
        if(ic1 is not None and
           ic2 is not None):
            pt1.append([ic1.line, ic1.sample])
            pt2.append([ic2.line, ic2.sample])
            ind.append(i)
    return [np.array(pt1), np.array(pt2), np.array(ind)]

def _outlier_reject_ransac(ind1, ind2_or_ref_image, tpcol, threshold,
                           igc1 = None, igc2 = None, ref_image = None):
    '''Internal function used to filter bad points between 2 images.'''
    if(not have_cv2):
        raise RuntimeError("This function requires the openCV python library cv2, which is not available.")
    if(igc1 is None):
        pt1, pt2, ind = _point_list(ind1, ind2_or_ref_image, tpcol)
    else:
        pt1, pt2, ind = _point_list_surface_proj(ind1, ind2_or_ref_image, tpcol,
                                                 igc1, igc2, ref_image)
    if(pt1.shape[0] < 1):
        return tpcol
    m, mask = cv2.findFundamentalMat(pt1, pt2, cv2.FM_RANSAC, threshold)
    # if we have too few points, we don't get a result. Really shouldn't
    # keep any of the tiepoints then
    if(mask is None):
        bad_pt = ind
    else:
        m = (mask == 0)
        bad_pt = ind[m[:,0]]
    return [tp for i, tp in enumerate(tpcol) if i not in bad_pt]

def outlier_reject_ransac(tpcol, ref_image = None, threshold = 3.0,
                          igccol = None):
    '''This remove outliers from a TiePointCollection. This fits the
    tiepoints between pairs of images to create the Fundamental Matrix, 
    rejecting outliers using Random sample consensus (RANSAC).

    Note that the Fundamental Matrix really only applies for epipolar
    geometry, e.g., a pair of frame pinhole cameras as single position.
    Our cameras often aren't this (e.g., a pushbroom camera). However often
    we are close enough that we can still used this approximation to look
    for outliers.

    If a reference image is passed in, we also look at rejecting GCPs that
    are outliers.

    If the Igccol is passed in, then we use that to project to the surface 
    first before looking for outliers. This can be useful for Igcs that are 
    too far from a frame camera, e.g., aircraft data'''
    nimg = tpcol[0].number_image
    res = tpcol
    log = logging.getLogger("geocal-python.tie_point_collect")
    log.info("Starting using RANSAC to reject outliers")
    if(igccol is not None):
        log.info("Projecting to surface first")
    len1 = len(tpcol)
    log.info("Starting %s" % tpcol)
    for i in range(nimg):
        for j in range(i + 1, nimg):
            if(igccol is None):
                res = _outlier_reject_ransac(i, j, res, threshold)
            else:
                igc1 = igccol.image_ground_connection(i)
                igc2 = igccol.image_ground_connection(j)
                if(ref_image is not None):
                    rimg = ref_image
                else:
                    mibase = cib01_mapinfo(igc1.resolution_meter())
                    mi = igc1.cover(mibase)
                    rimg = MemoryRasterImage(mi);
                res = _outlier_reject_ransac(i, j, res, threshold,
                                             igc1 = igc1, igc2 = igc2,
                                             ref_image = rimg)
    len2 = len(res)
    for i in range(nimg):
        if(ref_image is not None):
            if(igccol is None):
                res = _outlier_reject_ransac(i, ref_image, res, threshold)
            else:
                igc1 = igccol.image_ground_connection(i)
                igc2 = None
                res = _outlier_reject_ransac(i, ref_image, res, threshold,
                                             igc1 = igc1, igc2 = igc2,
                                             ref_image = ref_image)
                
    len3 = len(res)
    res2 = TiePointCollection()
    res2.extend(res)
    log.info("Removed %d tiepoints" % (len1 - len2))
    log.info("Removed %d more GCPs" % (len2 - len3))
    log.info("Ending %s" % res2)
    log.info("Completed using RANSAC to reject outliers")
    return res2

