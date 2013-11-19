# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _scale_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_scale_image', [dirname(__file__)])
        except ImportError:
            import _scale_image
            return _scale_image
        if fp is not None:
            try:
                _mod = imp.load_module('_scale_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _scale_image = swig_import_helper()
    del swig_import_helper
else:
    import _scale_image
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


SHARED_PTR_DISOWN = _scale_image.SHARED_PTR_DISOWN
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

import geocal.calc_raster
import geocal.raster_image_variable
import geocal.raster_image
import geocal.generic_object
import geocal.calc_raster_multi_band
import geocal.raster_image_multi_band_variable
import geocal.raster_image_multi_band
class ScaleImage(geocal.calc_raster.CalcRaster):
    """
    This is a simple adapter that multiples a underlying image by a scale
    factor.

    C++ includes: scale_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ScaleImage::ScaleImage(const boost::shared_ptr< RasterImage > &Data, double Scale_factor)

        """
        _scale_image.ScaleImage_swiginit(self,_scale_image.new_ScaleImage(*args))
    def _v_raw_data(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::ScaleImage::raw_data() const

        """
        return _scale_image.ScaleImage__v_raw_data(self)

    @property
    def raw_data(self):
        return self._v_raw_data()

    def _v_scale_factor(self):
        """
        double GeoCal::ScaleImage::scale_factor() const

        """
        return _scale_image.ScaleImage__v_scale_factor(self)

    @property
    def scale_factor(self):
        return self._v_scale_factor()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.raw_data,self.scale_factor)

    __swig_destroy__ = _scale_image.delete_ScaleImage
ScaleImage._v_raw_data = new_instancemethod(_scale_image.ScaleImage__v_raw_data,None,ScaleImage)
ScaleImage._v_scale_factor = new_instancemethod(_scale_image.ScaleImage__v_scale_factor,None,ScaleImage)
ScaleImage_swigregister = _scale_image.ScaleImage_swigregister
ScaleImage_swigregister(ScaleImage)

class ScaleImageMultiBand(geocal.calc_raster_multi_band.CalcRasterMultiBand):
    """
    This is a simple adapter that multiples a underlying image by a scale
    factor.

    C++ includes: scale_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ScaleImageMultiBand::ScaleImageMultiBand(const boost::shared_ptr< RasterImageMultiBand > &Data, double
        Scale_factor)

        """
        _scale_image.ScaleImageMultiBand_swiginit(self,_scale_image.new_ScaleImageMultiBand(*args))
    def _v_raw_data(self):
        """
        const boost::shared_ptr<RasterImageMultiBand>& GeoCal::ScaleImageMultiBand::raw_data() const

        """
        return _scale_image.ScaleImageMultiBand__v_raw_data(self)

    @property
    def raw_data(self):
        return self._v_raw_data()

    def _v_scale_factor(self):
        """
        double GeoCal::ScaleImageMultiBand::scale_factor() const

        """
        return _scale_image.ScaleImageMultiBand__v_scale_factor(self)

    @property
    def scale_factor(self):
        return self._v_scale_factor()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.raw_data,self.scale_factor)

    __swig_destroy__ = _scale_image.delete_ScaleImageMultiBand
ScaleImageMultiBand._v_raw_data = new_instancemethod(_scale_image.ScaleImageMultiBand__v_raw_data,None,ScaleImageMultiBand)
ScaleImageMultiBand._v_scale_factor = new_instancemethod(_scale_image.ScaleImageMultiBand__v_scale_factor,None,ScaleImageMultiBand)
ScaleImageMultiBand_swigregister = _scale_image.ScaleImageMultiBand_swigregister
ScaleImageMultiBand_swigregister(ScaleImageMultiBand)



