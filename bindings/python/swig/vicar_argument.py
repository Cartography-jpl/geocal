# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _vicar_argument

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _vicar_argument.SWIG_PyInstanceMethod_New
_swig_new_static_method = _vicar_argument.SWIG_PyStaticMethod_New

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

SWIG_MODULE_ALREADY_DONE = _vicar_argument.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _vicar_argument.delete_SwigPyIterator
    value = _swig_new_instance_method(_vicar_argument.SwigPyIterator_value)
    incr = _swig_new_instance_method(_vicar_argument.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_vicar_argument.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_vicar_argument.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_vicar_argument.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_vicar_argument.SwigPyIterator_copy)
    next = _swig_new_instance_method(_vicar_argument.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_vicar_argument.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_vicar_argument.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_vicar_argument.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _vicar_argument:
_vicar_argument.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _vicar_argument.SHARED_PTR_DISOWN

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
class VicarArgument(geocal_swig.generic_object.GenericObject):
    r"""

    This provides access to arguments passed by VICAR.

    Note an important limitation of VICAR, you can only initialize the
    argument handling once. This means that if you try to create multiple
    VicarArgument objects, you'll trigger a TAE error when you try to
    create the second object.

    VICAR uses a somewhat unusual mechanism to pass arguments to a program
    called from the TAE. Rather than putting the arguments on the command
    line to appear in Argv, the arguments get passed down a pipe (usually
    file logical 4) in a compressed format. I'm not sure why exactly it
    does this, probably some historical artifact from when it was
    developed on VMS. In any case, in order to fit into VICAR we need to
    parse arguments the same way.

    This class handles the arguments.

    C++ includes: vicar_argument.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, ARGC, copy_primary_input_label=False):
        r"""

        VicarArgument::VicarArgument(int Argc, char **Argv, bool copy_primary_input_label=false)
        Set up to parse VICAR arguments.

        VICAR arguments are accessed by keywords. This isn't a bad interface,
        but because we also want to support straight unix command line style
        arguments, we have the VicarArgument interface in terms of index
        number (first argument, second argument, etc.). Keyword_list gives the
        list of keywords in order that we want.

        VICAR has the convention that when creating a new file it
        automatically copies over all of the labels from the "primary input"
        - usually the first input file. The idea is that with old VICAR files
        most of the time the output image would have same size, map
        projection, etc. However, this doesn't fit well with the geocal usage
        at all. Much of the time the output file doesn't have any relationship
        to the input file. Also the label reading and writing happens in the
        background anyways, so there isn't a lot gained by the defaults. This
        can often cause problems, such as copying map projection data from an
        input file to an output file that isn't map projected.

        By default, we turn this copying off. You can get the old VICAR
        behavior by setting Copy_primary_input_label to true. 
        """
        _vicar_argument.VicarArgument_swiginit(self, _vicar_argument.new_VicarArgument(ARGC, copy_primary_input_label))
    write_out = _swig_new_instance_method(_vicar_argument.VicarArgument_write_out)
    type_string = _swig_new_instance_method(_vicar_argument.VicarArgument_type_string)
    type_count = _swig_new_instance_method(_vicar_argument.VicarArgument_type_count)
    arg_str = _swig_new_instance_method(_vicar_argument.VicarArgument_arg_str)
    arg_int = _swig_new_instance_method(_vicar_argument.VicarArgument_arg_int)
    arg_real = _swig_new_instance_method(_vicar_argument.VicarArgument_arg_real)
    __str__ = _swig_new_instance_method(_vicar_argument.VicarArgument___str__)

    def __getitem__(self, key):
      k = str(key).upper()
      t = self.type_string(k)
      if(t == "STRING"):
        r = self.arg_str(k)
      elif(t == "REAL"):
        r = self.arg_real(k)
      elif(t == "INT"):
        r = self.arg_int(k)
      if(self.type_count(k) == 1):
        r = r[0]
      return r

    def __setitem__(self, key, val):
      k = str(key).upper()
      t = self.type_string(k)
      if(t == "STRING"):
        self.write_out(k, str(val))
      elif(t == "REAL"):
        self.write_out(k, float(val))
      elif(t == "INT"):
        self.write_out(k, int(val))


    __swig_destroy__ = _vicar_argument.delete_VicarArgument

# Register VicarArgument in _vicar_argument:
_vicar_argument.VicarArgument_swigregister(VicarArgument)

__all__ = ["VicarArgument"]


