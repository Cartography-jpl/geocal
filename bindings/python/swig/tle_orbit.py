# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _tle_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_tle_orbit')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_tle_orbit')
    _tle_orbit = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_tle_orbit', [dirname(__file__)])
        except ImportError:
            import _tle_orbit
            return _tle_orbit
        try:
            _mod = imp.load_module('_tle_orbit', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _tle_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _tle_orbit
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
    __swig_destroy__ = _tle_orbit.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_tle_orbit.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_tle_orbit.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_tle_orbit.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_tle_orbit.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_tle_orbit.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_tle_orbit.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_tle_orbit.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_tle_orbit.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_tle_orbit.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_tle_orbit.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_tle_orbit.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_tle_orbit.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_tle_orbit.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_tle_orbit.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_tle_orbit.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_tle_orbit.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _tle_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _tle_orbit.SHARED_PTR_DISOWN

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
class TleOrbit(geocal_swig.orbit.Orbit):
    """

    This is an orbit that does the calculation based on a two-line element
    set (TLE, seehttps://en.wikipedia.org/wiki/Two-line_element_set).

    You can get TLE for a particular satellite and time
    fromhttps://www.space-track.org.

    C++ includes: tle_orbit.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Tle):
        """

        GeoCal::TleOrbit::TleOrbit(const std::string &Tle)
        Constructor.

        The TLE should be in the standard two-line element set format
        (seehttps://en.wikipedia.org/wiki/Two-line_element_set). 
        """
        _tle_orbit.TleOrbit_swiginit(self, _tle_orbit.new_TleOrbit(Tle))

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > TleOrbit::orbit_data(Time T) const

        """
        return _tle_orbit.TleOrbit_orbit_data(self, *args)


    def _v_tle(self):
        """

        const std::string& GeoCal::TleOrbit::tle() const
        The TLE that this orbit is using. 
        """
        return _tle_orbit.TleOrbit__v_tle(self)


    @property
    def tle(self):
        return self._v_tle()


    def _v_epoch(self):
        """

        const Time& GeoCal::TleOrbit::epoch() const
        The epoch for the TLE. 
        """
        return _tle_orbit.TleOrbit__v_epoch(self)


    @property
    def epoch(self):
        return self._v_epoch()


    def _v_revolution_number_at_epoch(self):
        """

        int TleOrbit::revolution_number_at_epoch() const
        Return the revolution number at the epoch. 
        """
        return _tle_orbit.TleOrbit__v_revolution_number_at_epoch(self)


    @property
    def revolution_number_at_epoch(self):
        return self._v_revolution_number_at_epoch()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _tle_orbit.delete_TleOrbit
TleOrbit.orbit_data = new_instancemethod(_tle_orbit.TleOrbit_orbit_data, None, TleOrbit)
TleOrbit._v_tle = new_instancemethod(_tle_orbit.TleOrbit__v_tle, None, TleOrbit)
TleOrbit._v_epoch = new_instancemethod(_tle_orbit.TleOrbit__v_epoch, None, TleOrbit)
TleOrbit._v_revolution_number_at_epoch = new_instancemethod(_tle_orbit.TleOrbit__v_revolution_number_at_epoch, None, TleOrbit)
TleOrbit_swigregister = _tle_orbit.TleOrbit_swigregister
TleOrbit_swigregister(TleOrbit)


__all__ = ["TleOrbit"]



