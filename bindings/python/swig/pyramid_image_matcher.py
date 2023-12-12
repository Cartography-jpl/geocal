# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _pyramid_image_matcher.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_pyramid_image_matcher')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_pyramid_image_matcher')
    _pyramid_image_matcher = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_pyramid_image_matcher', [dirname(__file__)])
        except ImportError:
            import _pyramid_image_matcher
            return _pyramid_image_matcher
        try:
            _mod = imp.load_module('_pyramid_image_matcher', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _pyramid_image_matcher = swig_import_helper()
    del swig_import_helper
else:
    import _pyramid_image_matcher
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


SWIG_MODULE_ALREADY_DONE = _pyramid_image_matcher.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _pyramid_image_matcher.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_pyramid_image_matcher.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_pyramid_image_matcher.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _pyramid_image_matcher.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _pyramid_image_matcher.SHARED_PTR_DISOWN

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
class PyramidImageMatcher(geocal_swig.image_matcher.ImageMatcher):
    """

    This takes an existing ImageMatcher, and it uses it on images in a
    pyramid fashion.

    We start with a given power of 2 (e.g., 8), average the raster data by
    that value and perform image matching.

    If that is successful, we then take the result of the previous match
    as the initial guess, then match on the finer grid by a factor p of 2
    (e.g., 4). If successful, we then proceed to the next level, ending at
    the full resolution image.

    If we match any level, then we call the match successful. However, the
    Line and Sample sigmas will get scaled the same way - so data matched
    at a coarser resolution is less accurate than matched at the finer
    resolution.

    C++ includes: pyramid_image_matcher.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Im, start_level, Only_accept_finest_resolution=False):
        """

        GeoCal::PyramidImageMatcher::PyramidImageMatcher(const boost::shared_ptr< ImageMatcher > &Im, int start_level, bool
        Only_accept_finest_resolution=false)
        Constructor.

        The start level is how coarse to do the initial matching. By default
        we accept any level of matching (just with a larger error), but you
        can optionally specify that we only accept a point if we can match at
        the finest resolution 
        """
        _pyramid_image_matcher.PyramidImageMatcher_swiginit(self, _pyramid_image_matcher.new_PyramidImageMatcher(Im, start_level, Only_accept_finest_resolution))

    def _v_start_level(self):
        """

        int GeoCal::PyramidImageMatcher::start_level() const
        Starting level, so we average by 2^start_level. 
        """
        return _pyramid_image_matcher.PyramidImageMatcher__v_start_level(self)


    @property
    def start_level(self):
        return self._v_start_level()


    def _v_only_accept_finest_resolution(self):
        """

        bool GeoCal::PyramidImageMatcher::only_accept_finest_resolution() const
        If true, we only accept points that can be matched at the finest
        resolution. 
        """
        return _pyramid_image_matcher.PyramidImageMatcher__v_only_accept_finest_resolution(self)


    @property
    def only_accept_finest_resolution(self):
        return self._v_only_accept_finest_resolution()


    def _v_underlying_matcher(self):
        """

        boost::shared_ptr<ImageMatcher> GeoCal::PyramidImageMatcher::underlying_matcher() const
        Underlying image matcher. 
        """
        return _pyramid_image_matcher.PyramidImageMatcher__v_underlying_matcher(self)


    @property
    def underlying_matcher(self):
        return self._v_underlying_matcher()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _pyramid_image_matcher.delete_PyramidImageMatcher
PyramidImageMatcher._v_start_level = new_instancemethod(_pyramid_image_matcher.PyramidImageMatcher__v_start_level, None, PyramidImageMatcher)
PyramidImageMatcher._v_only_accept_finest_resolution = new_instancemethod(_pyramid_image_matcher.PyramidImageMatcher__v_only_accept_finest_resolution, None, PyramidImageMatcher)
PyramidImageMatcher._v_underlying_matcher = new_instancemethod(_pyramid_image_matcher.PyramidImageMatcher__v_underlying_matcher, None, PyramidImageMatcher)
PyramidImageMatcher_swigregister = _pyramid_image_matcher.PyramidImageMatcher_swigregister
PyramidImageMatcher_swigregister(PyramidImageMatcher)


__all__ = ["PyramidImageMatcher"]



