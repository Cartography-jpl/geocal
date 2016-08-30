# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _geocal_gdal.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_gdal', [dirname(__file__)])
        except ImportError:
            import _geocal_gdal
            return _geocal_gdal
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_gdal', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_gdal = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_gdal
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


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


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



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
except:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _geocal_gdal.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_geocal_gdal.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geocal_gdal.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geocal_gdal.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geocal_gdal.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geocal_gdal.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geocal_gdal.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geocal_gdal.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geocal_gdal.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geocal_gdal.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geocal_gdal.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geocal_gdal.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geocal_gdal.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geocal_gdal.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geocal_gdal.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geocal_gdal.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geocal_gdal.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _geocal_gdal.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_geocal_gdal.SHARED_PTR_DISOWN_swigconstant(_geocal_gdal)
SHARED_PTR_DISOWN = _geocal_gdal.SHARED_PTR_DISOWN

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
class GdalBase(geocal_swig.generic_object.GenericObject):
    """

    Base class of Gdal<T>.

    This contains the portion that doesn't depend on T.

    C++ includes: geocal_gdal.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def _v_map_info(self, *args):
        """

        virtual void GeoCal::GdalBase::map_info(const MapInfo &M)

        """
        return _geocal_gdal.GdalBase__v_map_info(self, *args)


    @property
    def map_info(self):
        return self._v_map_info()

    @map_info.setter
    def map_info(self, value):
      self._v_map_info(value)


    def _v_raster_band(self):
        """

        const GDALRasterBand& GeoCal::GdalBase::raster_band() const
        The underlying GDALRasterBand object. 
        """
        return _geocal_gdal.GdalBase__v_raster_band(self)


    @property
    def raster_band(self):
        return self._v_raster_band()


    def _v_linear_unit_name(self):
        """

        std::string GdalBase::linear_unit_name() const
        Return linear units name. 
        """
        return _geocal_gdal.GdalBase__v_linear_unit_name(self)


    @property
    def linear_unit_name(self):
        return self._v_linear_unit_name()


    def _v_linear_unit_scale(self):
        """

        double GdalBase::linear_unit_scale() const
        Return linear units scale, which is the factor needed to multiple
        linear distance by to get meters. 
        """
        return _geocal_gdal.GdalBase__v_linear_unit_scale(self)


    @property
    def linear_unit_scale(self):
        return self._v_linear_unit_scale()


    def close(self):
        """

        void GeoCal::GdalBase::close()

        """
        return _geocal_gdal.GdalBase_close(self)


    def _v_is_closed(self):
        """

        bool GeoCal::GdalBase::is_closed() const

        """
        return _geocal_gdal.GdalBase__v_is_closed(self)


    @property
    def is_closed(self):
        return self._v_is_closed()


    def flush(self):
        """

        void GeoCal::GdalBase::flush() const
        Flush data to disk. 
        """
        return _geocal_gdal.GdalBase_flush(self)


    def _v_rpc(self):
        """

        void GeoCal::GdalBase::rpc(const Rpc &R)

        """
        return _geocal_gdal.GdalBase__v_rpc(self)


    @property
    def rpc(self):
        return self._v_rpc()

    driver_count = staticmethod(_geocal_gdal.GdalBase_driver_count)
    driver_short_name = staticmethod(_geocal_gdal.GdalBase_driver_short_name)
    driver_long_name = staticmethod(_geocal_gdal.GdalBase_driver_long_name)
    driver_help_url = staticmethod(_geocal_gdal.GdalBase_driver_help_url)
    driver_option_list = staticmethod(_geocal_gdal.GdalBase_driver_option_list)

    def __init__(self):
        _geocal_gdal.GdalBase_swiginit(self, _geocal_gdal.new_GdalBase())
    __swig_destroy__ = _geocal_gdal.delete_GdalBase
GdalBase._v_map_info = new_instancemethod(_geocal_gdal.GdalBase__v_map_info, None, GdalBase)
GdalBase._v_raster_band = new_instancemethod(_geocal_gdal.GdalBase__v_raster_band, None, GdalBase)
GdalBase._v_linear_unit_name = new_instancemethod(_geocal_gdal.GdalBase__v_linear_unit_name, None, GdalBase)
GdalBase._v_linear_unit_scale = new_instancemethod(_geocal_gdal.GdalBase__v_linear_unit_scale, None, GdalBase)
GdalBase.close = new_instancemethod(_geocal_gdal.GdalBase_close, None, GdalBase)
GdalBase._v_is_closed = new_instancemethod(_geocal_gdal.GdalBase__v_is_closed, None, GdalBase)
GdalBase.flush = new_instancemethod(_geocal_gdal.GdalBase_flush, None, GdalBase)
GdalBase._v_rpc = new_instancemethod(_geocal_gdal.GdalBase__v_rpc, None, GdalBase)
GdalBase.__str__ = new_instancemethod(_geocal_gdal.GdalBase___str__, None, GdalBase)
GdalBase_swigregister = _geocal_gdal.GdalBase_swigregister
GdalBase_swigregister(GdalBase)

def GdalBase_driver_count():
    return _geocal_gdal.GdalBase_driver_count()
GdalBase_driver_count = _geocal_gdal.GdalBase_driver_count

def GdalBase_driver_short_name(i):
    return _geocal_gdal.GdalBase_driver_short_name(i)
GdalBase_driver_short_name = _geocal_gdal.GdalBase_driver_short_name

def GdalBase_driver_long_name(i):
    return _geocal_gdal.GdalBase_driver_long_name(i)
GdalBase_driver_long_name = _geocal_gdal.GdalBase_driver_long_name

def GdalBase_driver_help_url(i):
    return _geocal_gdal.GdalBase_driver_help_url(i)
GdalBase_driver_help_url = _geocal_gdal.GdalBase_driver_help_url

def GdalBase_driver_option_list(i):
    return _geocal_gdal.GdalBase_driver_option_list(i)
GdalBase_driver_option_list = _geocal_gdal.GdalBase_driver_option_list



