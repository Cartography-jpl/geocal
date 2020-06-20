# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_quaternion_list_offset.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_orbit_quaternion_list_offset')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_orbit_quaternion_list_offset')
    _orbit_quaternion_list_offset = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_quaternion_list_offset', [dirname(__file__)])
        except ImportError:
            import _orbit_quaternion_list_offset
            return _orbit_quaternion_list_offset
        try:
            _mod = imp.load_module('_orbit_quaternion_list_offset', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _orbit_quaternion_list_offset = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_quaternion_list_offset
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
    __swig_destroy__ = _orbit_quaternion_list_offset.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_quaternion_list_offset.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_quaternion_list_offset.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_quaternion_list_offset.SHARED_PTR_DISOWN

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
class OrbitQuaternionListOffset(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This is an adapter to a OrbitQuaternionList where we add a fixed
    offset in spacecraft coordinates to the position.

    The original use of this is to model Ecostress, where we are far
    enough away from the center of the ISS that we need to account for the
    difference (we are about 25 meters or so away, which is a significant
    fraction of a pixel away). Note that this is similar to but distinct
    from a position offset in OrbitOffsetCorrection. There the correction
    is time dependent, and represented in whatever the native position is
    (e.g., ECI coordinates). Here, the correction is a know constant and
    is expressed in spacecraft coordinate - we have a fixed known
    orientation. Note that for most spacecraft we can ignore the position
    difference, the ISS is a special case because it is so large.

    We could have a general "Orbit" adapter, however again our use case
    is a OrbitQuaternionList (or more specifically, a HdfOrbit). We just
    use this particular case for performance reasons, overriding the
    cached orbit_data_create rather than doing the correction every time
    we create a orbit data.

    C++ includes: orbit_quaternion_list_offset.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Orbit_underlying, Position_offset_sc_coordinate):
        """

        GeoCal::OrbitQuaternionListOffset::OrbitQuaternionListOffset(const boost::shared_ptr< OrbitQuaternionList > &Orbit_underlying,
        const blitz::Array< double, 1 > &Position_offset_sc_coordinate)
        Constructor.

        Add a fixed offset to the position in meters, in space craft
        coordinate system. 
        """
        _orbit_quaternion_list_offset.OrbitQuaternionListOffset_swiginit(self, _orbit_quaternion_list_offset.new_OrbitQuaternionListOffset(Orbit_underlying, Position_offset_sc_coordinate))

    def _v_orbit_underlying(self):
        """

        boost::shared_ptr<OrbitQuaternionList> GeoCal::OrbitQuaternionListOffset::orbit_underlying() const
        Return the base orbit we are applying the offset to. 
        """
        return _orbit_quaternion_list_offset.OrbitQuaternionListOffset__v_orbit_underlying(self)


    @property
    def orbit_underlying(self):
        return self._v_orbit_underlying()


    def _v_position_offset_sc_coordinate(self):
        """

        const blitz::Array<double, 1>& GeoCal::OrbitQuaternionListOffset::position_offset_sc_coordinate() const
        Return the position offset, in the spacecraft coordinate system, in
        meters. 
        """
        return _orbit_quaternion_list_offset.OrbitQuaternionListOffset__v_position_offset_sc_coordinate(self)


    @property
    def position_offset_sc_coordinate(self):
        return self._v_position_offset_sc_coordinate()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_quaternion_list_offset.delete_OrbitQuaternionListOffset
OrbitQuaternionListOffset._v_orbit_underlying = new_instancemethod(_orbit_quaternion_list_offset.OrbitQuaternionListOffset__v_orbit_underlying, None, OrbitQuaternionListOffset)
OrbitQuaternionListOffset._v_position_offset_sc_coordinate = new_instancemethod(_orbit_quaternion_list_offset.OrbitQuaternionListOffset__v_position_offset_sc_coordinate, None, OrbitQuaternionListOffset)
OrbitQuaternionListOffset_swigregister = _orbit_quaternion_list_offset.OrbitQuaternionListOffset_swigregister
OrbitQuaternionListOffset_swigregister(OrbitQuaternionListOffset)

class OrbitScCoorOffset(geocal_swig.orbit.Orbit):
    """

    Like OrbitQuaternionListOffset, but for a more general orbit.

    C++ includes: orbit_quaternion_list_offset.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Orbit_underlying, Position_offset_sc_coordinate):
        """

        GeoCal::OrbitScCoorOffset::OrbitScCoorOffset(const boost::shared_ptr< Orbit > &Orbit_underlying, const
        blitz::Array< double, 1 > &Position_offset_sc_coordinate)
        Constructor.

        Add a fixed offset to the position in meters, in space craft
        coordinate system. 
        """
        _orbit_quaternion_list_offset.OrbitScCoorOffset_swiginit(self, _orbit_quaternion_list_offset.new_OrbitScCoorOffset(Orbit_underlying, Position_offset_sc_coordinate))

    def orbit_data(self, *args):
        """

        virtual boost::shared_ptr<OrbitData> GeoCal::OrbitScCoorOffset::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_quaternion_list_offset.OrbitScCoorOffset_orbit_data(self, *args)


    def _v_orbit_underlying(self):
        """

        boost::shared_ptr<Orbit> GeoCal::OrbitScCoorOffset::orbit_underlying() const
        Return the base orbit we are applying the offset to. 
        """
        return _orbit_quaternion_list_offset.OrbitScCoorOffset__v_orbit_underlying(self)


    @property
    def orbit_underlying(self):
        return self._v_orbit_underlying()


    def _v_position_offset_sc_coordinate(self):
        """

        const blitz::Array<double, 1>& GeoCal::OrbitScCoorOffset::position_offset_sc_coordinate() const
        Return the position offset, in the spacecraft coordinate system, in
        meters. 
        """
        return _orbit_quaternion_list_offset.OrbitScCoorOffset__v_position_offset_sc_coordinate(self)


    @property
    def position_offset_sc_coordinate(self):
        return self._v_position_offset_sc_coordinate()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_quaternion_list_offset.delete_OrbitScCoorOffset
OrbitScCoorOffset.orbit_data = new_instancemethod(_orbit_quaternion_list_offset.OrbitScCoorOffset_orbit_data, None, OrbitScCoorOffset)
OrbitScCoorOffset._v_orbit_underlying = new_instancemethod(_orbit_quaternion_list_offset.OrbitScCoorOffset__v_orbit_underlying, None, OrbitScCoorOffset)
OrbitScCoorOffset._v_position_offset_sc_coordinate = new_instancemethod(_orbit_quaternion_list_offset.OrbitScCoorOffset__v_position_offset_sc_coordinate, None, OrbitScCoorOffset)
OrbitScCoorOffset_swigregister = _orbit_quaternion_list_offset.OrbitScCoorOffset_swigregister
OrbitScCoorOffset_swigregister(OrbitScCoorOffset)


__all__ = ["OrbitQuaternionListOffset","OrbitScCoorOffset"]



