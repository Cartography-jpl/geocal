# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _dfunctor_with_derivative.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_dfunctor_with_derivative', [dirname(__file__)])
        except ImportError:
            import _dfunctor_with_derivative
            return _dfunctor_with_derivative
        if fp is not None:
            try:
                _mod = imp.load_module('_dfunctor_with_derivative', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _dfunctor_with_derivative = swig_import_helper()
    del swig_import_helper
else:
    import _dfunctor_with_derivative
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


SHARED_PTR_DISOWN = _dfunctor_with_derivative.SHARED_PTR_DISOWN
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

import geocal_swig.functor
import geocal_swig.generic_object
class DFunctorWithDerivative(geocal_swig.functor.DFunctor):
    """
    C++ includes: dfunctor_with_derivative.h

    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def df(self, *args):
        """
        virtual double GeoCal::DFunctorWithDerivative::df(double X) const =0
        Return df / dx. 
        """
        return _dfunctor_with_derivative.DFunctorWithDerivative_df(self, *args)

    def f_with_derivative(self, *args):
        """
        virtual AutoDerivative<double> GeoCal::DFunctorWithDerivative::f_with_derivative(double X) const =0
        Return df / dp_i as a AutoDerivative. 
        """
        return _dfunctor_with_derivative.DFunctorWithDerivative_f_with_derivative(self, *args)

    __swig_destroy__ = _dfunctor_with_derivative.delete_DFunctorWithDerivative
DFunctorWithDerivative.df = new_instancemethod(_dfunctor_with_derivative.DFunctorWithDerivative_df,None,DFunctorWithDerivative)
DFunctorWithDerivative.f_with_derivative = new_instancemethod(_dfunctor_with_derivative.DFunctorWithDerivative_f_with_derivative,None,DFunctorWithDerivative)
DFunctorWithDerivative_swigregister = _dfunctor_with_derivative.DFunctorWithDerivative_swigregister
DFunctorWithDerivative_swigregister(DFunctorWithDerivative)



