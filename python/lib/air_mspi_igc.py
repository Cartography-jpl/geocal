from geocal import *
from math import *
import scipy.optimize

class AirMspiIgc(ImageGroundConnection):
    '''This is an AirMSPI Ellipsoid projected file as a ImageGroundConnection.
    We scale the data from a float (range 0 to 1.0) to a 16 bit integer
    (range 0 to 32767).

    This class is in python, and is fairly slow. We typically start with
    this class, but then fit a RPC to this to speed up the processing.'''
    def __init__(self, fname, title = "Image", ellipsoid_height = 0,
                 group_name = "555nm_band"):
        ImageGroundConnection.__init__(self)
        # Save initial state, so we can pickle this.
        self.fname = fname
        self.group_name = group_name
        # Read in the full image mask, to figure out how we will subset
        # the rest of the data.

        # There are 2 formats, HDF 4 and HDF 5. We try 4 first, and if
        # it fails we then try the HDF 5 format. We can tell which by
        # using our HdfFile.is_hdf, which returns True only for HDF 5 files. 
        if(HdfFile.is_hdf(fname)):
            self.gdal_base = "HDF5:\"%s\"://HDFEOS/GRIDS/%s/Data_Fields/" % (fname, group_name)
        else:
            self.gdal_base = "HDF4_EOS:EOS_GRID:%s:%s:" % (fname, group_name)
        im = ImageMaskImage(GdalRasterImage(self.gdal_base + "I.mask"))
        self.bounding_box = im.unmasked_bounding_box()
        # Now read in the reset of the data
        self.image = ScaleImage(self.__gdal_data("I"), 32767.0)
        self.image_mask = ImageMaskImage(self.__gdal_data("I.mask"))
        self.ground_mask = GroundMaskImage(self.__gdal_data("I.mask"))
        self.vzen = self.__gdal_data("View_zenith")
        self.vaz = self.__gdal_data("View_azimuth")
        self.dem = SimpleDem(ellipsoid_height)
        self.title = title

    def __gdal_data(self, name):
        '''Short cut for reading data with GDAL, and subsetting'''
        t = GdalRasterImage(self.gdal_base + name)
        return SubRasterImage(t, *self.bounding_box)
                                         
    def __getstate__(self):
        return { "fname" : self.fname,
                 "title" : self.title,
                 "ellipsoid_height" : self.dem.h,
                 "group_name" : self.group_name }
        
    def __setstate__(self, dict):
        self.__init__(dict["fname"], dict["title"], dict["ellipsoid_height"],
                      dict["group_name"])

    def __to_lc(self, ic):
        '''Determine matrix that takes us to local coordinates for the given
        image location. This maps ECR direction to local coordinates.'''
        gc = self.image.ground_coordinate(ic)
        slat = sin(radians(gc.latitude))
        clat = cos(radians(gc.latitude))
        slon = sin(radians(gc.longitude))
        clon = cos(radians(gc.longitude))
        return np.array([[-clon * slat, -slon * slat,  clat],
                         [-slon,         clon,         0],
                         [-clon * clat,  -slon * clat, -slat]])

    def __from_lc(self, ic):
        '''Determine matrix that takes us from local coordinates for the 
        given location. This maps local coordinate to ECR direction.'''
        return np.transpose(self.__to_lc(ic))

    def view_zenith(self, ic):
        '''Return view zenith as degrees. This interpolates the underlying 
        data. Returns -999 if the data is masked'''
        if(self.image_mask.mask_ic(ic)):
            return -999
        return self.vzen.interpolate(ic)

    def view_zenith(self, ic):
        '''Return view zenith as degrees. This interpolates the underlying 
        data. Returns -999 if the data is masked'''
        if(self.image_mask.mask_ic(ic)):
            return -999
        return self.vzen.interpolate(ic)

    def view_azimuth(self, ic):
        '''Return view azimuth as degrees. This interpolates the underlying 
        data. Returns -999 if the data is masked'''
        if(self.image_mask.mask_ic(ic)):
            return -999
        ln = int(floor(ic.line))
        smp = int(floor(ic.sample))
        dln = ic.line - ln
        dsmp = ic.sample - smp
        pt = self.vaz.read_double(ln, smp, 2, 2).flatten()
        # Special handling near the transition
        if(pt[0] > 300):
            for i in range(len(pt)):
                if(pt[i] < 60):
                    pt[i] += 360
        else:
            for i in range(len(pt)):
                if(pt[i] > 300):
                    pt[i] -= 360
        res = pt[0] * (1 - dln) * (1 - dsmp) + \
            pt[1] * dln * (1 - dsmp) + \
            pt[2] * dln * dsmp + \
            pt[3] * (1 - dln) * dsmp
        if(res < 0):
            res += 360
        if(res > 360):
            res -= 360
        return res

    def cf_look_vector_lv(self, ic):
        '''Return look vector.'''
        if(self.image_mask.mask_ic(ic)):
            raise RuntimeError("Masked data at (%f, %f)" % (ic.line, ic.sample))
        az = self.view_azimuth(ic)
        zen = self.view_zenith(ic)
        lc = [-cos(radians(az)) * sin(radians(180 - zen)),
              -sin(radians(az)) * sin(radians(180 - zen)),
              -cos(radians(180 - zen))]
        return CartesianFixedLookVector(self.__from_lc(ic).dot(lc))

    def cf_look_vector_pos(self, ic):
        '''Return point along the look vector.'''
        return Ecr(self.image.ground_coordinate(ic, self.dem))

    def ground_coordinate_dem(self, ic, d):
        '''Determine what ground coordinate is seen the given DEM for the
        given ImageCoordinate'''
        lv, p = self.cf_look_vector(ic)
        resolution = 1.0
        return d.intersect(p, lv, resolution)

    def image_coordinate(self, gc):
        def func(x, self, gc):
            ic = ImageCoordinate(x[0], x[1])
            gc2 = self.ground_coordinate_dem(ic, 
                          SimpleDem(gc.height_reference_surface))
            return [gc.latitude - gc2.latitude, gc.longitude - gc2.longitude]
        xsol = scipy.optimize.root(func, [self.number_line / 2.0,
                                          self.number_sample / 2.0],
                                   args=(self, gc), tol = 0.01)
        if(not xsol['success']):
            raise RuntimeError("Couldn't find solution")
        return ImageCoordinate(xsol["x"][0], xsol["x"][1])

    def __str__(self):
        return '''AirMspiIgc:
  File name:        %s
  Group name:       %s
  Title:            %s
  Ellipsoid Height: %f''' % (self.fname, self.group_name, self.title, 
                             self.dem.h)
