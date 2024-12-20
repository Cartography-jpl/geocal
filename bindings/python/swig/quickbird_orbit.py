# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _quickbird_orbit

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _quickbird_orbit.SWIG_PyInstanceMethod_New
_swig_new_static_method = _quickbird_orbit.SWIG_PyStaticMethod_New

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

SWIG_MODULE_ALREADY_DONE = _quickbird_orbit.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _quickbird_orbit.delete_SwigPyIterator
    value = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_value)
    incr = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_copy)
    next = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_quickbird_orbit.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _quickbird_orbit:
_quickbird_orbit.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _quickbird_orbit.SHARED_PTR_DISOWN

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
import geocal_swig.orbit
import geocal_swig.observer
import geocal_swig.with_parameter
import geocal_swig.look_vector
class QuickBirdEphemeris(geocal_swig.generic_object.GenericObject):
    r"""

    This is a low level class that reads a Quickbird ephemeris file.

    You probably don't want to use this directly, but rather use the
    QuickBirdOrbit

    C++ includes: quickbird_orbit.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Fname):
        r"""

        GeoCal::QuickBirdEphemeris::QuickBirdEphemeris(const std::string &Fname)
        Read the quickbird ephemeris file. 
        """
        _quickbird_orbit.QuickBirdEphemeris_swiginit(self, _quickbird_orbit.new_QuickBirdEphemeris(Fname))
    _v_min_time = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris__v_min_time)

    @property
    def min_time(self):
        return self._v_min_time()

    _v_max_time = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris__v_max_time)

    @property
    def max_time(self):
        return self._v_max_time()

    _v_time_spacing = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris__v_time_spacing)

    @property
    def time_spacing(self):
        return self._v_time_spacing()

    _v_ephemeris = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris__v_ephemeris)

    @property
    def ephemeris(self):
        return self._v_ephemeris()

    _v_file_name = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris__v_file_name)

    @property
    def file_name(self):
        return self._v_file_name()

    __str__ = _swig_new_instance_method(_quickbird_orbit.QuickBirdEphemeris___str__)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdEphemeris

# Register QuickBirdEphemeris in _quickbird_orbit:
_quickbird_orbit.QuickBirdEphemeris_swigregister(QuickBirdEphemeris)
class QuickBirdAttitude(geocal_swig.generic_object.GenericObject):
    r"""

    This is a low level class that reads a Quickbird attitude file.

    You probably don't want to use this directly, but rather use the
    QuickBirdOrbit.

    Note a possible source of confusion. There are a few different
    conventions about the ordering of the quaternion coefficients. The
    boost library places the real part at the front, so we have a + b i c
    j + d k and the quaternion is 4-tuple (a, b, c, d). The convention
    used by quickbird data is q1 i + q2 j + q3 k + q4 with the 4-tuple is
    (q1, q2, q3, q4). That means when we bring this over to the boost
    library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

    The code in QuickBirdOrbit accounts for these different conventions,
    but if you are using this class directly you need to be aware of this
    difference.

    C++ includes: quickbird_orbit.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Fname):
        r"""

        GeoCal::QuickBirdAttitude::QuickBirdAttitude(const std::string &Fname)
        Read the quickbird attitude file. 
        """
        _quickbird_orbit.QuickBirdAttitude_swiginit(self, _quickbird_orbit.new_QuickBirdAttitude(Fname))
    _v_min_time = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude__v_min_time)

    @property
    def min_time(self):
        return self._v_min_time()

    _v_max_time = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude__v_max_time)

    @property
    def max_time(self):
        return self._v_max_time()

    _v_time_spacing = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude__v_time_spacing)

    @property
    def time_spacing(self):
        return self._v_time_spacing()

    _v_attitude = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude__v_attitude)

    @property
    def attitude(self):
        return self._v_attitude()

    _v_file_name = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude__v_file_name)

    @property
    def file_name(self):
        return self._v_file_name()

    __str__ = _swig_new_instance_method(_quickbird_orbit.QuickBirdAttitude___str__)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdAttitude

# Register QuickBirdAttitude in _quickbird_orbit:
_quickbird_orbit.QuickBirdAttitude_swigregister(QuickBirdAttitude)
class QuickBirdOrbit(geocal_swig.orbit.Orbit):
    r"""

    This is a Quickbird Orbit.

    This can be used for a rigorous model of Quickbird.

    Note an important limitation of the rigorous model vs. RPC. We don't
    currently account for atmospheric refraction, while the RPC does.
    Depending on the zenith angle, this can be somewhat important. From
    the approximate atmospheric model described in "Theoretical Basis of
    the SDP Toolkit Geolocation package for the ECS", Table 6-5 the
    linear displacement for a zenith angle of 10 is 0.549 meters, 20
    degress is 1.223 meters, and 30 degrees is 2.221. The typical
    Quickbird scene has something like 10 to 20 degree zenith angles, so
    this is a correction of 1 or 2 pixels.

    We will need to add atmospheric refraction in the future, but this
    hasn't been done yet.

    C++ includes: quickbird_orbit.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    orbit_data = _swig_new_instance_method(_quickbird_orbit.QuickBirdOrbit_orbit_data)
    _v_ephemeris_file_name = _swig_new_instance_method(_quickbird_orbit.QuickBirdOrbit__v_ephemeris_file_name)

    @property
    def ephemeris_file_name(self):
        return self._v_ephemeris_file_name()

    _v_attitude_file_name = _swig_new_instance_method(_quickbird_orbit.QuickBirdOrbit__v_attitude_file_name)

    @property
    def attitude_file_name(self):
        return self._v_attitude_file_name()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdOrbit

# Register QuickBirdOrbit in _quickbird_orbit:
_quickbird_orbit.QuickBirdOrbit_swigregister(QuickBirdOrbit)

__all__ = ["QuickBirdOrbit"]


