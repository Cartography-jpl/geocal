# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _image_mask_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_image_mask_image', [dirname(__file__)])
        except ImportError:
            import _image_mask_image
            return _image_mask_image
        if fp is not None:
            try:
                _mod = imp.load_module('_image_mask_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _image_mask_image = swig_import_helper()
    del swig_import_helper
else:
    import _image_mask_image
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
    __swig_destroy__ = _image_mask_image.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_image_mask_image.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_image_mask_image.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_image_mask_image.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_image_mask_image.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_image_mask_image.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_image_mask_image.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_image_mask_image.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_image_mask_image.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_image_mask_image.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_image_mask_image.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_image_mask_image.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_image_mask_image.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_image_mask_image.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_image_mask_image.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_image_mask_image.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_image_mask_image.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _image_mask_image.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _image_mask_image.SHARED_PTR_DISOWN
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

import geocal_swig.image_mask
import geocal_swig.generic_object
class ImageMaskImage(geocal_swig.image_mask.ImageMask):
    """
    This provides a ImageMask where the underlying data is a RasterImage.

    C++ includes: image_mask_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    OUTSIDE_MASKED = _image_mask_image.ImageMaskImage_OUTSIDE_MASKED
    OUTSIDE_NOT_MASKED = _image_mask_image.ImageMaskImage_OUTSIDE_NOT_MASKED
    OUTSIDE_ERROR = _image_mask_image.ImageMaskImage_OUTSIDE_ERROR
    def __init__(self, *args): 
        """
        GeoCal::ImageMaskImage::ImageMaskImage(boost::shared_ptr< RasterImage > Img, int Mask_value=0,
        OutsideHandling Handling=OUTSIDE_MASKED)
        Constructor.

        Values outside of the Image may or may not want to be marked as
        masked. 
        """
        _image_mask_image.ImageMaskImage_swiginit(self,_image_mask_image.new_ImageMaskImage(*args))
    def unmasked_bounding_box(self):
        """
        void ImageMaskImage::unmasked_bounding_box(int &Minline, int &Minsample, int &Numberline, int &Numbersample)
        const
        This returns the bounding box of the unmasked area.

        This can be useful for an image to chop off an outer edge that is
        completely masked. 
        """
        return _image_mask_image.ImageMaskImage_unmasked_bounding_box(self)

    def _v_raster_image(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::ImageMaskImage::raster_image_ptr() const
        Underlying RasterImage. 
        """
        return _image_mask_image.ImageMaskImage__v_raster_image(self)

    @property
    def raster_image(self):
        return self._v_raster_image()

    def _v_masked_value(self):
        """
        int GeoCal::ImageMaskImage::masked_value() const
        Value indicating masked data. 
        """
        return _image_mask_image.ImageMaskImage__v_masked_value(self)

    @property
    def masked_value(self):
        return self._v_masked_value()

    def _v_outside_handling(self):
        """
        OutsideHandling GeoCal::ImageMaskImage::outside_handling() const
        Handling for data outside of image. 
        """
        return _image_mask_image.ImageMaskImage__v_outside_handling(self)

    @property
    def outside_handling(self):
        return self._v_outside_handling()

    @classmethod
    def pickle_format_version(cls):
      return 2

    def __reduce__(self):
      return _new_from_init, (self.__class__, 2, self.raster_image,self.masked_value,self.outside_handling)

    __swig_destroy__ = _image_mask_image.delete_ImageMaskImage
ImageMaskImage.unmasked_bounding_box = new_instancemethod(_image_mask_image.ImageMaskImage_unmasked_bounding_box,None,ImageMaskImage)
ImageMaskImage._v_raster_image = new_instancemethod(_image_mask_image.ImageMaskImage__v_raster_image,None,ImageMaskImage)
ImageMaskImage._v_masked_value = new_instancemethod(_image_mask_image.ImageMaskImage__v_masked_value,None,ImageMaskImage)
ImageMaskImage._v_outside_handling = new_instancemethod(_image_mask_image.ImageMaskImage__v_outside_handling,None,ImageMaskImage)
ImageMaskImage_swigregister = _image_mask_image.ImageMaskImage_swigregister
ImageMaskImage_swigregister(ImageMaskImage)



