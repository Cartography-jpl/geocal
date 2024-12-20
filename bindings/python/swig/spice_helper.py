# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _spice_helper

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _spice_helper.SWIG_PyInstanceMethod_New
_swig_new_static_method = _spice_helper.SWIG_PyStaticMethod_New

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

SWIG_MODULE_ALREADY_DONE = _spice_helper.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _spice_helper.delete_SwigPyIterator
    value = _swig_new_instance_method(_spice_helper.SwigPyIterator_value)
    incr = _swig_new_instance_method(_spice_helper.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_spice_helper.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_spice_helper.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_spice_helper.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_spice_helper.SwigPyIterator_copy)
    next = _swig_new_instance_method(_spice_helper.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_spice_helper.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_spice_helper.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_spice_helper.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _spice_helper:
_spice_helper.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _spice_helper.SHARED_PTR_DISOWN

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

import geocal_swig.generic_object
import geocal_swig.look_vector
class SpiceHelper(object):
    r"""

    This class centralizes all of the CSPICE calls from the CSPICE library
    (available from NAIF athttp://naif.jpl.nasa.gov/naif/pds.html).

    This class handles initializing CSPICE, and provides one place to
    modify if we ever need to replace the dependency on this library.

    SPICE depends on various kernels. We load the file found at
    $SPICEDATA/geocal.ker. This file in turn loads a number of other
    kernels, you can edit this file to change which kernels are used.

    C++ includes: spice_helper.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    have_spice = _swig_new_static_method(_spice_helper.SpiceHelper_have_spice)
    name_to_body = _swig_new_static_method(_spice_helper.SpiceHelper_name_to_body)
    body_name = _swig_new_static_method(_spice_helper.SpiceHelper_body_name)
    kernel_data_int = _swig_new_static_method(_spice_helper.SpiceHelper_kernel_data_int)
    kernel_data_double = _swig_new_static_method(_spice_helper.SpiceHelper_kernel_data_double)
    kernel_data_array_double = _swig_new_static_method(_spice_helper.SpiceHelper_kernel_data_array_double)
    latsrf = _swig_new_static_method(_spice_helper.SpiceHelper_latsrf)
    fixed_frame_name = _swig_new_static_method(_spice_helper.SpiceHelper_fixed_frame_name)
    spice_available = _swig_new_static_method(_spice_helper.SpiceHelper_spice_available)
    spice_setup = _swig_new_static_method(_spice_helper.SpiceHelper_spice_setup)
    add_kernel = _swig_new_static_method(_spice_helper.SpiceHelper_add_kernel)
    kernel_loaded = _swig_new_static_method(_spice_helper.SpiceHelper_kernel_loaded)
    conversion_quaternion = _swig_new_static_method(_spice_helper.SpiceHelper_conversion_quaternion)
    state_vector = _swig_new_static_method(_spice_helper.SpiceHelper_state_vector)
    boresight_and_footprint = _swig_new_static_method(_spice_helper.SpiceHelper_boresight_and_footprint)
    surface_intersect = _swig_new_static_method(_spice_helper.SpiceHelper_surface_intersect)

    def __init__(self):
        _spice_helper.SpiceHelper_swiginit(self, _spice_helper.new_SpiceHelper())
    __swig_destroy__ = _spice_helper.delete_SpiceHelper

# Register SpiceHelper in _spice_helper:
_spice_helper.SpiceHelper_swigregister(SpiceHelper)

__all__ = ["SpiceHelper"]


