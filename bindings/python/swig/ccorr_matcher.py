# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _ccorr_matcher.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_ccorr_matcher')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_ccorr_matcher')
    _ccorr_matcher = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ccorr_matcher', [dirname(__file__)])
        except ImportError:
            import _ccorr_matcher
            return _ccorr_matcher
        try:
            _mod = imp.load_module('_ccorr_matcher', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _ccorr_matcher = swig_import_helper()
    del swig_import_helper
else:
    import _ccorr_matcher
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


SWIG_MODULE_ALREADY_DONE = _ccorr_matcher.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _ccorr_matcher.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_ccorr_matcher.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ccorr_matcher.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ccorr_matcher.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ccorr_matcher.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ccorr_matcher.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ccorr_matcher.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ccorr_matcher.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ccorr_matcher.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ccorr_matcher.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ccorr_matcher.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _ccorr_matcher.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ccorr_matcher.SHARED_PTR_DISOWN

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

import geocal_swig.image_matcher
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class CcorrMatcher(geocal_swig.image_matcher.ImageMatcher):
    """

    This class performs image matching.

    It does a normalized cross correlation match.

    C++ includes: ccorr_matcher.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Target_nline=37, Target_nsamp=37, Template_nline=9, Template_nsamp=9, Min_ccorr=0.3, Min_variance=0):
        """

        CcorrMatcher::CcorrMatcher(int Target_nline=37, int Target_nsamp=37, int Template_nline=9, int
        Template_nsamp=9, double Min_ccorr=0.3, double Min_variance=0)

        """
        _ccorr_matcher.CcorrMatcher_swiginit(self, _ccorr_matcher.new_CcorrMatcher(Target_nline, Target_nsamp, Template_nline, Template_nsamp, Min_ccorr, Min_variance))

    def _v_min_correlation(self):
        """

        double GeoCal::CcorrMatcher::min_correlation() const
        Minimum correlation threshold. 
        """
        return _ccorr_matcher.CcorrMatcher__v_min_correlation(self)


    @property
    def min_correlation(self):
        return self._v_min_correlation()


    def _v_min_variance(self):
        """

        double GeoCal::CcorrMatcher::min_variance() const
        Minimum variance threshold. 
        """
        return _ccorr_matcher.CcorrMatcher__v_min_variance(self)


    @property
    def min_variance(self):
        return self._v_min_variance()


    def _v_target_number_line(self):
        """

        int GeoCal::CcorrMatcher::target_number_line() const
        Number of lines in target window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_target_number_line(self)


    @property
    def target_number_line(self):
        return self._v_target_number_line()


    def _v_target_number_sample(self):
        """

        int GeoCal::CcorrMatcher::target_number_sample() const
        Number of samples in target window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_target_number_sample(self)


    @property
    def target_number_sample(self):
        return self._v_target_number_sample()


    def _v_template_number_line(self):
        """

        int GeoCal::CcorrMatcher::template_number_line() const
        Number of lines in template window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_template_number_line(self)


    @property
    def template_number_line(self):
        return self._v_template_number_line()


    def _v_template_number_sample(self):
        """

        int GeoCal::CcorrMatcher::template_number_sample() const
        Number of samples in template window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_template_number_sample(self)


    @property
    def template_number_sample(self):
        return self._v_template_number_sample()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _ccorr_matcher.delete_CcorrMatcher
CcorrMatcher._v_min_correlation = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_min_correlation, None, CcorrMatcher)
CcorrMatcher._v_min_variance = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_min_variance, None, CcorrMatcher)
CcorrMatcher._v_target_number_line = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_target_number_line, None, CcorrMatcher)
CcorrMatcher._v_target_number_sample = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_target_number_sample, None, CcorrMatcher)
CcorrMatcher._v_template_number_line = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_template_number_line, None, CcorrMatcher)
CcorrMatcher._v_template_number_sample = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_template_number_sample, None, CcorrMatcher)
CcorrMatcher_swigregister = _ccorr_matcher.CcorrMatcher_swigregister
CcorrMatcher_swigregister(CcorrMatcher)


__all__ = ["CcorrMatcher"]



