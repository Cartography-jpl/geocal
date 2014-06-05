# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _raster_subsample.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raster_subsample', [dirname(__file__)])
        except ImportError:
            import _raster_subsample
            return _raster_subsample
        if fp is not None:
            try:
                _mod = imp.load_module('_raster_subsample', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _raster_subsample = swig_import_helper()
    del swig_import_helper
else:
    import _raster_subsample
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


SHARED_PTR_DISOWN = _raster_subsample.SHARED_PTR_DISOWN
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

import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class RasterSubSample(geocal_swig.raster_image_variable.RasterImageVariable):
    """
    This subsamples a higher resolution RasterImage into a lower
    resolution one.

    Typically the pixel subsampling factor doesn't exactly divide the
    number of lines and samples of the full resolution data. We just trim
    the data to the largest low resolution pixel that completely fits in
    the higher resolution data.

    C++ includes: raster_subsample.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        RasterSubSample::RasterSubSample(const boost::shared_ptr< RasterImage > &Data, int
        Number_line_per_pixel, int Number_sample_per_pixel)
        Constructor. 
        """
        _raster_subsample.RasterSubSample_swiginit(self,_raster_subsample.new_RasterSubSample(*args))
    def _v_high_resolution_image(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::RasterSubSample::high_resolution_image_ptr() const
        Pointer to high resolution image that this object is based on. 
        """
        return _raster_subsample.RasterSubSample__v_high_resolution_image(self)

    @property
    def high_resolution_image(self):
        return self._v_high_resolution_image()

    def _v_number_line_per_pixel(self):
        """
        int GeoCal::RasterSubSample::number_line_per_pixel() const
        Number of lines of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_subsample.RasterSubSample__v_number_line_per_pixel(self)

    @property
    def number_line_per_pixel(self):
        return self._v_number_line_per_pixel()

    def _v_number_sample_per_pixel(self):
        """
        int GeoCal::RasterSubSample::number_sample_per_pixel() const
        Number of samples of high resolution data per pixel of this lower
        resolution RasterImage. 
        """
        return _raster_subsample.RasterSubSample__v_number_sample_per_pixel(self)

    @property
    def number_sample_per_pixel(self):
        return self._v_number_sample_per_pixel()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.high_resolution_image,self.number_line_per_pixel,self.number_sample_per_pixel)

    __swig_destroy__ = _raster_subsample.delete_RasterSubSample
RasterSubSample._v_high_resolution_image = new_instancemethod(_raster_subsample.RasterSubSample__v_high_resolution_image,None,RasterSubSample)
RasterSubSample._v_number_line_per_pixel = new_instancemethod(_raster_subsample.RasterSubSample__v_number_line_per_pixel,None,RasterSubSample)
RasterSubSample._v_number_sample_per_pixel = new_instancemethod(_raster_subsample.RasterSubSample__v_number_sample_per_pixel,None,RasterSubSample)
RasterSubSample_swigregister = _raster_subsample.RasterSubSample_swigregister
RasterSubSample_swigregister(RasterSubSample)



