# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _image_point_display.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_image_point_display', [dirname(__file__)])
        except ImportError:
            import _image_point_display
            return _image_point_display
        if fp is not None:
            try:
                _mod = imp.load_module('_image_point_display', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _image_point_display = swig_import_helper()
    del swig_import_helper
else:
    import _image_point_display
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


SHARED_PTR_DISOWN = _image_point_display.SHARED_PTR_DISOWN
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

import geocal.gdal_raster_image
import geocal.raster_image_tiled_file
import geocal.raster_image_variable
import geocal.raster_image
import geocal.generic_object
import geocal.geocal_exception
class ImagePointDisplay(geocal.gdal_raster_image.GdalRasterImage):
    """
    This class is used to generate a display of image points.

    This is useful for example to show the difference between initial and
    final location during image matching

    C++ includes: image_point_display.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ImagePointDisplay::ImagePointDisplay(const std::string &Fname, const std::string &Driver_name, int
        Number_line, int Number_sample, int Dt, const std::string
        &Options="", int Number_tile=4, int Tile_number_line=-1, int
        Tile_number_sample=-1)
        Constructor to write a new file. 
        """
        _image_point_display.ImagePointDisplay_swiginit(self,_image_point_display.new_ImagePointDisplay(*args))
    def image_point(self, *args):
        """
        void GeoCal::ImagePointDisplay::image_point(const GroundCoordinate &Gc, int Cindex)
        Display Point with the given color index. 
        """
        return _image_point_display.ImagePointDisplay_image_point(self, *args)

    __swig_destroy__ = _image_point_display.delete_ImagePointDisplay
ImagePointDisplay.image_point = new_instancemethod(_image_point_display.ImagePointDisplay_image_point,None,ImagePointDisplay)
ImagePointDisplay_swigregister = _image_point_display.ImagePointDisplay_swigregister
ImagePointDisplay_swigregister(ImagePointDisplay)



