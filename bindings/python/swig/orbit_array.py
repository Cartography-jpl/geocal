# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_array.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_orbit_array')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_orbit_array')
    _orbit_array = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_array', [dirname(__file__)])
        except ImportError:
            import _orbit_array
            return _orbit_array
        try:
            _mod = imp.load_module('_orbit_array', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _orbit_array = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_array
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
    __swig_destroy__ = _orbit_array.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_array.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_array.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_array.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_array.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_array.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_array.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_array.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_array.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_array.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_array.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_array.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_array.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_array.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_array.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_array.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_array.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_array.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_array.SHARED_PTR_DISOWN

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

import geocal_swig.orbit_quaternion_list
import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class OrbitArray_EciTod_TimeAcs(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This is an implementation of an Orbit that is directly given an arrays
    with time, position, velocity:

    Attitude_time - natt in size, time of attitude measurement as doubles.
    Attitude_quaternion - natt x 4 in size, quaternion to take same
    coordinate system as Position (e.g., ECI) Ephemeris_time - neph in
    size, time of ephemeris measurement as doubles Ephemeris_position -
    neph x 3 in size, position measurement as doubles in meters
    Ephemeris_velocity - neph x 3 in size, velocity measurement as doubles
    in meter/second Because it is useful, we allow the type of position
    measurement and time to be changed. The measurement class is passed
    in, e.g., Eci, as is a small wrapper to give the conversion to Time.

    The Attitude is always assumed to have the real part first, like boost
    library uses. We could probably add an option to change this if
    needed, but for now this is always the case.

    By default, the reported attitude goes from the spacecraft to the
    reference frame (e.g., ECI). However, you can optionally specify that
    the attitude goes in the other direction.

    Derived classes may have a way to populate the data (e.g. HdfOrbit),
    so an additional field indicates if the class should serialize this
    data itself or let the derived object handle this (e.g. HdfOrbit
    stores a file name, and gets the data from that file).

    C++ includes: orbit_array.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame=True, Serialize_data=True):
        """

        GeoCal::OrbitArray< PositionType, TimeCreatorType >::OrbitArray(const blitz::Array< double, 1 > &Eph_time, const blitz::Array<
        double, 2 > &Eph_pos, const blitz::Array< double, 2 > &Eph_vel, const
        blitz::Array< double, 1 > &Att_time, const blitz::Array< double, 2 >
        &Att_quat, bool Att_from_sc_to_ref_frame=true, bool
        Serialize_data=true)

        """
        _orbit_array.OrbitArray_EciTod_TimeAcs_swiginit(self, _orbit_array.new_OrbitArray_EciTod_TimeAcs(Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame, Serialize_data))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_array.delete_OrbitArray_EciTod_TimeAcs
OrbitArray_EciTod_TimeAcs_swigregister = _orbit_array.OrbitArray_EciTod_TimeAcs_swigregister
OrbitArray_EciTod_TimeAcs_swigregister(OrbitArray_EciTod_TimeAcs)

class OrbitArray_Eci_TimePgs(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This is an implementation of an Orbit that is directly given an arrays
    with time, position, velocity:

    Attitude_time - natt in size, time of attitude measurement as doubles.
    Attitude_quaternion - natt x 4 in size, quaternion to take same
    coordinate system as Position (e.g., ECI) Ephemeris_time - neph in
    size, time of ephemeris measurement as doubles Ephemeris_position -
    neph x 3 in size, position measurement as doubles in meters
    Ephemeris_velocity - neph x 3 in size, velocity measurement as doubles
    in meter/second Because it is useful, we allow the type of position
    measurement and time to be changed. The measurement class is passed
    in, e.g., Eci, as is a small wrapper to give the conversion to Time.

    The Attitude is always assumed to have the real part first, like boost
    library uses. We could probably add an option to change this if
    needed, but for now this is always the case.

    By default, the reported attitude goes from the spacecraft to the
    reference frame (e.g., ECI). However, you can optionally specify that
    the attitude goes in the other direction.

    Derived classes may have a way to populate the data (e.g. HdfOrbit),
    so an additional field indicates if the class should serialize this
    data itself or let the derived object handle this (e.g. HdfOrbit
    stores a file name, and gets the data from that file).

    C++ includes: orbit_array.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame=True, Serialize_data=True):
        """

        GeoCal::OrbitArray< PositionType, TimeCreatorType >::OrbitArray(const blitz::Array< double, 1 > &Eph_time, const blitz::Array<
        double, 2 > &Eph_pos, const blitz::Array< double, 2 > &Eph_vel, const
        blitz::Array< double, 1 > &Att_time, const blitz::Array< double, 2 >
        &Att_quat, bool Att_from_sc_to_ref_frame=true, bool
        Serialize_data=true)

        """
        _orbit_array.OrbitArray_Eci_TimePgs_swiginit(self, _orbit_array.new_OrbitArray_Eci_TimePgs(Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame, Serialize_data))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_array.delete_OrbitArray_Eci_TimePgs
OrbitArray_Eci_TimePgs_swigregister = _orbit_array.OrbitArray_Eci_TimePgs_swigregister
OrbitArray_Eci_TimePgs_swigregister(OrbitArray_Eci_TimePgs)

class OrbitArray_Eci_TimeJ2000(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This is an implementation of an Orbit that is directly given an arrays
    with time, position, velocity:

    Attitude_time - natt in size, time of attitude measurement as doubles.
    Attitude_quaternion - natt x 4 in size, quaternion to take same
    coordinate system as Position (e.g., ECI) Ephemeris_time - neph in
    size, time of ephemeris measurement as doubles Ephemeris_position -
    neph x 3 in size, position measurement as doubles in meters
    Ephemeris_velocity - neph x 3 in size, velocity measurement as doubles
    in meter/second Because it is useful, we allow the type of position
    measurement and time to be changed. The measurement class is passed
    in, e.g., Eci, as is a small wrapper to give the conversion to Time.

    The Attitude is always assumed to have the real part first, like boost
    library uses. We could probably add an option to change this if
    needed, but for now this is always the case.

    By default, the reported attitude goes from the spacecraft to the
    reference frame (e.g., ECI). However, you can optionally specify that
    the attitude goes in the other direction.

    Derived classes may have a way to populate the data (e.g. HdfOrbit),
    so an additional field indicates if the class should serialize this
    data itself or let the derived object handle this (e.g. HdfOrbit
    stores a file name, and gets the data from that file).

    C++ includes: orbit_array.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame=True, Serialize_data=True):
        """

        GeoCal::OrbitArray< PositionType, TimeCreatorType >::OrbitArray(const blitz::Array< double, 1 > &Eph_time, const blitz::Array<
        double, 2 > &Eph_pos, const blitz::Array< double, 2 > &Eph_vel, const
        blitz::Array< double, 1 > &Att_time, const blitz::Array< double, 2 >
        &Att_quat, bool Att_from_sc_to_ref_frame=true, bool
        Serialize_data=true)

        """
        _orbit_array.OrbitArray_Eci_TimeJ2000_swiginit(self, _orbit_array.new_OrbitArray_Eci_TimeJ2000(Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame, Serialize_data))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_array.delete_OrbitArray_Eci_TimeJ2000
OrbitArray_Eci_TimeJ2000_swigregister = _orbit_array.OrbitArray_Eci_TimeJ2000_swigregister
OrbitArray_Eci_TimeJ2000_swigregister(OrbitArray_Eci_TimeJ2000)

class OrbitArray_EciTod_TimeJ2000(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This is an implementation of an Orbit that is directly given an arrays
    with time, position, velocity:

    Attitude_time - natt in size, time of attitude measurement as doubles.
    Attitude_quaternion - natt x 4 in size, quaternion to take same
    coordinate system as Position (e.g., ECI) Ephemeris_time - neph in
    size, time of ephemeris measurement as doubles Ephemeris_position -
    neph x 3 in size, position measurement as doubles in meters
    Ephemeris_velocity - neph x 3 in size, velocity measurement as doubles
    in meter/second Because it is useful, we allow the type of position
    measurement and time to be changed. The measurement class is passed
    in, e.g., Eci, as is a small wrapper to give the conversion to Time.

    The Attitude is always assumed to have the real part first, like boost
    library uses. We could probably add an option to change this if
    needed, but for now this is always the case.

    By default, the reported attitude goes from the spacecraft to the
    reference frame (e.g., ECI). However, you can optionally specify that
    the attitude goes in the other direction.

    Derived classes may have a way to populate the data (e.g. HdfOrbit),
    so an additional field indicates if the class should serialize this
    data itself or let the derived object handle this (e.g. HdfOrbit
    stores a file name, and gets the data from that file).

    C++ includes: orbit_array.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame=True, Serialize_data=True):
        """

        GeoCal::OrbitArray< PositionType, TimeCreatorType >::OrbitArray(const blitz::Array< double, 1 > &Eph_time, const blitz::Array<
        double, 2 > &Eph_pos, const blitz::Array< double, 2 > &Eph_vel, const
        blitz::Array< double, 1 > &Att_time, const blitz::Array< double, 2 >
        &Att_quat, bool Att_from_sc_to_ref_frame=true, bool
        Serialize_data=true)

        """
        _orbit_array.OrbitArray_EciTod_TimeJ2000_swiginit(self, _orbit_array.new_OrbitArray_EciTod_TimeJ2000(Eph_time, Eph_pos, Eph_vel, Att_time, Att_quat, Att_from_sc_to_ref_frame, Serialize_data))

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_array.delete_OrbitArray_EciTod_TimeJ2000
OrbitArray_EciTod_TimeJ2000_swigregister = _orbit_array.OrbitArray_EciTod_TimeJ2000_swigregister
OrbitArray_EciTod_TimeJ2000_swigregister(OrbitArray_EciTod_TimeJ2000)


__all__ = ["OrbitArray_EciTod_TimeAcs","OrbitArray_Eci_TimePgs","OrbitArray_Eci_TimeJ2000","OrbitArray_EciTod_TimeJ2000"]


