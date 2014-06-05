# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _map_info.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_map_info', [dirname(__file__)])
        except ImportError:
            import _map_info
            return _map_info
        if fp is not None:
            try:
                _mod = imp.load_module('_map_info', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _map_info = swig_import_helper()
    del swig_import_helper
else:
    import _map_info
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except:
    weakref_proxy = lambda x: x


SHARED_PTR_DISOWN = _map_info.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_vector(cls, version, lst):
    '''Create a vector from a list.'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    for i in lst:
       inst.append(i)
    return inst

def _new_from_set(cls, version, *args):
    '''For use with pickle, covers common case where we use a set function 
    to assign the value'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__()
    inst.set(*args)
    return inst

import geocal_swig.generic_object
class MapInfo(geocal_swig.generic_object.GenericObject):
    """
    This contains information describing a map.

    This class is used to treat map projection information the same,
    regardless of the specific map projection used. However, the pixels of
    the map are required to be equally spaced in the X and Y coordinates
    of the underlying map projection.

    Note that the various coordinates (e.g., lrc_x()) are for the bounding
    box, not for the center of the lrc or ulc pixel.

    The parameters used in MapInfo are the affine parameters, see for
    example GeoTiff. Note though that by convention we use the middle of
    the ulc pixel is at (0,0), and the upper left corner of the pixel is
    at (-0.5, -0.5). The parameters on the other hand have the center of
    the ulc pixel at (0.5, 0.5) and the upper left corner of the pixel at
    (0, 0). This class handles this difference, but we mention this in
    this comment so you can understand where the various "0.5" terms
    come in.

    C++ includes: map_info.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::MapInfo::MapInfo()
        Default constructor. 
        """
        _map_info.MapInfo_swiginit(self,_map_info.new_MapInfo(*args))
    def _v_coordinate_converter(self):
        """
        const boost::shared_ptr<CoordinateConverter>& GeoCal::MapInfo::coordinate_converter_ptr() const
        Pointer to Coordinate converter used by map projection. 
        """
        return _map_info.MapInfo__v_coordinate_converter(self)

    @property
    def coordinate_converter(self):
        return self._v_coordinate_converter()

    def coordinate(self, *args):
        """
        void MapInfo::coordinate(const GroundCoordinate &Gc, double &Pixel_x_index, double
        &Pixel_y_index) const
        Determine pixel coordinates for the given ground coordinates.

        Note that this routine can be called with ground coordiantes outside
        of the bounding box of the map, it just returns pixel coordinates
        outside of the map in that case. 
        """
        return _map_info.MapInfo_coordinate(self, *args)

    def cover(self, *args):
        """
        MapInfo MapInfo::cover(const std::vector< boost::shared_ptr< GroundCoordinate > > &Pt, int
        boundary=0) const
        Create a MapInfo that has the same map projection and pixel resolution
        as this one, but covers the given set of points.

        An optional boundary can be given, this adds a boundary of the given
        number of pixels on each edge of the resulting MapInfo. 
        """
        return _map_info.MapInfo_cover(self, *args)

    def ground_coordinate(self, *args):
        """
        boost::shared_ptr< GroundCoordinate > MapInfo::ground_coordinate(double Pixel_x_index, double Pixel_y_index, const Dem &D) const
        Convert pixel coordinates to ground coordinates, and place on surface
        using DEM. 
        """
        return _map_info.MapInfo_ground_coordinate(self, *args)

    def intersection(self, *args):
        """
        MapInfo MapInfo::intersection(const MapInfo &Mi) const
        This finds a rectangle box in the map projection and pixel size of
        this class that contains the intersection with the second map info Mi.

        """
        return _map_info.MapInfo_intersection(self, *args)

    def map_union(self, *args):
        """
        MapInfo MapInfo::map_union(const MapInfo &Mi) const
        This finds a rectangle box in the map projection and pixel size of
        this class that contains the union with the second map info Mi.

        Note we call this "map_union" because "union" is a reserved word
        in C++. 
        """
        return _map_info.MapInfo_map_union(self, *args)

    def index_to_coordinate(self, *args):
        """
        void GeoCal::MapInfo::index_to_coordinate(double Pixel_x_index, double Pixel_y_index, double
        &Pixel_x_coordinate, double &Pixel_y_coordinate) const
        Convert pixel index to pixel coordinate. 
        """
        return _map_info.MapInfo_index_to_coordinate(self, *args)

    def coordinate_to_index(self, *args):
        """
        void GeoCal::MapInfo::coordinate_to_index(double Pixel_x_coordinate, double Pixel_y_coordinate, double
        &Pixel_x_index, double &Pixel_y_index) const
        Convert pixel coordinate to pixel index. 
        """
        return _map_info.MapInfo_coordinate_to_index(self, *args)

    def _v_lrc_x(self):
        """
        double GeoCal::MapInfo::lrc_x() const
        X of lower right corner.

        Note that this is for the outer edge of the pixel, not the center
        (i.e., for the bounding box). 
        """
        return _map_info.MapInfo__v_lrc_x(self)

    @property
    def lrc_x(self):
        return self._v_lrc_x()

    def _v_lrc_y(self):
        """
        double GeoCal::MapInfo::lrc_y() const
        Y of lower right corner.

        Note that this is for the outer edge of the pixel, not the center
        (i.e., for the bounding box). 
        """
        return _map_info.MapInfo__v_lrc_y(self)

    @property
    def lrc_y(self):
        return self._v_lrc_y()

    def _v_number_x_pixel(self):
        """
        int GeoCal::MapInfo::number_x_pixel() const
        Number of pixels in X direction. 
        """
        return _map_info.MapInfo__v_number_x_pixel(self)

    @property
    def number_x_pixel(self):
        return self._v_number_x_pixel()

    def _v_number_y_pixel(self):
        """
        int GeoCal::MapInfo::number_y_pixel() const
        Number of pixels in Y direction. 
        """
        return _map_info.MapInfo__v_number_y_pixel(self)

    @property
    def number_y_pixel(self):
        return self._v_number_y_pixel()

    def _v_resolution_meter(self):
        """
        double MapInfo::resolution_meter() const
        Return the approximate resolution of the MapInfo in meters.

        This looks at the center of the map, + 1 in the X and Y direction. We
        determine the distance between these points, and select the maximum of
        them. 
        """
        return _map_info.MapInfo__v_resolution_meter(self)

    @property
    def resolution_meter(self):
        return self._v_resolution_meter()

    def subset(self, *args):
        """
        MapInfo MapInfo::subset(int x_index, int y_index, int nx_pixel, int ny_pixel) const
        Return a MapInfo for a subset of this map info.

        Note that it is ok for x_index and y_index to be outside the range of
        the MapInfo, so you could use this function to get a MapInfo for a
        larger area or an area next to this one. 
        """
        return _map_info.MapInfo_subset(self, *args)

    def scale(self, *args):
        """
        MapInfo MapInfo::scale(double Number_x_per_pixel, double Number_y_per_pixel) const
        Create a MapInfo that has the same upper left corner, and a pixel size
        scaled by the given number of line and samples per pixel.

        Note that in general this won't exactly cover this MapInfo, we trim
        lower left corner if needed to be the largest MapInfo that fits in
        this MapInfo. 
        """
        return _map_info.MapInfo_scale(self, *args)

    def _v_ulc_x(self):
        """
        double GeoCal::MapInfo::ulc_x() const
        X of upper left corner.

        Note that this is for the outer edge of the pixel, not the center
        (i.e., for the bounding box). 
        """
        return _map_info.MapInfo__v_ulc_x(self)

    @property
    def ulc_x(self):
        return self._v_ulc_x()

    def _v_ulc_y(self):
        """
        double GeoCal::MapInfo::ulc_y() const
        Y of upper left corner.

        Note that this is for the outer edge of the pixel, not the center
        (i.e., for the bounding box). 
        """
        return _map_info.MapInfo__v_ulc_y(self)

    @property
    def ulc_y(self):
        return self._v_ulc_y()

    def _v_transform(self):
        """
        blitz::Array<double, 1> GeoCal::MapInfo::transform() const
        This is the affine geometric transformation used by the MapInfo.

        You don't normally use this directly, but it is useful when writing
        out file metadata, e.g., with Gdal SetGeoTransform. 
        """
        return _map_info.MapInfo__v_transform(self)

    @property
    def transform(self):
        return self._v_transform()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.coordinate_converter,self.transform,self.number_x_pixel,self.number_y_pixel)

    __swig_destroy__ = _map_info.delete_MapInfo
