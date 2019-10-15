from builtins import range
import geocal_swig
from math import *
import numpy as np
import os.path
import re
from .shape_file import *

def _new_from_init(cls, version, *args):
    '''Handle older versions'''
    if(cls.pickle_format_version() < version):
        raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    # Added Fit_height_offset in version 2. For version 1, just set this to
    # False
    if(version ==1):
        inst.__init__(args[0], args[1], args[2], args[3], None, None, False)
    else:
        inst.__init__(*args)
    return inst

class GdalImageGroundConnection(geocal_swig.RpcImageGroundConnection):
    '''This is a convenience class that both reads a GDAL image, and
    creates a RPC based ground connection. Note that you can edit
    the RPC if desired, just change the values of the attribute
    rpc'''
    def __init__(self, fname, dem, rpc = None, title=None, image_mask=None,
                 ground_mask=None, fit_height_offset=False):
        self.fname = fname
        img = geocal_swig.GdalMultiBand(fname)
        if(not rpc):
            rpc = img.gdal_raster_image(0).rpc
        if(img.number_band == 1):
            img = img.gdal_raster_image(0)
        if(not title):
            title = os.path.basename(fname)
        geocal_swig.RpcImageGroundConnection.__init__(self, rpc, dem, img, title, 
                                                 image_mask, ground_mask, 
                                                 fit_height_offset)

    @classmethod
    def pickle_format_version(cls):
        return 2

    def __reduce__(self):
        '''Base class RpcImageGroundConnection can be pickled, but the 
        init function for this class is different. Give a simple reduce 
        to handle this.'''
        return _new_from_init, (self.__class__, 
                                self.__class__.pickle_format_version(),
                                self.fname,
                                self.dem, self.rpc, self.title,
                                self.image_mask, self.ground_mask, 
                                self.fit_height_offset)

class VicarImageGroundConnection(geocal_swig.RpcImageGroundConnection):
    '''This is a convenience class that both reads a VICAR image, and
    creates a RPC based ground connection. Note that you can edit
    the RPC if desired, just change the values of the attribute
    rpc'''
    def __init__(self, fname, dem, rpc = None, title=None, image_mask=None,
                 ground_mask=None, fit_height_offset=False):
        self.fname = fname
        # Check for multiple bands
        m = re.match(r'(.*)(\d+):(\d+)(.*)', fname)
        if(m):
            base = m.group(1)
            baseend = m.group(4)
            img = RasterImageMultiBandVariable()
            for i in range(int(m.group(2)), int(m.group(3)) + 1):
                img.add_raster_image(geocal_swig.VicarLiteRasterImage("%s%d%s" % (base, i, baseend)))
            if(not rpc):
                rpc = img.raster_image(0).rpc
        else:
            img = geocal_swig.VicarLiteRasterImage(fname)
            if(not rpc):
                rpc = img.rpc
        if(not title):
            title = os.path.basename(fname)
        geocal_swig.RpcImageGroundConnection.__init__(self, rpc, dem, img, title, 
                                                 image_mask, ground_mask, 
                                                 fit_height_offset)

    @classmethod
    def is_vicar_file(cls, fname):
        '''Return true if file is a vicar file'''
        m = re.match(r'(.*)(\d+):(\d+)(.*)', fname)
        if(m):
            base = m.group(1)
            baseend = m.group(4)
            i = int(m.group(2))
            return geocal_swig.VicarLiteFile.is_vicar_file("%s%d%s" % (base, i, baseend))
        else:
            return geocal_swig.VicarLiteFile.is_vicar_file(fname)
        
    @classmethod
    def pickle_format_version(cls):
        return 2

    def __reduce__(self):
        '''Base class RpcImageGroundConnection can be pickled, but the 
        init function for this class is different. Give a simple reduce 
        to handle this.'''
        return _new_from_init, (self.__class__, 
                                self.__class__.pickle_format_version(),
                                self.fname,
                                self.dem, self.rpc, self.title,
                                self.image_mask, self.ground_mask, 
                                self.fit_height_offset)

def _view_angle(self, image_coordinate = None, delta_h = 100):
    '''This return the zenith and azimuth view angles for a particular
    image coordinate as seen on the surface.

    This uses the equations and definitions as given in "Level 1
    Georectification and Registration Algorithm Theoretical Basis" JPL
    D-11532, Rev. B section 6.3.1.

    We determine the view angle by intersecting with the surface and a
    second surface delta_h above the dem. We then take the difference of 
    the two to give the look angle. You can optionally supply the delta_h
    to use. For the look vector to be a useful concept it needs to be 
    close to a straight line, meaning that in most cases the size of delta_h
    shouldn't change the results much.

    The center of the image is used if you don't supply image coordinates.

    The zenith and azimuth angles are return in that order, in degrees.
    '''
    if(image_coordinate is None):
        image_coordinate = geocal_swig.ImageCoordinate(self.number_line / 2.0,
                                                  self.number_sample /  2.0)
    gc1 = self.ground_coordinate(image_coordinate)
    h = gc1.height_reference_surface
    d = geocal_swig.SimpleDem(h + delta_h)
    gc2 = self.ground_coordinate_dem(image_coordinate, d)
    e1 = gc1.convert_to_cf()
    e2 = gc2.convert_to_cf()
    slat = sin(radians(gc1.latitude))
    clat = cos(radians(gc1.latitude))
    slon = sin(radians(gc1.longitude))
    clon = cos(radians(gc1.longitude))
    # Matrix used to change to local coordinates, with local up as z
    # and local north as x
    to_lc = np.array([[-clon * slat, -slon * slat,  clat],
                      [-slon,         clon,         0],
                      [-clon * clat,  -slon * clat, -slat]])
    lv = np.array([e2.position[0] - e1.position[0],
                   e2.position[1] - e1.position[1],
                   e2.position[2] - e1.position[2]])
    lv = lv / sqrt(np.dot(lv, lv))
    lc_dir = to_lc.dot(lv)
    az = degrees(atan2(lc_dir[1], lc_dir[0]))
    if(az < 0) : az += 360
    zen = 180 - degrees(acos(lc_dir[2] / sqrt(np.dot(lc_dir, lc_dir))))
    return zen, az

geocal_swig.ImageGroundConnection.view_angle = _view_angle

def _footprint_geometry(self, cconver = geocal_swig.GeodeticConverter()):
    '''Return a ogr Geometry object describing the footprint of the 
    ImageGroundConnection. This includes the 4 corners of the image 
    projected to the surface.

    You can supply the coordinates to use, the default it Geodetic
    longitude and latitude.

    This can then be used to write information to a ShapeFile.'''
    corners = []
    for pt in [geocal_swig.ImageCoordinate(0,0), 
               geocal_swig.ImageCoordinate(self.image.number_line, 0), 
               geocal_swig.ImageCoordinate(self.image.number_line, 
                                      self.image.number_sample), 
               geocal_swig.ImageCoordinate(0, self.image.number_sample)]:
        x, y, z = cconver.convert_to_coordinate(self.ground_coordinate(pt))
        corners.append((x, y))
    return ShapeLayer.polygon_2d(corners)

if(have_shape_file):
    geocal_swig.ImageGroundConnection.footprint_geometry = _footprint_geometry

__all__ = ["GdalImageGroundConnection", "VicarImageGroundConnection"]    
