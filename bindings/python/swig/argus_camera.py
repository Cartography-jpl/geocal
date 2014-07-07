# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _argus_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_argus_camera', [dirname(__file__)])
        except ImportError:
            import _argus_camera
            return _argus_camera
        if fp is not None:
            try:
                _mod = imp.load_module('_argus_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _argus_camera = swig_import_helper()
    del swig_import_helper
else:
    import _argus_camera
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


SHARED_PTR_DISOWN = _argus_camera.SHARED_PTR_DISOWN
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

import geocal_swig.quaternion_camera
import geocal_swig.camera
import geocal_swig.generic_object
class ArgusCamera(geocal_swig.quaternion_camera.QuaternionCamera):
    """
    A model of the ARGUS camera.

    Right now, this doesn't account for any lens distortion, we model this
    as a pinhole camera. We may need to revisit this.

    C++ includes: argus_camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, Yaw=0, Pitch=0, Roll=0, Focal_length=400): 
        """
        ArgusCamera::ArgusCamera(double Yaw=0, double Pitch=0, double Roll=0, double Focal_length=400)
        Create a ARGUS camera.

        We read one of the image jpeg files to get camera metadata used to
        describe the camera. Yaw, Pitch and Roll are in degrees, Focal_length
        is in mm. 
        """
        _argus_camera.ArgusCamera_swiginit(self,_argus_camera.new_ArgusCamera(Yaw, Pitch, Roll, Focal_length))
    def _v_roll(self):
        """
        double GeoCal::ArgusCamera::roll() const
        Roll angle, in degrees. 
        """
        return _argus_camera.ArgusCamera__v_roll(self)

    @property
    def roll(self):
        return self._v_roll()

    def _v_pitch(self):
        """
        double GeoCal::ArgusCamera::pitch() const
        Pitch angle, in degrees. 
        """
        return _argus_camera.ArgusCamera__v_pitch(self)

    @property
    def pitch(self):
        return self._v_pitch()

    def _v_yaw(self):
        """
        double GeoCal::ArgusCamera::yaw() const
        Yaw angle, in degrees. 
        """
        return _argus_camera.ArgusCamera__v_yaw(self)

    @property
    def yaw(self):
        return self._v_yaw()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.yaw,self.pitch,self.roll,self.focal_length)

    __swig_destroy__ = _argus_camera.delete_ArgusCamera
ArgusCamera._v_roll = new_instancemethod(_argus_camera.ArgusCamera__v_roll,None,ArgusCamera)
ArgusCamera._v_pitch = new_instancemethod(_argus_camera.ArgusCamera__v_pitch,None,ArgusCamera)
ArgusCamera._v_yaw = new_instancemethod(_argus_camera.ArgusCamera__v_yaw,None,ArgusCamera)
ArgusCamera_swigregister = _argus_camera.ArgusCamera_swigregister
ArgusCamera_swigregister(ArgusCamera)



