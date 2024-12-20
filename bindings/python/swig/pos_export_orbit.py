# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _pos_export_orbit

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _pos_export_orbit.SWIG_PyInstanceMethod_New
_swig_new_static_method = _pos_export_orbit.SWIG_PyStaticMethod_New

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "this":
            set(self, name, value)
        elif name == "thisown":
            self.this.own(value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


import weakref

SWIG_MODULE_ALREADY_DONE = _pos_export_orbit.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pos_export_orbit.delete_SwigPyIterator
    value = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_value)
    incr = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_copy)
    next = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_pos_export_orbit.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _pos_export_orbit:
_pos_export_orbit.SwigPyIterator_swigregister(SwigPyIterator)
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
    r"""

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

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Fname, Epoch):
        r"""

        PosExportOrbit::PosExportOrbit(const std::string &Fname, const Time &Epoch)
        Read the given text file.

        Note that the time in the Applanix file is in GPS seconds of the week,
        but the actual week isn't in the file. This means we need to pass in
        the Epoch that the times are relative to. 
        """
        _pos_export_orbit.PosExportOrbit_swiginit(self, _pos_export_orbit.new_PosExportOrbit(Fname, Epoch))
    aircraft_orbit_data = _swig_new_instance_method(_pos_export_orbit.PosExportOrbit_aircraft_orbit_data)
    _v_file_name = _swig_new_instance_method(_pos_export_orbit.PosExportOrbit__v_file_name)

    @property
    def file_name(self):
        return self._v_file_name()

    _v_file_epoch = _swig_new_instance_method(_pos_export_orbit.PosExportOrbit__v_file_epoch)

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

# Register PosExportOrbit in _pos_export_orbit:
_pos_export_orbit.PosExportOrbit_swigregister(PosExportOrbit)

__all__ = ["PosExportOrbit"]


