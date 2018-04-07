# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _scale_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_scale_image')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_scale_image')
    _scale_image = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_scale_image', [dirname(__file__)])
        except ImportError:
            import _scale_image
            return _scale_image
        try:
            _mod = imp.load_module('_scale_image', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _scale_image = swig_import_helper()
    del swig_import_helper
else:
    import _scale_image
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
    __swig_destroy__ = _scale_image.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_scale_image.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_scale_image.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_scale_image.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_scale_image.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_scale_image.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_scale_image.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_scale_image.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_scale_image.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_scale_image.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_scale_image.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_scale_image.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_scale_image.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_scale_image.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_scale_image.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_scale_image.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_scale_image.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _scale_image.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _scale_image.SHARED_PTR_DISOWN

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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.calc_raster_multi_band
import geocal_swig.raster_image_multi_band_variable
import geocal_swig.raster_image_multi_band
import geocal_swig.image_ground_connection
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class ScaleImage(geocal_swig.calc_raster.CalcRaster):
    """

    This is a simple adapter that multiples a underlying image by a scale
    factor.

    C++ includes: scale_image.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Data, Scale_factor):
        """

        GeoCal::ScaleImage::ScaleImage(const boost::shared_ptr< RasterImage > &Data, double Scale_factor)

        """
        _scale_image.ScaleImage_swiginit(self, _scale_image.new_ScaleImage(Data, Scale_factor))

    def _v_raw_data(self):
        """

        const boost::shared_ptr<RasterImage>& GeoCal::ScaleImage::raw_data() const

        """
        return _scale_image.ScaleImage__v_raw_data(self)


    @property
    def raw_data(self):
        return self._v_raw_data()


    def _v_scale_factor(self):
        """

        double GeoCal::ScaleImage::scale_factor() const

        """
        return _scale_image.ScaleImage__v_scale_factor(self)


    @property
    def scale_factor(self):
        return self._v_scale_factor()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _scale_image.delete_ScaleImage
ScaleImage._v_raw_data = new_instancemethod(_scale_image.ScaleImage__v_raw_data, None, ScaleImage)
ScaleImage._v_scale_factor = new_instancemethod(_scale_image.ScaleImage__v_scale_factor, None, ScaleImage)
ScaleImage_swigregister = _scale_image.ScaleImage_swigregister
ScaleImage_swigregister(ScaleImage)

class ScaleImageMultiBand(geocal_swig.calc_raster_multi_band.CalcRasterMultiBand):
    """

    This is a simple adapter that multiples a underlying image by a scale
    factor.

    C++ includes: scale_image.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Data, Scale_factor):
        """

        GeoCal::ScaleImageMultiBand::ScaleImageMultiBand(const boost::shared_ptr< RasterImageMultiBand > &Data, double
        Scale_factor)

        """
        _scale_image.ScaleImageMultiBand_swiginit(self, _scale_image.new_ScaleImageMultiBand(Data, Scale_factor))

    def _v_raw_data(self):
        """

        const boost::shared_ptr<RasterImageMultiBand>& GeoCal::ScaleImageMultiBand::raw_data() const

        """
        return _scale_image.ScaleImageMultiBand__v_raw_data(self)


    @property
    def raw_data(self):
        return self._v_raw_data()


    def _v_scale_factor(self):
        """

        double GeoCal::ScaleImageMultiBand::scale_factor() const

        """
        return _scale_image.ScaleImageMultiBand__v_scale_factor(self)


    @property
    def scale_factor(self):
        return self._v_scale_factor()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _scale_image.delete_ScaleImageMultiBand
ScaleImageMultiBand._v_raw_data = new_instancemethod(_scale_image.ScaleImageMultiBand__v_raw_data, None, ScaleImageMultiBand)
ScaleImageMultiBand._v_scale_factor = new_instancemethod(_scale_image.ScaleImageMultiBand__v_scale_factor, None, ScaleImageMultiBand)
ScaleImageMultiBand_swigregister = _scale_image.ScaleImageMultiBand_swigregister
ScaleImageMultiBand_swigregister(ScaleImageMultiBand)

class ScaleImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnectionCopy):
    """

    C++ includes: scale_image.h

    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Igc, Scale_factor):
        """

        ScaleImageGroundConnection::ScaleImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Igc, double
        Scale_factor)
        Scale the image of an ImageGroundConnection. 
        """
        _scale_image.ScaleImageGroundConnection_swiginit(self, _scale_image.new_ScaleImageGroundConnection(Igc, Scale_factor))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _scale_image.delete_ScaleImageGroundConnection
ScaleImageGroundConnection_swigregister = _scale_image.ScaleImageGroundConnection_swigregister
ScaleImageGroundConnection_swigregister(ScaleImageGroundConnection)



