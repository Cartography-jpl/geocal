# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _refraction_sdp

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _refraction_sdp.SWIG_PyInstanceMethod_New
_swig_new_static_method = _refraction_sdp.SWIG_PyStaticMethod_New

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

SWIG_MODULE_ALREADY_DONE = _refraction_sdp.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _refraction_sdp.delete_SwigPyIterator
    value = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_value)
    incr = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_copy)
    next = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_refraction_sdp.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_refraction_sdp.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _refraction_sdp:
_refraction_sdp.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _refraction_sdp.SHARED_PTR_DISOWN

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

import geocal_swig.refraction
import geocal_swig.generic_object
class RefractionSdp(geocal_swig.refraction.Refraction):
    r"""

    This calculates refraction.

    The algorithm used was take from the SDP toolkit. The original code
    was written by Peter Noerdlinger as the SDP toolkit function
    PGS_CSC_SpaceRefract. The algorithm is described in detail in
    "Theoretical Basis of the SDP Toolkit Geolocation Package for the
    ECS".

    This depends on the index of refraction of air at the surface. We
    don't actually know this. There are various approximations, and the
    toolkit uses one described in the "Theoretical Basis of the SDP
    Toolkit Geolocation Package for the ECS".

    This class allows the index of refraction to simple be given. This
    allows us to just fit for this unknown using something like a
    simultanous bundle adjustment. Note that a reasonable range of values
    is something like 1.00026 to 1.00029, so you can vary from the initial
    prediction by a few 1e-5.

    C++ includes: refraction_sdp.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Altitude, Latitude, Index_refraction_surface=-1):
        r"""

        RefractionSdp::RefractionSdp(double Altitude, double Latitude, double Index_refraction_surface=-1)
        This sets up for a Refraction calculation.

        A representative altitude (in meters) and latitude (in degrees) is
        given. This doesn't depend very strongly on altitude and latitude, so
        unless you using widely varying points just using the center of you
        scene should be pretty much sufficient.

        The index of refraction at the surface can be supplied if desired,
        otherwise we calculate an approximate value. 
        """
        _refraction_sdp.RefractionSdp_swiginit(self, _refraction_sdp.new_RefractionSdp(Altitude, Latitude, Index_refraction_surface))
    _v_altitude = _swig_new_instance_method(_refraction_sdp.RefractionSdp__v_altitude)

    @property
    def altitude(self):
        return self._v_altitude()

    _v_latitude = _swig_new_instance_method(_refraction_sdp.RefractionSdp__v_latitude)

    @property
    def latitude(self):
        return self._v_latitude()

    _v_index_refraction_surface = _swig_new_instance_method(_refraction_sdp.RefractionSdp__v_index_refraction_surface)

    @property
    def index_refraction_surface(self):
        return self._v_index_refraction_surface()

    @index_refraction_surface.setter
    def index_refraction_surface(self, value):
      self._v_index_refraction_surface(value)

    surface_zenith = _swig_new_instance_method(_refraction_sdp.RefractionSdp_surface_zenith)
    displacement = _swig_new_instance_method(_refraction_sdp.RefractionSdp_displacement)
    refraction_apply = _swig_new_instance_method(_refraction_sdp.RefractionSdp_refraction_apply)
    refraction_reverse = _swig_new_instance_method(_refraction_sdp.RefractionSdp_refraction_reverse)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _refraction_sdp.delete_RefractionSdp

# Register RefractionSdp in _refraction_sdp:
_refraction_sdp.RefractionSdp_swigregister(RefractionSdp)

__all__ = ["RefractionSdp"]


