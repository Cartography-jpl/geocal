# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _spot_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_spot_camera', [dirname(__file__)])
        except ImportError:
            import _spot_camera
            return _spot_camera
        if fp is not None:
            try:
                _mod = imp.load_module('_spot_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _spot_camera = swig_import_helper()
    del swig_import_helper
else:
    import _spot_camera
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


SHARED_PTR_DISOWN = _spot_camera.SHARED_PTR_DISOWN
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

import geocal_swig.camera
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class SpotCamera(geocal_swig.camera.Camera):
    """
    This class models the SPOT camera.

    The camera parameters are given by an DIMAP file. This file is an XML
    file format. You can find documentation for this format
    athttp://www.spot.com/web/SICORP/452-sicorp-the-dimap-format.php.

    We don't actually read this file format. XML is a bit of a pain to
    deal with in C++, and is much easier to process in Ruby. So we set up
    an interface that just takes the PSI_X and PSI_Y values for each
    pixel. The Afids class Dimap processes the XML file and can use that
    to create an instance of this class.

    The various angles and so forth are described in the "SPOT Geometry
    Handbook", available athttp://www.spotimage.com/automne_modules_files
    /standard/public/p229_0b
    9c0d94a22e77aac09df2b360c73073SPOT_Geometry_Handbook.pdf

    C++ includes: spot_camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        SpotCamera::SpotCamera(const std::vector< double > &Psi_x, const std::vector< double >
        &Psi_y)
        Create a spot camera with the given PSI angles.

        Psi_x and Psi_y need to be equal in size, and have at least two
        values.

        The angles should be in radians.

        We require that Psi_y be strictly increasing. 
        """
        _spot_camera.SpotCamera_swiginit(self,_spot_camera.new_SpotCamera(*args))
    def _v_psi_x(self):
        """
        const std::vector<double> GeoCal::SpotCamera::psi_x() const

        """
        return _spot_camera.SpotCamera__v_psi_x(self)

    @property
    def psi_x(self):
        return self._v_psi_x()

    def _v_psi_y(self):
        """
        const std::vector<double> GeoCal::SpotCamera::psi_y() const

        """
        return _spot_camera.SpotCamera__v_psi_y(self)

    @property
    def psi_y(self):
        return self._v_psi_y()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.psi_x,self.psi_y)

    __swig_destroy__ = _spot_camera.delete_SpotCamera
SpotCamera._v_psi_x = new_instancemethod(_spot_camera.SpotCamera__v_psi_x,None,SpotCamera)
SpotCamera._v_psi_y = new_instancemethod(_spot_camera.SpotCamera__v_psi_y,None,SpotCamera)
SpotCamera_swigregister = _spot_camera.SpotCamera_swigregister
SpotCamera_swigregister(SpotCamera)



