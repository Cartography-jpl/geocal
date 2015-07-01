# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _auto_derivative.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_auto_derivative', [dirname(__file__)])
        except ImportError:
            import _auto_derivative
            return _auto_derivative
        if fp is not None:
            try:
                _mod = imp.load_module('_auto_derivative', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _auto_derivative = swig_import_helper()
    del swig_import_helper
else:
    import _auto_derivative
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
    __swig_destroy__ = _auto_derivative.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_auto_derivative.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_auto_derivative.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_auto_derivative.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_auto_derivative.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_auto_derivative.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_auto_derivative.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_auto_derivative.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_auto_derivative.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_auto_derivative.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_auto_derivative.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_auto_derivative.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_auto_derivative.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_auto_derivative.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_auto_derivative.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_auto_derivative.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_auto_derivative.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _auto_derivative.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _auto_derivative.SHARED_PTR_DISOWN
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
class AutoDerivativeDouble(geocal_swig.generic_object.GenericObject):
    """
    There are a number of tools that can be used to do "Automatic
    Differentiation" (see for
    examplehttp://www.autodiff.org/?module=Tools).

    We examined several of the tools, and while these packages have a
    number of advantages (in particular, the ability to run the
    calculation either forward or backwards) for our particular needs a
    simpler forward only calculation was selected. This uses a number of
    type T, along with the first order gradient with respect to a set of
    independent variables. We then overload the standard operations such
    as "+" and "*" to apply the chain rule, to propagate the
    derivatives forward.

    This is a newer field, so there doesn't seem to be standard
    terminology. In "Scientific and Engineering C++" by John Barton and
    Lee Nackman, this is call "Rall numbers" after a paper by L.B. Rall.
    On wikipedia athttp://en.wikipedia.org/wiki/Automatic_differentiation
    this is called "Automatic differentiation using dual numbers".

    As the existing automatic differentiation packages mature, we may want
    to revisit this choice and replace this class with a fuller library.

    This class is not as efficient as hand coding derivative calculation,
    although it is much easier to use. If profiling shows a particular
    bottle neck, you might want to hand code derivatives for that specific
    location, which can then be placed into a AutoDerivative for use
    elsewhere.

    See also ArrayAd which work with this class for Arrays of
    AutoDerivative.

    C++ includes: auto_derivative.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::AutoDerivative< T >::AutoDerivative(const AutoDerivative< T > &D)
        Copy constructor. This does a deep copy. 
        """
        _auto_derivative.AutoDerivativeDouble_swiginit(self,_auto_derivative.new_AutoDerivativeDouble(*args))
    def _v_number_variable(self):
        """
        int GeoCal::AutoDerivative< T >::number_variable() const
        Number of variables in gradient. 
        """
        return _auto_derivative.AutoDerivativeDouble__v_number_variable(self)

    @property
    def number_variable(self):
        return self._v_number_variable()

    def _v_is_constant(self):
        """
        bool GeoCal::AutoDerivative< T >::is_constant() const
        Is this object a constant (with a gradient() all zeros)? 
        """
        return _auto_derivative.AutoDerivativeDouble__v_is_constant(self)

    @property
    def is_constant(self):
        return self._v_is_constant()

    @property
    def value(self):
      return self._value()

    @value.setter
    def value(self, val):
      self._value_set(val)

    @property
    def gradient(self):
      return self._gradient()

    @gradient.setter
    def gradient(self,val):
        self._gradient_set(val)
      
    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _auto_derivative.delete_AutoDerivativeDouble
AutoDerivativeDouble._v_number_variable = new_instancemethod(_auto_derivative.AutoDerivativeDouble__v_number_variable,None,AutoDerivativeDouble)
AutoDerivativeDouble._v_is_constant = new_instancemethod(_auto_derivative.AutoDerivativeDouble__v_is_constant,None,AutoDerivativeDouble)
AutoDerivativeDouble.__lt__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___lt__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__eq__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___eq__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__iadd__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___iadd__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__isub__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___isub__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__imul__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___imul__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__idiv__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___idiv__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__str__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___str__,None,AutoDerivativeDouble)
AutoDerivativeDouble._value = new_instancemethod(_auto_derivative.AutoDerivativeDouble__value,None,AutoDerivativeDouble)
AutoDerivativeDouble._value_set = new_instancemethod(_auto_derivative.AutoDerivativeDouble__value_set,None,AutoDerivativeDouble)
AutoDerivativeDouble._gradient = new_instancemethod(_auto_derivative.AutoDerivativeDouble__gradient,None,AutoDerivativeDouble)
AutoDerivativeDouble._gradient_set = new_instancemethod(_auto_derivative.AutoDerivativeDouble__gradient_set,None,AutoDerivativeDouble)
AutoDerivativeDouble.__add__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___add__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__radd__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___radd__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__sub__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___sub__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__rsub__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___rsub__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__mul__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___mul__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__rmul__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___rmul__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__div__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___div__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__rdiv__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___rdiv__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__pow__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___pow__,None,AutoDerivativeDouble)
AutoDerivativeDouble.__rpow__ = new_instancemethod(_auto_derivative.AutoDerivativeDouble___rpow__,None,AutoDerivativeDouble)
AutoDerivativeDouble_swigregister = _auto_derivative.AutoDerivativeDouble_swigregister
AutoDerivativeDouble_swigregister(AutoDerivativeDouble)

