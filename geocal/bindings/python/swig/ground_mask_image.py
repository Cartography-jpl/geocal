# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ground_mask_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ground_mask_image', [dirname(__file__)])
        except ImportError:
            import _ground_mask_image
            return _ground_mask_image
        if fp is not None:
            try:
                _mod = imp.load_module('_ground_mask_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ground_mask_image = swig_import_helper()
    del swig_import_helper
else:
    import _ground_mask_image
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


SHARED_PTR_DISOWN = _ground_mask_image.SHARED_PTR_DISOWN
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

import geocal.ground_mask
import geocal.generic_object
class GroundMaskImage(geocal.ground_mask.GroundMask):
    """
    This provides a GroundMask where the underlying data is a RasterImage.

    We use the underlying map projection of the RasterImage we defining a
    region.

    For any point, we look at the four neighboring pixels. If any of the
    pixels are the mask_value, we say the point is masked, otherwise it is
    not masked. If we are outside of the RasterImage, then we say it is
    not masked or masked based on the flag passed in the constructor.

    An example of this kind of Mask is the Land/Water mask used by Vicar
    ("world_30as_lwm.img")

    C++ includes: ground_mask_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::GroundMaskImage::GroundMaskImage(boost::shared_ptr< RasterImage > Img, int Mask_value=0, bool
        Outside_is_masked=false)
        Constructor. 
        """
        _ground_mask_image.GroundMaskImage_swiginit(self,_ground_mask_image.new_GroundMaskImage(*args))
    def _v_raster_image(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::GroundMaskImage::raster_image_ptr() const
        Underlying RasterImage. 
        """
        return _ground_mask_image.GroundMaskImage__v_raster_image(self)

    @property
    def raster_image(self):
        return self._v_raster_image()

    def _v_masked_value(self):
        """
        int GeoCal::GroundMaskImage::masked_value() const
        Value indicating masked data. 
        """
        return _ground_mask_image.GroundMaskImage__v_masked_value(self)

    @property
    def masked_value(self):
        return self._v_masked_value()

    def _v_outside_is_masked(self):
        """
        bool GeoCal::GroundMaskImage::outside_is_masked() const
        Indicated if outside of the image is masked or not. 
        """
        return _ground_mask_image.GroundMaskImage__v_outside_is_masked(self)

    @property
    def outside_is_masked(self):
        return self._v_outside_is_masked()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.raster_image,self.masked_value,self.outside_is_masked)

    __swig_destroy__ = _ground_mask_image.delete_GroundMaskImage
GroundMaskImage._v_raster_image = new_instancemethod(_ground_mask_image.GroundMaskImage__v_raster_image,None,GroundMaskImage)
GroundMaskImage._v_masked_value = new_instancemethod(_ground_mask_image.GroundMaskImage__v_masked_value,None,GroundMaskImage)
GroundMaskImage._v_outside_is_masked = new_instancemethod(_ground_mask_image.GroundMaskImage__v_outside_is_masked,None,GroundMaskImage)
GroundMaskImage_swigregister = _ground_mask_image.GroundMaskImage_swigregister
GroundMaskImage_swigregister(GroundMaskImage)



