# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _tie_point

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _tie_point.SWIG_PyInstanceMethod_New
_swig_new_static_method = _tie_point.SWIG_PyStaticMethod_New

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

class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _tie_point.delete_SwigPyIterator
    value = _swig_new_instance_method(_tie_point.SwigPyIterator_value)
    incr = _swig_new_instance_method(_tie_point.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_tie_point.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_tie_point.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_tie_point.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_tie_point.SwigPyIterator_copy)
    next = _swig_new_instance_method(_tie_point.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_tie_point.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_tie_point.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_tie_point.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_tie_point.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_tie_point.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_tie_point.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_tie_point.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_tie_point.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_tie_point.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _tie_point:
_tie_point.SwigPyIterator_swigregister(SwigPyIterator)
SWIG_MODULE_ALREADY_DONE = _tie_point.SWIG_MODULE_ALREADY_DONE
SHARED_PTR_DISOWN = _tie_point.SHARED_PTR_DISOWN

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

import geocal_swig.generic_object
import geocal_swig.look_vector
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class TiePoint(geocal_swig.generic_object.GenericObject):
    r"""

    This is a tiepoint.

    This is little more than a structure.

    Note that we previously had TiePoint a python only object, and stored
    it as a python pickled object. The primary motivation for moving this
    to C++ is to be able to use the boost XML serialization, which is more
    stable and can support old versions. If you have an older tiepoint
    data using the old pickle format, you can either rerun the process
    that generated the tie points, or you can run the old version of the
    code, write the tiepoints out as the old MSPI format
    (write_old_mspi_format defined in python), and the read the old MSPI
    format using the new version of the code with the tiepoints in C++.
    This is just a temporary fix, going forward the intention is that the
    tiepoints will use the boost format and will automatically support
    older versions.

    C++ includes: tie_point.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""

        TiePoint::TiePoint(const TiePoint &Tp)
        Copy constructor. We make a deep copy of everything. 
        """
        _tie_point.TiePoint_swiginit(self, _tie_point.new_TiePoint(*args))
    _v_id = _swig_new_instance_method(_tie_point.TiePoint__v_id)

    @property
    def id(self):
        return self._v_id()

    @id.setter
    def id(self, value):
      self._v_id(value)

    _v_is_gcp = _swig_new_instance_method(_tie_point.TiePoint__v_is_gcp)

    @property
    def is_gcp(self):
        return self._v_is_gcp()

    @is_gcp.setter
    def is_gcp(self, value):
      self._v_is_gcp(value)

    _v_ground_location = _swig_new_instance_method(_tie_point.TiePoint__v_ground_location)

    @property
    def ground_location(self):
        return self._v_ground_location()

    @ground_location.setter
    def ground_location(self, value):
      self._v_ground_location(value)

    _v_number_image = _swig_new_instance_method(_tie_point.TiePoint__v_number_image)

    @property
    def number_image(self):
        return self._v_number_image()

    _v_number_image_location = _swig_new_instance_method(_tie_point.TiePoint__v_number_image_location)

    @property
    def number_image_location(self):
        return self._v_number_image_location()

    image_coordinate = _swig_new_instance_method(_tie_point.TiePoint_image_coordinate)
    line_sigma = _swig_new_instance_method(_tie_point.TiePoint_line_sigma)
    sample_sigma = _swig_new_instance_method(_tie_point.TiePoint_sample_sigma)
    __str__ = _swig_new_instance_method(_tie_point.TiePoint___str__)
    _v_ic = _swig_new_instance_method(_tie_point.TiePoint__v_ic)

    @property
    def ic(self):
        return self._v_ic()

    _v_ic_sigma = _swig_new_instance_method(_tie_point.TiePoint__v_ic_sigma)

    @property
    def ic_sigma(self):
        return self._v_ic_sigma()

    ic_pred = _swig_new_instance_method(_tie_point.TiePoint_ic_pred)
    ic_diff = _swig_new_instance_method(_tie_point.TiePoint_ic_diff)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _tie_point.delete_TiePoint

# Register TiePoint in _tie_point:
_tie_point.TiePoint_swigregister(TiePoint)
class TiePointVector(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr
    iterator = _swig_new_instance_method(_tie_point.TiePointVector_iterator)
    def __iter__(self):
        return self.iterator()
    __nonzero__ = _swig_new_instance_method(_tie_point.TiePointVector___nonzero__)
    __bool__ = _swig_new_instance_method(_tie_point.TiePointVector___bool__)
    __len__ = _swig_new_instance_method(_tie_point.TiePointVector___len__)
    __getslice__ = _swig_new_instance_method(_tie_point.TiePointVector___getslice__)
    __setslice__ = _swig_new_instance_method(_tie_point.TiePointVector___setslice__)
    __delslice__ = _swig_new_instance_method(_tie_point.TiePointVector___delslice__)
    __delitem__ = _swig_new_instance_method(_tie_point.TiePointVector___delitem__)
    __getitem__ = _swig_new_instance_method(_tie_point.TiePointVector___getitem__)
    __setitem__ = _swig_new_instance_method(_tie_point.TiePointVector___setitem__)
    pop = _swig_new_instance_method(_tie_point.TiePointVector_pop)
    append = _swig_new_instance_method(_tie_point.TiePointVector_append)
    empty = _swig_new_instance_method(_tie_point.TiePointVector_empty)
    size = _swig_new_instance_method(_tie_point.TiePointVector_size)
    swap = _swig_new_instance_method(_tie_point.TiePointVector_swap)
    begin = _swig_new_instance_method(_tie_point.TiePointVector_begin)
    end = _swig_new_instance_method(_tie_point.TiePointVector_end)
    rbegin = _swig_new_instance_method(_tie_point.TiePointVector_rbegin)
    rend = _swig_new_instance_method(_tie_point.TiePointVector_rend)
    clear = _swig_new_instance_method(_tie_point.TiePointVector_clear)
    get_allocator = _swig_new_instance_method(_tie_point.TiePointVector_get_allocator)
    pop_back = _swig_new_instance_method(_tie_point.TiePointVector_pop_back)
    erase = _swig_new_instance_method(_tie_point.TiePointVector_erase)

    def __init__(self, *args):
        _tie_point.TiePointVector_swiginit(self, _tie_point.new_TiePointVector(*args))
    push_back = _swig_new_instance_method(_tie_point.TiePointVector_push_back)
    front = _swig_new_instance_method(_tie_point.TiePointVector_front)
    back = _swig_new_instance_method(_tie_point.TiePointVector_back)
    assign = _swig_new_instance_method(_tie_point.TiePointVector_assign)
    resize = _swig_new_instance_method(_tie_point.TiePointVector_resize)
    insert = _swig_new_instance_method(_tie_point.TiePointVector_insert)
    reserve = _swig_new_instance_method(_tie_point.TiePointVector_reserve)
    capacity = _swig_new_instance_method(_tie_point.TiePointVector_capacity)
    __swig_destroy__ = _tie_point.delete_TiePointVector

# Register TiePointVector in _tie_point:
_tie_point.TiePointVector_swigregister(TiePointVector)
class TiePointCollection(geocal_swig.generic_object.GenericObject, TiePointVector):
    r"""

    This is a list of TiePoints with a few useful functions added.

    C++ includes: tie_point.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, *args):
        r"""

        GeoCal::TiePointCollection::TiePointCollection(const TiePointVector &Tlist)
        Constructor with an initial list of TiePoints. 
        """
        _tie_point.TiePointCollection_swiginit(self, _tie_point.new_TiePointCollection(*args))
    add_ibis_file = _swig_new_instance_method(_tie_point.TiePointCollection_add_ibis_file)
    data_array = _swig_new_instance_method(_tie_point.TiePointCollection_data_array)
    _v_number_gcp = _swig_new_instance_method(_tie_point.TiePointCollection__v_number_gcp)

    @property
    def number_gcp(self):
        return self._v_number_gcp()

    __str__ = _swig_new_instance_method(_tie_point.TiePointCollection___str__)

    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _tie_point.delete_TiePointCollection

# Register TiePointCollection in _tie_point:
_tie_point.TiePointCollection_swigregister(TiePointCollection)

__all__ = ["TiePoint","TiePointCollection","TiePointVector"]


