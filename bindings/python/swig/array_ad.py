# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _array_ad.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_array_ad', [dirname(__file__)])
        except ImportError:
            import _array_ad
            return _array_ad
        if fp is not None:
            try:
                _mod = imp.load_module('_array_ad', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _array_ad = swig_import_helper()
    del swig_import_helper
else:
    import _array_ad
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
    __swig_destroy__ = _array_ad.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_array_ad.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_array_ad.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_array_ad.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_array_ad.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_array_ad.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_array_ad.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_array_ad.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_array_ad.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_array_ad.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_array_ad.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_array_ad.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_array_ad.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_array_ad.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_array_ad.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_array_ad.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_array_ad.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _array_ad.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_array_ad.SHARED_PTR_DISOWN_swigconstant(_array_ad)
SHARED_PTR_DISOWN = _array_ad.SHARED_PTR_DISOWN

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

import numpy as np

def np_to_array_ad(a):
    '''Convert a numpy array of AutoDerivatives to a ArrayAd'''
    nvar = 0
    for i in a.flat:
        if(i.number_variable > 0):
            nvar = i.number_variable
            break
    if(len(a.shape) == 1):
        res = ArrayAd_double_1(a.shape[0], nvar)
        for i1 in range(res.rows):
            res[i1]  = a[i1]
    elif(len(a.shape) == 2):
        res = ArrayAd_double_2(a.shape[0], a.shape[1], nvar)
        for i1 in range(res.rows):
            for i2 in range(res.cols):
                res[i1,i2]  = a[i1, i2]
    elif(len(a.shape) == 3):
        res = ArrayAd_double_3(a.shape[0], a.shape[1], a.shape[2], nvar)
        for i1 in range(res.rows):
            for i2 in range(res.cols):
                for i3 in range(res.depth):
                    res[i1,i2, i3]  = a[i1, i2, i3]
    else:
        raise IndexError("np_to_array_ad only implemented for dimension <= 3")
    return res

class ArrayAd_double_1(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ArrayAd< T, D >::ArrayAd(const blitz::TinyVector< int, D > &Shape, int nvar)

        """
        _array_ad.ArrayAd_double_1_swiginit(self, _array_ad.new_ArrayAd_double_1(*args))

    def resize_number_variable(self, nvar):
        """

        void GeoCal::ArrayAd< T, D >::resize_number_variable(int nvar)

        """
        return _array_ad.ArrayAd_double_1_resize_number_variable(self, nvar)


    def resize(self, *args):
        """

        void GeoCal::ArrayAd< T, D >::resize(int n1, int n2, int n3, int n4, int n5, int nvar)

        """
        return _array_ad.ArrayAd_double_1_resize(self, *args)


    def _v_value(self):
        """

        blitz::Array<T, D>& GeoCal::ArrayAd< T, D >::value()

        """
        return _array_ad.ArrayAd_double_1__v_value(self)


    @property
    def value(self):
        return self._v_value()


    def _v_jacobian(self):
        """

        blitz::Array<T, D+1>& GeoCal::ArrayAd< T, D >::jacobian()

        """
        return _array_ad.ArrayAd_double_1__v_jacobian(self)


    @property
    def jacobian(self):
        return self._v_jacobian()


    def _v_rows(self):
        """

        int GeoCal::ArrayAd< T, D >::rows() const

        """
        return _array_ad.ArrayAd_double_1__v_rows(self)


    @property
    def rows(self):
        return self._v_rows()


    def _v_cols(self):
        """

        int GeoCal::ArrayAd< T, D >::cols() const

        """
        return _array_ad.ArrayAd_double_1__v_cols(self)


    @property
    def cols(self):
        return self._v_cols()


    def _v_depth(self):
        """

        int GeoCal::ArrayAd< T, D >::depth() const

        """
        return _array_ad.ArrayAd_double_1__v_depth(self)


    @property
    def depth(self):
        return self._v_depth()


    def _v_is_constant(self):
        """

        bool GeoCal::ArrayAd< T, D >::is_constant() const

        """
        return _array_ad.ArrayAd_double_1__v_is_constant(self)


    @property
    def is_constant(self):
        return self._v_is_constant()


    def _v_number_variable(self):
        """

        int GeoCal::ArrayAd< T, D >::number_variable() const

        """
        return _array_ad.ArrayAd_double_1__v_number_variable(self)


    @property
    def number_variable(self):
        return self._v_number_variable()


    def reference(self, V):
        """

        void GeoCal::ArrayAd< T, D >::reference(const ArrayAd< T, D > &V)

        """
        return _array_ad.ArrayAd_double_1_reference(self, V)


    def copy(self):
        """

        ArrayAd<T, D> GeoCal::ArrayAd< T, D >::copy() const

        """
        return _array_ad.ArrayAd_double_1_copy(self)


    def __array__(self):
        if(1 == 1):
            res = np.empty([self.rows], np.object)
            for i1 in range(self.rows):
                res[i1] = self[i1]
        elif(1 ==2):
            res = np.empty([self.rows, self.cols], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    res[i1,i2] = self[i1, i2]
        elif(1 ==3):
            res = np.empty([self.rows, self.cols, self.depth], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    for i3 in range(self.depth):
                        res[i1,i2, i3] = self[i1, i2, i3]
        else:
          raise IndexError("__array__ only implemented to dimensions <= 3")
        return res

    def slice_data(self, key):
      if not type(key) is tuple:
        key = (key,)

      ad_val = self.value[key]
      ad_jac = self.jacobian[key + (slice(None),)]

      num_dim = len(ad_val.shape)

      return eval("ArrayAd_double_%d" % num_dim)(ad_val, ad_jac)

    def __getitem__(self, index):
      if(1 == 1):
        if type(index) is slice:
          return self.slice_data(index)
        else:
          return self.read(index)
      else:
        if any(type(x) is slice for x in index):
          return self.slice_data(index)
        else:
          return self.read(*index)

    def __setitem__(self, index, val):
      if(1 == 1):
        if type(index) is slice:
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        self.write(index, val)
      else:
        if any(type(x) is slice for x in index):
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        t = list(index)
        t.append(val)
        self.write(*t)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _array_ad.delete_ArrayAd_double_1
ArrayAd_double_1.resize_number_variable = new_instancemethod(_array_ad.ArrayAd_double_1_resize_number_variable, None, ArrayAd_double_1)
ArrayAd_double_1.resize = new_instancemethod(_array_ad.ArrayAd_double_1_resize, None, ArrayAd_double_1)
ArrayAd_double_1._v_value = new_instancemethod(_array_ad.ArrayAd_double_1__v_value, None, ArrayAd_double_1)
ArrayAd_double_1._v_jacobian = new_instancemethod(_array_ad.ArrayAd_double_1__v_jacobian, None, ArrayAd_double_1)
ArrayAd_double_1._v_rows = new_instancemethod(_array_ad.ArrayAd_double_1__v_rows, None, ArrayAd_double_1)
ArrayAd_double_1._v_cols = new_instancemethod(_array_ad.ArrayAd_double_1__v_cols, None, ArrayAd_double_1)
ArrayAd_double_1._v_depth = new_instancemethod(_array_ad.ArrayAd_double_1__v_depth, None, ArrayAd_double_1)
ArrayAd_double_1._v_is_constant = new_instancemethod(_array_ad.ArrayAd_double_1__v_is_constant, None, ArrayAd_double_1)
ArrayAd_double_1._v_number_variable = new_instancemethod(_array_ad.ArrayAd_double_1__v_number_variable, None, ArrayAd_double_1)
ArrayAd_double_1.__str__ = new_instancemethod(_array_ad.ArrayAd_double_1___str__, None, ArrayAd_double_1)
ArrayAd_double_1.reference = new_instancemethod(_array_ad.ArrayAd_double_1_reference, None, ArrayAd_double_1)
ArrayAd_double_1.copy = new_instancemethod(_array_ad.ArrayAd_double_1_copy, None, ArrayAd_double_1)
ArrayAd_double_1.read = new_instancemethod(_array_ad.ArrayAd_double_1_read, None, ArrayAd_double_1)
ArrayAd_double_1.write = new_instancemethod(_array_ad.ArrayAd_double_1_write, None, ArrayAd_double_1)
ArrayAd_double_1_swigregister = _array_ad.ArrayAd_double_1_swigregister
ArrayAd_double_1_swigregister(ArrayAd_double_1)

class ArrayAd_double_2(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ArrayAd< T, D >::ArrayAd(const blitz::TinyVector< int, D > &Shape, int nvar)

        """
        _array_ad.ArrayAd_double_2_swiginit(self, _array_ad.new_ArrayAd_double_2(*args))

    def resize_number_variable(self, nvar):
        """

        void GeoCal::ArrayAd< T, D >::resize_number_variable(int nvar)

        """
        return _array_ad.ArrayAd_double_2_resize_number_variable(self, nvar)


    def resize(self, *args):
        """

        void GeoCal::ArrayAd< T, D >::resize(int n1, int n2, int n3, int n4, int n5, int nvar)

        """
        return _array_ad.ArrayAd_double_2_resize(self, *args)


    def _v_value(self):
        """

        blitz::Array<T, D>& GeoCal::ArrayAd< T, D >::value()

        """
        return _array_ad.ArrayAd_double_2__v_value(self)


    @property
    def value(self):
        return self._v_value()


    def _v_jacobian(self):
        """

        blitz::Array<T, D+1>& GeoCal::ArrayAd< T, D >::jacobian()

        """
        return _array_ad.ArrayAd_double_2__v_jacobian(self)


    @property
    def jacobian(self):
        return self._v_jacobian()


    def _v_rows(self):
        """

        int GeoCal::ArrayAd< T, D >::rows() const

        """
        return _array_ad.ArrayAd_double_2__v_rows(self)


    @property
    def rows(self):
        return self._v_rows()


    def _v_cols(self):
        """

        int GeoCal::ArrayAd< T, D >::cols() const

        """
        return _array_ad.ArrayAd_double_2__v_cols(self)


    @property
    def cols(self):
        return self._v_cols()


    def _v_depth(self):
        """

        int GeoCal::ArrayAd< T, D >::depth() const

        """
        return _array_ad.ArrayAd_double_2__v_depth(self)


    @property
    def depth(self):
        return self._v_depth()


    def _v_is_constant(self):
        """

        bool GeoCal::ArrayAd< T, D >::is_constant() const

        """
        return _array_ad.ArrayAd_double_2__v_is_constant(self)


    @property
    def is_constant(self):
        return self._v_is_constant()


    def _v_number_variable(self):
        """

        int GeoCal::ArrayAd< T, D >::number_variable() const

        """
        return _array_ad.ArrayAd_double_2__v_number_variable(self)


    @property
    def number_variable(self):
        return self._v_number_variable()


    def reference(self, V):
        """

        void GeoCal::ArrayAd< T, D >::reference(const ArrayAd< T, D > &V)

        """
        return _array_ad.ArrayAd_double_2_reference(self, V)


    def copy(self):
        """

        ArrayAd<T, D> GeoCal::ArrayAd< T, D >::copy() const

        """
        return _array_ad.ArrayAd_double_2_copy(self)


    def __array__(self):
        if(2 == 1):
            res = np.empty([self.rows], np.object)
            for i1 in range(self.rows):
                res[i1] = self[i1]
        elif(2 ==2):
            res = np.empty([self.rows, self.cols], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    res[i1,i2] = self[i1, i2]
        elif(2 ==3):
            res = np.empty([self.rows, self.cols, self.depth], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    for i3 in range(self.depth):
                        res[i1,i2, i3] = self[i1, i2, i3]
        else:
          raise IndexError("__array__ only implemented to dimensions <= 3")
        return res

    def slice_data(self, key):
      if not type(key) is tuple:
        key = (key,)

      ad_val = self.value[key]
      ad_jac = self.jacobian[key + (slice(None),)]

      num_dim = len(ad_val.shape)

      return eval("ArrayAd_double_%d" % num_dim)(ad_val, ad_jac)

    def __getitem__(self, index):
      if(2 == 1):
        if type(index) is slice:
          return self.slice_data(index)
        else:
          return self.read(index)
      else:
        if any(type(x) is slice for x in index):
          return self.slice_data(index)
        else:
          return self.read(*index)

    def __setitem__(self, index, val):
      if(2 == 1):
        if type(index) is slice:
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        self.write(index, val)
      else:
        if any(type(x) is slice for x in index):
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        t = list(index)
        t.append(val)
        self.write(*t)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _array_ad.delete_ArrayAd_double_2
ArrayAd_double_2.resize_number_variable = new_instancemethod(_array_ad.ArrayAd_double_2_resize_number_variable, None, ArrayAd_double_2)
ArrayAd_double_2.resize = new_instancemethod(_array_ad.ArrayAd_double_2_resize, None, ArrayAd_double_2)
ArrayAd_double_2._v_value = new_instancemethod(_array_ad.ArrayAd_double_2__v_value, None, ArrayAd_double_2)
ArrayAd_double_2._v_jacobian = new_instancemethod(_array_ad.ArrayAd_double_2__v_jacobian, None, ArrayAd_double_2)
ArrayAd_double_2._v_rows = new_instancemethod(_array_ad.ArrayAd_double_2__v_rows, None, ArrayAd_double_2)
ArrayAd_double_2._v_cols = new_instancemethod(_array_ad.ArrayAd_double_2__v_cols, None, ArrayAd_double_2)
ArrayAd_double_2._v_depth = new_instancemethod(_array_ad.ArrayAd_double_2__v_depth, None, ArrayAd_double_2)
ArrayAd_double_2._v_is_constant = new_instancemethod(_array_ad.ArrayAd_double_2__v_is_constant, None, ArrayAd_double_2)
ArrayAd_double_2._v_number_variable = new_instancemethod(_array_ad.ArrayAd_double_2__v_number_variable, None, ArrayAd_double_2)
ArrayAd_double_2.__str__ = new_instancemethod(_array_ad.ArrayAd_double_2___str__, None, ArrayAd_double_2)
ArrayAd_double_2.reference = new_instancemethod(_array_ad.ArrayAd_double_2_reference, None, ArrayAd_double_2)
ArrayAd_double_2.copy = new_instancemethod(_array_ad.ArrayAd_double_2_copy, None, ArrayAd_double_2)
ArrayAd_double_2.read = new_instancemethod(_array_ad.ArrayAd_double_2_read, None, ArrayAd_double_2)
ArrayAd_double_2.write = new_instancemethod(_array_ad.ArrayAd_double_2_write, None, ArrayAd_double_2)
ArrayAd_double_2_swigregister = _array_ad.ArrayAd_double_2_swigregister
ArrayAd_double_2_swigregister(ArrayAd_double_2)

class ArrayAd_double_3(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ArrayAd< T, D >::ArrayAd(const blitz::TinyVector< int, D > &Shape, int nvar)

        """
        _array_ad.ArrayAd_double_3_swiginit(self, _array_ad.new_ArrayAd_double_3(*args))

    def resize_number_variable(self, nvar):
        """

        void GeoCal::ArrayAd< T, D >::resize_number_variable(int nvar)

        """
        return _array_ad.ArrayAd_double_3_resize_number_variable(self, nvar)


    def resize(self, *args):
        """

        void GeoCal::ArrayAd< T, D >::resize(int n1, int n2, int n3, int n4, int n5, int nvar)

        """
        return _array_ad.ArrayAd_double_3_resize(self, *args)


    def _v_value(self):
        """

        blitz::Array<T, D>& GeoCal::ArrayAd< T, D >::value()

        """
        return _array_ad.ArrayAd_double_3__v_value(self)


    @property
    def value(self):
        return self._v_value()


    def _v_jacobian(self):
        """

        blitz::Array<T, D+1>& GeoCal::ArrayAd< T, D >::jacobian()

        """
        return _array_ad.ArrayAd_double_3__v_jacobian(self)


    @property
    def jacobian(self):
        return self._v_jacobian()


    def _v_rows(self):
        """

        int GeoCal::ArrayAd< T, D >::rows() const

        """
        return _array_ad.ArrayAd_double_3__v_rows(self)


    @property
    def rows(self):
        return self._v_rows()


    def _v_cols(self):
        """

        int GeoCal::ArrayAd< T, D >::cols() const

        """
        return _array_ad.ArrayAd_double_3__v_cols(self)


    @property
    def cols(self):
        return self._v_cols()


    def _v_depth(self):
        """

        int GeoCal::ArrayAd< T, D >::depth() const

        """
        return _array_ad.ArrayAd_double_3__v_depth(self)


    @property
    def depth(self):
        return self._v_depth()


    def _v_is_constant(self):
        """

        bool GeoCal::ArrayAd< T, D >::is_constant() const

        """
        return _array_ad.ArrayAd_double_3__v_is_constant(self)


    @property
    def is_constant(self):
        return self._v_is_constant()


    def _v_number_variable(self):
        """

        int GeoCal::ArrayAd< T, D >::number_variable() const

        """
        return _array_ad.ArrayAd_double_3__v_number_variable(self)


    @property
    def number_variable(self):
        return self._v_number_variable()


    def reference(self, V):
        """

        void GeoCal::ArrayAd< T, D >::reference(const ArrayAd< T, D > &V)

        """
        return _array_ad.ArrayAd_double_3_reference(self, V)


    def copy(self):
        """

        ArrayAd<T, D> GeoCal::ArrayAd< T, D >::copy() const

        """
        return _array_ad.ArrayAd_double_3_copy(self)


    def __array__(self):
        if(3 == 1):
            res = np.empty([self.rows], np.object)
            for i1 in range(self.rows):
                res[i1] = self[i1]
        elif(3 ==2):
            res = np.empty([self.rows, self.cols], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    res[i1,i2] = self[i1, i2]
        elif(3 ==3):
            res = np.empty([self.rows, self.cols, self.depth], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    for i3 in range(self.depth):
                        res[i1,i2, i3] = self[i1, i2, i3]
        else:
          raise IndexError("__array__ only implemented to dimensions <= 3")
        return res

    def slice_data(self, key):
      if not type(key) is tuple:
        key = (key,)

      ad_val = self.value[key]
      ad_jac = self.jacobian[key + (slice(None),)]

      num_dim = len(ad_val.shape)

      return eval("ArrayAd_double_%d" % num_dim)(ad_val, ad_jac)

    def __getitem__(self, index):
      if(3 == 1):
        if type(index) is slice:
          return self.slice_data(index)
        else:
          return self.read(index)
      else:
        if any(type(x) is slice for x in index):
          return self.slice_data(index)
        else:
          return self.read(*index)

    def __setitem__(self, index, val):
      if(3 == 1):
        if type(index) is slice:
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        self.write(index, val)
      else:
        if any(type(x) is slice for x in index):
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        t = list(index)
        t.append(val)
        self.write(*t)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _array_ad.delete_ArrayAd_double_3
ArrayAd_double_3.resize_number_variable = new_instancemethod(_array_ad.ArrayAd_double_3_resize_number_variable, None, ArrayAd_double_3)
ArrayAd_double_3.resize = new_instancemethod(_array_ad.ArrayAd_double_3_resize, None, ArrayAd_double_3)
ArrayAd_double_3._v_value = new_instancemethod(_array_ad.ArrayAd_double_3__v_value, None, ArrayAd_double_3)
ArrayAd_double_3._v_jacobian = new_instancemethod(_array_ad.ArrayAd_double_3__v_jacobian, None, ArrayAd_double_3)
ArrayAd_double_3._v_rows = new_instancemethod(_array_ad.ArrayAd_double_3__v_rows, None, ArrayAd_double_3)
ArrayAd_double_3._v_cols = new_instancemethod(_array_ad.ArrayAd_double_3__v_cols, None, ArrayAd_double_3)
ArrayAd_double_3._v_depth = new_instancemethod(_array_ad.ArrayAd_double_3__v_depth, None, ArrayAd_double_3)
ArrayAd_double_3._v_is_constant = new_instancemethod(_array_ad.ArrayAd_double_3__v_is_constant, None, ArrayAd_double_3)
ArrayAd_double_3._v_number_variable = new_instancemethod(_array_ad.ArrayAd_double_3__v_number_variable, None, ArrayAd_double_3)
ArrayAd_double_3.__str__ = new_instancemethod(_array_ad.ArrayAd_double_3___str__, None, ArrayAd_double_3)
ArrayAd_double_3.reference = new_instancemethod(_array_ad.ArrayAd_double_3_reference, None, ArrayAd_double_3)
ArrayAd_double_3.copy = new_instancemethod(_array_ad.ArrayAd_double_3_copy, None, ArrayAd_double_3)
ArrayAd_double_3.read = new_instancemethod(_array_ad.ArrayAd_double_3_read, None, ArrayAd_double_3)
ArrayAd_double_3.write = new_instancemethod(_array_ad.ArrayAd_double_3_write, None, ArrayAd_double_3)
ArrayAd_double_3_swigregister = _array_ad.ArrayAd_double_3_swigregister
ArrayAd_double_3_swigregister(ArrayAd_double_3)

class ArrayAd_double_4(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ArrayAd< T, D >::ArrayAd(const blitz::TinyVector< int, D > &Shape, int nvar)

        """
        _array_ad.ArrayAd_double_4_swiginit(self, _array_ad.new_ArrayAd_double_4(*args))

    def resize_number_variable(self, nvar):
        """

        void GeoCal::ArrayAd< T, D >::resize_number_variable(int nvar)

        """
        return _array_ad.ArrayAd_double_4_resize_number_variable(self, nvar)


    def resize(self, *args):
        """

        void GeoCal::ArrayAd< T, D >::resize(int n1, int n2, int n3, int n4, int n5, int nvar)

        """
        return _array_ad.ArrayAd_double_4_resize(self, *args)


    def _v_value(self):
        """

        blitz::Array<T, D>& GeoCal::ArrayAd< T, D >::value()

        """
        return _array_ad.ArrayAd_double_4__v_value(self)


    @property
    def value(self):
        return self._v_value()


    def _v_jacobian(self):
        """

        blitz::Array<T, D+1>& GeoCal::ArrayAd< T, D >::jacobian()

        """
        return _array_ad.ArrayAd_double_4__v_jacobian(self)


    @property
    def jacobian(self):
        return self._v_jacobian()


    def _v_rows(self):
        """

        int GeoCal::ArrayAd< T, D >::rows() const

        """
        return _array_ad.ArrayAd_double_4__v_rows(self)


    @property
    def rows(self):
        return self._v_rows()


    def _v_cols(self):
        """

        int GeoCal::ArrayAd< T, D >::cols() const

        """
        return _array_ad.ArrayAd_double_4__v_cols(self)


    @property
    def cols(self):
        return self._v_cols()


    def _v_depth(self):
        """

        int GeoCal::ArrayAd< T, D >::depth() const

        """
        return _array_ad.ArrayAd_double_4__v_depth(self)


    @property
    def depth(self):
        return self._v_depth()


    def _v_is_constant(self):
        """

        bool GeoCal::ArrayAd< T, D >::is_constant() const

        """
        return _array_ad.ArrayAd_double_4__v_is_constant(self)


    @property
    def is_constant(self):
        return self._v_is_constant()


    def _v_number_variable(self):
        """

        int GeoCal::ArrayAd< T, D >::number_variable() const

        """
        return _array_ad.ArrayAd_double_4__v_number_variable(self)


    @property
    def number_variable(self):
        return self._v_number_variable()


    def reference(self, V):
        """

        void GeoCal::ArrayAd< T, D >::reference(const ArrayAd< T, D > &V)

        """
        return _array_ad.ArrayAd_double_4_reference(self, V)


    def copy(self):
        """

        ArrayAd<T, D> GeoCal::ArrayAd< T, D >::copy() const

        """
        return _array_ad.ArrayAd_double_4_copy(self)


    def __array__(self):
        if(4 == 1):
            res = np.empty([self.rows], np.object)
            for i1 in range(self.rows):
                res[i1] = self[i1]
        elif(4 ==2):
            res = np.empty([self.rows, self.cols], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    res[i1,i2] = self[i1, i2]
        elif(4 ==3):
            res = np.empty([self.rows, self.cols, self.depth], np.object)
            for i1 in range(self.rows):
                for i2 in range(self.cols):
                    for i3 in range(self.depth):
                        res[i1,i2, i3] = self[i1, i2, i3]
        else:
          raise IndexError("__array__ only implemented to dimensions <= 3")
        return res

    def slice_data(self, key):
      if not type(key) is tuple:
        key = (key,)

      ad_val = self.value[key]
      ad_jac = self.jacobian[key + (slice(None),)]

      num_dim = len(ad_val.shape)

      return eval("ArrayAd_double_%d" % num_dim)(ad_val, ad_jac)

    def __getitem__(self, index):
      if(4 == 1):
        if type(index) is slice:
          return self.slice_data(index)
        else:
          return self.read(index)
      else:
        if any(type(x) is slice for x in index):
          return self.slice_data(index)
        else:
          return self.read(*index)

    def __setitem__(self, index, val):
      if(4 == 1):
        if type(index) is slice:
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        self.write(index, val)
      else:
        if any(type(x) is slice for x in index):
          raise NotImplementedError("__setitem__ can not be used for setting values to slices")
        t = list(index)
        t.append(val)
        self.write(*t)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _array_ad.delete_ArrayAd_double_4
ArrayAd_double_4.resize_number_variable = new_instancemethod(_array_ad.ArrayAd_double_4_resize_number_variable, None, ArrayAd_double_4)
ArrayAd_double_4.resize = new_instancemethod(_array_ad.ArrayAd_double_4_resize, None, ArrayAd_double_4)
ArrayAd_double_4._v_value = new_instancemethod(_array_ad.ArrayAd_double_4__v_value, None, ArrayAd_double_4)
ArrayAd_double_4._v_jacobian = new_instancemethod(_array_ad.ArrayAd_double_4__v_jacobian, None, ArrayAd_double_4)
ArrayAd_double_4._v_rows = new_instancemethod(_array_ad.ArrayAd_double_4__v_rows, None, ArrayAd_double_4)
ArrayAd_double_4._v_cols = new_instancemethod(_array_ad.ArrayAd_double_4__v_cols, None, ArrayAd_double_4)
ArrayAd_double_4._v_depth = new_instancemethod(_array_ad.ArrayAd_double_4__v_depth, None, ArrayAd_double_4)
ArrayAd_double_4._v_is_constant = new_instancemethod(_array_ad.ArrayAd_double_4__v_is_constant, None, ArrayAd_double_4)
ArrayAd_double_4._v_number_variable = new_instancemethod(_array_ad.ArrayAd_double_4__v_number_variable, None, ArrayAd_double_4)
ArrayAd_double_4.__str__ = new_instancemethod(_array_ad.ArrayAd_double_4___str__, None, ArrayAd_double_4)
ArrayAd_double_4.reference = new_instancemethod(_array_ad.ArrayAd_double_4_reference, None, ArrayAd_double_4)
ArrayAd_double_4.copy = new_instancemethod(_array_ad.ArrayAd_double_4_copy, None, ArrayAd_double_4)
ArrayAd_double_4.read = new_instancemethod(_array_ad.ArrayAd_double_4_read, None, ArrayAd_double_4)
ArrayAd_double_4.write = new_instancemethod(_array_ad.ArrayAd_double_4_write, None, ArrayAd_double_4)
ArrayAd_double_4_swigregister = _array_ad.ArrayAd_double_4_swigregister
ArrayAd_double_4_swigregister(ArrayAd_double_4)


__all__ = ["ArrayAd_double_1","ArrayAd_double_2","ArrayAd_double_3","ArrayAd_double_4"]



