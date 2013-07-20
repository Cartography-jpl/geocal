# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _swig_boost_array.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_swig_boost_array', [dirname(__file__)])
        except ImportError:
            import _swig_boost_array
            return _swig_boost_array
        if fp is not None:
            try:
                _mod = imp.load_module('_swig_boost_array', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _swig_boost_array = swig_import_helper()
    del swig_import_helper
else:
    import _swig_boost_array
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


class Array_double_20(object):
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
        _swig_boost_array.Array_double_20_swiginit(self,_swig_boost_array.new_Array_double_20())
    __swig_destroy__ = _swig_boost_array.delete_Array_double_20
Array_double_20._size = new_instancemethod(_swig_boost_array.Array_double_20__size,None,Array_double_20)
Array_double_20.__getitem__ = new_instancemethod(_swig_boost_array.Array_double_20___getitem__,None,Array_double_20)
Array_double_20.__setitem__ = new_instancemethod(_swig_boost_array.Array_double_20___setitem__,None,Array_double_20)
Array_double_20.print_to_string = new_instancemethod(_swig_boost_array.Array_double_20_print_to_string,None,Array_double_20)
Array_double_20_swigregister = _swig_boost_array.Array_double_20_swigregister
Array_double_20_swigregister(Array_double_20)

class Array_double_12(object):
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
        _swig_boost_array.Array_double_12_swiginit(self,_swig_boost_array.new_Array_double_12())
    __swig_destroy__ = _swig_boost_array.delete_Array_double_12
Array_double_12._size = new_instancemethod(_swig_boost_array.Array_double_12__size,None,Array_double_12)
Array_double_12.__getitem__ = new_instancemethod(_swig_boost_array.Array_double_12___getitem__,None,Array_double_12)
Array_double_12.__setitem__ = new_instancemethod(_swig_boost_array.Array_double_12___setitem__,None,Array_double_12)
Array_double_12.print_to_string = new_instancemethod(_swig_boost_array.Array_double_12_print_to_string,None,Array_double_12)
Array_double_12_swigregister = _swig_boost_array.Array_double_12_swigregister
Array_double_12_swigregister(Array_double_12)

class Array_double_14(object):
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
        _swig_boost_array.Array_double_14_swiginit(self,_swig_boost_array.new_Array_double_14())
    __swig_destroy__ = _swig_boost_array.delete_Array_double_14
Array_double_14._size = new_instancemethod(_swig_boost_array.Array_double_14__size,None,Array_double_14)
Array_double_14.__getitem__ = new_instancemethod(_swig_boost_array.Array_double_14___getitem__,None,Array_double_14)
Array_double_14.__setitem__ = new_instancemethod(_swig_boost_array.Array_double_14___setitem__,None,Array_double_14)
Array_double_14.print_to_string = new_instancemethod(_swig_boost_array.Array_double_14_print_to_string,None,Array_double_14)
Array_double_14_swigregister = _swig_boost_array.Array_double_14_swigregister
Array_double_14_swigregister(Array_double_14)

class Array_double_3(object):
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
        _swig_boost_array.Array_double_3_swiginit(self,_swig_boost_array.new_Array_double_3())
    __swig_destroy__ = _swig_boost_array.delete_Array_double_3
Array_double_3._size = new_instancemethod(_swig_boost_array.Array_double_3__size,None,Array_double_3)
Array_double_3.__getitem__ = new_instancemethod(_swig_boost_array.Array_double_3___getitem__,None,Array_double_3)
Array_double_3.__setitem__ = new_instancemethod(_swig_boost_array.Array_double_3___setitem__,None,Array_double_3)
Array_double_3.print_to_string = new_instancemethod(_swig_boost_array.Array_double_3_print_to_string,None,Array_double_3)
Array_double_3_swigregister = _swig_boost_array.Array_double_3_swigregister
Array_double_3_swigregister(Array_double_3)

class Array_bool_20(object):
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
        _swig_boost_array.Array_bool_20_swiginit(self,_swig_boost_array.new_Array_bool_20())
    __swig_destroy__ = _swig_boost_array.delete_Array_bool_20
Array_bool_20._size = new_instancemethod(_swig_boost_array.Array_bool_20__size,None,Array_bool_20)
Array_bool_20.__getitem__ = new_instancemethod(_swig_boost_array.Array_bool_20___getitem__,None,Array_bool_20)
Array_bool_20.__setitem__ = new_instancemethod(_swig_boost_array.Array_bool_20___setitem__,None,Array_bool_20)
Array_bool_20.print_to_string = new_instancemethod(_swig_boost_array.Array_bool_20_print_to_string,None,Array_bool_20)
Array_bool_20_swigregister = _swig_boost_array.Array_bool_20_swigregister
Array_bool_20_swigregister(Array_bool_20)



