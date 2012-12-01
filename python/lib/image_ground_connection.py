from geocal import *
from math import *
import numpy as np
import os.path

class GdalImageGroundConnection(geocal.RpcImageGroundConnection):
    '''This is a convenience class that both reads a GDAL image, and
    creates a RPC based ground connection. Note that you can edit
    the RPC if desired, just change the values of the attribute
    rpc'''
    def __init__(self, fname, dem, rpc = None, title=None):
        self.fname = fname
        img = GdalRasterImage(fname)
        if(not title):
            title = os.path.basename(fname)
        if(not rpc):
            rpc = img.rpc
        geocal.RpcImageGroundConnection.__init__(self, rpc, dem, img, title)

    def __reduce__(self):
        '''Base class RpcImageGroundConnection can be pickled, but the 
        init function for this class is different. Give a simple reduce 
        to handle this.'''
        return geocal._new_from_init, (self.__class__, 
                                       self.fname,
                                       self.dem, self.rpc, self.title)

class VicarImageGroundConnection(geocal.RpcImageGroundConnection):
    '''This is a convenience class that both reads a VICAR image, and
    creates a RPC based ground connection. Note that you can edit
    the RPC if desired, just change the values of the attribute
    rpc'''
    def __init__(self, fname, dem, rpc = None, title=None):
        self.fname = fname
        img = VicarLiteRasterImage(fname)
        if(not title):
            title = os.path.basename(fname)
        if(not rpc):
            rpc = img.rpc
        geocal.RpcImageGroundConnection.__init__(self, rpc, dem, img, title)

    def __reduce__(self):
        '''Base class RpcImageGroundConnection can be pickled, but the 
        init function for this class is different. Give a simple reduce 
        to handle this.'''
        return geocal._new_from_init, (self.__class__, 
                                       self.fname,
                                       self.dem, self.rpc, self.title)

def _view_angle(self, image_coordinate, delta_h = 100):
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

    The zenith and azimuth angles are return in that order, in degrees.
    '''
    gc1 = self.ground_coordinate(image_coordinate)
    h = gc1.height_reference_surface
    d = self.dem
    self.dem = SimpleDem(h + delta_h)
    gc2 = self.ground_coordinate(image_coordinate)
    self.dem = d
    e1 = Ecr(gc1)
    e2 = Ecr(gc2)
    slat = sin(radians(gc1.latitude))
    clat = cos(radians(gc1.latitude))
    slon = sin(radians(gc1.longitude))
    clon = cos(radians(gc1.longitude))
    # Matrix used to change to local coordinates, with local up as z
    # and local north as x
    to_lc = np.array([[-clon * slat, -slon * slat,  clat],
                      [-slon,         clon,         0],
                      [-clon * clat,  -slon * clat, -slat]])
    # I'm pretty sure this is the right direction of "follow the photon"
    # convention used by MISR
    lv = np.array([e2.position[0] - e1.position[0],
                   e2.position[1] - e1.position[1],
                   e2.position[2] - e1.position[2]])
    lc_dir = np.dot(to_lc, lv)
    az = degrees(atan2(lc_dir[1], lc_dir[0]))
    if(az < 0) : az += 360
    zen = 180 - degrees(acos(lc_dir[2] / sqrt(np.dot(lv, lv))))
    return zen, az

geocal.ImageGroundConnection.view_angle = _view_angle
