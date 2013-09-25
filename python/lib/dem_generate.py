from geocal import *
import math
import scipy.interpolate
from image_to_image_connection import *
from ray_intersect import *
import itertools
import multiprocessing
from multiprocessing import Pool
import numpy as np
import numpy.linalg as la
from igc_collection import *
from ply_file import *
import safe_matplotlib_import
import matplotlib.pyplot as plt
import logging

def _new_from_init(cls, version, *args):
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

class DemGenerateMB(geocal.CalcRasterMultiBand):
    '''This is an adapter than makes the more complicated DemGenerate 
    class look like just a CalcRasterMultiBand. We may eventually just
    merge this functionality into DemGenerate, but for now keeping this
    separate seems to be cleaner.
    '''
    def __init__(self, igc1, igc2, mi, interpolate_method = 'linear', 
                 image_matcher = CcorrLsmMatcher(),
                 surface_image1 = None, surface_image2 = None,
                 max_dist_good_point = 0.5,
                 buffer_size = 5):
        self.dem_generate = DemGenerate(igc1, igc2, mi, 
                            image_matcher = image_matcher,
                            surface_image1 = surface_image1,
                            surface_image2 = surface_image2,
                            max_dist_good_point = max_dist_good_point)
        self.interpolate_method = interpolate_method
        self.buffer_size = buffer_size
        self.map_info = mi
        CalcRasterMultiBand.__init__(self, mi, 2)

    @classmethod
    def pickle_format_version(cls):
        return 1

    def __reduce__(self):
        return _new_from_init, \
            (self.__class__, 
             self.__class__.pickle_format_version(),
             self.dem_generate.igc1,
             self.dem_generate.igc2,
             self.map_info,
             self.interpolate_method,
             self.dem_generate.image_matcher,
             self.dem_generate.surface_image1,
             self.dem_generate.surface_image2,
             self.dem_generate.max_dist_good_point,
             self.buffer_size,
             )

    def calc(self, lstart, sstart):
        self.dem_generate.aoi = \
            self.map_info.subset(sstart, lstart, self.data.shape[2],
                                 self.data.shape[1])
        self.data[0,:,:] = self.dem_generate.height_grid(\
               interpolate_method = self.interpolate_method, 
               buffer_size = self.buffer_size)
        self.data[1,:,:] = self.dem_generate.h_fill

class SurfacePointWrap(object):    
    '''Wrapper around demg.surface_point that can be pickled. We can't
    directly use pool.map on demg.surface_point because python can't 
    pickle a instance function'''
    def __init__(self, demg):
        self.demg = demg
    def __call__(self, it):
        return self.demg.surface_point(*it)

