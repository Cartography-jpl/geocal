# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _quickbird_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_quickbird_camera', [dirname(__file__)])
        except ImportError:
            import _quickbird_camera
            return _quickbird_camera
        if fp is not None:
            try:
                _mod = imp.load_module('_quickbird_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _quickbird_camera = swig_import_helper()
    del swig_import_helper
else:
    import _quickbird_camera
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


SHARED_PTR_DISOWN = _quickbird_camera.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
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

import geocal.camera
import geocal.generic_object
class QuickBirdCamera(geocal.camera.PushBroomCamera):
    """
    This class models a the QuickBird camera.

    It is used to convert ScLookVector to FrameCoordinate and vice versa.

    The QuickBird camera is described by metadata supplied with the
    QuickBird data, a ".GEO" file. This is potentially dynamic, and
    potentially accounts for nonlinearities in the optics. However, in
    practice all of the GEO file we have are exactly the same, and all the
    nonlinear terms are set to 0. I'm not sure if this is because the
    camera really is very stable and the optics very linear, or just that
    DigitalGlobe didn't do a very good job at geometric camera
    calibration. In any case, rather than reading in this file we just
    hardwire the coefficients into this class. This is something we can
    revisit in the future if needed.

    This model is for the Panchromatic band only. We could easily extend
    this to other bands, but right now we only work with the Panchromatic
    data.

    C++ includes: quickbird_camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self): 
        """
        QuickBirdCamera::QuickBirdCamera()
        Constructor.

        At this point, all of the camera coefficients are hardwired in this
        class. We can change this in the future if needed. 
        """
        _quickbird_camera.QuickBirdCamera_swiginit(self,_quickbird_camera.new_QuickBirdCamera())
    __swig_destroy__ = _quickbird_camera.delete_QuickBirdCamera
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, )

QuickBirdCamera_swigregister = _quickbird_camera.QuickBirdCamera_swigregister
QuickBirdCamera_swigregister(QuickBirdCamera)


