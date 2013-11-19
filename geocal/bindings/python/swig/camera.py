# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_camera', [dirname(__file__)])
        except ImportError:
            import _camera
            return _camera
        if fp is not None:
            try:
                _mod = imp.load_module('_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _camera = swig_import_helper()
    del swig_import_helper
else:
    import _camera
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
    __swig_destroy__ = _camera.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_camera.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_camera.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_camera.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_camera.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_camera.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_camera.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_camera.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_camera.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_camera.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_camera.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_camera.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_camera.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_camera.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_camera.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_camera.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_camera.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _camera.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _camera.SHARED_PTR_DISOWN
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

import geocal.generic_object
class Camera(geocal.generic_object.GenericObject):
    """
    This class models a frame camera.

    It is used to convert ScLookVector to FrameCoordinate and vice versa.

    C++ includes: camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    FORWARD = _camera.Camera_FORWARD
    AFTWARD = _camera.Camera_AFTWARD
    def integration_time(self, *args):
        """
        virtual double GeoCal::Camera::integration_time(int Band) const
        This is the integration time in seconds.

        The integration time is how long it takes the camera to collect data.
        It is important to model this, so that we can include movement of the
        spacecraft while the camera is in the middle of collecting data. If
        the integration time is very short, or if a particular derived class
        doesn't wish to model this, then the integration time can be set to 0
        seconds.

        The default version returns 0.0. 
        """
        return _camera.Camera_integration_time(self, *args)

    def _v_direction(self):
        """
        virtual Direction GeoCal::Camera::direction() const
        This gives the camera direction.

        This is intended for use with steep camera angles (e.g., MISR AF
        camera). For cameras that are near nadir looking, we can just
        arbitrarily pick a direction for it.

        The default version returns FORWARD. 
        """
        return _camera.Camera__v_direction(self)

    @property
    def direction(self):
        return self._v_direction()

    def _v_number_band(self):
        """
        virtual int GeoCal::Camera::number_band() const =0
        Number of bands in camera. 
        """
        return _camera.Camera__v_number_band(self)

    @property
    def number_band(self):
        return self._v_number_band()

    def number_line(self, *args):
        """
        virtual int GeoCal::Camera::number_line(int Band) const =0
        Number of lines in camera for given band. 
        """
        return _camera.Camera_number_line(self, *args)

    def number_sample(self, *args):
        """
        virtual int GeoCal::Camera::number_sample(int Band) const =0
        Number of samples in camera for given band. 
        """
        return _camera.Camera_number_sample(self, *args)

    def frame_coordinate(self, *args):
        """
        virtual FrameCoordinate GeoCal::Camera::frame_coordinate(const ScLookVector &Sl, int Band) const =0
        This converts from ScLookVector to FrameCoordinate for a given band.

        Note that the FrameCoordinate may be outside of the range (0,
        number_line(band) - 1), (0, number_sample(band) - 1), this just means
        the look vector is not with the active portion of the camera (i.e., it
        is not seen). 
        """
        return _camera.Camera_frame_coordinate(self, *args)

    def frame_line_coordinate(self, *args):
        """
        virtual double GeoCal::Camera::frame_line_coordinate(const ScLookVector &Sl, int Band) const
        This is similar to frame_coordinate, except it only calculates the
        line coordinate.

        For some camera models, it is more expensive to calculate the sample
        number than the line (e.g., MISR camera needs to use the pixel-theta
        measurements). For functions that only need the line information, it
        is wasteful to do the full calculation.

        The default version of this function just calls frame_coordinate and
        returns the line portion, but a derived class can override this
        default with a faster way of calculation the line information. 
        """
        return _camera.Camera_frame_line_coordinate(self, *args)

    def sc_look_vector(self, *args):
        """
        virtual ScLookVector GeoCal::Camera::sc_look_vector(const FrameCoordinate &F, int Band) const =0
        Convert from FrameCoordinate to ScLookVector.

        It is perfectly allowable for F.line to be outside the range (0,
        number_line(band) 1) or for F.sample to be outside the range (0,
        number_sample(band) - 1). The conversion will just act as if the
        camera has infinite extent. 
        """
        return _camera.Camera_sc_look_vector(self, *args)

    __swig_destroy__ = _camera.delete_Camera
Camera.integration_time = new_instancemethod(_camera.Camera_integration_time,None,Camera)
Camera._v_direction = new_instancemethod(_camera.Camera__v_direction,None,Camera)
Camera._v_number_band = new_instancemethod(_camera.Camera__v_number_band,None,Camera)
Camera.number_line = new_instancemethod(_camera.Camera_number_line,None,Camera)
Camera.number_sample = new_instancemethod(_camera.Camera_number_sample,None,Camera)
Camera.frame_coordinate = new_instancemethod(_camera.Camera_frame_coordinate,None,Camera)
Camera.frame_line_coordinate = new_instancemethod(_camera.Camera_frame_line_coordinate,None,Camera)
Camera.sc_look_vector = new_instancemethod(_camera.Camera_sc_look_vector,None,Camera)
Camera.__str__ = new_instancemethod(_camera.Camera___str__,None,Camera)
Camera_swigregister = _camera.Camera_swigregister
Camera_swigregister(Camera)

class PushBroomCamera(Camera):
    """
    This is a camera specialized to 1 line per band.

    C++ includes: camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _camera.delete_PushBroomCamera
PushBroomCamera_swigregister = _camera.PushBroomCamera_swigregister
PushBroomCamera_swigregister(PushBroomCamera)

class SimplePushBroomCamera(PushBroomCamera):
    """
    This is a simple PushBroomCamera.

    It is not intended as a realistic camera model, but rather as
    supplying a simple class to use for testing. The defaults are for the
    nominal MISR DF camera, red band.

    C++ includes: camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        SimplePushBroomCamera::SimplePushBroomCamera(double Beta=58 *Constant::deg_to_rad, double Delta=-2.7
        *Constant::deg_to_rad, double Epsilon=0, double Focal=123.8e-3, double
        line_pitch=18e-6, double sample_pitch=21e-6, int Number_sample=1504)
        This creates SimplePushBroomCamera with the given data.

        The default data is for the nominal MISR DF, red band. 
        """
        _camera.SimplePushBroomCamera_swiginit(self,_camera.new_SimplePushBroomCamera(*args))
    def _v_beta(self):
        """
        double GeoCal::SimplePushBroomCamera::beta() const

        """
        return _camera.SimplePushBroomCamera__v_beta(self)

    @property
    def beta(self):
        return self._v_beta()

    def _v_delta(self):
        """
        double GeoCal::SimplePushBroomCamera::delta() const

        """
        return _camera.SimplePushBroomCamera__v_delta(self)

    @property
    def delta(self):
        return self._v_delta()

    def _v_epsilon(self):
        """
        double GeoCal::SimplePushBroomCamera::epsilon() const

        """
        return _camera.SimplePushBroomCamera__v_epsilon(self)

    @property
    def epsilon(self):
        return self._v_epsilon()

    def _v_focal_length(self):
        """
        double GeoCal::SimplePushBroomCamera::focal_length() const

        """
        return _camera.SimplePushBroomCamera__v_focal_length(self)

    @property
    def focal_length(self):
        return self._v_focal_length()

    def _v_line_pitch(self):
        """
        double GeoCal::SimplePushBroomCamera::line_pitch() const

        """
        return _camera.SimplePushBroomCamera__v_line_pitch(self)

    @property
    def line_pitch(self):
        return self._v_line_pitch()

    def _v_sample_pitch(self):
        """
        double GeoCal::SimplePushBroomCamera::sample_pitch() const

        """
        return _camera.SimplePushBroomCamera__v_sample_pitch(self)

    @property
    def sample_pitch(self):
        return self._v_sample_pitch()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.beta,self.delta,self.epsilon,self.focal_length,self.line_pitch,self.sample_pitch,self.number_sample(0))

    __swig_destroy__ = _camera.delete_SimplePushBroomCamera
