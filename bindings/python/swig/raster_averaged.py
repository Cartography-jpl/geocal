# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _raster_averaged.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_averaged', [dirname(__file__)])
        except ImportError:
            import _raster_averaged
            return _raster_averaged
        if fp is not None:
            try:
                _mod = imp.load_module('_raster_averaged', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _raster_averaged = swig_import_helper()
    del swig_import_helper
else:
    import _raster_averaged
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


SHARED_PTR_DISOWN = _raster_averaged.SHARED_PTR_DISOWN
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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.calc_raster_multi_band
import geocal_swig.raster_image_multi_band_variable
import geocal_swig.raster_image_multi_band
import geocal_swig.image_mask
import geocal_swig.image_ground_connection
import geocal_swig.geocal_exception
class RasterAveraged(geocal_swig.calc_raster.CalcRaster):
    """
    This averages a higher resolution RasterImage into a lower resolution
    one.

    The data is a straight average of a given number of pixels in the line
    and sample direction of the full resolution data.

    Typically the pixel averaging factor doesn't exactly divide the number
    of lines and samples of the full resolution data. We just trim the
    data to the largest low resolution pixel that completely fits in the
    higher resolution data.

    This class calculates the average on the fly. Sometimes this is what
    you want, but if you are going to be using the resulting data a few
    times, you may want to use a MemoryRasterImage to generate a copy once
    and keep it in memory.

    C++ includes: raster_averaged.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        RasterAveraged::RasterAveraged(const boost::shared_ptr< RasterImage > &Data, int
        Number_line_per_pixel, int Number_sample_per_pixel, bool
        Ignore_zero=false)
        Constructor. 
        """
        _raster_averaged.RasterAveraged_swiginit(self,_raster_averaged.new_RasterAveraged(*args))
    def _v_high_resolution_image(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::RasterAveraged::high_resolution_image_ptr() const
        Pointer to high resolution image that this object is based on. 
        """
        return _raster_averaged.RasterAveraged__v_high_resolution_image(self)

    @property
    def high_resolution_image(self):
        return self._v_high_resolution_image()

    def _v_number_line_per_pixel(self):
        """
        int GeoCal::RasterAveraged::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.RasterAveraged__v_number_line_per_pixel(self)

    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()

    def _v_number_sample_per_pixel(self):
        """
        int GeoCal::RasterAveraged::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.RasterAveraged__v_number_sample_per_pixel(self)

    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()

    def _v_ignore_zero(self):
        """
        bool GeoCal::RasterAveraged::ignore_zero() const
        If true, we ignore zeros when calculating the average. 
        """
        return _raster_averaged.RasterAveraged__v_ignore_zero(self)

    @property
    def ignore_zero(self):
        return self._v_ignore_zero()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.high_resolution_image,self.number_line_per_pixel,self.number_sample_per_pixel,self.ignore_zero)

    __swig_destroy__ = _raster_averaged.delete_RasterAveraged
RasterAveraged._v_high_resolution_image = new_instancemethod(_raster_averaged.RasterAveraged__v_high_resolution_image,None,RasterAveraged)
RasterAveraged._v_number_line_per_pixel = new_instancemethod(_raster_averaged.RasterAveraged__v_number_line_per_pixel,None,RasterAveraged)
RasterAveraged._v_number_sample_per_pixel = new_instancemethod(_raster_averaged.RasterAveraged__v_number_sample_per_pixel,None,RasterAveraged)
RasterAveraged._v_ignore_zero = new_instancemethod(_raster_averaged.RasterAveraged__v_ignore_zero,None,RasterAveraged)
RasterAveraged_swigregister = _raster_averaged.RasterAveraged_swigregister
RasterAveraged_swigregister(RasterAveraged)

class RasterAveragedMultiBand(geocal_swig.calc_raster_multi_band.CalcRasterMultiBand):
    """
    This averages a higher resolution RasterImageMultiBand into a lower
    resolution one.

    The data is a straight average of a given number of pixels in the line
    and sample direction of the full resolution data.

    Typically the pixel averaging factor doesn't exactly divide the number
    of lines and samples of the full resolution data. We just trim the
    data to the largest low resolution pixel that completely fits in the
    higher resolution data.

    This class calculates the average on the fly. Sometimes this is what
    you want, but if you are going to be using the resulting data a few
    times, you may want to use a MemoryRasterImage to generate a copy once
    and keep it in memory.

    C++ includes: raster_averaged.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        RasterAveragedMultiBand::RasterAveragedMultiBand(const boost::shared_ptr< RasterImageMultiBand > &Data, int
        Number_line_per_pixel, int Number_sample_per_pixel, bool
        Ignore_zero=false)
        Constructor. 
        """
        _raster_averaged.RasterAveragedMultiBand_swiginit(self,_raster_averaged.new_RasterAveragedMultiBand(*args))
    def _v_high_resolution_image(self):
        """
        const boost::shared_ptr<RasterImageMultiBand>& GeoCal::RasterAveragedMultiBand::high_resolution_image_ptr() const
        Pointer to high resolution image that this object is based on. 
        """
        return _raster_averaged.RasterAveragedMultiBand__v_high_resolution_image(self)

    @property
    def high_resolution_image(self):
        return self._v_high_resolution_image()

    def _v_number_line_per_pixel(self):
        """
        int GeoCal::RasterAveragedMultiBand::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.RasterAveragedMultiBand__v_number_line_per_pixel(self)

    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()

    def _v_number_sample_per_pixel(self):
        """
        int GeoCal::RasterAveragedMultiBand::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.RasterAveragedMultiBand__v_number_sample_per_pixel(self)

    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()

    def _v_ignore_zero(self):
        """
        bool GeoCal::RasterAveragedMultiBand::ignore_zero() const
        If true, we ignore zeros when calculating the average. 
        """
        return _raster_averaged.RasterAveragedMultiBand__v_ignore_zero(self)

    @property
    def ignore_zero(self):
        return self._v_ignore_zero()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.high_resolution_image,self.number_line_per_pixel,self.number_sample_per_pixel,self.ignore_zero)

    __swig_destroy__ = _raster_averaged.delete_RasterAveragedMultiBand
RasterAveragedMultiBand._v_high_resolution_image = new_instancemethod(_raster_averaged.RasterAveragedMultiBand__v_high_resolution_image,None,RasterAveragedMultiBand)
RasterAveragedMultiBand._v_number_line_per_pixel = new_instancemethod(_raster_averaged.RasterAveragedMultiBand__v_number_line_per_pixel,None,RasterAveragedMultiBand)
RasterAveragedMultiBand._v_number_sample_per_pixel = new_instancemethod(_raster_averaged.RasterAveragedMultiBand__v_number_sample_per_pixel,None,RasterAveragedMultiBand)
RasterAveragedMultiBand._v_ignore_zero = new_instancemethod(_raster_averaged.RasterAveragedMultiBand__v_ignore_zero,None,RasterAveragedMultiBand)
RasterAveragedMultiBand_swigregister = _raster_averaged.RasterAveragedMultiBand_swigregister
RasterAveragedMultiBand_swigregister(RasterAveragedMultiBand)

class ImageMaskAveraged(geocal_swig.image_mask.ImageMask):
    """
    Take an existing image mask, and averages it like RasterAveraged.

    We mask a point if any of the points that make it up are masked. This
    does the calculation on fly.

    C++ includes: raster_averaged.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ImageMaskAveraged::ImageMaskAveraged(const boost::shared_ptr< ImageMask > &Data, int
        Number_line_per_pixel, int Number_sample_per_pixel)

        """
        _raster_averaged.ImageMaskAveraged_swiginit(self,_raster_averaged.new_ImageMaskAveraged(*args))
    def _v_high_resolution_image_mask(self):
        """
        const boost::shared_ptr<ImageMask>& GeoCal::ImageMaskAveraged::high_resolution_image_mask_ptr() const
        Pointer to high resolution image mask that this object is based on. 
        """
        return _raster_averaged.ImageMaskAveraged__v_high_resolution_image_mask(self)

    @property
    def high_resolution_image_mask(self):
        return self._v_high_resolution_image_mask()

    def _v_number_line_per_pixel(self):
        """
        int GeoCal::ImageMaskAveraged::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution ImageMatch. 
        """
        return _raster_averaged.ImageMaskAveraged__v_number_line_per_pixel(self)

    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()

    def _v_number_sample_per_pixel(self):
        """
        int GeoCal::ImageMaskAveraged::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution ImageMask. 
        """
        return _raster_averaged.ImageMaskAveraged__v_number_sample_per_pixel(self)

    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.high_resolution_image_mask,self.number_line_per_pixel,self.number_sample_per_pixel)

    __swig_destroy__ = _raster_averaged.delete_ImageMaskAveraged
ImageMaskAveraged._v_high_resolution_image_mask = new_instancemethod(_raster_averaged.ImageMaskAveraged__v_high_resolution_image_mask,None,ImageMaskAveraged)
ImageMaskAveraged._v_number_line_per_pixel = new_instancemethod(_raster_averaged.ImageMaskAveraged__v_number_line_per_pixel,None,ImageMaskAveraged)
ImageMaskAveraged._v_number_sample_per_pixel = new_instancemethod(_raster_averaged.ImageMaskAveraged__v_number_sample_per_pixel,None,ImageMaskAveraged)
ImageMaskAveraged_swigregister = _raster_averaged.ImageMaskAveraged_swigregister
ImageMaskAveraged_swigregister(ImageMaskAveraged)

class AveragedImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnection):
    """
    This takes an existing ImageGroundConnection and it averages the
    raster image, accounting for this in the ground/image calculation.

    You can either have the image averaged on the fly, or have it done
    once and kept in memory.

    C++ includes: raster_averaged.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AveragedImageGroundConnection::AveragedImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > Igc, int
        Number_line_per_pixel, int Number_sample_per_pixel, bool
        In_memory=false, bool Ignore_zero=false)
        Constructor for average.

        You can specify your choice of doing the average calculation on the
        fly, or once and kept in memory.

        Note that we set the title to the empty string, you can optionally set
        this to whatever after creating this object. 
        """
        _raster_averaged.AveragedImageGroundConnection_swiginit(self,_raster_averaged.new_AveragedImageGroundConnection(*args))
    def _v_original_image_ground_connection(self):
        """
        boost::shared_ptr<ImageGroundConnection> GeoCal::AveragedImageGroundConnection::original_image_ground_connection() const
        Underlying ImageGroundConnection. 
        """
        return _raster_averaged.AveragedImageGroundConnection__v_original_image_ground_connection(self)

    @property
    def original_image_ground_connection(self):
        return self._v_original_image_ground_connection()

    def _v_number_line_per_pixel(self):
        """
        int GeoCal::AveragedImageGroundConnection::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.AveragedImageGroundConnection__v_number_line_per_pixel(self)

    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()

    def _v_number_sample_per_pixel(self):
        """
        int GeoCal::AveragedImageGroundConnection::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_averaged.AveragedImageGroundConnection__v_number_sample_per_pixel(self)

    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()

    def _v_ignore_zero(self):
        """
        bool GeoCal::AveragedImageGroundConnection::ignore_zero() const
        If true, we ignore zeros when calculating the average. 
        """
        return _raster_averaged.AveragedImageGroundConnection__v_ignore_zero(self)

    @property
    def ignore_zero(self):
        return self._v_ignore_zero()

    def _v_in_memory(self):
        """
        bool GeoCal::AveragedImageGroundConnection::in_memory() const
        If true, calculation done once and is in memory. 
        """
        return _raster_averaged.AveragedImageGroundConnection__v_in_memory(self)

    @property
    def in_memory(self):
        return self._v_in_memory()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.original_image_ground_connection,self.number_line_per_pixel,self.number_sample_per_pixel,self.in_memory,self.ignore_zero)

    __swig_destroy__ = _raster_averaged.delete_AveragedImageGroundConnection
AveragedImageGroundConnection._v_original_image_ground_connection = new_instancemethod(_raster_averaged.AveragedImageGroundConnection__v_original_image_ground_connection,None,AveragedImageGroundConnection)
AveragedImageGroundConnection._v_number_line_per_pixel = new_instancemethod(_raster_averaged.AveragedImageGroundConnection__v_number_line_per_pixel,None,AveragedImageGroundConnection)
AveragedImageGroundConnection._v_number_sample_per_pixel = new_instancemethod(_raster_averaged.AveragedImageGroundConnection__v_number_sample_per_pixel,None,AveragedImageGroundConnection)
AveragedImageGroundConnection._v_ignore_zero = new_instancemethod(_raster_averaged.AveragedImageGroundConnection__v_ignore_zero,None,AveragedImageGroundConnection)
AveragedImageGroundConnection._v_in_memory = new_instancemethod(_raster_averaged.AveragedImageGroundConnection__v_in_memory,None,AveragedImageGroundConnection)
AveragedImageGroundConnection_swigregister = _raster_averaged.AveragedImageGroundConnection_swigregister
AveragedImageGroundConnection_swigregister(AveragedImageGroundConnection)



