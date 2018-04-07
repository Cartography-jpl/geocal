# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _raster_subsample.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_raster_subsample')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_raster_subsample')
    _raster_subsample = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_subsample', [dirname(__file__)])
        except ImportError:
            import _raster_subsample
            return _raster_subsample
        try:
            _mod = imp.load_module('_raster_subsample', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _raster_subsample = swig_import_helper()
    del swig_import_helper
else:
    import _raster_subsample
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
    __swig_destroy__ = _raster_subsample.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_raster_subsample.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_raster_subsample.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_raster_subsample.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_raster_subsample.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_raster_subsample.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_raster_subsample.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_raster_subsample.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_raster_subsample.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_raster_subsample.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_raster_subsample.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_raster_subsample.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_raster_subsample.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_raster_subsample.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_raster_subsample.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_raster_subsample.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_raster_subsample.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _raster_subsample.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _raster_subsample.SHARED_PTR_DISOWN

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
class RasterSubSample(geocal_swig.raster_image_variable.RasterImageVariable):
    """

    This subsamples a higher resolution RasterImage into a lower
    resolution one.

    Typically the pixel subsampling factor doesn't exactly divide the
    number of lines and samples of the full resolution data. We just trim
    the data to the largest low resolution pixel that completely fits in
    the higher resolution data.

    C++ includes: raster_subsample.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Data, Number_line_per_pixel, Number_sample_per_pixel):
        """

        RasterSubSample::RasterSubSample(const boost::shared_ptr< RasterImage > &Data, int
        Number_line_per_pixel, int Number_sample_per_pixel)
        Constructor. 
        """
        _raster_subsample.RasterSubSample_swiginit(self, _raster_subsample.new_RasterSubSample(Data, Number_line_per_pixel, Number_sample_per_pixel))

    def _v_high_resolution_image(self):
        """

        const boost::shared_ptr<RasterImage>& GeoCal::RasterSubSample::high_resolution_image_ptr() const
        Pointer to high resolution image that this object is based on. 
        """
        return _raster_subsample.RasterSubSample__v_high_resolution_image(self)


    @property
    def high_resolution_image(self):
        return self._v_high_resolution_image()


    def _v_number_line_per_pixel(self):
        """

        int GeoCal::RasterSubSample::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_subsample.RasterSubSample__v_number_line_per_pixel(self)


    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()


    def _v_number_sample_per_pixel(self):
        """

        int GeoCal::RasterSubSample::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_subsample.RasterSubSample__v_number_sample_per_pixel(self)


    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()


    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.high_resolution_image,self.number_line_per_pixel,self.number_sample_per_pixel)

    __swig_destroy__ = _raster_subsample.delete_RasterSubSample
RasterSubSample._v_high_resolution_image = new_instancemethod(_raster_subsample.RasterSubSample__v_high_resolution_image, None, RasterSubSample)
RasterSubSample._v_number_line_per_pixel = new_instancemethod(_raster_subsample.RasterSubSample__v_number_line_per_pixel, None, RasterSubSample)
RasterSubSample._v_number_sample_per_pixel = new_instancemethod(_raster_subsample.RasterSubSample__v_number_sample_per_pixel, None, RasterSubSample)
RasterSubSample_swigregister = _raster_subsample.RasterSubSample_swigregister
RasterSubSample_swigregister(RasterSubSample)



