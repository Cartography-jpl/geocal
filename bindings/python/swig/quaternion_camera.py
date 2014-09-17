# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _quaternion_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
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


SHARED_PTR_DISOWN = _quaternion_camera.SHARED_PTR_DISOWN
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

import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.camera
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
    def __init__(self, *args): 
        """
        GeoCal::QuaternionCamera::QuaternionCamera(boost::math::quaternion< double > Frame_to_sc_q, double Number_line,
        double Number_sample, double Line_pitch, double Sample_pitch, double
        Focal_length, const FrameCoordinate &Principal_point, FrameConvention
        Frame_convention=LINE_IS_X, FrameDirection
        Line_direction=INCREASE_IS_POSITIVE, FrameDirection
        Sample_direction=INCREASE_IS_POSITIVE)
        Create a QuaternionCamera.

        The orientation of the camera to the spacecraft to given by the
        quaternion that takes frame coordinates to spacecraft coordinates. The
        size of the camera and the line pitch, sample pitch, and focal length
        are given. By convention, these are given in mm. Finally the
        Principal_point (coordinates at center) are given. 
        """
        _quaternion_camera.QuaternionCamera_swiginit(self,_quaternion_camera.new_QuaternionCamera(*args))
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

    def principal_point(self, *args):
        """
        void GeoCal::QuaternionCamera::principal_point(int B, const FrameCoordinate &Fc)
        Set principal point of camera. 
        """
        return _quaternion_camera.QuaternionCamera_principal_point(self, *args)

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

    def sc_look_vector(self, *args):
        """
        virtual ScLookVector GeoCal::QuaternionCamera::sc_look_vector(const DcsLookVector &Dlv) const

        """
        return _quaternion_camera.QuaternionCamera_sc_look_vector(self, *args)

    def dcs_look_vector(self, *args):
        """
        DcsLookVector QuaternionCamera::dcs_look_vector(const FrameCoordinate &F, int Band) const
        Convert from FrameCoordinate to DcsLookVector.

        It is perfectly allowable for F.line to be outside the range (0,
        number_line(band) 1) or for F.sample to be outside the range (0,
        number_sample(band) - 1). The conversion will just act as if the
        camera has infinite extent. 
        """
        return _quaternion_camera.QuaternionCamera_dcs_look_vector(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.frame_to_sc,self.number_line(0),self.number_sample(0),self.line_pitch,self.sample_pitch,self.focal_length,self.principal_point(0),self.frame_convention,self.line_direction,self.sample_direction)

    __swig_destroy__ = _quaternion_camera.delete_QuaternionCamera
QuaternionCamera._v_focal_length = new_instancemethod(_quaternion_camera.QuaternionCamera__v_focal_length,None,QuaternionCamera)
QuaternionCamera.principal_point = new_instancemethod(_quaternion_camera.QuaternionCamera_principal_point,None,QuaternionCamera)
QuaternionCamera._v_line_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_line_pitch,None,QuaternionCamera)
QuaternionCamera._v_sample_pitch = new_instancemethod(_quaternion_camera.QuaternionCamera__v_sample_pitch,None,QuaternionCamera)
QuaternionCamera._v_frame_convention = new_instancemethod(_quaternion_camera.QuaternionCamera__v_frame_convention,None,QuaternionCamera)
QuaternionCamera._v_line_direction = new_instancemethod(_quaternion_camera.QuaternionCamera__v_line_direction,None,QuaternionCamera)
QuaternionCamera._v_sample_direction = new_instancemethod(_quaternion_camera.QuaternionCamera__v_sample_direction,None,QuaternionCamera)
QuaternionCamera._v_frame_to_sc = new_instancemethod(_quaternion_camera.QuaternionCamera__v_frame_to_sc,None,QuaternionCamera)
QuaternionCamera._v_ypr = new_instancemethod(_quaternion_camera.QuaternionCamera__v_ypr,None,QuaternionCamera)
QuaternionCamera._v_euler = new_instancemethod(_quaternion_camera.QuaternionCamera__v_euler,None,QuaternionCamera)
QuaternionCamera.sc_look_vector = new_instancemethod(_quaternion_camera.QuaternionCamera_sc_look_vector,None,QuaternionCamera)
QuaternionCamera.dcs_look_vector = new_instancemethod(_quaternion_camera.QuaternionCamera_dcs_look_vector,None,QuaternionCamera)
QuaternionCamera_swigregister = _quaternion_camera.QuaternionCamera_swigregister
QuaternionCamera_swigregister(QuaternionCamera)



