# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _vicar_ogr.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_ogr', [dirname(__file__)])
        except ImportError:
            import _vicar_ogr
            return _vicar_ogr
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_ogr', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_ogr = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_ogr
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
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

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _vicar_ogr.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_vicar_ogr.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_vicar_ogr.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_vicar_ogr.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_vicar_ogr.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_vicar_ogr.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_vicar_ogr.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_vicar_ogr.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_vicar_ogr.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_vicar_ogr.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_vicar_ogr.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_vicar_ogr.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_vicar_ogr.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_vicar_ogr.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_vicar_ogr.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_vicar_ogr.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_vicar_ogr.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _vicar_ogr.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_vicar_ogr.SHARED_PTR_DISOWN_swigconstant(_vicar_ogr)
SHARED_PTR_DISOWN = _vicar_ogr.SHARED_PTR_DISOWN

import os

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

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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

import geocal_swig.generic_object
import geocal_swig.raster_image
import geocal_swig.dem_map_info
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class VicarOgr(geocal_swig.generic_object.GenericObject):
    """

    This class is really part of VicarFile, but because of the
    complication in this software we separate this out into its own class.

    This class handles the reading and writing of the GeoTIFF map
    projection and coordinate transformation information, going to and
    from a MapInfo.

    AFIDS stores map projection information as text labels in a VICAR
    file. The text is GeoTIFF tags stored as text. We can't directly work
    with these to supply map projection information. Instead, we want to
    use the GDAL library to handle this (through the OgrCoordinate class).
    However, there is no easy mapping between GDAL which use the Well
    Known Text (WKT) to express its coordinate information and GeoTIFF
    which uses about 40 different tags for this information.

    The two systems contain similar information, so one possible approach
    would be to create a mapping between the two systems - e.g., Tag X
    corresponds to WKT node Y. While possible, this would result in a
    large amount of code.

    As an alternative, we take advantage of the ability of GDAL to create
    and read GeoTIFF files. The GDAL library contains all of the code
    connecting the two, which we don't want to duplicate.

    This class creates a temporary GeoTIFF file, and either writes map
    projection information using GDAL and a MapInfo, or from the VICAR
    GeoTIFF information. We then read the file and go the other way,
    creating a MapInfo or the metadata for a VICAR file. The temporary
    file is then removed.

    This is a bit awkward, but this is the best approach I could come up
    with to map VICAR and GDAL together.

    C++ includes: vicar_ogr.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        """

        VicarOgr::VicarOgr()
        Constructor.

        Normally we only have one of these objects around, although nothing is
        hurt if you create multiple copies. 
        """
        _vicar_ogr.VicarOgr_swiginit(self, _vicar_ogr.new_VicarOgr())

    def vicar_to_gtiff(self, *args):
        """

        void VicarOgr::vicar_to_gtiff(const VicarLiteFile &F, const std::string &Fname)
        Create a 1 pixel geotiff file that contains all the geotiff labels
        from the Vicar file. 
        """
        return _vicar_ogr.VicarOgr_vicar_to_gtiff(self, *args)


    def from_vicar(self, *args):
        """

        MapInfo VicarOgr::from_vicar(const VicarLiteFile &F)
        Read the metadata from a VicarLiteFile, and use to create a MapInfo
        based on GDAL. 
        """
        return _vicar_ogr.VicarOgr_from_vicar(self, *args)


    def to_vicar(self, Mi, F):
        """

        void VicarOgr::to_vicar(const MapInfo &Mi, VicarFile &F)
        Use a MapInfo to write the GeoTIFF metadata for a Vicar File. 
        """
        return _vicar_ogr.VicarOgr_to_vicar(self, Mi, F)

    __swig_destroy__ = _vicar_ogr.delete_VicarOgr
VicarOgr.vicar_to_gtiff = new_instancemethod(_vicar_ogr.VicarOgr_vicar_to_gtiff, None, VicarOgr)
VicarOgr.from_vicar = new_instancemethod(_vicar_ogr.VicarOgr_from_vicar, None, VicarOgr)
VicarOgr.to_vicar = new_instancemethod(_vicar_ogr.VicarOgr_to_vicar, None, VicarOgr)
VicarOgr_swigregister = _vicar_ogr.VicarOgr_swigregister
VicarOgr_swigregister(VicarOgr)


__all__ = ["VicarOgr"]



