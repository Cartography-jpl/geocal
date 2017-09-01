# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_rational_polynomial.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_rational_polynomial', [dirname(__file__)])
        except ImportError:
            import _rsm_rational_polynomial
            return _rsm_rational_polynomial
        if fp is not None:
            try:
                _mod = imp.load_module('_rsm_rational_polynomial', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _rsm_rational_polynomial = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_rational_polynomial
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
    __swig_destroy__ = _rsm_rational_polynomial.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_rational_polynomial.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_rational_polynomial.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_rsm_rational_polynomial.SHARED_PTR_DISOWN_swigconstant(_rsm_rational_polynomial)
SHARED_PTR_DISOWN = _rsm_rational_polynomial.SHARED_PTR_DISOWN

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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class RsmRationalPolynomial(geocal_swig.generic_object.GenericObject):
    """

    This is a RsmRationalPolynomial.

    C++ includes: rsm_rational_polynomial.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Np_x, Np_y, Np_z, Dp_x, Dp_y, Dp_z, N_max_order=-1, D_max_order=-1):
        """

        RsmRationalPolynomial::RsmRationalPolynomial(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y, int Dp_z, int
        N_max_order=-1, int D_max_order=-1)
        Constructor.

        You indicated the order of the polynomial in each dimension for both
        the numerator and denominator. This particular constructor uses the
        same dimensions for both the line and the sample, there is no actual
        requirement in a Rsm that this be the case (although it is pretty
        common).

        You can limit the over all order of cross terms by supplying a
        max_order for the numerator, denominator, or both. This holds all
        cross terms with a total order > max to 0. The default is to not
        restrict the cross terms. 
        """
        _rsm_rational_polynomial.RsmRationalPolynomial_swiginit(self, _rsm_rational_polynomial.new_RsmRationalPolynomial(Np_x, Np_y, Np_z, Dp_x, Dp_y, Dp_z, N_max_order, D_max_order))

    def image_coordinate(self, *args):
        """

        blitz::Array< double, 4 > RsmRationalPolynomial::image_coordinate(const blitz::Array< double, 3 > &X, const blitz::Array< double, 3 >
        &Y, const blitz::Array< double, 3 > &Z) const
        Apply the rational polynomial to the given X, Y, and Z value.

        This converts a whole array of points at once. The results have the
        first dimension of size 2, the first value is the line and the second
        the sample. 
        """
        return _rsm_rational_polynomial.RsmRationalPolynomial_image_coordinate(self, *args)


    def set_rpc_coeff(self, R):
        """

        void RsmRationalPolynomial::set_rpc_coeff(const Rpc &R)
        Set a RsmPolynomial to match from a Rpc.

        This is really only useful for testing, comparing against our RPC code

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial_set_rpc_coeff(self, R)


    def fit_offset_and_scale(self, Min_line, Max_line, Min_sample, Max_sample, Min_x, Max_x, Min_y, Max_y, Min_z, Max_z):
        """

        void RsmRationalPolynomial::fit_offset_and_scale(double Min_line, double Max_line, double Min_sample, double
        Max_sample, double Min_x, double Max_x, double Min_y, double Max_y,
        double Min_z, double Max_z)
        Find the scale and offset values to cover the given range of data. 
        """
        return _rsm_rational_polynomial.RsmRationalPolynomial_fit_offset_and_scale(self, Min_line, Max_line, Min_sample, Max_sample, Min_x, Max_x, Min_y, Max_y, Min_z, Max_z)


    def fit(self, Line, Sample, X, Y, Z):
        """

        void RsmRationalPolynomial::fit(const std::vector< double > &Line, const std::vector< double >
        &Sample, const std::vector< double > &X, const std::vector< double >
        &Y, const std::vector< double > &Z)
        Adjust the parameters of line and sample numerator and denominator to
        fit the given data.

        Note that this uses the existing scale and offset values, so you
        should make sure to set that first (e.g., call fit_offset_and_scale).

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial_fit(self, Line, Sample, X, Y, Z)


    def image_coordinate_jacobian(self, X, Y, Z):
        """

        blitz::Array< double, 2 > RsmRationalPolynomial::image_coordinate_jacobian(double X, double Y, double Z) const
        This the jacobian of the line, sample with respect to X, Y, Z.

        This is a 2x3 matrix. 
        """
        return _rsm_rational_polynomial.RsmRationalPolynomial_image_coordinate_jacobian(self, X, Y, Z)


    def _v_line_offset(self):
        """

        double GeoCal::RsmRationalPolynomial::line_offset() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_line_offset(self)


    @property
    def line_offset(self):
        return self._v_line_offset()


    def _v_line_scale(self):
        """

        double GeoCal::RsmRationalPolynomial::line_scale() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_line_scale(self)


    @property
    def line_scale(self):
        return self._v_line_scale()


    def _v_sample_offset(self):
        """

        double GeoCal::RsmRationalPolynomial::sample_offset() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_sample_offset(self)


    @property
    def sample_offset(self):
        return self._v_sample_offset()


    def _v_sample_scale(self):
        """

        double GeoCal::RsmRationalPolynomial::sample_scale() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_sample_scale(self)


    @property
    def sample_scale(self):
        return self._v_sample_scale()


    def _v_x_offset(self):
        """

        double GeoCal::RsmRationalPolynomial::x_offset() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_x_offset(self)


    @property
    def x_offset(self):
        return self._v_x_offset()


    def _v_x_scale(self):
        """

        double GeoCal::RsmRationalPolynomial::x_scale() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_x_scale(self)


    @property
    def x_scale(self):
        return self._v_x_scale()


    def _v_y_offset(self):
        """

        double GeoCal::RsmRationalPolynomial::y_offset() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_y_offset(self)


    @property
    def y_offset(self):
        return self._v_y_offset()


    def _v_y_scale(self):
        """

        double GeoCal::RsmRationalPolynomial::y_scale() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_y_scale(self)


    @property
    def y_scale(self):
        return self._v_y_scale()


    def _v_z_offset(self):
        """

        double GeoCal::RsmRationalPolynomial::z_offset() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_z_offset(self)


    @property
    def z_offset(self):
        return self._v_z_offset()


    def _v_z_scale(self):
        """

        double GeoCal::RsmRationalPolynomial::z_scale() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_z_scale(self)


    @property
    def z_scale(self):
        return self._v_z_scale()


    def _v_line_numerator(self):
        """

        const RsmPolynomial& GeoCal::RsmRationalPolynomial::line_numerator() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_line_numerator(self)


    @property
    def line_numerator(self):
        return self._v_line_numerator()


    def _v_line_denominator(self):
        """

        const RsmPolynomial& GeoCal::RsmRationalPolynomial::line_denominator() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_line_denominator(self)


    @property
    def line_denominator(self):
        return self._v_line_denominator()


    def _v_sample_numerator(self):
        """

        const RsmPolynomial& GeoCal::RsmRationalPolynomial::sample_numerator() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_sample_numerator(self)


    @property
    def sample_numerator(self):
        return self._v_sample_numerator()


    def _v_sample_denominator(self):
        """

        const RsmPolynomial& GeoCal::RsmRationalPolynomial::sample_denominator() const

        """
        return _rsm_rational_polynomial.RsmRationalPolynomial__v_sample_denominator(self)


    @property
    def sample_denominator(self):
        return self._v_sample_denominator()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_rational_polynomial.delete_RsmRationalPolynomial
RsmRationalPolynomial.__str__ = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial___str__, None, RsmRationalPolynomial)
RsmRationalPolynomial.image_coordinate = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial_image_coordinate, None, RsmRationalPolynomial)
RsmRationalPolynomial.set_rpc_coeff = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial_set_rpc_coeff, None, RsmRationalPolynomial)
RsmRationalPolynomial.fit_offset_and_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial_fit_offset_and_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial.fit = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial_fit, None, RsmRationalPolynomial)
RsmRationalPolynomial.image_coordinate_jacobian = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial_image_coordinate_jacobian, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_line_offset = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_line_offset, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_line_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_line_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_sample_offset = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_sample_offset, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_sample_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_sample_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_x_offset = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_x_offset, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_x_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_x_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_y_offset = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_y_offset, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_y_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_y_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_z_offset = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_z_offset, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_z_scale = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_z_scale, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_line_numerator = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_line_numerator, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_line_denominator = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_line_denominator, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_sample_numerator = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_sample_numerator, None, RsmRationalPolynomial)
RsmRationalPolynomial._v_sample_denominator = new_instancemethod(_rsm_rational_polynomial.RsmRationalPolynomial__v_sample_denominator, None, RsmRationalPolynomial)
RsmRationalPolynomial_swigregister = _rsm_rational_polynomial.RsmRationalPolynomial_swigregister
RsmRationalPolynomial_swigregister(RsmRationalPolynomial)