MapInfo._v_coordinate_converter = new_instancemethod(_map_info.MapInfo__v_coordinate_converter,None,MapInfo)
MapInfo.coordinate = new_instancemethod(_map_info.MapInfo_coordinate,None,MapInfo)
MapInfo.cover = new_instancemethod(_map_info.MapInfo_cover,None,MapInfo)
MapInfo.ground_coordinate = new_instancemethod(_map_info.MapInfo_ground_coordinate,None,MapInfo)
MapInfo.intersection = new_instancemethod(_map_info.MapInfo_intersection,None,MapInfo)
MapInfo.map_union = new_instancemethod(_map_info.MapInfo_map_union,None,MapInfo)
MapInfo.index_to_coordinate = new_instancemethod(_map_info.MapInfo_index_to_coordinate,None,MapInfo)
MapInfo.coordinate_to_index = new_instancemethod(_map_info.MapInfo_coordinate_to_index,None,MapInfo)
MapInfo._v_lrc_x = new_instancemethod(_map_info.MapInfo__v_lrc_x,None,MapInfo)
MapInfo._v_lrc_y = new_instancemethod(_map_info.MapInfo__v_lrc_y,None,MapInfo)
MapInfo._v_number_x_pixel = new_instancemethod(_map_info.MapInfo__v_number_x_pixel,None,MapInfo)
MapInfo._v_number_y_pixel = new_instancemethod(_map_info.MapInfo__v_number_y_pixel,None,MapInfo)
MapInfo._v_resolution_meter = new_instancemethod(_map_info.MapInfo__v_resolution_meter,None,MapInfo)
MapInfo.subset = new_instancemethod(_map_info.MapInfo_subset,None,MapInfo)
MapInfo.scale = new_instancemethod(_map_info.MapInfo_scale,None,MapInfo)
MapInfo._v_ulc_x = new_instancemethod(_map_info.MapInfo__v_ulc_x,None,MapInfo)
MapInfo._v_ulc_y = new_instancemethod(_map_info.MapInfo__v_ulc_y,None,MapInfo)
MapInfo._v_transform = new_instancemethod(_map_info.MapInfo__v_transform,None,MapInfo)
MapInfo.__str__ = new_instancemethod(_map_info.MapInfo___str__,None,MapInfo)
MapInfo_swigregister = _map_info.MapInfo_swigregister
MapInfo_swigregister(MapInfo)



