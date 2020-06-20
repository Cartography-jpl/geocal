# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _ground_mspi_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_ground_mspi_orbit')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_ground_mspi_orbit')
    _ground_mspi_orbit = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ground_mspi_orbit', [dirname(__file__)])
        except ImportError:
            import _ground_mspi_orbit
            return _ground_mspi_orbit
        try:
            _mod = imp.load_module('_ground_mspi_orbit', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _ground_mspi_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _ground_mspi_orbit
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
    __swig_destroy__ = _ground_mspi_orbit.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ground_mspi_orbit.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ground_mspi_orbit.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _ground_mspi_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ground_mspi_orbit.SHARED_PTR_DISOWN

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

import geocal_swig.look_vector
import geocal_swig.generic_object
import geocal_swig.orbit
import geocal_swig.observer
import geocal_swig.with_parameter
class GroundMspiOrbit(geocal_swig.orbit.Orbit):
    """

    This models the MSPI ground orbit.

    We don't actually move, of course, but the zenith angle does change
    over time.

    C++ includes: ground_mspi_orbit.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Start_time, Pos, Azimuth, Start_elevation_angle, Rotation_rate):
        """

        GeoCal::GroundMspiOrbit::GroundMspiOrbit(const Time &Start_time, boost::shared_ptr< GroundCoordinate > &Pos,
        double Azimuth, double Start_elevation_angle, double Rotation_rate)
        Constructor.

        The angles should be in degrees, and the rate is in degrees per
        second. 
        """
        _ground_mspi_orbit.GroundMspiOrbit_swiginit(self, _ground_mspi_orbit.new_GroundMspiOrbit(Start_time, Pos, Azimuth, Start_elevation_angle, Rotation_rate))

    def _v_start_time(self):
        """

        Time GeoCal::GroundMspiOrbit::start_time() const
        Starting time. 
        """
        return _ground_mspi_orbit.GroundMspiOrbit__v_start_time(self)


    @property
    def start_time(self):
        return self._v_start_time()


    def _v_position(self):
        """

        const boost::shared_ptr<GroundCoordinate>& GeoCal::GroundMspiOrbit::position() const
        Position. 
        """
        return _ground_mspi_orbit.GroundMspiOrbit__v_position(self)


    @property
    def position(self):
        return self._v_position()


    def _v_azimuth(self):
        """

        double GeoCal::GroundMspiOrbit::azimuth() const
        Azimuth angle, in degrees. 
        """
        return _ground_mspi_orbit.GroundMspiOrbit__v_azimuth(self)


    @property
    def azimuth(self):
        return self._v_azimuth()


    def _v_start_elevation_angle(self):
        """

        double GeoCal::GroundMspiOrbit::start_elevation_angle() const
        Starting elevation angle, in degrees. 
        """
        return _ground_mspi_orbit.GroundMspiOrbit__v_start_elevation_angle(self)


    @property
    def start_elevation_angle(self):
        return self._v_start_elevation_angle()


    def _v_rotation_rate(self):
        """

        double GeoCal::GroundMspiOrbit::rotation_rate() const
        Rotation rate of elevation angle, in deg/sec. 
        """
        return _ground_mspi_orbit.GroundMspiOrbit__v_rotation_rate(self)


    @property
    def rotation_rate(self):
        return self._v_rotation_rate()


    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > GroundMspiOrbit::orbit_data(const TimeWithDerivative &T) const

        """
        return _ground_mspi_orbit.GroundMspiOrbit_orbit_data(self, *args)


    def __reduce__(self):
      return _new_from_serialization_dir, (os.getcwd(), geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ground_mspi_orbit.delete_GroundMspiOrbit
GroundMspiOrbit._v_start_time = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit__v_start_time, None, GroundMspiOrbit)
GroundMspiOrbit._v_position = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit__v_position, None, GroundMspiOrbit)
GroundMspiOrbit._v_azimuth = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit__v_azimuth, None, GroundMspiOrbit)
GroundMspiOrbit._v_start_elevation_angle = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit__v_start_elevation_angle, None, GroundMspiOrbit)
GroundMspiOrbit._v_rotation_rate = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit__v_rotation_rate, None, GroundMspiOrbit)
GroundMspiOrbit.orbit_data = new_instancemethod(_ground_mspi_orbit.GroundMspiOrbit_orbit_data, None, GroundMspiOrbit)
GroundMspiOrbit_swigregister = _ground_mspi_orbit.GroundMspiOrbit_swigregister
GroundMspiOrbit_swigregister(GroundMspiOrbit)


__all__ = ["GroundMspiOrbit"]



