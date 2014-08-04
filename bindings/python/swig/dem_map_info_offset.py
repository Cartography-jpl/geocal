# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _dem_map_info_offset.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_dem_map_info_offset', [dirname(__file__)])
        except ImportError:
            import _dem_map_info_offset
            return _dem_map_info_offset
        if fp is not None:
            try:
                _mod = imp.load_module('_dem_map_info_offset', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _dem_map_info_offset = swig_import_helper()
    del swig_import_helper
else:
    import _dem_map_info_offset
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


SHARED_PTR_DISOWN = _dem_map_info_offset.SHARED_PTR_DISOWN
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

import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.generic_object
import geocal_swig.look_vector
class DemMapInfoOffset(geocal_swig.dem_map_info.DemMapInfo):
    """
    This applies a fixed height offset to a DemMapInfo.

    C++ includes: dem_map_info_offset.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::DemMapInfoOffset::DemMapInfoOffset(const boost::shared_ptr< DemMapInfo > &Dem_underlying, double
        Height_offset)
        Constructor.

        Height_offset should be in meters, and is added to the underlying DEM.

        """
        _dem_map_info_offset.DemMapInfoOffset_swiginit(self,_dem_map_info_offset.new_DemMapInfoOffset(*args))
    def _v_dem_underlying(self):
        """
        const boost::shared_ptr<DemMapInfo>& GeoCal::DemMapInfoOffset::dem_underlying() const
        Underlying DEM. 
        """
        return _dem_map_info_offset.DemMapInfoOffset__v_dem_underlying(self)

    @property
    def dem_underlying(self):
        return self._v_dem_underlying()

    def _v_height_offset(self):
        """
        double GeoCal::DemMapInfoOffset::height_offset() const
        Height offset in meters added to underlying DEM. 
        """
        return _dem_map_info_offset.DemMapInfoOffset__v_height_offset(self)

    @property
    def height_offset(self):
        return self._v_height_offset()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.dem_underlying,self.height_offset)

    __swig_destroy__ = _dem_map_info_offset.delete_DemMapInfoOffset
DemMapInfoOffset._v_dem_underlying = new_instancemethod(_dem_map_info_offset.DemMapInfoOffset__v_dem_underlying,None,DemMapInfoOffset)
DemMapInfoOffset._v_height_offset = new_instancemethod(_dem_map_info_offset.DemMapInfoOffset__v_height_offset,None,DemMapInfoOffset)
DemMapInfoOffset_swigregister = _dem_map_info_offset.DemMapInfoOffset_swigregister
DemMapInfoOffset_swigregister(DemMapInfoOffset)



