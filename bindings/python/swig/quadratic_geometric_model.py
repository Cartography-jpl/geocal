# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _quadratic_geometric_model.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_quadratic_geometric_model', [dirname(__file__)])
        except ImportError:
            import _quadratic_geometric_model
            return _quadratic_geometric_model
        if fp is not None:
            try:
                _mod = imp.load_module('_quadratic_geometric_model', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _quadratic_geometric_model = swig_import_helper()
    del swig_import_helper
else:
    import _quadratic_geometric_model
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
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
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _quadratic_geometric_model.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_quadratic_geometric_model.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_quadratic_geometric_model.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _quadratic_geometric_model.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _quadratic_geometric_model.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

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
        QuadraticGeometricModel::QuadraticGeometricModel(FitType ft=LINEAR, double Magnify_line=1.0, double
        Magnify_sample=1.0)
        Constructor. This creates an identity transformation. 
        """
        _quadratic_geometric_model.QuadraticGeometricModel_swiginit(self,_quadratic_geometric_model.new_QuadraticGeometricModel(*args))
    def fit_transformation(self, *args):
        """
        void QuadraticGeometricModel::fit_transformation(const GeometricTiePoints &Tp)
        Fit the transformation in the least squares sense to match the given
        set of tiepoints. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel_fit_transformation(self, *args)

    def _v_transformation(self):
        """
        const blitz::Array<double, 1> GeoCal::QuadraticGeometricModel::transformation() const
        Transformation, which is the coefficients of the polynomial.

        We have

        x = trans(0)*px+trans(1)*py+trans(2)+trans(3)*px*px+
        trans(4)*py*py+trans(5)*px*py y =
        trans(6)*px+trans(7)*py+trans(8)+trans(9)*px*px+
        trans(10)*py*py+trans(11)*px*py 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_transformation(self)

    @property
    def transformation(self):
        return self._v_transformation()

    def _v_magnify_line(self):
        """
        double GeoCal::QuadraticGeometricModel::magnify_line() const
        Magnification factor to apply in line direction. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_magnify_line(self)

    @property
    def magnify_line(self):
        return self._v_magnify_line()

    def _v_magnify_sample(self):
        """
        double GeoCal::QuadraticGeometricModel::magnify_sample() const
        Magnification factor to apply sample direction. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_magnify_sample(self)

    @property
    def magnify_sample(self):
        return self._v_magnify_sample()

    def _v_fit_type(self):
        """
        FitType GeoCal::QuadraticGeometricModel::fit_type() const
        Type of fit to do. 
        """
        return _quadratic_geometric_model.QuadraticGeometricModel__v_fit_type(self)

    @property
    def fit_type(self):
        return self._v_fit_type()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.transformation,self.fit_type,self.magnify_line,self.magnify_sample)

    __swig_destroy__ = _quadratic_geometric_model.delete_QuadraticGeometricModel
QuadraticGeometricModel.fit_transformation = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel_fit_transformation,None,QuadraticGeometricModel)
QuadraticGeometricModel._v_transformation = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_transformation,None,QuadraticGeometricModel)
QuadraticGeometricModel._v_magnify_line = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_magnify_line,None,QuadraticGeometricModel)
QuadraticGeometricModel._v_magnify_sample = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_magnify_sample,None,QuadraticGeometricModel)
QuadraticGeometricModel._v_fit_type = new_instancemethod(_quadratic_geometric_model.QuadraticGeometricModel__v_fit_type,None,QuadraticGeometricModel)
QuadraticGeometricModel_swigregister = _quadratic_geometric_model.QuadraticGeometricModel_swigregister
QuadraticGeometricModel_swigregister(QuadraticGeometricModel)



