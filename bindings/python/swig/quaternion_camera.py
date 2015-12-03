# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _quaternion_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_quaternion_camera', [dirname(__file__)])
        except ImportError:
            import _quaternion_camera
            return _quaternion_camera
        if fp is not None:
            try:
                _mod = imp.load_module('_quaternion_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _quaternion_camera = swig_import_helper()
    del swig_import_helper
else:
    import _quaternion_camera
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
    __swig_destroy__ = _quaternion_camera.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_quaternion_camera.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_quaternion_camera.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_quaternion_camera.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_quaternion_camera.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_quaternion_camera.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_quaternion_camera.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_quaternion_camera.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_quaternion_camera.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_quaternion_camera.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_quaternion_camera.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_quaternion_camera.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_quaternion_camera.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_quaternion_camera.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_quaternion_camera.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_quaternion_camera.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_quaternion_camera.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _quaternion_camera.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_quaternion_camera.SHARED_PTR_DISOWN_swigconstant(_quaternion_camera)
SHARED_PTR_DISOWN = _quaternion_camera.SHARED_PTR_DISOWN

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
import geocal_swig.observer
import geocal_swig.camera
import geocal_swig.with_parameter
class QuaternionCamera(geocal_swig.camera.Camera):
    """

    A lot of cameras follow the model of "rotate into the detector space,
    do a nonlinear correction".

    This class supports this by handling the rotation of a ScLookVector
    and conversion to FrameCoordinate (and vice versa). This base class
    doesn't model any nonlinear corrections - we just model a pinhole. But
    derived classes can override the dcs_to_focal_plane and
    focal_plane_to_dcs functions to put in whatever functionality is
    desired.

    There are 2 conventions used for the frame coordinates. The convention
    used by some cameras we have implemented is that the line direction is
    +x, and the sample direction is +y.

    However, another convention is that line goes in the +y direction and
    sample goes in +x direction. This is what was used in the VICAR
    procedure sc2rpc. Note that this is more than just a rotation from the
    other convention, it is both a rotation and a reflection (so the
    chirality is different).

    We support both conventions, depending on the setting of the
    frame_convention.

    In addition to either x or y direction for line, we can have
    increasing line go in the positive direction or negative direction.
    Likewise for sample.

    C++ includes: quaternion_camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    LINE_IS_X = _quaternion_camera.QuaternionCamera_LINE_IS_X
    LINE_IS_Y = _quaternion_camera.QuaternionCamera_LINE_IS_Y
    INCREASE_IS_POSITIVE = _quaternion_camera.QuaternionCamera_INCREASE_IS_POSITIVE
    INCREASE_IS_NEGATIVE = _quaternion_camera.QuaternionCamera_INCREASE_IS_NEGATIVE

    def _v_focal_length(self, *args):
        """

        void GeoCal::QuaternionCamera::focal_length(double V)
        Set focal length, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_focal_length(self, *args)


    @property
    def focal_length(self):
        return self._v_focal_length()

    @focal_length.setter
    def focal_length(self, value):
      self._v_focal_length(value)


    def _v_focal_length_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::focal_length_with_derivative(const AutoDerivative< double > &V)
        Set focal length, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_focal_length_with_derivative(self, *args)


    @property
    def focal_length_with_derivative(self):
        return self._v_focal_length_with_derivative()

    @focal_length_with_derivative.setter
    def focal_length_with_derivative(self, value):
      self._v_focal_length_with_derivative(value)


    def principal_point(self, *args):
        """

        void GeoCal::QuaternionCamera::principal_point(int B, const FrameCoordinate &Fc)
        Set principal point of camera. 
        """
        return _quaternion_camera.QuaternionCamera_principal_point(self, *args)


    def principal_point_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::principal_point_with_derivative(int B, const FrameCoordinateWithDerivative &Fc)
        Set principal point of camera. 
        """
        return _quaternion_camera.QuaternionCamera_principal_point_with_derivative(self, *args)


    def _v_line_pitch(self, *args):
        """

        void GeoCal::QuaternionCamera::line_pitch(double Lp)
        Set CCD pitch, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_line_pitch(self, *args)


    @property
    def line_pitch(self):
        return self._v_line_pitch()

    @line_pitch.setter
    def line_pitch(self, value):
      self._v_line_pitch(value)


    def _v_line_pitch_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::line_pitch_with_derivative(const AutoDerivative< double > &Lp)
        Set CCD pitch, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_line_pitch_with_derivative(self, *args)


    @property
    def line_pitch_with_derivative(self):
        return self._v_line_pitch_with_derivative()

    @line_pitch_with_derivative.setter
    def line_pitch_with_derivative(self, value):
      self._v_line_pitch_with_derivative(value)


    def _v_sample_pitch(self, *args):
        """

        void GeoCal::QuaternionCamera::sample_pitch(double Sp)
        Set CCD pitch, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_sample_pitch(self, *args)


    @property
    def sample_pitch(self):
        return self._v_sample_pitch()

    @sample_pitch.setter
    def sample_pitch(self, value):
      self._v_sample_pitch(value)


    def _v_sample_pitch_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::sample_pitch_with_derivative(const AutoDerivative< double > &Sp)
        Set CCD pitch, in mm. 
        """
        return _quaternion_camera.QuaternionCamera__v_sample_pitch_with_derivative(self, *args)


    @property
    def sample_pitch_with_derivative(self):
        return self._v_sample_pitch_with_derivative()

    @sample_pitch_with_derivative.setter
    def sample_pitch_with_derivative(self, value):
      self._v_sample_pitch_with_derivative(value)


    def _v_frame_convention(self, *args):
        """

        void GeoCal::QuaternionCamera::frame_convention(FrameConvention Frame_convention)
        Set frame convention. 
        """
        return _quaternion_camera.QuaternionCamera__v_frame_convention(self, *args)


    @property
    def frame_convention(self):
        return self._v_frame_convention()

    @frame_convention.setter
    def frame_convention(self, value):
      self._v_frame_convention(value)


    def _v_line_direction(self, *args):
        """

        void GeoCal::QuaternionCamera::line_direction(FrameDirection Line_direction)
        Set line direction. 
        """
        return _quaternion_camera.QuaternionCamera__v_line_direction(self, *args)


    @property
    def line_direction(self):
        return self._v_line_direction()

    @line_direction.setter
    def line_direction(self, value):
      self._v_line_direction(value)


    def _v_sample_direction(self, *args):
        """

        void GeoCal::QuaternionCamera::sample_direction(FrameDirection Sample_direction)
        Set sample direction. 
        """
        return _quaternion_camera.QuaternionCamera__v_sample_direction(self, *args)


    @property
    def sample_direction(self):
        return self._v_sample_direction()

    @sample_direction.setter
    def sample_direction(self, value):
      self._v_sample_direction(value)


    def _v_frame_to_sc(self, *args):
        """

        void GeoCal::QuaternionCamera::frame_to_sc(const boost::math::quaternion< double > &frame_to_sc_q)
        Set frame to spacecraft quaternion. 
        """
        return _quaternion_camera.QuaternionCamera__v_frame_to_sc(self, *args)


    @property
    def frame_to_sc(self):
        return self._v_frame_to_sc()

    @frame_to_sc.setter
    def frame_to_sc(self, value):
      self._v_frame_to_sc(value)


    def _v_frame_to_sc_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::frame_to_sc_with_derivative(const boost::math::quaternion< AutoDerivative< double > >
        &frame_to_sc_q)
        Set frame to spacecraft quaternion. 
        """
        return _quaternion_camera.QuaternionCamera__v_frame_to_sc_with_derivative(self, *args)


    @property
    def frame_to_sc_with_derivative(self):
        return self._v_frame_to_sc_with_derivative()

    @frame_to_sc_with_derivative.setter
    def frame_to_sc_with_derivative(self, value):
      self._v_frame_to_sc_with_derivative(value)


    def _v_ypr(self, *args):
        """

        void GeoCal::QuaternionCamera::ypr(const blitz::Array< double, 1 > &Ypr)
        Update the frame_to_sc using the given yaw, pitch, roll angles in
        radians. 
        """
        return _quaternion_camera.QuaternionCamera__v_ypr(self, *args)


    @property
    def ypr(self):
        return self._v_ypr()

    @ypr.setter
    def ypr(self, value):
      self._v_ypr(value)


    def _v_ypr_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::ypr_with_derivative(const blitz::Array< AutoDerivative< double >, 1 > &Ypr)
        Update the frame_to_sc using the given yaw, pitch, roll angles in
        radians. 
        """
        return _quaternion_camera.QuaternionCamera__v_ypr_with_derivative(self, *args)


    @property
    def ypr_with_derivative(self):
        return self._v_ypr_with_derivative()

    @ypr_with_derivative.setter
    def ypr_with_derivative(self, value):
      self._v_ypr_with_derivative(value)


    def _v_euler(self, *args):
        """

        void GeoCal::QuaternionCamera::euler(const blitz::Array< double, 1 > &Euler)
        Update the frame_to_sc using the given Euler angles epsilon, beta,
        data in radians. 
        """
        return _quaternion_camera.QuaternionCamera__v_euler(self, *args)


    @property
    def euler(self):
        return self._v_euler()

    @euler.setter
    def euler(self, value):
      self._v_euler(value)


    def _v_euler_with_derivative(self, *args):
        """

        void GeoCal::QuaternionCamera::euler_with_derivative(const ArrayAd< double, 1 > &Euler)

        """
        return _quaternion_camera.QuaternionCamera__v_euler_with_derivative(self, *args)


    @property
    def euler_with_derivative(self):
        return self._v_euler_with_derivative()

    @euler_with_derivative.setter
    def euler_with_derivative(self, value):
      self._v_euler_with_derivative(value)


    def sc_look_vector(self, *args):
        """

        virtual ScLookVector GeoCal::QuaternionCamera::sc_look_vector(const DcsLookVector &Dlv) const

        """
        return _quaternion_camera.QuaternionCamera_sc_look_vector(self, *args)


    def dcs_look_vector(self, *args):
        """

        DcsLookVectorWithDerivative QuaternionCamera::dcs_look_vector(const FrameCoordinateWithDerivative &F, int Band) const

        """
        return _quaternion_camera.QuaternionCamera_dcs_look_vector(self, *args)


    def _v_fit_epsilon(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_epsilon(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_epsilon(self, *args)


    @property
    def fit_epsilon(self):
        return self._v_fit_epsilon()

    @fit_epsilon.setter
    def fit_epsilon(self, value):
      self._v_fit_epsilon(value)


    def _v_fit_beta(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_beta(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_beta(self, *args)


    @property
    def fit_beta(self):
        return self._v_fit_beta()

    @fit_beta.setter
    def fit_beta(self, value):
      self._v_fit_beta(value)


    def _v_fit_delta(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_delta(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_delta(self, *args)


    @property
    def fit_delta(self):
        return self._v_fit_delta()

    @fit_delta.setter
    def fit_delta(self, value):
      self._v_fit_delta(value)


    def _v_fit_line_pitch(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_line_pitch(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_line_pitch(self, *args)


    @property
    def fit_line_pitch(self):
        return self._v_fit_line_pitch()

    @fit_line_pitch.setter
    def fit_line_pitch(self, value):
      self._v_fit_line_pitch(value)


    def _v_fit_sample_pitch(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_sample_pitch(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_sample_pitch(self, *args)


    @property
    def fit_sample_pitch(self):
        return self._v_fit_sample_pitch()

    @fit_sample_pitch.setter
    def fit_sample_pitch(self, value):
      self._v_fit_sample_pitch(value)


    def _v_fit_focal_length(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_focal_length(bool V)

        """
        return _quaternion_camera.QuaternionCamera__v_fit_focal_length(self, *args)


    @property
    def fit_focal_length(self):
        return self._v_fit_focal_length()

    @fit_focal_length.setter
    def fit_focal_length(self, value):
      self._v_fit_focal_length(value)


    def fit_principal_point_line(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_principal_point_line(bool V, int Band=0)

        """
        return _quaternion_camera.QuaternionCamera_fit_principal_point_line(self, *args)


    def fit_principal_point_sample(self, *args):
        """

        void GeoCal::QuaternionCamera::fit_principal_point_sample(bool V, int Band=0)

        """
        return _quaternion_camera.QuaternionCamera_fit_principal_point_sample(self, *args)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)


    def __init__(self, *args):
        """

        GeoCal::QuaternionCamera::QuaternionCamera(boost::math::quaternion< double > Frame_to_sc_q, double Number_line,
        double Number_sample, double Line_pitch, double Sample_pitch, double
        Focal_length, const FrameCoordinate &Principal_point, FrameConvention
        Frame_convention, FrameDirection Line_direction, FrameDirection
        Sample_direction, const blitz::Array< bool, 1 > &Parameter_mask)

        """
        _quaternion_camera.QuaternionCamera_swiginit(self, _quaternion_camera.new_QuaternionCamera(*args))
    __swig_destroy__ = _quaternion_camera.delete_QuaternionCamera
QuaternionCamera._v_focal_length = new_instancemethod(_quaternion_camera.QuaternionCamera__v_focal_length, None, QuaternionCamera)
QuaternionCamera._v_focal_length_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_focal_length_with_derivative, None, QuaternionCamera)
QuaternionCamera.principal_point = new_instancemethod(_quaternion_camera.QuaternionCamera_principal_point, None, QuaternionCamera)
QuaternionCamera.principal_point_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera_principal_point_with_derivative, None, QuaternionCamera)
QuaternionCamera._v_line_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_line_pitch, None, QuaternionCamera)
QuaternionCamera._v_line_pitch_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_line_pitch_with_derivative, None, QuaternionCamera)
QuaternionCamera._v_sample_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_sample_pitch, None, QuaternionCamera)
QuaternionCamera._v_sample_pitch_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_sample_pitch_with_derivative, None, QuaternionCamera)
QuaternionCamera._v_frame_convention = new_instancemethod(_quaternion_camera.QuaternionCamera__v_frame_convention, None, QuaternionCamera)
QuaternionCamera._v_line_direction = new_instancemethod(_quaternion_camera.QuaternionCamera__v_line_direction, None, QuaternionCamera)
QuaternionCamera._v_sample_direction = new_instancemethod(_quaternion_camera.QuaternionCamera__v_sample_direction, None, QuaternionCamera)
QuaternionCamera._v_frame_to_sc = new_instancemethod(_quaternion_camera.QuaternionCamera__v_frame_to_sc, None, QuaternionCamera)
QuaternionCamera._v_frame_to_sc_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_frame_to_sc_with_derivative, None, QuaternionCamera)
QuaternionCamera._v_ypr = new_instancemethod(_quaternion_camera.QuaternionCamera__v_ypr, None, QuaternionCamera)
QuaternionCamera._v_ypr_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_ypr_with_derivative, None, QuaternionCamera)
QuaternionCamera._v_euler = new_instancemethod(_quaternion_camera.QuaternionCamera__v_euler, None, QuaternionCamera)
QuaternionCamera._v_euler_with_derivative = new_instancemethod(_quaternion_camera.QuaternionCamera__v_euler_with_derivative, None, QuaternionCamera)
QuaternionCamera.sc_look_vector = new_instancemethod(_quaternion_camera.QuaternionCamera_sc_look_vector, None, QuaternionCamera)
QuaternionCamera.dcs_look_vector = new_instancemethod(_quaternion_camera.QuaternionCamera_dcs_look_vector, None, QuaternionCamera)
QuaternionCamera._v_fit_epsilon = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_epsilon, None, QuaternionCamera)
QuaternionCamera._v_fit_beta = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_beta, None, QuaternionCamera)
QuaternionCamera._v_fit_delta = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_delta, None, QuaternionCamera)
QuaternionCamera._v_fit_line_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_line_pitch, None, QuaternionCamera)
QuaternionCamera._v_fit_sample_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_sample_pitch, None, QuaternionCamera)
QuaternionCamera._v_fit_focal_length = new_instancemethod(_quaternion_camera.QuaternionCamera__v_fit_focal_length, None, QuaternionCamera)
QuaternionCamera.fit_principal_point_line = new_instancemethod(_quaternion_camera.QuaternionCamera_fit_principal_point_line, None, QuaternionCamera)
QuaternionCamera.fit_principal_point_sample = new_instancemethod(_quaternion_camera.QuaternionCamera_fit_principal_point_sample, None, QuaternionCamera)
QuaternionCamera_swigregister = _quaternion_camera.QuaternionCamera_swigregister
QuaternionCamera_swigregister(QuaternionCamera)



