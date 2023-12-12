# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _vicar_argument.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_vicar_argument')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_vicar_argument')
    _vicar_argument = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_argument', [dirname(__file__)])
        except ImportError:
            import _vicar_argument
            return _vicar_argument
        try:
            _mod = imp.load_module('_vicar_argument', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _vicar_argument = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_argument
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


SWIG_MODULE_ALREADY_DONE = _vicar_argument.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _vicar_argument.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_vicar_argument.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_vicar_argument.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_vicar_argument.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_vicar_argument.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_vicar_argument.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_vicar_argument.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_vicar_argument.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_vicar_argument.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_vicar_argument.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_vicar_argument.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_vicar_argument.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_vicar_argument.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_vicar_argument.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_vicar_argument.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_vicar_argument.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_vicar_argument.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _vicar_argument.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

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
    """

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

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, ARGC, copy_primary_input_label=False):
        """

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

    def write_out(self, *args):
        """

        void VicarArgument::write_out(const std::string &Keyword, const std::string &Val, int Max_len=250)

        """
        return _vicar_argument.VicarArgument_write_out(self, *args)


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
VicarArgument.write_out = new_instancemethod(_vicar_argument.VicarArgument_write_out, None, VicarArgument)
VicarArgument.type_string = new_instancemethod(_vicar_argument.VicarArgument_type_string, None, VicarArgument)
VicarArgument.type_count = new_instancemethod(_vicar_argument.VicarArgument_type_count, None, VicarArgument)
VicarArgument.arg_str = new_instancemethod(_vicar_argument.VicarArgument_arg_str, None, VicarArgument)
VicarArgument.arg_int = new_instancemethod(_vicar_argument.VicarArgument_arg_int, None, VicarArgument)
VicarArgument.arg_real = new_instancemethod(_vicar_argument.VicarArgument_arg_real, None, VicarArgument)
VicarArgument.__str__ = new_instancemethod(_vicar_argument.VicarArgument___str__, None, VicarArgument)
VicarArgument_swigregister = _vicar_argument.VicarArgument_swigregister
VicarArgument_swigregister(VicarArgument)


__all__ = ["VicarArgument"]