SimplePushBroomCamera._v_beta = new_instancemethod(_camera.SimplePushBroomCamera__v_beta,None,SimplePushBroomCamera)
SimplePushBroomCamera._v_delta = new_instancemethod(_camera.SimplePushBroomCamera__v_delta,None,SimplePushBroomCamera)
SimplePushBroomCamera._v_epsilon = new_instancemethod(_camera.SimplePushBroomCamera__v_epsilon,None,SimplePushBroomCamera)
SimplePushBroomCamera._v_focal_length = new_instancemethod(_camera.SimplePushBroomCamera__v_focal_length,None,SimplePushBroomCamera)
SimplePushBroomCamera._v_line_pitch = new_instancemethod(_camera.SimplePushBroomCamera__v_line_pitch,None,SimplePushBroomCamera)
SimplePushBroomCamera._v_sample_pitch = new_instancemethod(_camera.SimplePushBroomCamera__v_sample_pitch,None,SimplePushBroomCamera)
SimplePushBroomCamera_swigregister = _camera.SimplePushBroomCamera_swigregister
SimplePushBroomCamera_swigregister(SimplePushBroomCamera)

class Vector_Camera(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _camera.Vector_Camera_swiginit(self,_camera.new_Vector_Camera(*args))
    __swig_destroy__ = _camera.delete_Vector_Camera
Vector_Camera.iterator = new_instancemethod(_camera.Vector_Camera_iterator,None,Vector_Camera)
Vector_Camera.__nonzero__ = new_instancemethod(_camera.Vector_Camera___nonzero__,None,Vector_Camera)
Vector_Camera.__bool__ = new_instancemethod(_camera.Vector_Camera___bool__,None,Vector_Camera)
Vector_Camera.__len__ = new_instancemethod(_camera.Vector_Camera___len__,None,Vector_Camera)
Vector_Camera.pop = new_instancemethod(_camera.Vector_Camera_pop,None,Vector_Camera)
Vector_Camera.__getslice__ = new_instancemethod(_camera.Vector_Camera___getslice__,None,Vector_Camera)
Vector_Camera.__setslice__ = new_instancemethod(_camera.Vector_Camera___setslice__,None,Vector_Camera)
Vector_Camera.__delslice__ = new_instancemethod(_camera.Vector_Camera___delslice__,None,Vector_Camera)
Vector_Camera.__delitem__ = new_instancemethod(_camera.Vector_Camera___delitem__,None,Vector_Camera)
Vector_Camera.__getitem__ = new_instancemethod(_camera.Vector_Camera___getitem__,None,Vector_Camera)
Vector_Camera.__setitem__ = new_instancemethod(_camera.Vector_Camera___setitem__,None,Vector_Camera)
Vector_Camera.append = new_instancemethod(_camera.Vector_Camera_append,None,Vector_Camera)
Vector_Camera.empty = new_instancemethod(_camera.Vector_Camera_empty,None,Vector_Camera)
Vector_Camera.size = new_instancemethod(_camera.Vector_Camera_size,None,Vector_Camera)
Vector_Camera.clear = new_instancemethod(_camera.Vector_Camera_clear,None,Vector_Camera)
Vector_Camera.swap = new_instancemethod(_camera.Vector_Camera_swap,None,Vector_Camera)
Vector_Camera.get_allocator = new_instancemethod(_camera.Vector_Camera_get_allocator,None,Vector_Camera)
Vector_Camera.begin = new_instancemethod(_camera.Vector_Camera_begin,None,Vector_Camera)
Vector_Camera.end = new_instancemethod(_camera.Vector_Camera_end,None,Vector_Camera)
Vector_Camera.rbegin = new_instancemethod(_camera.Vector_Camera_rbegin,None,Vector_Camera)
Vector_Camera.rend = new_instancemethod(_camera.Vector_Camera_rend,None,Vector_Camera)
Vector_Camera.pop_back = new_instancemethod(_camera.Vector_Camera_pop_back,None,Vector_Camera)
Vector_Camera.erase = new_instancemethod(_camera.Vector_Camera_erase,None,Vector_Camera)
Vector_Camera.push_back = new_instancemethod(_camera.Vector_Camera_push_back,None,Vector_Camera)
Vector_Camera.front = new_instancemethod(_camera.Vector_Camera_front,None,Vector_Camera)
Vector_Camera.back = new_instancemethod(_camera.Vector_Camera_back,None,Vector_Camera)
Vector_Camera.assign = new_instancemethod(_camera.Vector_Camera_assign,None,Vector_Camera)
Vector_Camera.resize = new_instancemethod(_camera.Vector_Camera_resize,None,Vector_Camera)
Vector_Camera.insert = new_instancemethod(_camera.Vector_Camera_insert,None,Vector_Camera)
Vector_Camera.reserve = new_instancemethod(_camera.Vector_Camera_reserve,None,Vector_Camera)
Vector_Camera.capacity = new_instancemethod(_camera.Vector_Camera_capacity,None,Vector_Camera)
Vector_Camera_swigregister = _camera.Vector_Camera_swigregister
Vector_Camera_swigregister(Vector_Camera)



