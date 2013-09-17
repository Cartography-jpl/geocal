# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _map_reprojected_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_map_reprojected_image', [dirname(__file__)])
        except ImportError:
            import _map_reprojected_image
            return _map_reprojected_image
        if fp is not None:
            try:
                _mod = imp.load_module('_map_reprojected_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _map_reprojected_image = swig_import_helper()
    del swig_import_helper
else:
    import _map_reprojected_image
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


SHARED_PTR_DISOWN = _map_reprojected_image.SHARED_PTR_DISOWN
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

import geocal.raster_image_variable
import geocal.raster_image
import geocal.generic_object
class MapReprojectedImage(geocal.raster_image_variable.RasterImageVariable):
    """
    This is a RasterImage that has been resampled to a different map
    projection and/or a different resolution.

    To do this we do two steps:

    We calculate roughly what the difference in resolution is between the
    original and final MapInfo. We do this by looking at the center pixel
    of the original data and the pixel +1 in line and sample. We then use
    RasterAveraged to average the original data to roughly the resolution
    of the final MapInfo. If the final MapInfo is near the same resolution
    as the original, or if it has a higher resolution, then we don't do
    any averaging.

    We then interpolate the possibly averaged data to the final
    projection.

    It is ok if the final MapInfo contains areas outside of the original
    data. For any pixel outside of the original data, we just return a
    value of 0.

    This class calculates the reprojected data on the fly. Sometimes this
    is what you want, but if you are going to be using the resulting data
    a few times, you may want to use a MemoryRasterImage to generate a
    copy once and keep it in memory.

    C++ includes: map_reprojected_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        MapReprojectedImage::MapReprojectedImage(const boost::shared_ptr< RasterImage > Img_in, const MapInfo &Mi)
        Constructor.

        We resample Img_in to the projection and resolution given by MapInfo.

        """
        _map_reprojected_image.MapReprojectedImage_swiginit(self,_map_reprojected_image.new_MapReprojectedImage(*args))
    def _v_raw_image(self):
        """
        boost::shared_ptr<RasterImage> GeoCal::MapReprojectedImage::raw_image_ptr() const

        """
        return _map_reprojected_image.MapReprojectedImage__v_raw_image(self)

    @property
    def raw_image(self):
        return self._v_raw_image()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.raw_image,self.map_info)

    __swig_destroy__ = _map_reprojected_image.delete_MapReprojectedImage
MapReprojectedImage._v_raw_image = new_instancemethod(_map_reprojected_image.MapReprojectedImage__v_raw_image,None,MapReprojectedImage)
MapReprojectedImage.write = new_instancemethod(_map_reprojected_image.MapReprojectedImage_write,None,MapReprojectedImage)
MapReprojectedImage_swigregister = _map_reprojected_image.MapReprojectedImage_swigregister
MapReprojectedImage_swigregister(MapReprojectedImage)