class AutoDerivativeRefDouble(geocal_swig.generic_object.GenericObject):
    """
    Helper class that gives us a reference that we can assign a
    AutoDerivative to and write into the correct space in a ArrayAd.

    C++ includes: auto_derivative.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::AutoDerivativeRef< T >::AutoDerivativeRef(T &V)

        """
        _auto_derivative.AutoDerivativeRefDouble_swiginit(self,_auto_derivative.new_AutoDerivativeRefDouble(*args))
    def _v_value(self):
        """
        T GeoCal::AutoDerivativeRef< T >::value() const

        """
        return _auto_derivative.AutoDerivativeRefDouble__v_value(self)

    @property
    def value(self):
        return self._v_value()

    def _v_gradient(self):
        """
        const blitz::Array<T, 1>& GeoCal::AutoDerivativeRef< T >::gradient() const

        """
        return _auto_derivative.AutoDerivativeRefDouble__v_gradient(self)

    @property
    def gradient(self):
        return self._v_gradient()

    __swig_destroy__ = _auto_derivative.delete_AutoDerivativeRefDouble
AutoDerivativeRefDouble._v_value = new_instancemethod(_auto_derivative.AutoDerivativeRefDouble__v_value,None,AutoDerivativeRefDouble)
AutoDerivativeRefDouble._v_gradient = new_instancemethod(_auto_derivative.AutoDerivativeRefDouble__v_gradient,None,AutoDerivativeRefDouble)
AutoDerivativeRefDouble.__str__ = new_instancemethod(_auto_derivative.AutoDerivativeRefDouble___str__,None,AutoDerivativeRefDouble)
AutoDerivativeRefDouble_swigregister = _auto_derivative.AutoDerivativeRefDouble_swigregister
AutoDerivativeRefDouble_swigregister(AutoDerivativeRefDouble)

