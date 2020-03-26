# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _lsm_matcher.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_lsm_matcher')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_lsm_matcher')
    _lsm_matcher = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_lsm_matcher', [dirname(__file__)])
        except ImportError:
            import _lsm_matcher
            return _lsm_matcher
        try:
            _mod = imp.load_module('_lsm_matcher', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _lsm_matcher = swig_import_helper()
    del swig_import_helper
else:
    import _lsm_matcher
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
    __swig_destroy__ = _lsm_matcher.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_lsm_matcher.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_lsm_matcher.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_lsm_matcher.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_lsm_matcher.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_lsm_matcher.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_lsm_matcher.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_lsm_matcher.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_lsm_matcher.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_lsm_matcher.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_lsm_matcher.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_lsm_matcher.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_lsm_matcher.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_lsm_matcher.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_lsm_matcher.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_lsm_matcher.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_lsm_matcher.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _lsm_matcher.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _lsm_matcher.SHARED_PTR_DISOWN

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

import geocal_swig.image_matcher
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class LsmMatcher(geocal_swig.image_matcher.ImageMatcher):
    """

    This class performs image matching.

    This does a nonlinear least squares match. It also calculates an
    estimate of the error in its matching.

    We attempt to reach a goal of precision_goal in determining a0 and b0.
    If at any iteration, the uncertainty in determining a0 and b0 is
    greater than max_sigma, then we give up on doing LSM. If the final
    uncertainty in determining a0 and b0 is larger than
    precision_requirement, then we give up on doing LSM. We now also check
    radiometric uncertainty at each iteration as described in MISR SDS
    DFM-0245-I. The maximum allowed radiometric uncertainty is calculated
    by multiplying the factor radiometric_uncertainty_factor by the
    minimum of the template and target window sigmas. After each iteration
    in the lsm the uncertainty in the calculation of h0 is greater than
    this threshold we stop doing LSM and return unsuccessful.

    The minimum uncertainty that will be returned is precision_goal (i.e.,
    if the estimated uncertainty is smaller than this value, then
    precision_goal is returned instead).

    The model used is to resample the target using an affine
    transformation combined with a linear correction to the radiometry:

    g'(i, j) = h0 + h1 * g(a0 + a1 * i + a2 * j, b0 + b1 * i + b2 * j)

    Where g is the target, g' is the template.

    As a convention, internal to this class we index things so g'(0, 0) is
    the center of the template.

    We solve for the parameters giving the best fit against the template.

    C++ includes: lsm_matcher.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Number_line=21, Number_sample=21, Border_size=3, Precision_goal=0.0625, Precision_requirement=0.2, Max_sigma=0.5, Rad_uncertainty_factor=2.0, Precision_min_geo_goal=0.15, Precision_min_rad_goal=1):
        """

        BZ_END_STENCIL LsmMatcher::LsmMatcher(int Number_line=21, int Number_sample=21, int Border_size=3, double
        Precision_goal=0.0625, double Precision_requirement=0.2, double
        Max_sigma=0.5, double Rad_uncertainty_factor=2.0, double
        Precision_min_geo_goal=0.15, double Precision_min_rad_goal=1)
        Constructor.

        Default values were tuned for MISR imagery, but work well for other
        imagery. 
        """
        _lsm_matcher.LsmMatcher_swiginit(self, _lsm_matcher.new_LsmMatcher(Number_line, Number_sample, Border_size, Precision_goal, Precision_requirement, Max_sigma, Rad_uncertainty_factor, Precision_min_geo_goal, Precision_min_rad_goal))

    def _v_number_line(self):
        """

        int GeoCal::LsmMatcher::number_line() const
        Number of lines in target and template. 
        """
        return _lsm_matcher.LsmMatcher__v_number_line(self)


    @property
    def number_line(self):
        return self._v_number_line()


    def _v_number_sample(self):
        """

        int GeoCal::LsmMatcher::number_sample() const
        Number of samples in target and template. 
        """
        return _lsm_matcher.LsmMatcher__v_number_sample(self)


    @property
    def number_sample(self):
        return self._v_number_sample()


    def _v_border_size(self):
        """

        int GeoCal::LsmMatcher::border_size() const
        Border added to target to allow for template/target shift. 
        """
        return _lsm_matcher.LsmMatcher__v_border_size(self)


    @property
    def border_size(self):
        return self._v_border_size()


    def _v_precision_goal(self):
        """

        double GeoCal::LsmMatcher::precision_goal() const
        Goal in doing solving LSM. 
        """
        return _lsm_matcher.LsmMatcher__v_precision_goal(self)


    @property
    def precision_goal(self):
        return self._v_precision_goal()


    def _v_precision_requirement(self):
        """

        double GeoCal::LsmMatcher::precision_requirement() const
        Requirement for LSM success. 
        """
        return _lsm_matcher.LsmMatcher__v_precision_requirement(self)


    @property
    def precision_requirement(self):
        return self._v_precision_requirement()


    def _v_max_sigma(self):
        """

        double GeoCal::LsmMatcher::max_sigma() const
        If at any stagee in solving LSM uncertainty is greater than this
        value, then give up doing image matching. 
        """
        return _lsm_matcher.LsmMatcher__v_max_sigma(self)


    @property
    def max_sigma(self):
        return self._v_max_sigma()


    def _v_rad_uncertainty_factor(self):
        """

        double GeoCal::LsmMatcher::rad_uncertainty_factor() const
        This factor is used to scale the maximum allowable radiometric
        uncertainty while solving LSM. 
        """
        return _lsm_matcher.LsmMatcher__v_rad_uncertainty_factor(self)


    @property
    def rad_uncertainty_factor(self):
        return self._v_rad_uncertainty_factor()


    def _v_precision_min_geo_goal(self):
        """

        double GeoCal::LsmMatcher::precision_min_geo_goal() const
        Minimum geometric goal for LSM to success. 
        """
        return _lsm_matcher.LsmMatcher__v_precision_min_geo_goal(self)


    @property
    def precision_min_geo_goal(self):
        return self._v_precision_min_geo_goal()


    def _v_precision_min_rad_goal(self):
        """

        double GeoCal::LsmMatcher::precision_min_rad_goal() const
        Minimum radiometric goal for LSM to success. 
        """
        return _lsm_matcher.LsmMatcher__v_precision_min_rad_goal(self)


    @property
    def precision_min_rad_goal(self):
        return self._v_precision_min_rad_goal()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _lsm_matcher.delete_LsmMatcher
LsmMatcher._v_number_line = new_instancemethod(_lsm_matcher.LsmMatcher__v_number_line, None, LsmMatcher)
LsmMatcher._v_number_sample = new_instancemethod(_lsm_matcher.LsmMatcher__v_number_sample, None, LsmMatcher)
LsmMatcher._v_border_size = new_instancemethod(_lsm_matcher.LsmMatcher__v_border_size, None, LsmMatcher)
LsmMatcher._v_precision_goal = new_instancemethod(_lsm_matcher.LsmMatcher__v_precision_goal, None, LsmMatcher)
LsmMatcher._v_precision_requirement = new_instancemethod(_lsm_matcher.LsmMatcher__v_precision_requirement, None, LsmMatcher)
LsmMatcher._v_max_sigma = new_instancemethod(_lsm_matcher.LsmMatcher__v_max_sigma, None, LsmMatcher)
LsmMatcher._v_rad_uncertainty_factor = new_instancemethod(_lsm_matcher.LsmMatcher__v_rad_uncertainty_factor, None, LsmMatcher)
LsmMatcher._v_precision_min_geo_goal = new_instancemethod(_lsm_matcher.LsmMatcher__v_precision_min_geo_goal, None, LsmMatcher)
LsmMatcher._v_precision_min_rad_goal = new_instancemethod(_lsm_matcher.LsmMatcher__v_precision_min_rad_goal, None, LsmMatcher)
LsmMatcher_swigregister = _lsm_matcher.LsmMatcher_swigregister
LsmMatcher_swigregister(LsmMatcher)


__all__ = ["LsmMatcher"]



