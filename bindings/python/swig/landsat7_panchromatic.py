# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _landsat7_panchromatic.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_landsat7_panchromatic', [dirname(__file__)])
        except ImportError:
            import _landsat7_panchromatic
            return _landsat7_panchromatic
        if fp is not None:
            try:
                _mod = imp.load_module('_landsat7_panchromatic', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _landsat7_panchromatic = swig_import_helper()
    del swig_import_helper
else:
    import _landsat7_panchromatic
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


SHARED_PTR_DISOWN = _landsat7_panchromatic.SHARED_PTR_DISOWN
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

import geocal_swig.cart_lab_multifile
import geocal_swig.raster_multifile
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class Landsat7Panchromatic(geocal_swig.cart_lab_multifile.GdalCartLabMultifile):
    """
    This is used to read the global Landsat 7 panchromatic band.

    This data has 15 meter resolution. The geolocation accuracy was
    estimated by Tom Logan at 1/3 pixel or so (so 5m). Not sure how
    accurate this estimate is, perhaps Tom has a better estimate.

    C++ includes: landsat7_panchromatic.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        Landsat7Panchromatic::Landsat7Panchromatic(const std::string &Dir)
        Constructor.

        You can provide the directory to look for Landsat 7 panchromatic data,
        or if you leave this blank we use the value of environment variable
        LANDSAT7_PANCHROMATIC_15M. 
        """
        _landsat7_panchromatic.Landsat7Panchromatic_swiginit(self,_landsat7_panchromatic.new_Landsat7Panchromatic(*args))
    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _landsat7_panchromatic.delete_Landsat7Panchromatic
Landsat7Panchromatic_swigregister = _landsat7_panchromatic.Landsat7Panchromatic_swigregister
Landsat7Panchromatic_swigregister(Landsat7Panchromatic)



