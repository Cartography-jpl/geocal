# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _pos_export_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_pos_export_orbit')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_pos_export_orbit')
    _pos_export_orbit = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pos_export_orbit', [dirname(__file__)])
        except ImportError:
            import _pos_export_orbit
            return _pos_export_orbit
        try:
            _mod = imp.load_module('_pos_export_orbit', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _pos_export_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _pos_export_orbit
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


SWIG_MODULE_ALREADY_DONE = _pos_export_orbit.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pos_export_orbit.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_pos_export_orbit.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_pos_export_orbit.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_pos_export_orbit.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_pos_export_orbit.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_pos_export_orbit.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_pos_export_orbit.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_pos_export_orbit.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_pos_export_orbit.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_pos_export_orbit.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_pos_export_orbit.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _pos_export_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _pos_export_orbit.SHARED_PTR_DISOWN

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

import geocal_swig.orbit_quaternion_list
import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
import geocal_swig.look_vector
class PosExportOrbit(geocal_swig.orbit_quaternion_list.OrbitQuaternionList):
    """

    This read a POS export file to supply an Orbit.

    This is a text file that is written by the Applanix POS Export
    Utility.

    Note that the time in the POS export file is in GPS second of the
    week. This starts at midnight UTC time between Saturday and Sunday.
    Presumably if we happen to be flying at this time, the time will
    reset. We don't have any handling in place for this, we'll need to add
    handling if this ever becomes an issue.

    C++ includes: pos_export_orbit.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Fname, Epoch):
        """

        PosExportOrbit::PosExportOrbit(const std::string &Fname, const Time &Epoch)
        Read the given text file.

        Note that the time in the Applanix file is in GPS seconds of the week,
        but the actual week isn't in the file. This means we need to pass in
        the Epoch that the times are relative to. 
        """
        _pos_export_orbit.PosExportOrbit_swiginit(self, _pos_export_orbit.new_PosExportOrbit(Fname, Epoch))

    def aircraft_orbit_data(self, T):
        """

        const AircraftOrbitData & PosExportOrbit::aircraft_orbit_data(const Time &T) const
        Return the AircraftOrbitData closest to a given Time.

        This doesn't interpolate, it just return the data point in the file
        closest to the given time. 
        """
        return _pos_export_orbit.PosExportOrbit_aircraft_orbit_data(self, T)


    def _v_file_name(self):
        """

        const std::string& GeoCal::PosExportOrbit::file_name() const

        """
        return _pos_export_orbit.PosExportOrbit__v_file_name(self)


    @property
    def file_name(self):
        return self._v_file_name()


    def _v_file_epoch(self):
        """

        const Time& GeoCal::PosExportOrbit::file_epoch() const

        """
        return _pos_export_orbit.PosExportOrbit__v_file_epoch(self)


    @property
    def file_epoch(self):
        return self._v_file_epoch()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _pos_export_orbit.delete_PosExportOrbit
PosExportOrbit.aircraft_orbit_data = new_instancemethod(_pos_export_orbit.PosExportOrbit_aircraft_orbit_data, None, PosExportOrbit)
PosExportOrbit._v_file_name = new_instancemethod(_pos_export_orbit.PosExportOrbit__v_file_name, None, PosExportOrbit)
PosExportOrbit._v_file_epoch = new_instancemethod(_pos_export_orbit.PosExportOrbit__v_file_epoch, None, PosExportOrbit)
PosExportOrbit_swigregister = _pos_export_orbit.PosExportOrbit_swigregister
PosExportOrbit_swigregister(PosExportOrbit)


__all__ = ["PosExportOrbit"]



