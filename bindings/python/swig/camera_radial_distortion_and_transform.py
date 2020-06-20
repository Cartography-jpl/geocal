# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _camera_radial_distortion_and_transform.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_camera_radial_distortion_and_transform')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_camera_radial_distortion_and_transform')
    _camera_radial_distortion_and_transform = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_camera_radial_distortion_and_transform', [dirname(__file__)])
        except ImportError:
            import _camera_radial_distortion_and_transform
            return _camera_radial_distortion_and_transform
        try:
            _mod = imp.load_module('_camera_radial_distortion_and_transform', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _camera_radial_distortion_and_transform = swig_import_helper()
    del swig_import_helper
else:
    import _camera_radial_distortion_and_transform
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
    __swig_destroy__ = _camera_radial_distortion_and_transform.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_camera_radial_distortion_and_transform.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _camera_radial_distortion_and_transform.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _camera_radial_distortion_and_transform.SHARED_PTR_DISOWN

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

import geocal_swig.camera_radial_distortion
import geocal_swig.quaternion_camera
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.camera
import geocal_swig.with_parameter
class CameraRadialDistortionAndTransform(geocal_swig.camera_radial_distortion.CameraRadialDistortion):
    """

    This is a CameraRadialDistortion, with the addition of a supplied
    transform.

    This is the form of the HiRISE camera. I'm not sure how general this
    is, perhaps this should just be called HiRISE.

    C++ includes: camera_radial_distortion_and_transform.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Frame_to_sc_q, K_distort, Number_line, Number_sample, Line_pitch, Sample_pitch, Focal_length, Bin_mode, Ccd_off, Ccd_cen, T_off, T_m, Tinv_off, Tinv_m):
        """

        GeoCal::CameraRadialDistortionAndTransform::CameraRadialDistortionAndTransform(boost::math::quaternion< double > Frame_to_sc_q, const blitz::Array<
        double, 1 > &K_distort, double Number_line, double Number_sample,
        double Line_pitch, double Sample_pitch, double Focal_length, int
        Bin_mode, const blitz::Array< double, 1 > &Ccd_off, const
        blitz::Array< double, 1 > &Ccd_cen, const blitz::Array< double, 1 >
        &T_off, const blitz::Array< double, 2 > &T_m, const blitz::Array<
        double, 1 > &Tinv_off, const blitz::Array< double, 2 > &Tinv_m)

        """
        _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform_swiginit(self, _camera_radial_distortion_and_transform.new_CameraRadialDistortionAndTransform(Frame_to_sc_q, K_distort, Number_line, Number_sample, Line_pitch, Sample_pitch, Focal_length, Bin_mode, Ccd_off, Ccd_cen, T_off, T_m, Tinv_off, Tinv_m))

    def _v_bin_mode(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::bin_mode(int V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_bin_mode(self, *args)


    @property
    def bin_mode(self):
        return self._v_bin_mode()

    @bin_mode.setter
    def bin_mode(self, value):
      self._v_bin_mode(value)


    def _v_ccd_off(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::ccd_off(const blitz::Array< double, 1 > &V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_ccd_off(self, *args)


    @property
    def ccd_off(self):
        return self._v_ccd_off()

    @ccd_off.setter
    def ccd_off(self, value):
      self._v_ccd_off(value)


    def _v_ccd_cen(self, *args):
        """

        const blitz::Array<double, 1>& GeoCal::CameraRadialDistortionAndTransform::ccd_cen() const

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_ccd_cen(self, *args)


    @property
    def ccd_cen(self):
        return self._v_ccd_cen()

    @ccd_cen.setter
    def ccd_cen(self, value):
      self._v_ccd_cen(value)


    def _v_t_off(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::t_off(const blitz::Array< double, 1 > &V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_t_off(self, *args)


    @property
    def t_off(self):
        return self._v_t_off()

    @t_off.setter
    def t_off(self, value):
      self._v_t_off(value)


    def _v_tinv_off(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::tinv_off(const blitz::Array< double, 1 > &V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_tinv_off(self, *args)


    @property
    def tinv_off(self):
        return self._v_tinv_off()

    @tinv_off.setter
    def tinv_off(self, value):
      self._v_tinv_off(value)


    def _v_t_m(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::t_m(const blitz::Array< double, 2 > &V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_t_m(self, *args)


    @property
    def t_m(self):
        return self._v_t_m()

    @t_m.setter
    def t_m(self, value):
      self._v_t_m(value)


    def _v_tinv_m(self, *args):
        """

        void GeoCal::CameraRadialDistortionAndTransform::tinv_m(const blitz::Array< double, 2 > &V)

        """
        return _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_tinv_m(self, *args)


    @property
    def tinv_m(self):
        return self._v_tinv_m()

    @tinv_m.setter
    def tinv_m(self, value):
      self._v_tinv_m(value)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _camera_radial_distortion_and_transform.delete_CameraRadialDistortionAndTransform
CameraRadialDistortionAndTransform._v_bin_mode = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_bin_mode, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_ccd_off = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_ccd_off, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_ccd_cen = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_ccd_cen, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_t_off = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_t_off, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_tinv_off = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_tinv_off, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_t_m = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_t_m, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform._v_tinv_m = new_instancemethod(_camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform__v_tinv_m, None, CameraRadialDistortionAndTransform)
CameraRadialDistortionAndTransform_swigregister = _camera_radial_distortion_and_transform.CameraRadialDistortionAndTransform_swigregister
CameraRadialDistortionAndTransform_swigregister(CameraRadialDistortionAndTransform)


__all__ = ["CameraRadialDistortionAndTransform"]



