# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _geocal_fftw.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_fftw', [dirname(__file__)])
        except ImportError:
            import _geocal_fftw
            return _geocal_fftw
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_fftw', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_fftw = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_fftw
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


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


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



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
except:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _geocal_fftw.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_geocal_fftw.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geocal_fftw.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geocal_fftw.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geocal_fftw.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geocal_fftw.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geocal_fftw.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geocal_fftw.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geocal_fftw.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geocal_fftw.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geocal_fftw.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geocal_fftw.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geocal_fftw.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geocal_fftw.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geocal_fftw.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geocal_fftw.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geocal_fftw.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _geocal_fftw.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_geocal_fftw.SHARED_PTR_DISOWN_swigconstant(_geocal_fftw)
SHARED_PTR_DISOWN = _geocal_fftw.SHARED_PTR_DISOWN

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
class Fftw2dForward(geocal_swig.generic_object.GenericObject):
    """

    This is a light wrapper around the FFTW library.

    This maintains the lifetime of plans. This is for a 2D real transform,
    going forward to a complex results.

    C++ includes: geocal_fftw.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Nrow, Ncol):
        """

        Fftw2dForward::Fftw2dForward(int Nrow, int Ncol)
        Constructor. 
        """
        _geocal_fftw.Fftw2dForward_swiginit(self, _geocal_fftw.new_Fftw2dForward(Nrow, Ncol))
    data_in = _swig_property(_geocal_fftw.Fftw2dForward_data_in_get, _geocal_fftw.Fftw2dForward_data_in_set)
    data_out = _swig_property(_geocal_fftw.Fftw2dForward_data_out_get, _geocal_fftw.Fftw2dForward_data_out_set)

    def run_fft(self):
        """

        void GeoCal::Fftw2dForward::run_fft()
        Run the FFT on the data in data_in, with the results going to
        data_out.

        Note that data_in may possibly be destroyed. 
        """
        return _geocal_fftw.Fftw2dForward_run_fft(self)

    __swig_destroy__ = _geocal_fftw.delete_Fftw2dForward
Fftw2dForward.run_fft = new_instancemethod(_geocal_fftw.Fftw2dForward_run_fft, None, Fftw2dForward)
Fftw2dForward.__str__ = new_instancemethod(_geocal_fftw.Fftw2dForward___str__, None, Fftw2dForward)
Fftw2dForward_swigregister = _geocal_fftw.Fftw2dForward_swigregister
Fftw2dForward_swigregister(Fftw2dForward)

class Fftw2dBackward(geocal_swig.generic_object.GenericObject):
    """

    This is a light wrapper around the FFTW library.

    This maintains the lifetime of plans. This is for a 2D real transform,
    going backward from a complex value to real results.

    Note that the FFTW library calculates an unnormalized transform. This
    means that a forward calculation followed by a backward calculation
    will return a results Nrow * Ncol * original data.

    C++ includes: geocal_fftw.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Nrow, Ncol):
        """

        Fftw2dBackward::Fftw2dBackward(int Nrow, int Ncol)
        Constructor. 
        """
        _geocal_fftw.Fftw2dBackward_swiginit(self, _geocal_fftw.new_Fftw2dBackward(Nrow, Ncol))
    data_in = _swig_property(_geocal_fftw.Fftw2dBackward_data_in_get, _geocal_fftw.Fftw2dBackward_data_in_set)
    data_out = _swig_property(_geocal_fftw.Fftw2dBackward_data_out_get, _geocal_fftw.Fftw2dBackward_data_out_set)

    def run_fft(self):
        """

        void GeoCal::Fftw2dBackward::run_fft()
        Run the FFT on the data in data_in, with the results going to
        data_out.

        Note that data_in may possibly be destroyed. 
        """
        return _geocal_fftw.Fftw2dBackward_run_fft(self)

    __swig_destroy__ = _geocal_fftw.delete_Fftw2dBackward
Fftw2dBackward.run_fft = new_instancemethod(_geocal_fftw.Fftw2dBackward_run_fft, None, Fftw2dBackward)
Fftw2dBackward.__str__ = new_instancemethod(_geocal_fftw.Fftw2dBackward___str__, None, Fftw2dBackward)
Fftw2dBackward_swigregister = _geocal_fftw.Fftw2dBackward_swigregister
Fftw2dBackward_swigregister(Fftw2dBackward)


__all__ = ["Fftw2dForward","Fftw2dBackward"]



