# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _raw_raster_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_raw_raster_image', [dirname(__file__)])
        except ImportError:
            import _raw_raster_image
            return _raw_raster_image
        if fp is not None:
            try:
                _mod = imp.load_module('_raw_raster_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _raw_raster_image = swig_import_helper()
    del swig_import_helper
else:
    import _raw_raster_image
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


SHARED_PTR_DISOWN = _raw_raster_image.SHARED_PTR_DISOWN
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

import geocal_swig.raster_image
import geocal_swig.generic_object
class RawRasterImage(geocal_swig.raster_image.RasterImage):
    """
    This uses libraw to read RAW format.

    This is the format used by some cameras, e.g., Cannon RAW format. You
    can see the details about what can be read athttp://www.libraw.org.

    Note that although this reads the RAW format, the data is in the RAW
    space, the DN values don't mean much without translating them to RGB
    color space. You may want to instead use the program dcraw which
    converts to PPM or Tiff format and includes translating to color plus
    making 8 bit.

    C++ includes: raw_raster_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        RawRasterImage::RawRasterImage(const std::string &Fname, int Band=0)
        Read in the given file with the given Band (0 based). 
        """
        _raw_raster_image.RawRasterImage_swiginit(self,_raw_raster_image.new_RawRasterImage(*args))
    def read_all(*args):
        """
        std::vector< boost::shared_ptr< RasterImage > > RawRasterImage::read_all(const std::string &Fname)
        These read all three bands of a file at once.

        This is faster than reading each band separately. 
        """
        return _raw_raster_image.RawRasterImage_read_all(*args)

    read_all = staticmethod(read_all)
    def _v_file_name(self):
        """
        const std::string& GeoCal::RawRasterImage::file_name() const

        """
        return _raw_raster_image.RawRasterImage__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_band(self):
        """
        int GeoCal::RawRasterImage::band() const

        """
        return _raw_raster_image.RawRasterImage__v_band(self)

    @property
    def band(self):
        return self._v_band()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.band)

    __swig_destroy__ = _raw_raster_image.delete_RawRasterImage
RawRasterImage._v_file_name = new_instancemethod(_raw_raster_image.RawRasterImage__v_file_name,None,RawRasterImage)
RawRasterImage._v_band = new_instancemethod(_raw_raster_image.RawRasterImage__v_band,None,RawRasterImage)
RawRasterImage_swigregister = _raw_raster_image.RawRasterImage_swigregister
RawRasterImage_swigregister(RawRasterImage)

def RawRasterImage_read_all(*args):
  """
    std::vector< boost::shared_ptr< RasterImage > > RawRasterImage::read_all(const std::string &Fname)
    These read all three bands of a file at once.

    This is faster than reading each band separately. 
    """
  return _raw_raster_image.RawRasterImage_read_all(*args)



