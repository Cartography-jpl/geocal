# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _dem_map_info.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_dem_map_info', [dirname(__file__)])
        except ImportError:
            import _dem_map_info
            return _dem_map_info
        if fp is not None:
            try:
                _mod = imp.load_module('_dem_map_info', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _dem_map_info = swig_import_helper()
    del swig_import_helper
else:
    import _dem_map_info
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


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _dem_map_info.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_dem_map_info.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_dem_map_info.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_dem_map_info.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_dem_map_info.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_dem_map_info.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_dem_map_info.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_dem_map_info.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_dem_map_info.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_dem_map_info.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_dem_map_info.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_dem_map_info.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_dem_map_info.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_dem_map_info.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_dem_map_info.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_dem_map_info.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_dem_map_info.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _dem_map_info.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _dem_map_info.SHARED_PTR_DISOWN
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

import geocal_swig.dem
import geocal_swig.generic_object
class DemMapInfo(geocal_swig.dem.Dem):
    """
    C++ includes: dem_map_info.h

    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def _v_datum(self):
        """
        const boost::shared_ptr<Datum> GeoCal::DemMapInfo::datum_ptr() const
        Pointer to datum. 
        """
        return _dem_map_info.DemMapInfo__v_datum(self)

    @property
    def datum(self):
        return self._v_datum()

    def _v_map_info(self):
        """
        const MapInfo& GeoCal::DemMapInfo::map_info() const
        MapInfo of underlying data. 
        """
        return _dem_map_info.DemMapInfo__v_map_info(self)

    @property
    def map_info(self):
        return self._v_map_info()

    def height_datum(self, *args):
        """
        double DemMapInfo::height_datum(const Geodetic &Gp) const

        """
        return _dem_map_info.DemMapInfo_height_datum(self, *args)

    def _v_outside_dem_is_error(self):
        """
        bool GeoCal::DemMapInfo::outside_dem_is_error() const
        If true, then calling with values outside the range of area of the DEM
        triggers an exception, otherwise we just return a height of 0. 
        """
        return _dem_map_info.DemMapInfo__v_outside_dem_is_error(self)

    @property
    def outside_dem_is_error(self):
        return self._v_outside_dem_is_error()

    __swig_destroy__ = _dem_map_info.delete_DemMapInfo
DemMapInfo._v_datum = new_instancemethod(_dem_map_info.DemMapInfo__v_datum,None,DemMapInfo)
DemMapInfo._v_map_info = new_instancemethod(_dem_map_info.DemMapInfo__v_map_info,None,DemMapInfo)
DemMapInfo.height_datum = new_instancemethod(_dem_map_info.DemMapInfo_height_datum,None,DemMapInfo)
DemMapInfo._v_outside_dem_is_error = new_instancemethod(_dem_map_info.DemMapInfo__v_outside_dem_is_error,None,DemMapInfo)
DemMapInfo_swigregister = _dem_map_info.DemMapInfo_swigregister
DemMapInfo_swigregister(DemMapInfo)



