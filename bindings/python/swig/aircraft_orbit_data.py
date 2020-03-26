# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _aircraft_orbit_data.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_aircraft_orbit_data')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_aircraft_orbit_data')
    _aircraft_orbit_data = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_aircraft_orbit_data', [dirname(__file__)])
        except ImportError:
            import _aircraft_orbit_data
            return _aircraft_orbit_data
        try:
            _mod = imp.load_module('_aircraft_orbit_data', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _aircraft_orbit_data = swig_import_helper()
    del swig_import_helper
else:
    import _aircraft_orbit_data
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
    __swig_destroy__ = _aircraft_orbit_data.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_aircraft_orbit_data.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_aircraft_orbit_data.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _aircraft_orbit_data.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _aircraft_orbit_data.SHARED_PTR_DISOWN

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

import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class AircraftOrbitData(geocal_swig.orbit.QuaternionOrbitData):
    """

    This is a single navigation file record for aircraft data.

    This was originally written for data as reported by Applanix ephemeris
    and attitude, but it is likely to be useful for other INU untis.

    Note that we frequently don't have access to a direct measurement of
    the velocity. Rather than trying to do some clever estimate of the
    velocity, we just set it to 0. The velocity is only used for the
    abberation of light calculation, which isn't important for aircraft
    speeds. We can revisit this choice if needed, it wouldn't be too hard
    to use a couple of position measurements at nearby times to estimate
    the velocity.

    C++ includes: aircraft_orbit_data.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    GEODETIC_VERTICAL = _aircraft_orbit_data.AircraftOrbitData_GEODETIC_VERTICAL
    GEOCENTRIC_VERTICAL = _aircraft_orbit_data.AircraftOrbitData_GEOCENTRIC_VERTICAL

    def __init__(self, *args):
        """

        AircraftOrbitData::AircraftOrbitData(const Time &Tm, const GroundCoordinate &Position, const Time &Tm2,
        const GroundCoordinate &Position2, double Roll, double Pitch, double
        Heading, VerticalDefinition V=GEODETIC_VERTICAL)
        Often with Aircraft data we don't have a direct measure of velocity.

        This calculates a simple velocity by taking a second position and a
        different time and assuming a constant velocity between the values. 
        """
        _aircraft_orbit_data.AircraftOrbitData_swiginit(self, _aircraft_orbit_data.new_AircraftOrbitData(*args))

    def _v_local_north_to_ecr(self):
        """

        boost::math::quaternion< double > AircraftOrbitData::local_north_to_ecr() const
        The local north to ECR quaternion. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_local_north_to_ecr(self)


    @property
    def local_north_to_ecr(self):
        return self._v_local_north_to_ecr()


    def _v_body_to_local_north(self):
        """

        boost::math::quaternion< double > AircraftOrbitData::body_to_local_north() const
        The body to local north quaternion. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_body_to_local_north(self)


    @property
    def body_to_local_north(self):
        return self._v_body_to_local_north()


    def _v_position_geodetic(self):
        """

        const Geodetic& GeoCal::AircraftOrbitData::position_geodetic() const
        Position of aircraft. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_position_geodetic(self)


    @property
    def position_geodetic(self):
        return self._v_position_geodetic()


    def _v_roll(self):
        """

        double GeoCal::AircraftOrbitData::roll() const
        Roll, in degrees. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_roll(self)


    @property
    def roll(self):
        return self._v_roll()


    def _v_pitch(self):
        """

        double GeoCal::AircraftOrbitData::pitch() const
        Pitch, in degrees. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_pitch(self)


    @property
    def pitch(self):
        return self._v_pitch()


    def _v_heading(self):
        """

        double GeoCal::AircraftOrbitData::heading() const
        Heading, in degrees. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_heading(self)


    @property
    def heading(self):
        return self._v_heading()


    def _v_vertical_definition(self):
        """

        VerticalDefinition GeoCal::AircraftOrbitData::vertical_definition() const
        Vertical direction yaw, pitch, and heading are defined relative to.

        I'm pretty sure this should be GEODETIC_VERTICAL in most cases, but we
        allow this to be different. 
        """
        return _aircraft_orbit_data.AircraftOrbitData__v_vertical_definition(self)


    @property
    def vertical_definition(self):
        return self._v_vertical_definition()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _aircraft_orbit_data.delete_AircraftOrbitData
AircraftOrbitData._v_local_north_to_ecr = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_local_north_to_ecr, None, AircraftOrbitData)
AircraftOrbitData._v_body_to_local_north = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_body_to_local_north, None, AircraftOrbitData)
AircraftOrbitData._v_position_geodetic = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_position_geodetic, None, AircraftOrbitData)
AircraftOrbitData._v_roll = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_roll, None, AircraftOrbitData)
AircraftOrbitData._v_pitch = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_pitch, None, AircraftOrbitData)
AircraftOrbitData._v_heading = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_heading, None, AircraftOrbitData)
AircraftOrbitData._v_vertical_definition = new_instancemethod(_aircraft_orbit_data.AircraftOrbitData__v_vertical_definition, None, AircraftOrbitData)
AircraftOrbitData_swigregister = _aircraft_orbit_data.AircraftOrbitData_swigregister
AircraftOrbitData_swigregister(AircraftOrbitData)


__all__ = ["AircraftOrbitData"]



