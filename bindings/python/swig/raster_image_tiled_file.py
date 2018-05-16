# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _raster_image_tiled_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_raster_image_tiled_file')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_raster_image_tiled_file')
    _raster_image_tiled_file = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_image_tiled_file', [dirname(__file__)])
        except ImportError:
            import _raster_image_tiled_file
            return _raster_image_tiled_file
        try:
            _mod = imp.load_module('_raster_image_tiled_file', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _raster_image_tiled_file = swig_import_helper()
    del swig_import_helper
else:
    import _raster_image_tiled_file
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
    __swig_destroy__ = _raster_image_tiled_file.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_raster_image_tiled_file.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_raster_image_tiled_file.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _raster_image_tiled_file.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _raster_image_tiled_file.SHARED_PTR_DISOWN

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

import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class RasterImageTiledFile(geocal_swig.raster_image_variable.RasterImageVariable):
    """

    This is a RasterImage that is implemented as a TiledFile<T, 2>.

    C++ includes: raster_image_tiled_file.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::RasterImageTiledFile::RasterImageTiledFile(const boost::shared_ptr< TiledFileBase< 2 > > &D, const MapInfo &M)

        """
        _raster_image_tiled_file.RasterImageTiledFile_swiginit(self, _raster_image_tiled_file.new_RasterImageTiledFile(*args))

    def _v_tile_file_base(self):
        """

        const boost::shared_ptr<TiledFileBase<2> >& GeoCal::RasterImageTiledFile::tile_file_base_ptr() const
        Underlying data. 
        """
        return _raster_image_tiled_file.RasterImageTiledFile__v_tile_file_base(self)


    @property
    def tile_file_base(self):
        return self._v_tile_file_base()


    def flush(self):
        """

        virtual void GeoCal::RasterImageTiledFile::flush() const
        Flush data to disk. 
        """
        return _raster_image_tiled_file.RasterImageTiledFile_flush(self)


    def _v_number_swap(self):
        """

        unsigned int GeoCal::RasterImageTiledFile::number_swap() const
        Number of times we have swapped a tile since reset_number_swap called.

        """
        return _raster_image_tiled_file.RasterImageTiledFile__v_number_swap(self)


    @property
    def number_swap(self):
        return self._v_number_swap()


    def _v_number_tile(self):
        """

        int GeoCal::RasterImageTiledFile::number_tile() const
        Number of tiles. 
        """
        return _raster_image_tiled_file.RasterImageTiledFile__v_number_tile(self)


    @property
    def number_tile(self):
        return self._v_number_tile()


    def reset_number_swap(self):
        """

        void GeoCal::RasterImageTiledFile::reset_number_swap()
        Reset number of swap counter to 0. 
        """
        return _raster_image_tiled_file.RasterImageTiledFile_reset_number_swap(self)

    __swig_destroy__ = _raster_image_tiled_file.delete_RasterImageTiledFile
RasterImageTiledFile._v_tile_file_base = new_instancemethod(_raster_image_tiled_file.RasterImageTiledFile__v_tile_file_base, None, RasterImageTiledFile)
RasterImageTiledFile.flush = new_instancemethod(_raster_image_tiled_file.RasterImageTiledFile_flush, None, RasterImageTiledFile)
RasterImageTiledFile._v_number_swap = new_instancemethod(_raster_image_tiled_file.RasterImageTiledFile__v_number_swap, None, RasterImageTiledFile)
RasterImageTiledFile._v_number_tile = new_instancemethod(_raster_image_tiled_file.RasterImageTiledFile__v_number_tile, None, RasterImageTiledFile)
RasterImageTiledFile.reset_number_swap = new_instancemethod(_raster_image_tiled_file.RasterImageTiledFile_reset_number_swap, None, RasterImageTiledFile)
RasterImageTiledFile_swigregister = _raster_image_tiled_file.RasterImageTiledFile_swigregister
RasterImageTiledFile_swigregister(RasterImageTiledFile)


__all__ = ["RasterImageTiledFile"]



