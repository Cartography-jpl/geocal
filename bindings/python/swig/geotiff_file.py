# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _geotiff_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_geotiff_file')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_geotiff_file')
    _geotiff_file = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geotiff_file', [dirname(__file__)])
        except ImportError:
            import _geotiff_file
            return _geotiff_file
        try:
            _mod = imp.load_module('_geotiff_file', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _geotiff_file = swig_import_helper()
    del swig_import_helper
else:
    import _geotiff_file
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

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


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


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
except __builtin__.Exception:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _geotiff_file.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_geotiff_file.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geotiff_file.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geotiff_file.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geotiff_file.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geotiff_file.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geotiff_file.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geotiff_file.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geotiff_file.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geotiff_file.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geotiff_file.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geotiff_file.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geotiff_file.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geotiff_file.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geotiff_file.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geotiff_file.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geotiff_file.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _geotiff_file.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _geotiff_file.SHARED_PTR_DISOWN

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
class GeotiffFile(geocal_swig.generic_object.GenericObject):
    """

    Provide direct access to a GeoTiff file.

    Note that this is a pretty low level class, you almost certainly don't
    want to be using this but instead want something like a
    GdalRasterImage. But this class if useful if you need to do low level
    things with a geotiff file like directly read and write geotiff tags.

    A note on the hardcode values/protypes:

    Normally we wouldn't have prototypes defined directly in this class.
    However, we have a special case. We know that GDAL will supply the
    GeoTIFF library, because if it isn't found on the system GDAL will use
    its own private copy. However, we don't know that the GeoTIFF headers
    will be available. GDAL doesn't install them, and unless there is a
    another copy of GeoTIFF available they won't be found. Since it is
    entirely possible to have GDAL installed but GeoTIFF not, we don't
    want to add a dependency on this library that we don't really need.
    So, we define the prototypes here. There is a risk that these
    prototypes will become out of date, in which case we'll need to update
    these. But the GeoTIFF library hasn't changed in some time, so the
    trade against adding a dependency is probably a good one.

    C++ includes: geotiff_file.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    GTModelTypeGeoKey = _geotiff_file.GeotiffFile_GTModelTypeGeoKey
    GTRasterTypeGeoKey = _geotiff_file.GeotiffFile_GTRasterTypeGeoKey
    GTCitationGeoKey = _geotiff_file.GeotiffFile_GTCitationGeoKey
    GeographicTypeGeoKey = _geotiff_file.GeotiffFile_GeographicTypeGeoKey
    GeogCitationGeoKey = _geotiff_file.GeotiffFile_GeogCitationGeoKey
    GeogGeodeticDatumGeoKey = _geotiff_file.GeotiffFile_GeogGeodeticDatumGeoKey
    GeogPrimeMeridianGeoKey = _geotiff_file.GeotiffFile_GeogPrimeMeridianGeoKey
    GeogLinearUnitsGeoKey = _geotiff_file.GeotiffFile_GeogLinearUnitsGeoKey
    GeogLinearUnitSizeGeoKey = _geotiff_file.GeotiffFile_GeogLinearUnitSizeGeoKey
    GeogAngularUnitsGeoKey = _geotiff_file.GeotiffFile_GeogAngularUnitsGeoKey
    GeogAngularUnitSizeGeoKey = _geotiff_file.GeotiffFile_GeogAngularUnitSizeGeoKey
    GeogEllipsoidGeoKey = _geotiff_file.GeotiffFile_GeogEllipsoidGeoKey
    GeogSemiMajorAxisGeoKey = _geotiff_file.GeotiffFile_GeogSemiMajorAxisGeoKey
    GeogSemiMinorAxisGeoKey = _geotiff_file.GeotiffFile_GeogSemiMinorAxisGeoKey
    GeogInvFlatteningGeoKey = _geotiff_file.GeotiffFile_GeogInvFlatteningGeoKey
    GeogAzimuthUnitsGeoKey = _geotiff_file.GeotiffFile_GeogAzimuthUnitsGeoKey
    GeogPrimeMeridianLongGeoKey = _geotiff_file.GeotiffFile_GeogPrimeMeridianLongGeoKey
    ProjectedCSTypeGeoKey = _geotiff_file.GeotiffFile_ProjectedCSTypeGeoKey
    PCSCitationGeoKey = _geotiff_file.GeotiffFile_PCSCitationGeoKey
    ProjectionGeoKey = _geotiff_file.GeotiffFile_ProjectionGeoKey
    ProjCoordTransGeoKey = _geotiff_file.GeotiffFile_ProjCoordTransGeoKey
    ProjLinearUnitsGeoKey = _geotiff_file.GeotiffFile_ProjLinearUnitsGeoKey
    ProjLinearUnitSizeGeoKey = _geotiff_file.GeotiffFile_ProjLinearUnitSizeGeoKey
    ProjStdParallel1GeoKey = _geotiff_file.GeotiffFile_ProjStdParallel1GeoKey
    ProjStdParallelGeoKey = _geotiff_file.GeotiffFile_ProjStdParallelGeoKey
    ProjStdParallel2GeoKey = _geotiff_file.GeotiffFile_ProjStdParallel2GeoKey
    ProjNatOriginLongGeoKey = _geotiff_file.GeotiffFile_ProjNatOriginLongGeoKey
    ProjOriginLongGeoKey = _geotiff_file.GeotiffFile_ProjOriginLongGeoKey
    ProjNatOriginLatGeoKey = _geotiff_file.GeotiffFile_ProjNatOriginLatGeoKey
    ProjOriginLatGeoKey = _geotiff_file.GeotiffFile_ProjOriginLatGeoKey
    ProjFalseEastingGeoKey = _geotiff_file.GeotiffFile_ProjFalseEastingGeoKey
    ProjFalseNorthingGeoKey = _geotiff_file.GeotiffFile_ProjFalseNorthingGeoKey
    ProjFalseOriginLongGeoKey = _geotiff_file.GeotiffFile_ProjFalseOriginLongGeoKey
    ProjFalseOriginLatGeoKey = _geotiff_file.GeotiffFile_ProjFalseOriginLatGeoKey
    ProjFalseOriginEastingGeoKey = _geotiff_file.GeotiffFile_ProjFalseOriginEastingGeoKey
    ProjFalseOriginNorthingGeoKey = _geotiff_file.GeotiffFile_ProjFalseOriginNorthingGeoKey
    ProjCenterLongGeoKey = _geotiff_file.GeotiffFile_ProjCenterLongGeoKey
    ProjCenterLatGeoKey = _geotiff_file.GeotiffFile_ProjCenterLatGeoKey
    ProjCenterEastingGeoKey = _geotiff_file.GeotiffFile_ProjCenterEastingGeoKey
    ProjCenterNorthingGeoKey = _geotiff_file.GeotiffFile_ProjCenterNorthingGeoKey
    ProjScaleAtNatOriginGeoKey = _geotiff_file.GeotiffFile_ProjScaleAtNatOriginGeoKey
    ProjScaleAtOriginGeoKey = _geotiff_file.GeotiffFile_ProjScaleAtOriginGeoKey
    ProjScaleAtCenterGeoKey = _geotiff_file.GeotiffFile_ProjScaleAtCenterGeoKey
    ProjAzimuthAngleGeoKey = _geotiff_file.GeotiffFile_ProjAzimuthAngleGeoKey
    ProjStraightVertPoleLongGeoKey = _geotiff_file.GeotiffFile_ProjStraightVertPoleLongGeoKey
    ProjRectifiedGridAngleGeoKey = _geotiff_file.GeotiffFile_ProjRectifiedGridAngleGeoKey
    VerticalCSTypeGeoKey = _geotiff_file.GeotiffFile_VerticalCSTypeGeoKey
    VerticalCitationGeoKey = _geotiff_file.GeotiffFile_VerticalCitationGeoKey
    VerticalDatumGeoKey = _geotiff_file.GeotiffFile_VerticalDatumGeoKey
    VerticalUnitsGeoKey = _geotiff_file.GeotiffFile_VerticalUnitsGeoKey
    TYPE_BYTE = _geotiff_file.GeotiffFile_TYPE_BYTE
    TYPE_SHORT = _geotiff_file.GeotiffFile_TYPE_SHORT
    TYPE_LONG = _geotiff_file.GeotiffFile_TYPE_LONG
    TYPE_RATIONAL = _geotiff_file.GeotiffFile_TYPE_RATIONAL
    TYPE_ASCII = _geotiff_file.GeotiffFile_TYPE_ASCII
    TYPE_FLOAT = _geotiff_file.GeotiffFile_TYPE_FLOAT
    TYPE_DOUBLE = _geotiff_file.GeotiffFile_TYPE_DOUBLE
    TYPE_SBYTE = _geotiff_file.GeotiffFile_TYPE_SBYTE
    TYPE_SSHORT = _geotiff_file.GeotiffFile_TYPE_SSHORT
    TYPE_SLONG = _geotiff_file.GeotiffFile_TYPE_SLONG
    TYPE_UNKNOWN = _geotiff_file.GeotiffFile_TYPE_UNKNOWN

    def __init__(self, Fname, Mode):
        """

        GeoCal::GeotiffFile::GeotiffFile(const std::string &Fname, const std::string &Mode)
        Open given file name with the given mode.

        Mode should be "r", "w", or "a". 
        """
        _geotiff_file.GeotiffFile_swiginit(self, _geotiff_file.new_GeotiffFile(Fname, Mode))

    def _v_file_name(self):
        """

        const std::string& GeoCal::GeotiffFile::file_name() const
        Return the file name we have open. 
        """
        return _geotiff_file.GeotiffFile__v_file_name(self)


    @property
    def file_name(self):
        return self._v_file_name()


    def _v_mode(self):
        """

        const std::string& GeoCal::GeotiffFile::mode() const
        Return mode the file was opened with. 
        """
        return _geotiff_file.GeotiffFile__v_mode(self)


    @property
    def mode(self):
        return self._v_mode()


    def key_type(K):
        """

        GeotiffFile::tagtype_t GeotiffFile::key_type(geokey_t K)
        Return the type for the given key. 
        """
        return _geotiff_file.GeotiffFile_key_type(K)

    key_type = staticmethod(key_type)

    def geotiff_tag_ascii():
        """

        const std::vector< int > & GeotiffFile::geotiff_tag_ascii()
        Return the list of tags that take ASCII data. 
        """
        return _geotiff_file.GeotiffFile_geotiff_tag_ascii()

    geotiff_tag_ascii = staticmethod(geotiff_tag_ascii)

    def geotiff_tag_double():
        """

        const std::vector< int > & GeotiffFile::geotiff_tag_double()
        Return the list of tags that take double data. 
        """
        return _geotiff_file.GeotiffFile_geotiff_tag_double()

    geotiff_tag_double = staticmethod(geotiff_tag_double)

    def geotiff_tag_short():
        """

        const std::vector< int > & GeotiffFile::geotiff_tag_short()
        Return the list of tags that take short data. 
        """
        return _geotiff_file.GeotiffFile_geotiff_tag_short()

    geotiff_tag_short = staticmethod(geotiff_tag_short)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _geotiff_file.delete_GeotiffFile
GeotiffFile._v_file_name = new_instancemethod(_geotiff_file.GeotiffFile__v_file_name, None, GeotiffFile)
GeotiffFile._v_mode = new_instancemethod(_geotiff_file.GeotiffFile__v_mode, None, GeotiffFile)
GeotiffFile.__str__ = new_instancemethod(_geotiff_file.GeotiffFile___str__, None, GeotiffFile)
GeotiffFile_swigregister = _geotiff_file.GeotiffFile_swigregister
GeotiffFile_swigregister(GeotiffFile)

def GeotiffFile_key_type(K):
    """

    GeotiffFile::tagtype_t GeotiffFile::key_type(geokey_t K)
    Return the type for the given key. 
    """
    return _geotiff_file.GeotiffFile_key_type(K)

def GeotiffFile_geotiff_tag_ascii():
    """

    const std::vector< int > & GeotiffFile::geotiff_tag_ascii()
    Return the list of tags that take ASCII data. 
    """
    return _geotiff_file.GeotiffFile_geotiff_tag_ascii()

def GeotiffFile_geotiff_tag_double():
    """

    const std::vector< int > & GeotiffFile::geotiff_tag_double()
    Return the list of tags that take double data. 
    """
    return _geotiff_file.GeotiffFile_geotiff_tag_double()

def GeotiffFile_geotiff_tag_short():
    """

    const std::vector< int > & GeotiffFile::geotiff_tag_short()
    Return the list of tags that take short data. 
    """
    return _geotiff_file.GeotiffFile_geotiff_tag_short()


__all__ = ["GeotiffFile"]



