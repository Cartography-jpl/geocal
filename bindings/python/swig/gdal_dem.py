# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _gdal_dem.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_gdal_dem')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_gdal_dem')
    _gdal_dem = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_gdal_dem', [dirname(__file__)])
        except ImportError:
            import _gdal_dem
            return _gdal_dem
        try:
            _mod = imp.load_module('_gdal_dem', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _gdal_dem = swig_import_helper()
    del swig_import_helper
else:
    import _gdal_dem
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


SWIG_MODULE_ALREADY_DONE = _gdal_dem.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _gdal_dem.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_gdal_dem.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_gdal_dem.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_gdal_dem.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_gdal_dem.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_gdal_dem.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_gdal_dem.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_gdal_dem.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_gdal_dem.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_gdal_dem.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_gdal_dem.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_gdal_dem.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_gdal_dem.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_gdal_dem.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_gdal_dem.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_gdal_dem.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_gdal_dem.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _gdal_dem.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _gdal_dem.SHARED_PTR_DISOWN

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
    return geocal_swig.serialize_function.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_function.serialize_read_binary(data)
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

import geocal_swig.dem_tiled_file
import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.generic_object
class GdalDem(geocal_swig.dem_tiled_file.DemTiledFile):
    """

    This uses a Gdal<T> as a DEM.

    The type of T should be one we can convert to a double, and should
    give the height in meters above sea level. We do a bilinear
    interpolation to get values between points, and we optionally return a
    height of 0 outside of the range of where we have data (or
    alternatively, throw an exception).

    Right now, the datum needs to be passed in. I don't believe there is
    any information available in the files to let us know which datum to
    use.

    C++ includes: gdal_dem.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GdalDem::GdalDem(const std::string &Fname, const boost::shared_ptr< Datum >
        &D=boost::shared_ptr< Datum >(new NoDatum()), int Band_id=1, bool
        Outside_dem_is_error=false, int Number_tile=4, int
        Tile_number_line=-1, int Tile_number_sample=-1)
        Constructor that reads an existing file to get the Dem data.

        The default Datum used if none is supplied is DidDatum, which is
        relative to WGS-84. If Outside_dem_is_error is false, we return a
        height of 0 outside of the range of the data, otherwise an exception
        is thrown when an attempt is made to retrieve an height outside of the
        range.

        You can optionally pass the desired tile size. If you don't pass this
        in, we use the blocking size of the underlying dataset. 
        """
        _gdal_dem.GdalDem_swiginit(self, _gdal_dem.new_GdalDem(*args))

    def _v_file_name(self):
        """

        const std::string& GeoCal::GdalDem::file_name() const

        """
        return _gdal_dem.GdalDem__v_file_name(self)


    @property
    def file_name(self):
        return self._v_file_name()


    def _v_band_id(self):
        """

        int GeoCal::GdalDem::band_id() const

        """
        return _gdal_dem.GdalDem__v_band_id(self)


    @property
    def band_id(self):
        return self._v_band_id()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _gdal_dem.delete_GdalDem
GdalDem.elevation = new_instancemethod(_gdal_dem.GdalDem_elevation, None, GdalDem)
GdalDem._v_file_name = new_instancemethod(_gdal_dem.GdalDem__v_file_name, None, GdalDem)
GdalDem._v_band_id = new_instancemethod(_gdal_dem.GdalDem__v_band_id, None, GdalDem)
GdalDem_swigregister = _gdal_dem.GdalDem_swigregister
GdalDem_swigregister(GdalDem)


__all__ = ["GdalDem"]



