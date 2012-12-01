from geocal import *
import numpy as np
import numpy.linalg as la
import math

class RayIntersect2:
    '''This class is used to find the intersection of two or more rays
    on the surface. Note that in general the points don\'t actually 
    intersect, this finds the values that are closest to the rays.'''
    def __init__(self, igc_coll, 
                 delta_height = 10, max_ground_covariance = 20 * 20):
        self.igc_coll = igc_coll
        self.delta_height = delta_height
        self.max_ground_covariance = max_ground_covariance
        if(self.igc_coll.number_image < 2):
            raise RuntimeError("Need to have at least 2 images in IgcCollection")
    
    def ray_intersect(self, tie_point):
        '''This takes a tie point and fills in the ground location by
        finding an approximate intersection of all the image locations
        in the tie point.

        In general, the rays don\'t intersect in the same location. We
        look at the rays pairwise, and average the two_ray_intersect.
        If the points aren\'t sufficiently close together (given by
        max_ground_covariance), then we return None.'''
        mean = np.array([0.0,0,0])
        var = np.array([0.0,0,0])
        cnt = 0
        max_dist = 0
        for i1 in range(self.igc_coll.number_image):
            if(not tie_point.image_location[i1]): continue
            for i2 in range(i1 + 1, self.igc_coll.number_image):
                if(tie_point.image_location[i2]): break
            else:
                break
            t, skew = self.two_ray_intersect(tie_point.image_location[i1][0],
                                             tie_point.image_location[i2][0],
                                             i1, i2)
            max_dist = max(abs(skew), max_dist)
            t = np.array([t.position[0], t.position[1], t.position[2]])
            mean += t
            var += t * t
            cnt +=1
        if(cnt < 1):
            return None
        mean = mean / cnt
        if(cnt > 1):
            var = (var - (mean * mean) * cnt)/ (cnt - 1)
        else:
            var = np.array([0.0,0,0])
        var = [max(v, max_dist * max_dist) for v in var]
        for v in var:
            if(v > self.max_ground_covariance):
                return None
        tie_point.ground_location = Ecr(mean[0], mean[1], mean[2])
        return tie_point

    def two_ray_intersect(self, ic1, ic2, index_1 = 0, index_2 = 1):
        '''Find the intersection of the ground for two rays given by
        two image coordinates. Can optionally supply the index into
        igc_coll, the default is that this is the first two entries.
        
        This returns the value as a np.array of Ecr position, and the
        distance
        '''
        ri = RayIntersect(self.igc_coll.image_ground_connection(index_1),
                          self.igc_coll.image_ground_connection(index_2))
        return ri.two_ray_intersect(ic1, ic2)
        
        
