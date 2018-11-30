# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _usgs_dem.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_usgs_dem')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_usgs_dem')
    _usgs_dem = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_usgs_dem', [dirname(__file__)])
        except ImportError:
            import _usgs_dem
            return _usgs_dem
        try:
            _mod = imp.load_module('_usgs_dem', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _usgs_dem = swig_import_helper()
    del swig_import_helper
else:
    import _usgs_dem
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
    __swig_destroy__ = _usgs_dem.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_usgs_dem.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_usgs_dem.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_usgs_dem.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_usgs_dem.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_usgs_dem.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_usgs_dem.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_usgs_dem.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_usgs_dem.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_usgs_dem.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_usgs_dem.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_usgs_dem.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_usgs_dem.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_usgs_dem.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_usgs_dem.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_usgs_dem.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_usgs_dem.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _usgs_dem.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _usgs_dem.SHARED_PTR_DISOWN

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

import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.generic_object
import geocal_swig.cart_lab_multifile
import geocal_swig.raster_multifile
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class UsgsDemData(geocal_swig.cart_lab_multifile.GdalCartLabMultifile):
    """

    This is used to read the USGS DEM data.

    This then get used by UsgsDem. Although you can use this class
    directly, generally you'll use this through UsgsDem class.

    If we don't have data for a particular location, we return FILL_VALUE.

    C++ includes: usgs_dem.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    FILL_VALUE = _usgs_dem.UsgsDemData_FILL_VALUE

    def __init__(self, Dir, No_coverage_is_error=True, Number_line_per_tile=-1, Number_sample_per_tile=-1, Number_tile_each_file=4, Number_file=4):
        """

        UsgsDemData::UsgsDemData(const std::string &Dir, bool No_coverage_is_error=true, int
        Number_line_per_tile=-1, int Number_sample_per_tile=-1, int
        Number_tile_each_file=4, int Number_file=4)
        Constructor.

        You can provide the directory to look for USGS DEM data, or if you
        leave this blank we use the value of environment variable USGSDATA.

        We don't have USGS files that completely cover the area. If you ask
        for a point outside of the area this can either be treated as an
        error, or alternatively you can return a value of FILL_VALUE instead.
        This is controlled by No_coverage_is_error.

        There are two kinds of tiling going on. At the top level, we have a
        number of files open at one time, given by Number_file. For each file,
        we read it with tiles with the given Number_line_per_tile x
        Number_sample_per_tile, having up to Number_tile_each_file tiles. If
        the Number_line_per_tile or Number_sample_per_tile is -1 we read the
        entire file. 
        """
        _usgs_dem.UsgsDemData_swiginit(self, _usgs_dem.new_UsgsDemData(Dir, No_coverage_is_error, Number_line_per_tile, Number_sample_per_tile, Number_tile_each_file, Number_file))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _usgs_dem.delete_UsgsDemData
UsgsDemData_swigregister = _usgs_dem.UsgsDemData_swigregister
UsgsDemData_swigregister(UsgsDemData)

class UsgsDem(geocal_swig.dem_map_info.DemMapInfo):
    """

    This class provides access to the USGS Dem.

    C++ includes: usgs_dem.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        UsgsDem::UsgsDem(const std::string &Dir, bool Outside_dem_is_error=true, const
        boost::shared_ptr< Datum > &D=boost::shared_ptr< Datum >())
        Constructor.

        You can provide the directory to look for USGS DEM data, or if you
        leave this blank we use the value of environment variable USGSDATA.

        We don't have USGS files that completely cover the area. If you ask
        for a point outside of the area this can either be treated as an
        error, or alternatively you can return a value of 0 instead. This is
        controlled by Outside_dem_is_error. 
        """
        _usgs_dem.UsgsDem_swiginit(self, _usgs_dem.new_UsgsDem(*args))

    def _v_directory_base(self):
        """

        const std::string& GeoCal::UsgsDem::directory_base() const
        Database base directory. 
        """
        return _usgs_dem.UsgsDem__v_directory_base(self)


    @property
    def directory_base(self):
        return self._v_directory_base()


    def _v_number_file_read(self):
        """

        int GeoCal::UsgsDem::number_file_read() const
        Number of files we have read. 
        """
        return _usgs_dem.UsgsDem__v_number_file_read(self)


    @property
    def number_file_read(self):
        return self._v_number_file_read()


    def reset_number_file_read(self):
        """

        void GeoCal::UsgsDem::reset_number_file_read()
        Set number of files we have read back to zero. 
        """
        return _usgs_dem.UsgsDem_reset_number_file_read(self)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _usgs_dem.delete_UsgsDem
UsgsDem._v_directory_base = new_instancemethod(_usgs_dem.UsgsDem__v_directory_base, None, UsgsDem)
UsgsDem._v_number_file_read = new_instancemethod(_usgs_dem.UsgsDem__v_number_file_read, None, UsgsDem)
UsgsDem.reset_number_file_read = new_instancemethod(_usgs_dem.UsgsDem_reset_number_file_read, None, UsgsDem)
UsgsDem_swigregister = _usgs_dem.UsgsDem_swigregister
UsgsDem_swigregister(UsgsDem)


__all__ = ["UsgsDemData","UsgsDem"]



