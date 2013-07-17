# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _worldview2_cloudmask.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_worldview2_cloudmask', [dirname(__file__)])
        except ImportError:
            import _worldview2_cloudmask
            return _worldview2_cloudmask
        if fp is not None:
            try:
                _mod = imp.load_module('_worldview2_cloudmask', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _worldview2_cloudmask = swig_import_helper()
    del swig_import_helper
else:
    import _worldview2_cloudmask
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


SHARED_PTR_DISOWN = _worldview2_cloudmask.SHARED_PTR_DISOWN
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

import geocal.calc_raster
import geocal.raster_image_variable
import geocal.raster_image
import geocal.generic_object
class WorldView2CloudMask(geocal.calc_raster.CalcRaster):
    """
    This create a cloud mask using WorldView2 mulispectral data.

    This cloud mask has two parts:

    A simple threshold on the coastal blue band to detect clouds.

    A ratio comparision of (band 8 - band 1) / band 1 < threshold to
    detect cloud shadows.

    To help with false positives, we have a minimum cloud size. This is
    used to average the data, and only if the lower resolution data shows
    a cloud or a shadow do we mark a pixel as cloudy or in shadow.

    We use reflectance data. It can be convenient to scale the reflectance
    data (e.g., go from 0 to 32767 to give half word VICAR data). This
    class doesn't care, as long as the Cloud_threshold is scaled the same
    way.

    C++ includes: worldview2_cloudmask.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    CLEAR = _worldview2_cloudmask.WorldView2CloudMask_CLEAR
    SHADOW = _worldview2_cloudmask.WorldView2CloudMask_SHADOW
    CLOUDY = _worldview2_cloudmask.WorldView2CloudMask_CLOUDY
    def __init__(self, *args): 
        """
        WorldView2CloudMask::WorldView2CloudMask(const RasterImageMultiBand &Mul_spec_data, int Min_cloud_size, double
        Cloud_threshold, double Shadow_threshold)
        Constructor. 
        """
        _worldview2_cloudmask.WorldView2CloudMask_swiginit(self,_worldview2_cloudmask.new_WorldView2CloudMask(*args))
    __swig_destroy__ = _worldview2_cloudmask.delete_WorldView2CloudMask
WorldView2CloudMask_swigregister = _worldview2_cloudmask.WorldView2CloudMask_swigregister
WorldView2CloudMask_swigregister(WorldView2CloudMask)