class ArrayAutoDerivativeDouble_1(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _auto_derivative.ArrayAutoDerivativeDouble_1_swiginit(self,_auto_derivative.new_ArrayAutoDerivativeDouble_1(*args))
    __swig_destroy__ = _auto_derivative.delete_ArrayAutoDerivativeDouble_1
ArrayAutoDerivativeDouble_1.data = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_data,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.size = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_size,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.read = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_read,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.write = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_write,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.datav = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_datav,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.shape0 = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_shape0,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.shape1 = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_shape1,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.shape2 = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_shape2,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1.shape3 = new_instancemethod(_auto_derivative.ArrayAutoDerivativeDouble_1_shape3,None,ArrayAutoDerivativeDouble_1)
ArrayAutoDerivativeDouble_1_swigregister = _auto_derivative.ArrayAutoDerivativeDouble_1_swigregister
ArrayAutoDerivativeDouble_1_swigregister(ArrayAutoDerivativeDouble_1)

class BoostArrayAutoDerivativeDouble_3(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    @property
    def size(self):
       return self._size()

    def set(self, v):
       '''Set the value to the given value. The value should be a container
       that we can enumerate over to fill in the value of the array'''
       for i, t in enumerate(v):
          if(i >= self.size):
              raise RuntimeError("Value must be exactly %d in size" % self.size)
          self[i] = t
       if(i != self.size - 1):
           raise RuntimeError("Value must be exactly %d in size" % self.size)

    def __iter__(self):
       for i in range(self.size):
           yield self[i]

    def __reduce__(self):
       return _new_from_set, (self.__class__, list(self))

    def __init__(self): 
        _auto_derivative.BoostArrayAutoDerivativeDouble_3_swiginit(self,_auto_derivative.new_BoostArrayAutoDerivativeDouble_3())
    __swig_destroy__ = _auto_derivative.delete_BoostArrayAutoDerivativeDouble_3
BoostArrayAutoDerivativeDouble_3._size = new_instancemethod(_auto_derivative.BoostArrayAutoDerivativeDouble_3__size,None,BoostArrayAutoDerivativeDouble_3)
BoostArrayAutoDerivativeDouble_3.__getitem__ = new_instancemethod(_auto_derivative.BoostArrayAutoDerivativeDouble_3___getitem__,None,BoostArrayAutoDerivativeDouble_3)
BoostArrayAutoDerivativeDouble_3.__setitem__ = new_instancemethod(_auto_derivative.BoostArrayAutoDerivativeDouble_3___setitem__,None,BoostArrayAutoDerivativeDouble_3)
BoostArrayAutoDerivativeDouble_3.__str__ = new_instancemethod(_auto_derivative.BoostArrayAutoDerivativeDouble_3___str__,None,BoostArrayAutoDerivativeDouble_3)
BoostArrayAutoDerivativeDouble_3_swigregister = _auto_derivative.BoostArrayAutoDerivativeDouble_3_swigregister
BoostArrayAutoDerivativeDouble_3_swigregister(BoostArrayAutoDerivativeDouble_3)


def sqrt(*args):
  """
    GeoCal::AutoDerivative< double > std::sqrt(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.sqrt(*args)

def log(*args):
  """
    GeoCal::AutoDerivative< double > std::log(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.log(*args)

def log10(*args):
  """
    GeoCal::AutoDerivative< double > std::log10(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.log10(*args)

def exp(*args):
  """
    GeoCal::AutoDerivative< double > std::exp(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.exp(*args)

def sin(*args):
  """
    GeoCal::AutoDerivative< double > std::sin(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.sin(*args)

def asin(*args):
  """
    GeoCal::AutoDerivative< double > std::asin(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.asin(*args)

def cos(*args):
  """
    GeoCal::AutoDerivative< double > std::cos(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.cos(*args)

def acos(*args):
  """
    GeoCal::AutoDerivative< double > std::acos(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.acos(*args)

def tan(*args):
  """
    GeoCal::AutoDerivative< double > std::tan(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.tan(*args)

def atan(*args):
  """
    GeoCal::AutoDerivative< double > std::atan(const GeoCal::AutoDerivative< double > &x)

    """
  return _auto_derivative.atan(*args)
class vector_auto_derivative(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _auto_derivative.vector_auto_derivative_swiginit(self,_auto_derivative.new_vector_auto_derivative(*args))
    __swig_destroy__ = _auto_derivative.delete_vector_auto_derivative
vector_auto_derivative.iterator = new_instancemethod(_auto_derivative.vector_auto_derivative_iterator,None,vector_auto_derivative)
vector_auto_derivative.__nonzero__ = new_instancemethod(_auto_derivative.vector_auto_derivative___nonzero__,None,vector_auto_derivative)
vector_auto_derivative.__bool__ = new_instancemethod(_auto_derivative.vector_auto_derivative___bool__,None,vector_auto_derivative)
vector_auto_derivative.__len__ = new_instancemethod(_auto_derivative.vector_auto_derivative___len__,None,vector_auto_derivative)
vector_auto_derivative.pop = new_instancemethod(_auto_derivative.vector_auto_derivative_pop,None,vector_auto_derivative)
vector_auto_derivative.__getslice__ = new_instancemethod(_auto_derivative.vector_auto_derivative___getslice__,None,vector_auto_derivative)
vector_auto_derivative.__setslice__ = new_instancemethod(_auto_derivative.vector_auto_derivative___setslice__,None,vector_auto_derivative)
vector_auto_derivative.__delslice__ = new_instancemethod(_auto_derivative.vector_auto_derivative___delslice__,None,vector_auto_derivative)
vector_auto_derivative.__delitem__ = new_instancemethod(_auto_derivative.vector_auto_derivative___delitem__,None,vector_auto_derivative)
vector_auto_derivative.__getitem__ = new_instancemethod(_auto_derivative.vector_auto_derivative___getitem__,None,vector_auto_derivative)
vector_auto_derivative.__setitem__ = new_instancemethod(_auto_derivative.vector_auto_derivative___setitem__,None,vector_auto_derivative)
vector_auto_derivative.append = new_instancemethod(_auto_derivative.vector_auto_derivative_append,None,vector_auto_derivative)
vector_auto_derivative.empty = new_instancemethod(_auto_derivative.vector_auto_derivative_empty,None,vector_auto_derivative)
vector_auto_derivative.size = new_instancemethod(_auto_derivative.vector_auto_derivative_size,None,vector_auto_derivative)
vector_auto_derivative.clear = new_instancemethod(_auto_derivative.vector_auto_derivative_clear,None,vector_auto_derivative)
vector_auto_derivative.swap = new_instancemethod(_auto_derivative.vector_auto_derivative_swap,None,vector_auto_derivative)
vector_auto_derivative.get_allocator = new_instancemethod(_auto_derivative.vector_auto_derivative_get_allocator,None,vector_auto_derivative)
vector_auto_derivative.begin = new_instancemethod(_auto_derivative.vector_auto_derivative_begin,None,vector_auto_derivative)
vector_auto_derivative.end = new_instancemethod(_auto_derivative.vector_auto_derivative_end,None,vector_auto_derivative)
vector_auto_derivative.rbegin = new_instancemethod(_auto_derivative.vector_auto_derivative_rbegin,None,vector_auto_derivative)
vector_auto_derivative.rend = new_instancemethod(_auto_derivative.vector_auto_derivative_rend,None,vector_auto_derivative)
vector_auto_derivative.pop_back = new_instancemethod(_auto_derivative.vector_auto_derivative_pop_back,None,vector_auto_derivative)
vector_auto_derivative.erase = new_instancemethod(_auto_derivative.vector_auto_derivative_erase,None,vector_auto_derivative)
vector_auto_derivative.push_back = new_instancemethod(_auto_derivative.vector_auto_derivative_push_back,None,vector_auto_derivative)
vector_auto_derivative.front = new_instancemethod(_auto_derivative.vector_auto_derivative_front,None,vector_auto_derivative)
vector_auto_derivative.back = new_instancemethod(_auto_derivative.vector_auto_derivative_back,None,vector_auto_derivative)
vector_auto_derivative.assign = new_instancemethod(_auto_derivative.vector_auto_derivative_assign,None,vector_auto_derivative)
vector_auto_derivative.resize = new_instancemethod(_auto_derivative.vector_auto_derivative_resize,None,vector_auto_derivative)
vector_auto_derivative.insert = new_instancemethod(_auto_derivative.vector_auto_derivative_insert,None,vector_auto_derivative)
vector_auto_derivative.reserve = new_instancemethod(_auto_derivative.vector_auto_derivative_reserve,None,vector_auto_derivative)
vector_auto_derivative.capacity = new_instancemethod(_auto_derivative.vector_auto_derivative_capacity,None,vector_auto_derivative)
vector_auto_derivative_swigregister = _auto_derivative.vector_auto_derivative_swigregister
vector_auto_derivative_swigregister(vector_auto_derivative)



