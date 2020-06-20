# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _map_info.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_map_info')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_map_info')
    _map_info = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_map_info', [dirname(__file__)])
        except ImportError:
            import _map_info
            return _map_info
        try:
            _mod = imp.load_module('_map_info', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _map_info = swig_import_helper()
    del swig_import_helper
else:
    import _map_info
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except __builtin__.Exception:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _map_info.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_map_info.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_map_info.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_map_info.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_map_info.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_map_info.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_map_info.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_map_info.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_map_info.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_map_info.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_map_info.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_map_info.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_map_info.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_map_info.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_map_info.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_map_info.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_map_info.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _map_info.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _map_info.SHARED_PTR_DISOWN

import os

def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

def _new_from_serialization(data):
    return geocal_swig.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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

    We have a field indicating if the pixels are points or area. Note that
    we follow the GDAL convention - the value of the field doesn't change
    the geotiff parameters or any of the other fields. (-0.5, -0.5) is the
    ulc of the area, regardless of if we are point or area. But the value
    of this tells how to interpret the pixels in the RasterImage that this
    MapInfo is attached to - either as points or averages over area.

    C++ includes: map_info.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        MapInfo::MapInfo(const boost::shared_ptr< CoordinateConverter > &Conv, const
        blitz::Array< double, 1 > &Param, int Number_x_pixel, int
        Number_y_pixel, bool Is_point=false)
        Constructor that takes the affine parameters.

        Note that the parameters should be such that the ulc is at coordinates
        -0.5, 0.5. This is the same as "area based pixels", if you are using
        Geotiff. 
        """
        _map_info.MapInfo_swiginit(self, _map_info.new_MapInfo(*args))

    def _v_coordinate_converter(self):
        """

        const boost::shared_ptr<CoordinateConverter>& GeoCal::MapInfo::coordinate_converter_ptr() const
        Pointer to Coordinate converter used by map projection. 
        """
        return _map_info.MapInfo__v_coordinate_converter(self)


    @property
    def coordinate_converter(self):
        return self._v_coordinate_converter()


    def _v_is_point(self):
        """

        bool GeoCal::MapInfo::is_point() const
        True if we should interpret pixel as a point.

        Note that this doesn't change the coordinate calculation at all,
        (-0.5,-0.5) is still the coordinate of the area covered by the pixel.

        """
        return _map_info.MapInfo__v_is_point(self)


    @property
    def is_point(self):
        return self._v_is_point()


    def coordinate(self, Gc):
        """

        void MapInfo::coordinate(const Geodetic &Gc, double &Pixel_x_index, double &Pixel_y_index)
        const

        """
        return _map_info.MapInfo_coordinate(self, Gc)


    def cover(self, Pt, boundary=0):
        """

        MapInfo MapInfo::cover(const std::vector< boost::shared_ptr< GroundCoordinate > > &Pt, int
        boundary=0) const
        Create a MapInfo that has the same map projection and pixel resolution
        as this one, but covers the given set of points.

        An optional boundary can be given, this adds a boundary of the given
        number of pixels on each edge of the resulting MapInfo. 
        """
        return _map_info.MapInfo_cover(self, Pt, boundary)


    def ground_coordinate(self, *args):
        """

        boost::shared_ptr< GroundCoordinate > MapInfo::ground_coordinate(double Pixel_x_index, double Pixel_y_index) const
        Convert pixel coordinates to ground coordinates. 
        """
        return _map_info.MapInfo_ground_coordinate(self, *args)


    def intersection(self, Mi):
        """

        MapInfo MapInfo::intersection(const MapInfo &Mi) const
        This finds a rectangle box in the map projection and pixel size of
        this class that contains the intersection with the second map info Mi.

        """
        return _map_info.MapInfo_intersection(self, Mi)


    def map_union(self, Mi):
        """

        MapInfo MapInfo::map_union(const MapInfo &Mi) const
        This finds a rectangle box in the map projection and pixel size of
        this class that contains the union with the second map info Mi.

        Note we call this "map_union" because "union" is a reserved word
        in C++. 
        """
        return _map_info.MapInfo_map_union(self, Mi)


    def index_to_coordinate(self, *args):
        """

        void GeoCal::MapInfo::index_to_coordinate(const blitz::Array< double, 2 > &Pixel_x_index, const blitz::Array<
        double, 2 > &Pixel_y_index, blitz::Array< double, 2 >
        &Pixel_x_coordinate, blitz::Array< double, 2 > &Pixel_y_coordinate)
        const

        """
        return _map_info.MapInfo_index_to_coordinate(self, *args)


    def coordinate_to_index(self, *args):
        """

        void GeoCal::MapInfo::coordinate_to_index(const blitz::Array< double, 2 > &Pixel_x_coordinate, const
        blitz::Array< double, 2 > &Pixel_y_coordinate, blitz::Array< double, 2
        > &Pixel_x_index, blitz::Array< double, 2 > &Pixel_y_index) const

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

        MapInfo MapInfo::subset(double x_index, double y_index, int nx_pixel, int ny_pixel) const
        Return a MapInfo for a subset of this map info.

        Note that it is ok for x_index and y_index to be outside the range of
        the MapInfo, so you could use this function to get a MapInfo for a
        larger area or an area next to this one. 
        """
        return _map_info.MapInfo_subset(self, *args)


    def scale(self, Number_x_per_pixel, Number_y_per_pixel):
        """

        MapInfo MapInfo::scale(double Number_x_per_pixel, double Number_y_per_pixel) const
        Create a MapInfo that has the same upper left corner, and a pixel size
        scaled by the given number of line and samples per pixel.

        Note that in general this won't exactly cover this MapInfo, we trim
        lower left corner if needed to be the largest MapInfo that fits in
        this MapInfo. 
        """
        return _map_info.MapInfo_scale(self, Number_x_per_pixel, Number_y_per_pixel)


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


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _map_info.delete_MapInfo
MapInfo._v_coordinate_converter = new_instancemethod(_map_info.MapInfo__v_coordinate_converter, None, MapInfo)
MapInfo._v_is_point = new_instancemethod(_map_info.MapInfo__v_is_point, None, MapInfo)
MapInfo.coordinate = new_instancemethod(_map_info.MapInfo_coordinate, None, MapInfo)
MapInfo.cover = new_instancemethod(_map_info.MapInfo_cover, None, MapInfo)
MapInfo.ground_coordinate = new_instancemethod(_map_info.MapInfo_ground_coordinate, None, MapInfo)
MapInfo.intersection = new_instancemethod(_map_info.MapInfo_intersection, None, MapInfo)
MapInfo.map_union = new_instancemethod(_map_info.MapInfo_map_union, None, MapInfo)
MapInfo.index_to_coordinate = new_instancemethod(_map_info.MapInfo_index_to_coordinate, None, MapInfo)
MapInfo.coordinate_to_index = new_instancemethod(_map_info.MapInfo_coordinate_to_index, None, MapInfo)
MapInfo._v_lrc_x = new_instancemethod(_map_info.MapInfo__v_lrc_x, None, MapInfo)
MapInfo._v_lrc_y = new_instancemethod(_map_info.MapInfo__v_lrc_y, None, MapInfo)
MapInfo._v_number_x_pixel = new_instancemethod(_map_info.MapInfo__v_number_x_pixel, None, MapInfo)
MapInfo._v_number_y_pixel = new_instancemethod(_map_info.MapInfo__v_number_y_pixel, None, MapInfo)
MapInfo._v_resolution_meter = new_instancemethod(_map_info.MapInfo__v_resolution_meter, None, MapInfo)
MapInfo.subset = new_instancemethod(_map_info.MapInfo_subset, None, MapInfo)
MapInfo.scale = new_instancemethod(_map_info.MapInfo_scale, None, MapInfo)
MapInfo._v_ulc_x = new_instancemethod(_map_info.MapInfo__v_ulc_x, None, MapInfo)
MapInfo._v_ulc_y = new_instancemethod(_map_info.MapInfo__v_ulc_y, None, MapInfo)
MapInfo._v_transform = new_instancemethod(_map_info.MapInfo__v_transform, None, MapInfo)
MapInfo.__str__ = new_instancemethod(_map_info.MapInfo___str__, None, MapInfo)
MapInfo_swigregister = _map_info.MapInfo_swigregister
MapInfo_swigregister(MapInfo)


__all__ = ["MapInfo"]



