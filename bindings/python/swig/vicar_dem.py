# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.4
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _vicar_dem.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_dem', [dirname(__file__)])
        except ImportError:
            import _vicar_dem
            return _vicar_dem
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_dem', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_dem = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_dem
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


SHARED_PTR_DISOWN = _vicar_dem.SHARED_PTR_DISOWN
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

import geocal_swig.dem_tiled_file
import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.generic_object
import geocal_swig.geocal_exception
class VicarDem(geocal_swig.dem_tiled_file.DemTiledFile):
    """
    This uses a VicarTiledFile<T> as a DEM.

    The type of T should be one we can convert to a double, and should
    give the height in meters above sea level. We do a bilinear
    interpolation to get values between points, and we optionally return a
    height of 0 outside of the range of where we have data (or
    alternatively, throw an exception).

    The default datum used is NoDatum, e.g., the heights in the VICAR file
    are relative to WGS-84. If desired, a different Datum can be given.

    C++ includes: vicar_dem.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::VicarDem::VicarDem(const std::string &Fname, bool Outside_dem_is_error=false, int
        Number_line_per_tile=100, int Number_tile=4, const boost::shared_ptr<
        Datum > &D=boost::shared_ptr< Datum >(new NoDatum()))
        Open VICAR file as a DEM. 
        """
        _vicar_dem.VicarDem_swiginit(self,_vicar_dem.new_VicarDem(*args))
    def _v_vicar_file(self):
        """
        boost::shared_ptr<VicarFile> GeoCal::VicarDem::vicar_file_ptr() const

        """
        return _vicar_dem.VicarDem__v_vicar_file(self)

    @property
    def vicar_file(self):
        return self._v_vicar_file()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.vicar_file.file_name,self.outside_dem_is_error,self.number_line_per_tile,self.number_tile,self.datum)

    __swig_destroy__ = _vicar_dem.delete_VicarDem
VicarDem._v_vicar_file = new_instancemethod(_vicar_dem.VicarDem__v_vicar_file,None,VicarDem)
VicarDem.elevation = new_instancemethod(_vicar_dem.VicarDem_elevation,None,VicarDem)
VicarDem_swigregister = _vicar_dem.VicarDem_swigregister
VicarDem_swigregister(VicarDem)



