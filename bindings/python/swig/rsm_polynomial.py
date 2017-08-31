# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_polynomial.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_polynomial', [dirname(__file__)])
        except ImportError:
            import _rsm_polynomial
            return _rsm_polynomial
        if fp is not None:
            try:
                _mod = imp.load_module('_rsm_polynomial', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _rsm_polynomial = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_polynomial
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
    __swig_destroy__ = _rsm_polynomial.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_polynomial.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_polynomial.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_polynomial.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_polynomial.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_polynomial.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_polynomial.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_polynomial.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_polynomial.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_polynomial.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_polynomial.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_polynomial.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_rsm_polynomial.SHARED_PTR_DISOWN_swigconstant(_rsm_polynomial)
SHARED_PTR_DISOWN = _rsm_polynomial.SHARED_PTR_DISOWN

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
class RsmPolynomial(geocal_swig.generic_object.GenericObject):
    """

    This is used to handle a single polynomial for use with a
    RsmRationalPolynomial.

    This is a low level class, you don't normally use this directly
    (although you certainly could if it was useful).

    Note we might want to consider making this a template based on the
    sizes so we can specialize/optimize the code. We'll see what the
    performance is like.

    C++ includes: rsm_polynomial.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Np_x, Np_y, Np_z, Is_denominator=False, Max_order=-1):
        """

        RsmPolynomial::RsmPolynomial(int Np_x, int Np_y, int Np_z, bool Is_denominator=false, int
        Max_order=-1)
        Constructor.

        You indicated the order of the polynomial in each dimension, and if
        this is a denominator (where we hold the constant term to 1.0 by
        convention, and don't fit for this). The maximum order of cross terms
        can be given, or left as -1 in which case we don't limit the cross
        terms. 
        """
        _rsm_polynomial.RsmPolynomial_swiginit(self, _rsm_polynomial.new_RsmPolynomial(Np_x, Np_y, Np_z, Is_denominator, Max_order))

    def jacobian(self, X, Y, Z):
        """

        blitz::Array< double, 2 > RsmPolynomial::jacobian(const blitz::Array< double, 1 > &X, const blitz::Array< double, 1 >
        &Y, const blitz::Array< double, 1 > &Z) const

        """
        return _rsm_polynomial.RsmPolynomial_jacobian(self, X, Y, Z)


    def _v_coefficient(self):
        """

        const blitz::Array<double, 3>& GeoCal::RsmPolynomial::coefficient() const
        The full set of coefficients for the polynomial. 
        """
        return _rsm_polynomial.RsmPolynomial__v_coefficient(self)


    @property
    def coefficient(self):
        return self._v_coefficient()


    def _v_fitted_coefficent(self, *args):
        """

        void RsmPolynomial::fitted_coefficent(const blitz::Array< double, 1 > &Fc)
        Set the value of the set of coefficients that we are fitting for. 
        """
        return _rsm_polynomial.RsmPolynomial__v_fitted_coefficent(self, *args)


    @property
    def fitted_coefficent(self):
        return self._v_fitted_coefficent()

    @fitted_coefficent.setter
    def fitted_coefficent(self, value):
      self._v_fitted_coefficent(value)


    def _v_is_denominator(self):
        """

        bool GeoCal::RsmPolynomial::is_denominator() const
        True if this is a denominator polynomial.

        We hold the constant term at 1.0 by convention, and don't fit for
        this. 
        """
        return _rsm_polynomial.RsmPolynomial__v_is_denominator(self)


    @property
    def is_denominator(self):
        return self._v_is_denominator()


    def _v_max_order(self):
        """

        int GeoCal::RsmPolynomial::max_order() const
        Maximum order cross term we fit for, or -1 if we don't restrict this.

        """
        return _rsm_polynomial.RsmPolynomial__v_max_order(self)


    @property
    def max_order(self):
        return self._v_max_order()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_polynomial.delete_RsmPolynomial
RsmPolynomial.__str__ = new_instancemethod(_rsm_polynomial.RsmPolynomial___str__, None, RsmPolynomial)
RsmPolynomial.__call__ = new_instancemethod(_rsm_polynomial.RsmPolynomial___call__, None, RsmPolynomial)
RsmPolynomial.jacobian = new_instancemethod(_rsm_polynomial.RsmPolynomial_jacobian, None, RsmPolynomial)
RsmPolynomial._v_coefficient = new_instancemethod(_rsm_polynomial.RsmPolynomial__v_coefficient, None, RsmPolynomial)
RsmPolynomial._v_fitted_coefficent = new_instancemethod(_rsm_polynomial.RsmPolynomial__v_fitted_coefficent, None, RsmPolynomial)
RsmPolynomial._v_is_denominator = new_instancemethod(_rsm_polynomial.RsmPolynomial__v_is_denominator, None, RsmPolynomial)
RsmPolynomial._v_max_order = new_instancemethod(_rsm_polynomial.RsmPolynomial__v_max_order, None, RsmPolynomial)
RsmPolynomial_swigregister = _rsm_polynomial.RsmPolynomial_swigregister
RsmPolynomial_swigregister(RsmPolynomial)



