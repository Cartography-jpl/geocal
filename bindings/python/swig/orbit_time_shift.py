# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_time_shift.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_orbit_time_shift')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_orbit_time_shift')
    _orbit_time_shift = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_time_shift', [dirname(__file__)])
        except ImportError:
            import _orbit_time_shift
            return _orbit_time_shift
        try:
            _mod = imp.load_module('_orbit_time_shift', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _orbit_time_shift = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_time_shift
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


SWIG_MODULE_ALREADY_DONE = _orbit_time_shift.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _orbit_time_shift.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_time_shift.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_time_shift.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_time_shift.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_time_shift.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_time_shift.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_time_shift.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_time_shift.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_time_shift.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_time_shift.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_time_shift.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_time_shift.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_time_shift.SHARED_PTR_DISOWN

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

import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
import geocal_swig.look_vector
class OrbitTimeShift(geocal_swig.orbit.Orbit):
    """

    This is a simple adapter for an underlying orbit that allows us to
    "shift" the time of orbit.

    For a time "t", this returns orbit data from the underlying data at
    t-toffset, but with the time for the orbit data changed to "t".

    This is useful for simulations, where we may have an underlying orbit
    that otherwise matches some desired characteristics but that is at the
    wrong time (e.g., the orbit passes over a desired point but at local
    night when a simulation requires local day). It is possible this might
    end up having other uses (e.g., correct for a known time error), but
    the simulation was what we originally developed this for.

    C++ includes: orbit_time_shift.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Orbit_original, Time_shift):
        """

        OrbitTimeShift::OrbitTimeShift(const boost::shared_ptr< Orbit > Orbit_original, double Time_shift)
        Create an orbit that uses the data for the Orbit_original, shifted by
        the given Time_shift. 
        """
        _orbit_time_shift.OrbitTimeShift_swiginit(self, _orbit_time_shift.new_OrbitTimeShift(Orbit_original, Time_shift))

    def orbit_data(self, *args):
        """

        virtual boost::shared_ptr<OrbitData> GeoCal::OrbitTimeShift::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_time_shift.OrbitTimeShift_orbit_data(self, *args)


    def _v_orbit_original(self):
        """

        const boost::shared_ptr<Orbit> GeoCal::OrbitTimeShift::orbit_original() const
        The original orbit. 
        """
        return _orbit_time_shift.OrbitTimeShift__v_orbit_original(self)


    @property
    def orbit_original(self):
        return self._v_orbit_original()


    def _v_time_shift(self):
        """

        double GeoCal::OrbitTimeShift::time_shift() const
        Time shift applied to orbit, in seconds. 
        """
        return _orbit_time_shift.OrbitTimeShift__v_time_shift(self)


    @property
    def time_shift(self):
        return self._v_time_shift()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_time_shift.delete_OrbitTimeShift
OrbitTimeShift.orbit_data = new_instancemethod(_orbit_time_shift.OrbitTimeShift_orbit_data, None, OrbitTimeShift)
OrbitTimeShift._v_orbit_original = new_instancemethod(_orbit_time_shift.OrbitTimeShift__v_orbit_original, None, OrbitTimeShift)
OrbitTimeShift._v_time_shift = new_instancemethod(_orbit_time_shift.OrbitTimeShift__v_time_shift, None, OrbitTimeShift)
OrbitTimeShift_swigregister = _orbit_time_shift.OrbitTimeShift_swigregister
OrbitTimeShift_swigregister(OrbitTimeShift)


__all__ = ["OrbitTimeShift"]



