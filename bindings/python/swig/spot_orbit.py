# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _spot_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_spot_orbit')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_spot_orbit')
    _spot_orbit = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_spot_orbit', [dirname(__file__)])
        except ImportError:
            import _spot_orbit
            return _spot_orbit
        try:
            _mod = imp.load_module('_spot_orbit', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _spot_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _spot_orbit
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


SWIG_MODULE_ALREADY_DONE = _spot_orbit.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _spot_orbit.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_spot_orbit.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_spot_orbit.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_spot_orbit.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_spot_orbit.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_spot_orbit.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_spot_orbit.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_spot_orbit.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_spot_orbit.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_spot_orbit.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_spot_orbit.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_spot_orbit.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_spot_orbit.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_spot_orbit.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_spot_orbit.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_spot_orbit.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_spot_orbit.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _spot_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _spot_orbit.SHARED_PTR_DISOWN

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
class SpotOrbit(geocal_swig.orbit.Orbit):
    """

    This class models the SPOT orbit.

    The orbit parameters are given by an DIMAP file. This file is an XML
    file format. You can find documentation for this format
    athttp://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

    We don't actually read this file format. XML is a bit of a pain to
    deal with in C++, and is much easier to process in Ruby. So we set up
    an interface that just takes the Ephemeris and Attitude measurements.
    The Afids class Dimap processes the XML file and can use that to
    create an instance of this class.

    The various angles and so forth are described in the "SPOT Geometry
    Handbook", available athttp://www.spotimage.com/automne_modules_files
    /standard/public/p229_0b
    9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf

    A note for a developer working with this class. The definition of
    space craft and orbital coordinate used internally has a different
    sign convention than we've used on other instruments such as MISR.

    We use the conventions spelled out in the "SPOT Geometry Handbook"
    consistently in this class, even though this is different than what is
    used in the MISR ATB. In particular, the Z axis points from earth
    center to spacecraft, not spacecraft to earth center as we've used on
    MISR. This is all accounted for by this class, for the outside user
    everything works as expected. But if you are working internally on
    this class this might seem strange.

    Note also that according to the "SPOT Geometry Handbook" the
    reported attitudes are relative to -X, -Y, and Z axis. This means that
    pitch and roll (but not yaw) have the opposite sign of what you would
    expect. The Ypr given to this routine should match what is supplied by
    SPOT in its DIMAP file - we account for the strange sign in this
    class.

    C++ includes: spot_orbit.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > SpotOrbit::orbit_data(const TimeWithDerivative &T) const

        """
        return _spot_orbit.SpotOrbit_orbit_data(self, *args)


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _spot_orbit.delete_SpotOrbit
SpotOrbit.orbit_data = new_instancemethod(_spot_orbit.SpotOrbit_orbit_data, None, SpotOrbit)
SpotOrbit_swigregister = _spot_orbit.SpotOrbit_swigregister
SpotOrbit_swigregister(SpotOrbit)


__all__ = ["SpotOrbit"]



