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

def dem_generate_tile(dem_generate, file_name, offset, lstart, sstart, 
                      tile_nline, tile_nsamp, nline, nsamp):
    '''This is a function that wraps the calculation of tile of height
    data in a memory mapped file. This is really just meant for the program
    setup_dem_job.

    offset is the number of bytes to skip in the file to get to the data.
    The tile is described by lstart, sstart, tile_nline, and tile_nsamp. The
    full size of the data we are writing is given by nline and nsamp'''
    h = dem_generate.height_grid()
    d = np.memmap(file_name, dtype = np.float32, mode='r+', offset=offset,
                  shape=(nline, nsamp))
    d[lstart:(lstart + tile_nline),sstart:(sstart + tile_nsamp)] = h
    d.flush()

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
    map projected data for the two image.'''
    def __init__(self, image_ground_connnection1, 
                 image_ground_connnection2, 
                 aoi, image_matcher = CcorrLsmMatcher(),
                 surface_image1 = None, surface_image2 = None,
                 max_dist_good_point = 0.5):
        self.igc1 = image_ground_connnection1
        self.igc2 = image_ground_connnection2
        self.max_dist_good_point = max_dist_good_point
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
                      max_dist_good_point = dict["maxd"])
        self.h = dict["h"]
        self.r = dict["r"]

    def surface_point(self, lstart, sstart, lend, send):
        '''Calculate surface points'''
        return self.dem_match.surface_point(lstart, sstart, lend, send,
                                            self.stride, self.stride)

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
        
        
    def height_all(self, pool = None):
        '''Find list of surface points by exploring the entire AOI.
        
        Because this can take a while to run, you can optionally supply
        a multiprocessing.Pool. If this is given, we use that pool 
        of processors to do this calculation.'''
        lstart, sstart, lend, send = self.range_image1()
        if(pool is None):
            return self.surface_point(lstart, sstart, lend, send)
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
                            min(ss + sstep, send)])
        func = SurfacePointWrap(self)
        res = pool.map(func, arg)
        return np.concatenate(res)
        
    def height_grid(self, pool = None):
        '''Determine surface points from conjugate points, and resample to
        the AOI grid. Returns the height.

        Because this can take a while to run, you can optionally supply
        a multiprocessing.Pool. If this is given, we use that pool 
        of processors to do this calculation.'''
        self.r = self.height_all(pool)
        g = np.zeros((self.aoi.number_y_pixel, 
                      self.aoi.number_x_pixel, 2))
        for i in range(self.aoi.number_x_pixel):
            for j in range(self.aoi.number_y_pixel):
                g[j,i,0] = self.aoi.ground_coordinate(i, j).latitude
                g[j,i,1] = self.aoi.ground_coordinate(i, j).longitude
        if len(self.r) > 0:
            self.h = scipy.interpolate.griddata(self.r[:,0:2], self.r[:, 2], g, 
                                                method = "nearest") 
        else:
            self.h = np.zeros((self.aoi.number_y_pixel,
                               self.aoi.number_x_pixel))
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

        # Import is here rather than top of the function. We may be running this
        # in an environment without an interactive display, so this import will fail.
        # As long as the user doesn't call this function, this isn't a problem.
        import matplotlib.pyplot as plt

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
