# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _velocity_aberration.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_velocity_aberration')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_velocity_aberration')
    _velocity_aberration = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_velocity_aberration', [dirname(__file__)])
        except ImportError:
            import _velocity_aberration
            return _velocity_aberration
        try:
            _mod = imp.load_module('_velocity_aberration', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _velocity_aberration = swig_import_helper()
    del swig_import_helper
else:
    import _velocity_aberration
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
    __swig_destroy__ = _velocity_aberration.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_velocity_aberration.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_velocity_aberration.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_velocity_aberration.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_velocity_aberration.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_velocity_aberration.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_velocity_aberration.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_velocity_aberration.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_velocity_aberration.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_velocity_aberration.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_velocity_aberration.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_velocity_aberration.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_velocity_aberration.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_velocity_aberration.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_velocity_aberration.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_velocity_aberration.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_velocity_aberration.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _velocity_aberration.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _velocity_aberration.SHARED_PTR_DISOWN

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

import geocal_swig.generic_object
import geocal_swig.look_vector
class VelocityAberration(geocal_swig.generic_object.GenericObject):
    """

    C++ includes: velocity_aberration.h

    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def velocity_aberration_apply(self, Spacecraft_pos, Gc_no_aberration, Velocity_cf):
        """

        virtual CartesianFixedLookVector GeoCal::VelocityAberration::velocity_aberration_apply(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
        &Gc_no_aberration, const boost::array< double, 3 > &Velocity_cf) const
        =0
        Apply a velocity aberration correction, taking a ground coordinate w/o
        aberration.

        We return the CartesianFixedLookVector including aberration. 
        """
        return _velocity_aberration.VelocityAberration_velocity_aberration_apply(self, Spacecraft_pos, Gc_no_aberration, Velocity_cf)


    def velocity_aberration_reverse(self, Spacecraft_pos, Gc_with_aberration, Velocity_cf):
        """

        virtual CartesianFixedLookVector GeoCal::VelocityAberration::velocity_aberration_reverse(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
        &Gc_with_aberration, const boost::array< double, 3 > &Velocity_cf)
        const =0
        Reverse of velocity_aberration_apply. 
        """
        return _velocity_aberration.VelocityAberration_velocity_aberration_reverse(self, Spacecraft_pos, Gc_with_aberration, Velocity_cf)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _velocity_aberration.delete_VelocityAberration
VelocityAberration.velocity_aberration_apply = new_instancemethod(_velocity_aberration.VelocityAberration_velocity_aberration_apply, None, VelocityAberration)
VelocityAberration.velocity_aberration_reverse = new_instancemethod(_velocity_aberration.VelocityAberration_velocity_aberration_reverse, None, VelocityAberration)
VelocityAberration.__str__ = new_instancemethod(_velocity_aberration.VelocityAberration___str__, None, VelocityAberration)
VelocityAberration_swigregister = _velocity_aberration.VelocityAberration_swigregister
VelocityAberration_swigregister(VelocityAberration)

class NoVelocityAberration(VelocityAberration):
    """

    Implementation that doesn't do a correction.

    This isn't something you would normally want, VelocityAberration is
    generally an important effect to include. But this can be useful in
    test code, e.g., for comparing to the MSP library with velocity
    aberration turned off in order to focus on other differences.

    C++ includes: velocity_aberration.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        """

        GeoCal::NoVelocityAberration::NoVelocityAberration()

        """
        _velocity_aberration.NoVelocityAberration_swiginit(self, _velocity_aberration.new_NoVelocityAberration())

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _velocity_aberration.delete_NoVelocityAberration
NoVelocityAberration_swigregister = _velocity_aberration.NoVelocityAberration_swigregister
NoVelocityAberration_swigregister(NoVelocityAberration)

class VelocityAberrationFirstOrder(VelocityAberration):
    """

    Implementation that just duplicates the first order velocity
    aberration that is the default for OrbitData.

    We have this in the same VelocityAberration framework.

    C++ includes: velocity_aberration.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        """

        GeoCal::VelocityAberrationFirstOrder::VelocityAberrationFirstOrder()

        """
        _velocity_aberration.VelocityAberrationFirstOrder_swiginit(self, _velocity_aberration.new_VelocityAberrationFirstOrder())

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _velocity_aberration.delete_VelocityAberrationFirstOrder
VelocityAberrationFirstOrder_swigregister = _velocity_aberration.VelocityAberrationFirstOrder_swigregister
VelocityAberrationFirstOrder_swigregister(VelocityAberrationFirstOrder)


__all__ = ["VelocityAberration","NoVelocityAberration","VelocityAberrationFirstOrder"]


