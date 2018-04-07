# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _quadratic_geometric_model.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_quadratic_geometric_model')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_quadratic_geometric_model')
    _quadratic_geometric_model = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_quadratic_geometric_model', [dirname(__file__)])
        except ImportError:
            import _quadratic_geometric_model
            return _quadratic_geometric_model
        try:
            _mod = imp.load_module('_quadratic_geometric_model', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _quadratic_geometric_model = swig_import_helper()
    del swig_import_helper
else:
    import _quadratic_geometric_model
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
    __swig_destroy__ = _quadratic_geometric_model.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _quadratic_geometric_model.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _quadratic_geometric_model.SHARED_PTR_DISOWN

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

import geocal_swig.geometric_model
import geocal_swig.generic_object
import geocal_swig.observer
class QuadraticGeometricModel(geocal_swig.geometric_model.GeometricModel):
    """

    This is a GeometricModel that provides a quadratic polynomial
    geometric model.

    C++ includes: quadratic_geometric_model.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    LINEAR = _quadratic_geometric_model.QuadraticGeometricModel_LINEAR
    QUADRATIC = _quadratic_geometric_model.QuadraticGeometricModel_QUADRATIC

    def __init__(self, *args):
        """

        QuadraticGeometricModel::QuadraticGeometricModel(const boost::shared_ptr< GeometricTiePoints > &Tp, int Min_tp_to_fit,
        FitType ft=LINEAR, double Magnify_line=1.0, double Magnify_sample=1.0)
        Constructor.

        This fits the set of Tp, or creates an identity transformation if Tp
        is null. 
        """
        _quadratic_geometric_model.QuadraticGeometricModel_swiginit(self, _quadratic_geometric_model.new_QuadraticGeometricModel(*args))

    def _v_tie_points(self):
        """

        const boost::shared_ptr<GeometricTiePoints>& GeoCal::QuadraticGeometricModel::tie_points() const
        Tiepoints used for Model.

        This may be null if we aren't actually using tiepoints.

        Adding points to tie_points() (or removing them) will automatically
        update this transformation, if the number of tie_points() is >=
        min_number_tie_point_to_fit(). 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_tie_points(self)


    @property
    def tie_points(self):
        return self._v_tie_points()


    def fit_transformation(self, Tp):
        """

        void QuadraticGeometricModel::fit_transformation(const GeometricTiePoints &Tp)
        Fit the transformation in the least squares sense to match the given
        set of tiepoints. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel_fit_transformation(self, Tp)


    def _v_transformation(self):
        """

        blitz::Array<double, 1>& GeoCal::QuadraticGeometricModel::transformation()

        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_transformation(self)


    @property
    def transformation(self):
        return self._v_transformation()


    def _v_inverse_transformation(self):
        """

        blitz::Array<double, 1>& GeoCal::QuadraticGeometricModel::inverse_transformation()

        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_inverse_transformation(self)


    @property
    def inverse_transformation(self):
        return self._v_inverse_transformation()


    def _v_magnify_line(self, *args):
        """

        void GeoCal::QuadraticGeometricModel::magnify_line(double v)

        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_magnify_line(self, *args)


    @property
    def magnify_line(self):
        return self._v_magnify_line()

    @magnify_line.setter
    def magnify_line(self, value):
      self._v_magnify_line(value)


    def _v_magnify_sample(self, *args):
        """

        void GeoCal::QuadraticGeometricModel::magnify_sample(double v)

        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_magnify_sample(self, *args)


    @property
    def magnify_sample(self):
        return self._v_magnify_sample()

    @magnify_sample.setter
    def magnify_sample(self, value):
      self._v_magnify_sample(value)


    def _v_fit_type(self):
        """

        FitType GeoCal::QuadraticGeometricModel::fit_type() const
        Type of fit to do. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_fit_type(self)


    @property
    def fit_type(self):
        return self._v_fit_type()


    def _v_min_number_tie_point_to_fit(self):
        """

        int GeoCal::QuadraticGeometricModel::min_number_tie_point_to_fit() const
        This is the minimum number of tie_points() before we automatically
        update our fit. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_min_number_tie_point_to_fit(self)


    @property
    def min_number_tie_point_to_fit(self):
        return self._v_min_number_tie_point_to_fit()


    def _v_enough_tie_point_to_fit(self):
        """

        bool GeoCal::QuadraticGeometricModel::enough_tie_point_to_fit() const
        True if we 1) have tie_points() and 2) number of tie points >=
        min_number_tie_point_to_fit(). 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_enough_tie_point_to_fit(self)


    @property
    def enough_tie_point_to_fit(self):
        return self._v_enough_tie_point_to_fit()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _quadratic_geometric_model.delete_QuadraticGeometricModel
QuadraticGeometricModel._v_tie_points = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_tie_points, None, QuadraticGeometricModel)
QuadraticGeometricModel.fit_transformation = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel_fit_transformation, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_transformation = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_transformation, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_inverse_transformation = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_inverse_transformation, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_magnify_line = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_magnify_line, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_magnify_sample = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_magnify_sample, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_fit_type = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_fit_type, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_min_number_tie_point_to_fit = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_min_number_tie_point_to_fit, None, QuadraticGeometricModel)
QuadraticGeometricModel._v_enough_tie_point_to_fit = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_enough_tie_point_to_fit, None, QuadraticGeometricModel)
QuadraticGeometricModel_swigregister = _quadratic_geometric_model.QuadraticGeometricModel_swigregister
QuadraticGeometricModel_swigregister(QuadraticGeometricModel)



