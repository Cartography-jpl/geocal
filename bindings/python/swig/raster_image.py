# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _raster_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_raster_image')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_raster_image')
    _raster_image = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_image', [dirname(__file__)])
        except ImportError:
            import _raster_image
            return _raster_image
        try:
            _mod = imp.load_module('_raster_image', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _raster_image = swig_import_helper()
    del swig_import_helper
else:
    import _raster_image
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
    __swig_destroy__ = _raster_image.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_raster_image.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_raster_image.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_raster_image.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_raster_image.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_raster_image.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_raster_image.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_raster_image.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_raster_image.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_raster_image.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_raster_image.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_raster_image.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_raster_image.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_raster_image.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_raster_image.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_raster_image.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_raster_image.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _raster_image.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _raster_image.SHARED_PTR_DISOWN

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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class RasterImageTileIterator(geocal_swig.generic_object.GenericObject):
    """

    It can be useful to step through a Raster Image in tiles.

    This is a helper class for RasterImage to do this.

    C++ includes: raster_image.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Ri):
        """

        RasterImageTileIterator::RasterImageTileIterator(const RasterImage &Ri, int Number_tile_line, int Number_tile_sample)
        Constructor, where we supply a tile size to use (which may be
        different than Ri's tile size). 
        """
        _raster_image.RasterImageTileIterator_swiginit(self, _raster_image.new_RasterImageTileIterator(Ri))

    def _v_istart(self):
        """

        int GeoCal::RasterImageTileIterator::istart() const

        """
        return _raster_image.RasterImageTileIterator__v_istart(self)


    @property
    def istart(self):
        return self._v_istart()


    def _v_jstart(self):
        """

        int GeoCal::RasterImageTileIterator::jstart() const

        """
        return _raster_image.RasterImageTileIterator__v_jstart(self)


    @property
    def jstart(self):
        return self._v_jstart()


    def _v_iend(self):
        """

        int GeoCal::RasterImageTileIterator::iend() const

        """
        return _raster_image.RasterImageTileIterator__v_iend(self)


    @property
    def iend(self):
        return self._v_iend()


    def _v_jend(self):
        """

        int GeoCal::RasterImageTileIterator::jend() const

        """
        return _raster_image.RasterImageTileIterator__v_jend(self)


    @property
    def jend(self):
        return self._v_jend()


    def _v_number_line(self):
        """

        int GeoCal::RasterImageTileIterator::number_line() const

        """
        return _raster_image.RasterImageTileIterator__v_number_line(self)


    @property
    def number_line(self):
        return self._v_number_line()


    def _v_number_sample(self):
        """

        int GeoCal::RasterImageTileIterator::number_sample() const

        """
        return _raster_image.RasterImageTileIterator__v_number_sample(self)


    @property
    def number_sample(self):
        return self._v_number_sample()


    def _v_end(self):
        """

        bool GeoCal::RasterImageTileIterator::end() const

        """
        return _raster_image.RasterImageTileIterator__v_end(self)


    @property
    def end(self):
        return self._v_end()

    __swig_destroy__ = _raster_image.delete_RasterImageTileIterator
RasterImageTileIterator._v_istart = new_instancemethod(_raster_image.RasterImageTileIterator__v_istart, None, RasterImageTileIterator)
RasterImageTileIterator._v_jstart = new_instancemethod(_raster_image.RasterImageTileIterator__v_jstart, None, RasterImageTileIterator)
RasterImageTileIterator._v_iend = new_instancemethod(_raster_image.RasterImageTileIterator__v_iend, None, RasterImageTileIterator)
RasterImageTileIterator._v_jend = new_instancemethod(_raster_image.RasterImageTileIterator__v_jend, None, RasterImageTileIterator)
RasterImageTileIterator._v_number_line = new_instancemethod(_raster_image.RasterImageTileIterator__v_number_line, None, RasterImageTileIterator)
RasterImageTileIterator._v_number_sample = new_instancemethod(_raster_image.RasterImageTileIterator__v_number_sample, None, RasterImageTileIterator)
RasterImageTileIterator._v_end = new_instancemethod(_raster_image.RasterImageTileIterator__v_end, None, RasterImageTileIterator)
RasterImageTileIterator.next = new_instancemethod(_raster_image.RasterImageTileIterator_next, None, RasterImageTileIterator)
RasterImageTileIterator.__str__ = new_instancemethod(_raster_image.RasterImageTileIterator___str__, None, RasterImageTileIterator)
RasterImageTileIterator_swigregister = _raster_image.RasterImageTileIterator_swigregister
RasterImageTileIterator_swigregister(RasterImageTileIterator)

class RasterImage(geocal_swig.generic_object.GenericObject):
    """

    This is a general image class.

    Conceptually, an image is just a two dimensional array of integers,
    although it is not necessarily implemented this way. This is an
    abstract class, containing behavior common to all images. It handles
    keeping track of the size of the image, but all other behaviour needs
    to be defined by subclasses.

    It is common for Raster data to be stored on disk as a series of
    tiles. It can be significantly more efficient to access and write the
    data in tiles. To support this, we provide a "preferred" tile size
    given by number_tile_line() and number_tile_sample(). Classes using a
    RasterImage are free to ignore this tiling - data can be read and
    written in any order desired, and the only impact of tiles is in the
    performance.

    It is very common to want to step through a file "efficiently".
    There is a macro "RASTER_FOREACH(Img, IND1, IND2)" that steps
    through a given raster image, setting the index variable IND1 and IND2
    to the line and samples. This steps through by tiles, so it avoids
    reading the same tile multiple times. If you need to step through by
    tiles (rather than looping through all line and samples), the class
    RasterImageTileIterator can be used.

    The default tile size is the entire image, derived classes can supply
    other values if desired.

    A RasterImage may or may not have a Rpc, Rsm, or MapInfo associated
    with it. You can query has_rpc(), has_rsm(), and has_mapinfo() to find
    out if it has this metadata, and if it does you can access this by
    rpc(), rsm() and map_info().

    C++ includes: raster_image.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined")
    __repr__ = _swig_repr

    def overview(self, Min_number_sample):
        """

        virtual boost::shared_ptr<RasterImage> GeoCal::RasterImage::overview(int Min_number_sample) const
        Some RasterImage may have overviews which are lower resolution images.

        If we do have these, then this will return the lower resolution
        RasterImage that has at least the given number of samples. If we don't
        have an overview, this returns a null - which the calling routine
        needs to be ready to handle. 
        """
        return _raster_image.RasterImage_overview(self, Min_number_sample)


    def interpolate(self, *args):
        """

        blitz::Array< double, 2 > RasterImage::interpolate(double Line, double Sample, int Number_line, int Number_sample,
        double Fill_value=0.0) const
        Interpolate a region, starting with the given fractional line and
        sample.

        This is a bilinear interpolation. This does padding with the given
        fill value. 
        """
        return _raster_image.RasterImage_interpolate(self, *args)


    def _v_copy_needs_double(self):
        """

        virtual bool GeoCal::RasterImage::copy_needs_double() const
        As an optimization, we assume when copying that data can be
        represented as a int.

        That is true of many images. But we actually need to use double, then
        we need to know that in the copy command. This function indicates if
        we need a double or not. 
        """
        return _raster_image.RasterImage__v_copy_needs_double(self)


    @property
    def copy_needs_double(self):
        return self._v_copy_needs_double()


    def _v_number_line(self):
        """

        virtual int GeoCal::RasterImage::number_line() const =0
        Return number of lines in the image. 
        """
        return _raster_image.RasterImage__v_number_line(self)


    @property
    def number_line(self):
        return self._v_number_line()


    def _v_number_sample(self):
        """

        virtual int GeoCal::RasterImage::number_sample() const =0
        Return number of samples in the image. 
        """
        return _raster_image.RasterImage__v_number_sample(self)


    @property
    def number_sample(self):
        return self._v_number_sample()


    def _v_number_tile_line(self):
        """

        virtual int GeoCal::RasterImage::number_tile_line() const
        Number of lines in the preferred tile size.

        Default is the entire image. 
        """
        return _raster_image.RasterImage__v_number_tile_line(self)


    @property
    def number_tile_line(self):
        return self._v_number_tile_line()


    def _v_number_tile_sample(self):
        """

        virtual int GeoCal::RasterImage::number_tile_sample() const
        Number of samples in the preferred tile size.

        Default is the entire image. 
        """
        return _raster_image.RasterImage__v_number_tile_sample(self)


    @property
    def number_tile_sample(self):
        return self._v_number_tile_sample()


    def unchecked_read_double(self, Line, Sample):
        """

        virtual double GeoCal::RasterImage::unchecked_read_double(int Line, int Sample) const =0

        """
        return _raster_image.RasterImage_unchecked_read_double(self, Line, Sample)


    def read_with_pad(self, Lstart, Sstart, Number_line, Number_sample, Fill_value=0):
        """

        blitz::Array< int, 2 > RasterImage::read_with_pad(int Lstart, int Sstart, int Number_line, int Number_sample, int
        Fill_value=0) const
        Return a subset of the image.

        This variation of read allows the data read to extend past the actual
        RasterImage (e.g., negative start line). In this case, we return the
        given fill value for the outside area. 
        """
        return _raster_image.RasterImage_read_with_pad(self, Lstart, Sstart, Number_line, Number_sample, Fill_value)


    def read_double(self, Lstart, Sstart, Number_line, Number_sample):
        """

        blitz::Array< double, 2 > RasterImage::read_double(int Lstart, int Sstart, int Number_line, int Number_sample) const
        Some raster images actually are floating point values (e.g., a
        CalcRaster).

        This function gives access to the underlying floating point numbers.
        The default function here just returns the result of read as a double,
        but derived classes can override this. 
        """
        return _raster_image.RasterImage_read_double(self, Lstart, Sstart, Number_line, Number_sample)


    def read_double_with_pad(self, Lstart, Sstart, Number_line, Number_sample, Fill_value=0.0):
        """

        blitz::Array< double, 2 > RasterImage::read_double_with_pad(int Lstart, int Sstart, int Number_line, int Number_sample, double
        Fill_value=0.0) const
        Return a subset of the image.

        This variation of read allows the data read to extend past the actual
        RasterImage (e.g., negative start line). In this case, we return the
        given fill value for the outside area. 
        """
        return _raster_image.RasterImage_read_double_with_pad(self, Lstart, Sstart, Number_line, Number_sample, Fill_value)


    def read(self, *args):
        """

        blitz::Array< int, 2 > RasterImage::read(int Lstart, int Sstart, int Number_line, int Number_sample) const
        Return a subset of the image. 
        """
        return _raster_image.RasterImage_read(self, *args)


    def write(self, *args):
        """

        void RasterImage::write(int Lstart, int Sstart, const blitz::Array< int, 2 > &A)

        """
        return _raster_image.RasterImage_write(self, *args)


    def _v_has_map_info(self):
        """

        virtual bool GeoCal::RasterImage::has_map_info() const
        Indicate if we have MapInfo.

        Functions available if we have MapInfo data.

        The default is false, but derived classes can override this. 
        """
        return _raster_image.RasterImage__v_has_map_info(self)


    @property
    def has_map_info(self):
        return self._v_has_map_info()


    def _v_map_info(self):
        """

        virtual const MapInfo& GeoCal::RasterImage::map_info() const
        MapInfo for image. 
        """
        return _raster_image.RasterImage__v_map_info(self)


    @property
    def map_info(self):
        return self._v_map_info()


    def ground_coordinate(self, *args):
        """

        boost::shared_ptr<GroundCoordinate> GeoCal::RasterImage::ground_coordinate(double Line, double Sample, const Dem &D) const
        Shortcut to calling mapinfo().ground_coordinate. 
        """
        return _raster_image.RasterImage_ground_coordinate(self, *args)


    def coordinate(self, *args):
        """

        ImageCoordinate GeoCal::RasterImage::coordinate(const GroundCoordinate &Gc) const
        Shortcut to calling mapinfo().coordinate. 
        """
        return _raster_image.RasterImage_coordinate(self, *args)


    def _v_grid_center_line_resolution(self):
        """

        double RasterImage::grid_center_line_resolution() const
        This calculates the grid resolution in meters for the center of the
        grid in the line direction.

        This is just the distance between The center pixel number_line() / 2,
        number_sample() / 2 and the pixel number_line() / 2 + 1,
        number_sample() / 2. 
        """
        return _raster_image.RasterImage__v_grid_center_line_resolution(self)


    @property
    def grid_center_line_resolution(self):
        return self._v_grid_center_line_resolution()


    def _v_grid_center_sample_resolution(self):
        """

        double RasterImage::grid_center_sample_resolution() const
        This calculates the grid resolution in meters for the center of the
        grid in the sample direction.

        This is just the distance between The center pixel number_line() / 2,
        number_sample() / 2 and the pixel number_line() / 2, number_sample() /
        2 + 1. 
        """
        return _raster_image.RasterImage__v_grid_center_sample_resolution(self)


    @property
    def grid_center_sample_resolution(self):
        return self._v_grid_center_sample_resolution()


    def _v_has_rpc(self):
        """

        virtual bool GeoCal::RasterImage::has_rpc() const
        Indicate if we have Rpc.

        Functions available if we have RPC

        The default is false, but derived classes can override this. 
        """
        return _raster_image.RasterImage__v_has_rpc(self)


    @property
    def has_rpc(self):
        return self._v_has_rpc()


    def _v_rpc(self):
        """

        boost::shared_ptr<Rpc> GeoCal::RasterImage::rpc_ptr() const
        Pointer version of rpc(). This makes python happier. 
        """
        return _raster_image.RasterImage__v_rpc(self)


    @property
    def rpc(self):
        return self._v_rpc()


    def _v_has_rsm(self):
        """

        virtual bool GeoCal::RasterImage::has_rsm() const
        Indicate if we have Rsm.

        Functions available if we have RSM

        The default is false, but derived classes can override this. 
        """
        return _raster_image.RasterImage__v_has_rsm(self)


    @property
    def has_rsm(self):
        return self._v_has_rsm()


    def _v_rsm(self):
        """

        virtual boost::shared_ptr<Rsm> GeoCal::RasterImage::rsm() const
        Rsm for image. 
        """
        return _raster_image.RasterImage__v_rsm(self)


    @property
    def rsm(self):
        return self._v_rsm()

    __swig_destroy__ = _raster_image.delete_RasterImage
RasterImage.overview = new_instancemethod(_raster_image.RasterImage_overview, None, RasterImage)
RasterImage.interpolate = new_instancemethod(_raster_image.RasterImage_interpolate, None, RasterImage)
RasterImage._v_copy_needs_double = new_instancemethod(_raster_image.RasterImage__v_copy_needs_double, None, RasterImage)
RasterImage._v_number_line = new_instancemethod(_raster_image.RasterImage__v_number_line, None, RasterImage)
RasterImage._v_number_sample = new_instancemethod(_raster_image.RasterImage__v_number_sample, None, RasterImage)
RasterImage._v_number_tile_line = new_instancemethod(_raster_image.RasterImage__v_number_tile_line, None, RasterImage)
RasterImage._v_number_tile_sample = new_instancemethod(_raster_image.RasterImage__v_number_tile_sample, None, RasterImage)
RasterImage.unchecked_read_double = new_instancemethod(_raster_image.RasterImage_unchecked_read_double, None, RasterImage)
RasterImage.read_with_pad = new_instancemethod(_raster_image.RasterImage_read_with_pad, None, RasterImage)
RasterImage.read_double = new_instancemethod(_raster_image.RasterImage_read_double, None, RasterImage)
RasterImage.read_double_with_pad = new_instancemethod(_raster_image.RasterImage_read_double_with_pad, None, RasterImage)
RasterImage.read = new_instancemethod(_raster_image.RasterImage_read, None, RasterImage)
RasterImage.write = new_instancemethod(_raster_image.RasterImage_write, None, RasterImage)
RasterImage.__str__ = new_instancemethod(_raster_image.RasterImage___str__, None, RasterImage)
RasterImage._v_has_map_info = new_instancemethod(_raster_image.RasterImage__v_has_map_info, None, RasterImage)
RasterImage._v_map_info = new_instancemethod(_raster_image.RasterImage__v_map_info, None, RasterImage)
RasterImage.ground_coordinate = new_instancemethod(_raster_image.RasterImage_ground_coordinate, None, RasterImage)
RasterImage.coordinate = new_instancemethod(_raster_image.RasterImage_coordinate, None, RasterImage)
RasterImage._v_grid_center_line_resolution = new_instancemethod(_raster_image.RasterImage__v_grid_center_line_resolution, None, RasterImage)
RasterImage._v_grid_center_sample_resolution = new_instancemethod(_raster_image.RasterImage__v_grid_center_sample_resolution, None, RasterImage)
RasterImage._v_has_rpc = new_instancemethod(_raster_image.RasterImage__v_has_rpc, None, RasterImage)
RasterImage._v_rpc = new_instancemethod(_raster_image.RasterImage__v_rpc, None, RasterImage)
RasterImage._v_has_rsm = new_instancemethod(_raster_image.RasterImage__v_has_rsm, None, RasterImage)
RasterImage._v_rsm = new_instancemethod(_raster_image.RasterImage__v_rsm, None, RasterImage)
RasterImage_swigregister = _raster_image.RasterImage_swigregister
RasterImage_swigregister(RasterImage)

class ArrayRasterImage(geocal_swig.generic_object.GenericObject):
    """

    This is a array of Raster Image.

    This isn't much more than a std::vector, but we do have added support
    in python of being able to serialize this.

    C++ includes: raster_image.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        """

        GeoCal::ArrayRasterImage::ArrayRasterImage()
        Create an empty array. 
        """
        _raster_image.ArrayRasterImage_swiginit(self, _raster_image.new_ArrayRasterImage())

    def append(self, Img):
        """

        void GeoCal::ArrayRasterImage::push_back(const boost::shared_ptr< RasterImage > &Img)
        Add to the array. 
        """
        return _raster_image.ArrayRasterImage_append(self, Img)


    def size(self):
        """

        int GeoCal::ArrayRasterImage::size() const
        Return size. 
        """
        return _raster_image.ArrayRasterImage_size(self)


    def __getitem__(self, index):
      return self.get(index)

    def __setitem__(self, index, val):
      self.set(index, val)

    def __len__(self):
      return self.size()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _raster_image.delete_ArrayRasterImage
ArrayRasterImage.append = new_instancemethod(_raster_image.ArrayRasterImage_append, None, ArrayRasterImage)
ArrayRasterImage.size = new_instancemethod(_raster_image.ArrayRasterImage_size, None, ArrayRasterImage)
ArrayRasterImage.get = new_instancemethod(_raster_image.ArrayRasterImage_get, None, ArrayRasterImage)
ArrayRasterImage.set = new_instancemethod(_raster_image.ArrayRasterImage_set, None, ArrayRasterImage)
ArrayRasterImage.__str__ = new_instancemethod(_raster_image.ArrayRasterImage___str__, None, ArrayRasterImage)
ArrayRasterImage_swigregister = _raster_image.ArrayRasterImage_swigregister
ArrayRasterImage_swigregister(ArrayRasterImage)


def copy_no_fill(Img_in, Img_out, Fill_value=0, diagnostic=False):
    """

    void GeoCal::copy_no_fill(const RasterImage &Img_in, RasterImage &Img_out, int Fill_value=0,
    bool diagnostic=false)
    This copies one image to another. The images should be the same size.
    Setting Diagnostic to true causes messages to be printed as we do the
    copying.

    This differs from copy by skipping copying all values that are equal
    to the Fill_value.

    This can be used to create a simple mosaic by copying multiple images
    to a common image. 
    """
    return _raster_image.copy_no_fill(Img_in, Img_out, Fill_value, diagnostic)
class Vector_RasterImage(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _raster_image.Vector_RasterImage_swiginit(self, _raster_image.new_Vector_RasterImage(*args))
    __swig_destroy__ = _raster_image.delete_Vector_RasterImage
Vector_RasterImage.iterator = new_instancemethod(_raster_image.Vector_RasterImage_iterator, None, Vector_RasterImage)
Vector_RasterImage.__nonzero__ = new_instancemethod(_raster_image.Vector_RasterImage___nonzero__, None, Vector_RasterImage)
Vector_RasterImage.__bool__ = new_instancemethod(_raster_image.Vector_RasterImage___bool__, None, Vector_RasterImage)
Vector_RasterImage.__len__ = new_instancemethod(_raster_image.Vector_RasterImage___len__, None, Vector_RasterImage)
Vector_RasterImage.__getslice__ = new_instancemethod(_raster_image.Vector_RasterImage___getslice__, None, Vector_RasterImage)
Vector_RasterImage.__setslice__ = new_instancemethod(_raster_image.Vector_RasterImage___setslice__, None, Vector_RasterImage)
Vector_RasterImage.__delslice__ = new_instancemethod(_raster_image.Vector_RasterImage___delslice__, None, Vector_RasterImage)
Vector_RasterImage.__delitem__ = new_instancemethod(_raster_image.Vector_RasterImage___delitem__, None, Vector_RasterImage)
Vector_RasterImage.__getitem__ = new_instancemethod(_raster_image.Vector_RasterImage___getitem__, None, Vector_RasterImage)
Vector_RasterImage.__setitem__ = new_instancemethod(_raster_image.Vector_RasterImage___setitem__, None, Vector_RasterImage)
Vector_RasterImage.pop = new_instancemethod(_raster_image.Vector_RasterImage_pop, None, Vector_RasterImage)
Vector_RasterImage.append = new_instancemethod(_raster_image.Vector_RasterImage_append, None, Vector_RasterImage)
Vector_RasterImage.empty = new_instancemethod(_raster_image.Vector_RasterImage_empty, None, Vector_RasterImage)
Vector_RasterImage.size = new_instancemethod(_raster_image.Vector_RasterImage_size, None, Vector_RasterImage)
Vector_RasterImage.swap = new_instancemethod(_raster_image.Vector_RasterImage_swap, None, Vector_RasterImage)
Vector_RasterImage.begin = new_instancemethod(_raster_image.Vector_RasterImage_begin, None, Vector_RasterImage)
Vector_RasterImage.end = new_instancemethod(_raster_image.Vector_RasterImage_end, None, Vector_RasterImage)
Vector_RasterImage.rbegin = new_instancemethod(_raster_image.Vector_RasterImage_rbegin, None, Vector_RasterImage)
Vector_RasterImage.rend = new_instancemethod(_raster_image.Vector_RasterImage_rend, None, Vector_RasterImage)
Vector_RasterImage.clear = new_instancemethod(_raster_image.Vector_RasterImage_clear, None, Vector_RasterImage)
Vector_RasterImage.get_allocator = new_instancemethod(_raster_image.Vector_RasterImage_get_allocator, None, Vector_RasterImage)
Vector_RasterImage.pop_back = new_instancemethod(_raster_image.Vector_RasterImage_pop_back, None, Vector_RasterImage)
Vector_RasterImage.erase = new_instancemethod(_raster_image.Vector_RasterImage_erase, None, Vector_RasterImage)
Vector_RasterImage.push_back = new_instancemethod(_raster_image.Vector_RasterImage_push_back, None, Vector_RasterImage)
Vector_RasterImage.front = new_instancemethod(_raster_image.Vector_RasterImage_front, None, Vector_RasterImage)
Vector_RasterImage.back = new_instancemethod(_raster_image.Vector_RasterImage_back, None, Vector_RasterImage)
Vector_RasterImage.assign = new_instancemethod(_raster_image.Vector_RasterImage_assign, None, Vector_RasterImage)
Vector_RasterImage.resize = new_instancemethod(_raster_image.Vector_RasterImage_resize, None, Vector_RasterImage)
Vector_RasterImage.insert = new_instancemethod(_raster_image.Vector_RasterImage_insert, None, Vector_RasterImage)
Vector_RasterImage.reserve = new_instancemethod(_raster_image.Vector_RasterImage_reserve, None, Vector_RasterImage)
Vector_RasterImage.capacity = new_instancemethod(_raster_image.Vector_RasterImage_capacity, None, Vector_RasterImage)
Vector_RasterImage_swigregister = _raster_image.Vector_RasterImage_swigregister
Vector_RasterImage_swigregister(Vector_RasterImage)


__all__ = ["copy_no_fill","Vector_RasterImage","RasterImage","ArrayRasterImage","RasterImageTileIterator"]



