# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_des.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_orbit_des')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_orbit_des')
    _orbit_des = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_des', [dirname(__file__)])
        except ImportError:
            import _orbit_des
            return _orbit_des
        try:
            _mod = imp.load_module('_orbit_des', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _orbit_des = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_des
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
    __swig_destroy__ = _orbit_des.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_des.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_des.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_des.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_des.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_des.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_des.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_des.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_des.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_des.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_des.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_des.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_des.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_des.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_des.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_des.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_des.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_des.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_des.SHARED_PTR_DISOWN

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
import geocal_swig.orbit
import geocal_swig.observer
import geocal_swig.with_parameter
class PosCsephb(geocal_swig.generic_object.GenericObject):
    """

    This handles position reading, writing, and interpolation.

    This uses the NITF DES CSEPHB (See the SNIP documentation).

    Note that this class doesn't read and write the full DES, only the
    data portion. It works with the python code found in
    geocal_des_extension.py.

    The CSEPHB DES doens't contain velocity. We calculate the velocity
    from the positions.

    Note that the the CSEPHB data is like a NITF TRE. But because it is a
    DES, it is potentially much larger. For efficiency, we read and write
    the data as a a large blitz::Array<char, 1>. This allows the data on
    the python side to be a numpy array, which is potentially memory
    mapped. This data is pretty much just a std::string like any TRE, but
    without the overhead of handling a dynamic size.

    C++ includes: orbit_des.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        PosCsephb::PosCsephb(const Orbit &Orb, const Time &Min_time, const Time &Max_time, double
        Tstep)
        Constructor.

        We sample the position of the given Orbit at fixed spaces times. This
        version goes from the Min_time, up to Max_time (or more accurately,
        the largest time Min_time i * Tstep that is <= Max_time).w 
        """
        _orbit_des.PosCsephb_swiginit(self, _orbit_des.new_PosCsephb(*args))

    def pos_vel(self, *args):
        """

        blitz::Array< AutoDerivative< double >, 1 > PosCsephb::pos_vel(const TimeWithDerivative &T) const
        Return position and velocity for the given time. 
        """
        return _orbit_des.PosCsephb_pos_vel(self, *args)


    def _v_is_cf(self):
        """

        bool GeoCal::PosCsephb::is_cf() const
        True if data is CartesianFixed.

        If false, then data is CartesianInertial. 
        """
        return _orbit_des.PosCsephb__v_is_cf(self)


    @property
    def is_cf(self):
        return self._v_is_cf()


    def _v_min_time(self):
        """

        const Time& GeoCal::PosCsephb::min_time() const
        Minimum time we have data for. 
        """
        return _orbit_des.PosCsephb__v_min_time(self)


    @property
    def min_time(self):
        return self._v_min_time()


    def _v_max_time(self):
        """

        Time GeoCal::PosCsephb::max_time() const
        Maximum time we have data for. 
        """
        return _orbit_des.PosCsephb__v_max_time(self)


    @property
    def max_time(self):
        return self._v_max_time()


    def _v_time_step(self):
        """

        double GeoCal::PosCsephb::time_step() const
        Time step between position data, in seconds. 
        """
        return _orbit_des.PosCsephb__v_time_step(self)


    @property
    def time_step(self):
        return self._v_time_step()


    def test_print(self, Os):
        """

        void GeoCal::PosCsephb::test_print(std::ostream &Os) const

        """
        return _orbit_des.PosCsephb_test_print(self, Os)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_des.delete_PosCsephb
PosCsephb.pos_vel = new_instancemethod(_orbit_des.PosCsephb_pos_vel, None, PosCsephb)
PosCsephb._v_is_cf = new_instancemethod(_orbit_des.PosCsephb__v_is_cf, None, PosCsephb)
PosCsephb._v_min_time = new_instancemethod(_orbit_des.PosCsephb__v_min_time, None, PosCsephb)
PosCsephb._v_max_time = new_instancemethod(_orbit_des.PosCsephb__v_max_time, None, PosCsephb)
PosCsephb._v_time_step = new_instancemethod(_orbit_des.PosCsephb__v_time_step, None, PosCsephb)
PosCsephb.test_print = new_instancemethod(_orbit_des.PosCsephb_test_print, None, PosCsephb)
PosCsephb.__str__ = new_instancemethod(_orbit_des.PosCsephb___str__, None, PosCsephb)
PosCsephb_swigregister = _orbit_des.PosCsephb_swigregister
PosCsephb_swigregister(PosCsephb)

class OrbitDes(geocal_swig.orbit.Orbit):
    """

    This combines a PosCsephb and AttCsattb into an Orbit.

    Note that we calculate things for every time point. If you are using
    this a bit you may want to create a OrbitQuaternionList from this
    Orbit to speed things up.

    C++ includes: orbit_des.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > OrbitDes::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_des.OrbitDes_orbit_data(self, *args)


    def _v_pos_csephb(self):
        """

        const boost::shared_ptr<PosCsephb>& GeoCal::OrbitDes::pos_csephb() const

        """
        return _orbit_des.OrbitDes__v_pos_csephb(self)


    @property
    def pos_csephb(self):
        return self._v_pos_csephb()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_des.delete_OrbitDes
OrbitDes.orbit_data = new_instancemethod(_orbit_des.OrbitDes_orbit_data, None, OrbitDes)
OrbitDes._v_pos_csephb = new_instancemethod(_orbit_des.OrbitDes__v_pos_csephb, None, OrbitDes)
OrbitDes_swigregister = _orbit_des.OrbitDes_swigregister
OrbitDes_swigregister(OrbitDes)


__all__ = ["PosCsephb","OrbitDes"]



