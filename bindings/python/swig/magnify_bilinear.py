# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _magnify_bilinear.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_magnify_bilinear', [dirname(__file__)])
        except ImportError:
            import _magnify_bilinear
            return _magnify_bilinear
        if fp is not None:
            try:
                _mod = imp.load_module('_magnify_bilinear', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _magnify_bilinear = swig_import_helper()
    del swig_import_helper
else:
    import _magnify_bilinear
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


SHARED_PTR_DISOWN = _magnify_bilinear.SHARED_PTR_DISOWN
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
import geocal_swig.image_ground_connection
import geocal_swig.geocal_exception
class MagnifyBilinear(geocal_swig.calc_raster.CalcRaster):
    """
    This creates a magnified image of a RasterImage.

    We do a bilinear interpolation to get the values

    C++ includes: magnify_bilinear.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        MagnifyBilinear::MagnifyBilinear(const boost::shared_ptr< RasterImage > &Data, int Magfactor, int
        Number_tile=4)
        Constructor. 
        """
        _magnify_bilinear.MagnifyBilinear_swiginit(self,_magnify_bilinear.new_MagnifyBilinear(*args))
    __swig_destroy__ = _magnify_bilinear.delete_MagnifyBilinear
MagnifyBilinear_swigregister = _magnify_bilinear.MagnifyBilinear_swigregister
MagnifyBilinear_swigregister(MagnifyBilinear)

class MagnifyBilinearImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnection):
    """
    This create an ImageGroundConnection that is magnified the same way
    MagnifyBilinear does.

    C++ includes: magnify_bilinear.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::MagnifyBilinearImageGroundConnection::MagnifyBilinearImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Ig_original, int
        Magfactor)
        Constructor. 
        """
        _magnify_bilinear.MagnifyBilinearImageGroundConnection_swiginit(self,_magnify_bilinear.new_MagnifyBilinearImageGroundConnection(*args))
    def cf_look_vector(self, *args):
        """
        virtual void GeoCal::MagnifyBilinearImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
        boost::shared_ptr< CartesianFixed > &P) const

        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection_cf_look_vector(self, *args)

    def _v_original_image_ground_connection(self):
        """
        boost::shared_ptr<ImageGroundConnection> GeoCal::MagnifyBilinearImageGroundConnection::original_image_ground_connection() const
        Underlying ImageGroundConnection. 
        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection__v_original_image_ground_connection(self)

    @property
    def original_image_ground_connection(self):
        return self._v_original_image_ground_connection()

    def _v_magnification_factor(self):
        """
        int GeoCal::MagnifyBilinearImageGroundConnection::magnification_factor() const
        Return magnification factor. 
        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection__v_magnification_factor(self)

    @property
    def magnification_factor(self):
        return self._v_magnification_factor()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.original_image_ground_connection,self.magnification_factor)

    __swig_destroy__ = _magnify_bilinear.delete_MagnifyBilinearImageGroundConnection
MagnifyBilinearImageGroundConnection.cf_look_vector = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection_cf_look_vector,None,MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection._v_original_image_ground_connection = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection__v_original_image_ground_connection,None,MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection._v_magnification_factor = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection__v_magnification_factor,None,MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection_swigregister = _magnify_bilinear.MagnifyBilinearImageGroundConnection_swigregister
MagnifyBilinearImageGroundConnection_swigregister(MagnifyBilinearImageGroundConnection)



