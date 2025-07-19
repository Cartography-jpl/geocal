from builtins import str
from builtins import range
from builtins import object
from geocal_swig import *
import numpy as np
import math
from .lm_optimize import *
from scipy.sparse import block_diag
import logging


class RayIntersect2(object):
    """This class is used to find the intersection of two or more rays
    on the surface. Note that in general the points don\'t actually
    intersect, this finds the values that are closest to the rays."""

    def __init__(self, igccol, delta_height=10, max_ground_covariance=20 * 20):
        self.igccol = igccol
        self.delta_height = delta_height
        self.max_ground_covariance = max_ground_covariance
        if self.igccol.number_image < 2:
            raise RuntimeError("Need to have at least 2 images in IgcCollection")
        # Sample CartesianFixed point, which we can use the create function
        # with to create other instances of this type.
        self.sample_cf_pt = None

    def ray_intersect(self, tie_point):
        """This takes a tie point and fills in the ground location by
        finding an approximate intersection of all the image locations
        in the tie point.

        In general, the rays don\'t intersect in the same location. We
        look at the rays pairwise, and average the two_ray_intersect.
        If the points aren\'t sufficiently close together (given by
        max_ground_covariance), then we return None."""
        mean = np.array([0.0, 0, 0])
        var = np.array([0.0, 0, 0])
        cnt = 0
        max_dist = 0
        for i1 in range(self.igccol.number_image):
            if not tie_point.image_coordinate(i1):
                continue
            for i2 in range(i1 + 1, self.igccol.number_image):
                if tie_point.image_coordinate(i2):
                    break
            else:
                break
            t, skew = self.two_ray_intersect(
                tie_point.image_coordinate(i1), tie_point.image_coordinate(i2), i1, i2
            )
            if self.sample_cf_pt is None:
                self.sample_cf_pt = t
            if type(t) != type(self.sample_cf_pt):
                raise RuntimeError(
                    "Got unexpected type from self.two_ray_intersect. Original sample point is %s and point returned is %s"
                    % (self.sample_cf_pt, t)
                )
            max_dist = max(abs(skew), max_dist)
            t = np.array([t.position[0], t.position[1], t.position[2]])
            mean += t
            var += t * t
            cnt += 1
        if cnt < 1:
            return None
        mean = mean / cnt
        if cnt > 1:
            var = (var - (mean * mean) * cnt) / (cnt - 1)
        else:
            var = np.array([0.0, 0, 0])
        var = [max(v, max_dist * max_dist) for v in var]
        for v in var:
            if v > self.max_ground_covariance:
                logging.getLogger("geocal-python.ray_intersect").debug(
                    "Point with %d matches rejected by ray intersect, sqrt variance %f"
                    % (cnt, math.sqrt(v))
                )
                return None
        tie_point.ground_location = self.sample_cf_pt.create(
            [mean[0], mean[1], mean[2]]
        )
        return tie_point

    def two_ray_intersect(self, ic1, ic2, index_1=0, index_2=1):
        """Find the intersection of the ground for two rays given by
        two image coordinates. Can optionally supply the index into
        igccol, the default is that this is the first two entries.

        This returns the value as a np.array of CartesianFixed position, and
        the distance.
        """
        ri = RayIntersect(
            self.igccol.image_ground_connection(index_1),
            self.igccol.image_ground_connection(index_2),
        )
        return ri.two_ray_intersect(ic1, ic2)


class RayIntersect3(object):
    """This class is used to find the intersection of two or more rays
    on the surface. Note that in general the points don\'t actually
    intersect, this finds the values that minimizes the residual."""

    def __init__(self, igccol, delta_height=10, max_ground_covariance=20 * 20):
        self.ri = RayIntersect2(igccol, delta_height, max_ground_covariance)
        self.igccol = igccol

    def coll_eq(self, x):
        res = np.empty(2 * self.tp.number_image_location)
        j = 0
        pt = self.create_cf(x[0], x[1], x[2])
        for i in range(self.tp.number_image):
            if self.tp.image_coordinate(i):
                ic = self.tp.image_coordinate(i)
                try:
                    icpred = self.igccol.image_coordinate(i, pt)
                    res[j] = (ic.line - icpred.line) / self.tp.line_sigma(i)
                    res[j + 1] = (ic.sample - icpred.sample) / self.tp.sample_sigma(i)
                except RuntimeError as e:
                    if str(e) != "ImageGroundConnectionFailed":
                        raise e
                    res[j] = 0
                    res[j + 1] = 0
                j += 2
        return res

    def coll_jac(self, x):
        res = np.empty((2 * self.tp.number_image_location, 3))
        j = 0
        pt = self.create_cf(x[0], x[1], x[2])
        for i in range(self.tp.number_image):
            if self.tp.image_coordinate(i):
                ic = self.tp.image_coordinate(i)
                try:
                    jac = self.igccol.image_coordinate_jac_cf(i, pt)
                    res[j, :] = -jac[0, :] / self.tp.line_sigma(i)
                    res[j + 1, :] = -jac[1, :] / self.tp.sample_sigma(i)
                except RuntimeError as e:
                    if str(e) != "ImageGroundConnectionFailed":
                        raise e
                    res[j, :] = 0
                    res[j + 1, :] = 0
                j += 2
        return block_diag((res,), format="csr")

    def create_cf(self, x, y, z):
        return self.ri.sample_cf_pt.create([x, y, z])

    def ray_intersect(self, tp):
        # Start with what RayIntersect2 gives as the ground location
        tp = self.ri.ray_intersect(tp)
        if tp is None:
            return None
        self.tp = tp
        x0 = self.tp.ground_location.position
        x = lm_optimize(self.coll_eq, x0, self.coll_jac)
        tp.ground_location = self.create_cf(x[0], x[1], x[2])
        return tp


__all__ = ["RayIntersect2", "RayIntersect3"]
