# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _camera_rational_polynomial.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_camera_rational_polynomial')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_camera_rational_polynomial')
    _camera_rational_polynomial = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_camera_rational_polynomial', [dirname(__file__)])
        except ImportError:
            import _camera_rational_polynomial
            return _camera_rational_polynomial
        try:
            _mod = imp.load_module('_camera_rational_polynomial', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _camera_rational_polynomial = swig_import_helper()
    del swig_import_helper
else:
    import _camera_rational_polynomial
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


SWIG_MODULE_ALREADY_DONE = _camera_rational_polynomial.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _camera_rational_polynomial.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_camera_rational_polynomial.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_camera_rational_polynomial.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _camera_rational_polynomial.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _camera_rational_polynomial.SHARED_PTR_DISOWN

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
    return geocal_swig.serialize_function.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_function.serialize_read_binary(data)
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

import geocal_swig.quaternion_camera
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.camera
import geocal_swig.with_parameter
class CameraRationalPolyomial(geocal_swig.quaternion_camera.QuaternionCamera):
    """

    This is a QuaternionCamera where the nonlinearity of the camera is
    described as a rational polynomial in the line and sample.

    This is the formulation Mike Burl uses in his matlab code, and we want
    to implement this so we can take his camera models for use in GeoCal.

    C++ includes: camera_rational_polynomial.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Nu, Nv, Xi, U0, V0, Pld_q_c, Kappa, Kappa_inverse):
        """

        GeoCal::CameraRationalPolyomial::CameraRationalPolyomial(int Nu, int Nv, double Xi, double U0, double V0,
        boost::math::quaternion< double > Pld_q_c, const blitz::Array< double,
        2 > &Kappa, const blitz::Array< double, 2 > &Kappa_inverse)
        This makes a QuaternionCamera using Mike Burls conventions.

        He works with (u,v), which is what we call (sample,line) (so
        reversed). xi is the field angle of a pixel. He doesn't have an
        explicit focal length (since he works in radians), so be convention we
        set the focal length to 1 and then xi is what we use for line and
        sample pitch. The principle point is (V0, U0) He uses the term Pld_q_c
        for what we call Frame_to_sc_q.

        *** NOTE *** I may have this reversed, if so we might need an inverse
        in here 
        """
        _camera_rational_polynomial.CameraRationalPolyomial_swiginit(self, _camera_rational_polynomial.new_CameraRationalPolyomial(Nu, Nv, Xi, U0, V0, Pld_q_c, Kappa, Kappa_inverse))

    def _v_kappa(self):
        """

        const blitz::Array<double, 2>& GeoCal::CameraRationalPolyomial::kappa() const
        Kappa, used to describe nonlinearity. 
        """
        return _camera_rational_polynomial.CameraRationalPolyomial__v_kappa(self)


    @property
    def kappa(self):
        return self._v_kappa()


    def _v_kappa_inverse(self):
        """

        const blitz::Array<double, 2>& GeoCal::CameraRationalPolyomial::kappa_inverse() const
        Inverse of Kappa. 
        """
        return _camera_rational_polynomial.CameraRationalPolyomial__v_kappa_inverse(self)


    @property
    def kappa_inverse(self):
        return self._v_kappa_inverse()


    def apply_rational(self, X, Coeff):
        """

        blitz::Array< double, 1 > CameraRationalPolyomial::apply_rational(const blitz::Array< double, 1 > &X, const blitz::Array< double, 2 >
        &Coeff) const
        This is the apply_rational function found in Mike Burl's matlab code.

        """
        return _camera_rational_polynomial.CameraRationalPolyomial_apply_rational(self, X, Coeff)


    def construct_chi_matrix(self, X, ord):
        """

        blitz::Array< double, 1 > CameraRationalPolyomial::construct_chi_matrix(const blitz::Array< double, 1 > &X, int ord) const
        This is the construct_chi_matrix function found in Mike Burl's matlab
        code. 
        """
        return _camera_rational_polynomial.CameraRationalPolyomial_construct_chi_matrix(self, X, ord)


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _camera_rational_polynomial.delete_CameraRationalPolyomial
CameraRationalPolyomial._v_kappa = new_instancemethod(_camera_rational_polynomial.CameraRationalPolyomial__v_kappa, None, CameraRationalPolyomial)
CameraRationalPolyomial._v_kappa_inverse = new_instancemethod(_camera_rational_polynomial.CameraRationalPolyomial__v_kappa_inverse, None, CameraRationalPolyomial)
CameraRationalPolyomial.apply_rational = new_instancemethod(_camera_rational_polynomial.CameraRationalPolyomial_apply_rational, None, CameraRationalPolyomial)
CameraRationalPolyomial.construct_chi_matrix = new_instancemethod(_camera_rational_polynomial.CameraRationalPolyomial_construct_chi_matrix, None, CameraRationalPolyomial)
CameraRationalPolyomial_swigregister = _camera_rational_polynomial.CameraRationalPolyomial_swigregister
CameraRationalPolyomial_swigregister(CameraRationalPolyomial)


__all__ = ["CameraRationalPolyomial"]