class DemGenerate:
    '''This uses image matching between two images to determine a surface
    Dem. The aoi should be a MapInfo giving the desired ground coverage.

    We can either do image matching in the original unprojected image (good
    when two image are close to the same angle), or we can do the image 
    matching on the surface. By default we use the original image, but you
    can optionally supply two surface images to use instead. These should be
    map projected data for the two image.

    We do a range calculation by default to decide how much of the
    image to look at for height (so we don't do unneeded
    matching). This breaks down for AirMISP data. Might come up with a
    more general solution at some point, but for now just skip this
    step if "all_image" is True.
    '''
    def __init__(self, image_ground_connnection1, 
                 image_ground_connnection2, 
                 aoi, image_matcher = CcorrLsmMatcher(),
                 surface_image1 = None, surface_image2 = None,
                 max_dist_good_point = 0.5,
                 all_image = False):
        self.igc1 = image_ground_connnection1
        self.igc2 = image_ground_connnection2
        self.max_dist_good_point = max_dist_good_point
        self.all_image = all_image
        self.initial_dem = self.igc1.dem
        self.surface_image1 = surface_image1
        self.surface_image2 = surface_image2
        self.image_matcher = image_matcher
        if(surface_image1 is None or
           surface_image2 is None):
            self.itoim = IgcImageToImageMatch(self.igc1, self.igc2, 
                                              self.image_matcher)
        else:
            self.itoim = SurfaceImageToImageMatch(self.igc1, 
                                                  self.surface_image1,
                                                  self.igc2,
                                                  self.surface_image2,
                                                  image_matcher)
        self.ri = RayIntersect(self.igc1, self.igc2)
        self.aoi = aoi
        self.dem_match = DemMatch(self.itoim, self.ri, self.max_dist_good_point)
        self.stride = int(round(self.aoi.resolution_meter / 
                                self.igc1.resolution_meter()))
        self.h = None
        self.r = None

    def __getstate__(self):
        return { "igc1" : self.igc1, "igc2" : self.igc2, 
                 "all_image" : self.all_image,
                 "aoi" : self.aoi, 
                 "im" : self.image_matcher,
                 "maxd" : self.max_dist_good_point,
                 "simg1" : self.surface_image1,
                 "simg2" : self.surface_image2,
                 "h" : self.h,
                 "r" : self.r,
                 }

    def __setstate__(self, dict):
        self.__init__(dict["igc1"], dict["igc2"], 
                      dict["aoi"], image_matcher = dict["im"],
                      surface_image1 = dict["simg1"],
                      surface_image2 = dict["simg2"],
                      max_dist_good_point = dict["maxd"],
                      all_image = dict["all_image"])
        self.h = dict["h"]
        self.r = dict["r"]

    def surface_point(self, lstart, sstart, lend, send, include_image = False):
        '''Calculate surface points'''
        r = self.dem_match.surface_point(lstart, sstart, lend, send,
                                         self.stride, self.stride, 
                                         include_image)
        log = logging.getLogger("afids-python.dem_generate")
        log.info("Number point:   %d" %(self.dem_match.number_point))
        log.info("Number match:   %d" %(self.dem_match.number_match))
        log.info("Number success: %d" %(self.dem_match.number_success))
        log.info("All distance:\n%s" % self.dem_match.all_distance_stat)
        log.info("Good distance:\n%s" % self.dem_match.good_distance_stat)
        log.info("Points not matched:")
        log.info("  Image masked:                 %d" \
            %(self.dem_match.diagnostic[1]))
        log.info("  Too close to image edge:      %d" \
            %(self.dem_match.diagnostic[2]))
        log.info("  Variance too low:             %d" \
            %(self.dem_match.diagnostic[3]))
        log.info("  Correlation too low:          %d" \
            %(self.dem_match.diagnostic[4]))
        log.info("  Exceed max sigma:             %d" \
            %(self.dem_match.diagnostic[5]))
        log.info("  Exceed max radiance variance: %d" \
            %(self.dem_match.diagnostic[6]))
        log.info("  Exceed precision requirement: %d" \
            %(self.dem_match.diagnostic[7]))
        log.info("  Move past target:             %d" \
            %(self.dem_match.diagnostic[8]))
        log.info("  Solve failed:                 %d" \
            %(self.dem_match.diagnostic[9]))
        log.info("  Unknown:                      %d" \
            %(self.dem_match.diagnostic[10]))
        return r

    def find_intersection(self, ic1, ic2):
        '''Find the ground intersection of two conjugate image coordinates.
        Note that in general the points don't actually intersect, this finds
        the value that is closest to both points.'''
        p, dist = self.ri.two_ray_intersect(ic1, ic2)
        gres = Geodetic(p)
        if(dist > 1.0): 
            raise ValueError("Failure of epipolar constraint")
        return gres

    def write_ply(self, filename, scale_z = 1.0):
        '''Write out a PLY file. We write this is pixel coordinates so 
        things are on the same scale. We could have a variation that 
        writes out cartesian coordinates if useful, but for now we just
        use pixel coordinates of aoi (translating height to pixels by
        using aoi.resolution_meter). You can form an exaggerated height
        by passing a scale to use.'''
        with PlyFile(filename) as fh:
            fh.vertex = np.zeros(self.r.shape)
            fh.vertex[:,2] = self.r[:,2] / self.aoi.resolution_meter * scale_z
            for i in range(self.r.shape[0]):
                fh.vertex[i,1], fh.vertex[i,0] = \
                    self.aoi.coordinate(Geodetic(self.r[i,0], self.r[i,1]))
        
        
    def height_all(self, pool = None, include_image = False, buffer_size = 5):
        '''Find list of surface points by exploring the entire AOI.
        
        Because this can take a while to run, you can optionally supply
        a multiprocessing.Pool. If this is given, we use that pool 
        of processors to do this calculation.
        '''
        if(self.all_image):
            lstart = 0
            sstart = 0
            lend = self.igc1.number_line
            send = self.igc1.number_sample
        else:
            lstart, sstart, lend, send = self.range_image1()
            lstart -= self.stride * buffer_size
            sstart -= self.stride * buffer_size
            lend += self.stride * buffer_size
            send += self.stride * buffer_size
        if(pool is None):
            return self.surface_point(lstart, sstart, lend, send, include_image)
        if(lstart > sstart):
            lstep = (lend - lstart) / multiprocessing.cpu_count()
            sstep = send - sstart
        else:
            lstep = lend - lstart
            sstep = (send - sstart) / multiprocessing.cpu_count()
        arg = []
        for ls in range(lstart, lend, lstep):
            for ss in range(sstart, send, sstep):
                arg.append([ls, ss, min(ls + lstep, lend),
                            min(ss + sstep, send), include_image])
        func = SurfacePointWrap(self)
        res = pool.map(func, arg)
        return np.concatenate(res)
        
    def height_grid(self, fill_value = -9999.0, pool = None, 
                    include_image = False, interpolate_method = 'nearest',
                    buffer_size = 5):
        '''Determine surface points from conjugate points, and resample to
        the AOI grid. Returns the height.

        We only fill grid cells that contain a height retrieval, there is
        no interpolation of filling in missing data. We may come up with
        a way to do that, but for now we just expect to have a lots of
        holes in the data. Grid cells with no height get assigned the
        given fill value.

        Because this can take a while to run, you can optionally supply
        a multiprocessing.Pool. If this is given, we use that pool 
        of processors to do this calculation.

        You can specify include_image as True, and then we return the height 
        map projected, and also the height in image 1 and image 2 space.'''
        self.r = self.height_all(pool, include_image, buffer_size = buffer_size)
        self.h = np.empty((self.aoi.number_y_pixel,
                           self.aoi.number_x_pixel))
        self.h[:] = fill_value
        if(include_image):
            self.img1_h = np.empty((self.igc1.number_line, 
                                    self.igc1.number_sample))
            self.img2_h = np.empty((self.igc2.number_line, 
                                    self.igc2.number_sample))
            self.img1_h[:] = fill_value
            self.img2_h[:] = fill_value
        for i in range(self.r.shape[0]):
            gp = Geodetic(self.r[i,0], self.r[i,1])
            sample,line = self.aoi.coordinate(gp)
            if(line >= 0 and line < self.h.shape[0] and
               sample >= 0 and sample < self.h.shape[1]):
                self.h[int(line), int(sample)] = self.r[i, 2]
            if(include_image):
                self.img1_h[int(round(self.r[i,3])), 
                            int(round(self.r[i,4]))] = self.r[i,2]
                self.img2_h[int(round(self.r[i,5])), 
                            int(round(self.r[i,6]))] = self.r[i,2]

        # For now, we'll also get the filled data. This will probably
        # go away to be replaced with something better in the future
        g = np.zeros((self.aoi.number_y_pixel, 
                      self.aoi.number_x_pixel, 2))
        for i in range(self.aoi.number_x_pixel):
            for j in range(self.aoi.number_y_pixel):
                g[j,i,0] = self.aoi.ground_coordinate(i, j).latitude
                g[j,i,1] = self.aoi.ground_coordinate(i, j).longitude
        if len(self.r) > 0:
            self.h_fill = scipy.interpolate.griddata(self.r[:,0:2], 
                                          self.r[:, 2], g, 
                                          method = interpolate_method,
                                          fill_value = fill_value) 
        else:
            self.h_fill = np.zeros((self.aoi.number_y_pixel,
                                    self.aoi.number_x_pixel))
            self.h_fill[:] = fill_value
        if(include_image):
            return self.h, self.img1_h, self.img2_h
        else:
            return self.h

    def dem_orig(self):
        dv_orig = np.zeros((self.aoi.number_y_pixel, 
                            self.aoi.number_x_pixel))
        for i in range(self.aoi.number_x_pixel):
            for j in range(self.aoi.number_y_pixel):
                dv_orig[j, i] = self.initial_dem.height_reference_surface \
                    (self.aoi.ground_coordinate(i, j))
        return dv_orig

    def plot_res(self, aoi_img = None):
        '''This is a utility that can be run after doing height_grid.
        It shows a plot summarizing the changes to the DEM and where
        we have found matches.

        You can optionally pass in an image to show in the plot. If passed
        in, this should be map projected and cover the same area as self.aoi.
        If you leave this out, then we just leave those plots empty.'''


        dv_orig = self.dem_orig()
        plt.subplot(221)
        #plt.imshow(dv_orig, vmin = 1300, vmax=1320)
        plt.imshow(dv_orig)
        plt.colorbar()
        plt.title("Original DEM")
        plt.subplot(222)
        #plt.imshow(self.h, vmin=1300, vmax=1320)
        plt.imshow(self.h)
        plt.colorbar()
        plt.title("High Res DEM")
        plt.subplot(223)
        plt.imshow(self.h - dv_orig)
        plt.colorbar()
        plt.title("HRES - Orig")
        if(aoi_img is not None):
            plt.subplot(224)
            data = aoi_img.read(0, 0, aoi_img.number_line, 
                                aoi_img.number_sample)
            plt.imshow(data, cmap = plt.cm.gray)
            plt.title("Image 1")
            plt.figure(2)
            x = []
            y = []
            for i in range(self.r.shape[0]):
                ic = aoi_img.coordinate(Geodetic(self.r[i, 0], 
                                                 self.r[i, 1]))
                if(ic.line >= 0 and ic.line < aoi_img.number_line and
                   ic.sample >= 0 and ic.sample < aoi_img.number_sample):
                    y.append(ic.line)
                    x.append(ic.sample)
            plt.subplot(111)
            plt.plot(x, y, '.')
            plt.imshow(data, cmap = plt.cm.gray)
        plt.show()


    def match(self, img1_line, img1_sample):
        '''Do image matching between image 1 and image 2, returning either
        None if this fails or a pair [ic1, ic2] which is the image coordinates
        in image 1 and the match in image 2'''
        ic1 = ImageCoordinate(img1_line, img1_sample)
        ic2, lsigma, ssigma, success, diagnostic = self.itoim.match(ic1)
        return [ic1, ic2] if(success) else [None, None]

    def range_image1(self):
        '''This determines the line and sample range in image 1 to covers
        the area of interest (aoi). This is just approximate, we map the
        four corners of the aoi into image 1 and determine the bounding
        box of this.'''
        g1 = self.aoi.ground_coordinate(0, 0)
        g2 = self.aoi.ground_coordinate(0, self.aoi.number_y_pixel)
        g3 = self.aoi.ground_coordinate(self.aoi.number_x_pixel, 0)
        g4 = self.aoi.ground_coordinate(self.aoi.number_x_pixel, 
                                        self.aoi.number_y_pixel)
        line = []
        sample = []
        for g in (g1, g2, g3, g4):
            ic = self.igc1.image_coordinate(self.initial_dem.surface_point(g))
            line.append(ic.line)
            sample.append(ic.sample)
        lstart = int(math.floor(min(line)))
        lend = int(math.ceil(max(line)))
        sstart = int(math.floor(min(sample)))
        send = int(math.ceil(max(sample)))
        return [lstart, sstart, lend, send]
