# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _apply_mask

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _apply_mask.SWIG_PyInstanceMethod_New
_swig_new_static_method = _apply_mask.SWIG_PyStaticMethod_New

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "this":
            set(self, name, value)
        elif name == "thisown":
            self.this.own(value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


import weakref

SWIG_MODULE_ALREADY_DONE = _apply_mask.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _apply_mask.delete_SwigPyIterator
    value = _swig_new_instance_method(_apply_mask.SwigPyIterator_value)
    incr = _swig_new_instance_method(_apply_mask.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_apply_mask.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_apply_mask.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_apply_mask.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_apply_mask.SwigPyIterator_copy)
    next = _swig_new_instance_method(_apply_mask.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_apply_mask.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_apply_mask.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_apply_mask.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _apply_mask:
_apply_mask.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _apply_mask.SHARED_PTR_DISOWN

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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class ApplyMask(geocal_swig.calc_raster.CalcRaster):
    r"""

    This applies a mask to a RasterImage.

    The data either passes through the mask, or it set to a value of 0
    (the normal gore value we use) if a second RasterImage indicates that
    this is masked.

    C++ includes: apply_mask.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Data, M, Good_value=0):
        r"""

        GeoCal::ApplyMask::ApplyMask(const boost::shared_ptr< RasterImage > &Data, const
        boost::shared_ptr< RasterImage > &M, int Good_value=0)
        Constructor.

        Parameters:
        -----------

        Data:  Underlying data

        M:  Mask image

        Good_value:  Pixels with this value in the the image M allow 
        """
        _apply_mask.ApplyMask_swiginit(self, _apply_mask.new_ApplyMask(Data, M, Good_value))
    _v_raw_data = _swig_new_instance_method(_apply_mask.ApplyMask__v_raw_data)

    @property
    def raw_data(self):
        return self._v_raw_data()

    _v_mask = _swig_new_instance_method(_apply_mask.ApplyMask__v_mask)

    @property
    def mask(self):
        return self._v_mask()

    _v_good_value = _swig_new_instance_method(_apply_mask.ApplyMask__v_good_value)

    @property
    def good_value(self):
        return self._v_good_value()


    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.raw_data,self.mask,self.good_value)

    __swig_destroy__ = _apply_mask.delete_ApplyMask

# Register ApplyMask in _apply_mask:
_apply_mask.ApplyMask_swigregister(ApplyMask)

__all__ = ["ApplyMask"]


