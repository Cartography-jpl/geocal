# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _geocal_gsl_root.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_gsl_root', [dirname(__file__)])
        except ImportError:
            import _geocal_gsl_root
            return _geocal_gsl_root
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_gsl_root', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_gsl_root = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_gsl_root
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
    __swig_destroy__ = _geocal_gsl_root.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_geocal_gsl_root.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geocal_gsl_root.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geocal_gsl_root.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geocal_gsl_root.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geocal_gsl_root.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geocal_gsl_root.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geocal_gsl_root.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geocal_gsl_root.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geocal_gsl_root.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geocal_gsl_root.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _geocal_gsl_root.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _geocal_gsl_root.SHARED_PTR_DISOWN
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

def gsl_root(*args):
  """
    double GeoCal::gsl_root(const DFunctor &F, double Xmin, double Xmax, double Eps=1e-6, double
    Eps_abs=1e-8)
    This finds the root of a Double -> Double function, without a
    derivative available.

    The solution found is in the bracketed range Xmin <= X <= Xmax. We
    find a solution when we have bracketed it within a range xlow, xhigh,
    with xhigh - xlow < Eps.

    If we can't find a solution, we throw a ConvergenceFailure exception.

    """
  return _geocal_gsl_root.gsl_root(*args)

def gsl_root_with_derivative(*args):
  """
    AutoDerivative< double > GeoCal::gsl_root_with_derivative(const DFunctorWithDerivative &F, double Xmin, double Xmax, double
    Eps=1e-6, double Eps_abs=1e-8)
    This finds the root of a function, and propagates the derivative of
    the solution with respect to any parameters in the function (i.e., we
    aren't talking about the derivative wrt X here). 
    """
  return _geocal_gsl_root.gsl_root_with_derivative(*args)

def root_list(*args):
  """
    std::vector< AutoDerivative< double > > GeoCal::root_list(const DFunctorWithDerivative &F, double Xmin, double Xmax, double
    Root_minimum_spacing, double Eps=1e-6)
    This will find a (possible empty) list of roots of a function, where
    the roots have a seperation of at least the supplied minimum
    separation.

    This function is useful for finding roots when you don't know how many
    solutions there are in the given range. It will find all roots,
    provided that they have a seperation larger then Root_minimum_spacing,
    and return the list of solutions. This list is ordered from smallest
    to greatest.

    This function works by sampling the Functor with a spacing of
    Root_minimum_spacing. If the function changes sign between one spacing
    and the next, the routine root is called between those spacings and
    the results is added to the root list.

    This will not finds roots that are closer together then the supplied
    minimum spacing. (Limitation) 
    """
  return _geocal_gsl_root.root_list(*args)


