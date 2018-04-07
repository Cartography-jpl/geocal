# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _ogr_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_ogr_coordinate')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_ogr_coordinate')
    _ogr_coordinate = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ogr_coordinate', [dirname(__file__)])
        except ImportError:
            import _ogr_coordinate
            return _ogr_coordinate
        try:
            _mod = imp.load_module('_ogr_coordinate', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _ogr_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _ogr_coordinate
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
    __swig_destroy__ = _ogr_coordinate.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_ogr_coordinate.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ogr_coordinate.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ogr_coordinate.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ogr_coordinate.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ogr_coordinate.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ogr_coordinate.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ogr_coordinate.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ogr_coordinate.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ogr_coordinate.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ogr_coordinate.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _ogr_coordinate.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ogr_coordinate.SHARED_PTR_DISOWN

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
import geocal_swig.ground_coordinate
import geocal_swig.look_vector
import geocal_swig.coordinate_converter
import geocal_swig.dem
import geocal_swig.observer
import geocal_swig.with_parameter
class OgrWrapper(geocal_swig.generic_object.GenericObject):
    """

    This is a wrapper around the OGRSpatialReference class.

    We hold onto a OGRSpatialReference class and a transformation from
    that coordinate system to the Geodetic coordinate system. This class
    handles the lifetime of these objects. This is separated out from
    OgrCoordinate because typically we'll have lots of OgrCoordinates that
    have the same OgrWrapper.

    C++ includes: ogr_coordinate.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        OgrWrapper::OgrWrapper(const boost::shared_ptr< OGRSpatialReference > &Ogr)
        Constructor, from an existing OGRSpatialReference. 
        """
        _ogr_coordinate.OgrWrapper_swiginit(self, _ogr_coordinate.new_OgrWrapper(*args))

    def from_epsg(Epsg_id):
        """

        boost::shared_ptr< OgrWrapper > OgrWrapper::from_epsg(int Epsg_id)
        Create a OgrWrapper for a coordinate system given by the EPSG ID.

        You can look the EPSG code up for various coordinate systems
        athttp://www.epsg-registry.org 
        """
        return _ogr_coordinate.OgrWrapper_from_epsg(Epsg_id)

    from_epsg = staticmethod(from_epsg)

    def from_proj4(Proj4_string):
        """

        boost::shared_ptr< OgrWrapper > OgrWrapper::from_proj4(const std::string &Proj4_string)
        Create a OgrWrapper for a coordinate system from a Proj 4 string. 
        """
        return _ogr_coordinate.OgrWrapper_from_proj4(Proj4_string)

    from_proj4 = staticmethod(from_proj4)

    def _v_ogr(self):
        """

        const boost::shared_ptr<OGRSpatialReference>& GeoCal::OgrWrapper::ogr_ptr() const

        """
        return _ogr_coordinate.OgrWrapper__v_ogr(self)


    @property
    def ogr(self):
        return self._v_ogr()


    def _v_transform(self):
        """

        const OGRCoordinateTransformation* GeoCal::OgrWrapper::transform() const
        Return transformation that takes us from our coordinate system to
        Geodetic/Planetocentric. 
        """
        return _ogr_coordinate.OgrWrapper__v_transform(self)


    @property
    def transform(self):
        return self._v_transform()


    def _v_inverse_transform(self):
        """

        const OGRCoordinateTransformation* GeoCal::OgrWrapper::inverse_transform() const
        Return inverse of transform().

        This goes from Geodetic/Planetocentric to our coordinate system. 
        """
        return _ogr_coordinate.OgrWrapper__v_inverse_transform(self)


    @property
    def inverse_transform(self):
        return self._v_inverse_transform()


    def _v_cf_transform(self):
        """

        const OGRCoordinateTransformation* GeoCal::OgrWrapper::cf_transform() const
        Return transformation that takes us from our coordinate system to
        CartesianFixed.

        Is null for other planets. 
        """
        return _ogr_coordinate.OgrWrapper__v_cf_transform(self)


    @property
    def cf_transform(self):
        return self._v_cf_transform()


    def _v_cf_inverse_transform(self):
        """

        const OGRCoordinateTransformation* GeoCal::OgrWrapper::cf_inverse_transform() const
        Return inverse of cf_transform().

        This goes from CartesianFixed to our coordinate system. Is null for
        other planets 
        """
        return _ogr_coordinate.OgrWrapper__v_cf_inverse_transform(self)


    @property
    def cf_inverse_transform(self):
        return self._v_cf_inverse_transform()


    def _v_projected_cs_type_geo_key(self):
        """

        std::string OgrWrapper::projected_cs_type_geo_key() const
        When converting to the GEOTIFF header format used by VICAR, this is
        the value that the PROJECTEDCSTYPEGEOKEY should have.

        This is just the EPSG id written as text. 
        """
        return _ogr_coordinate.OgrWrapper__v_projected_cs_type_geo_key(self)


    @property
    def projected_cs_type_geo_key(self):
        return self._v_projected_cs_type_geo_key()


    def _v_pcs_citation_geo_key(self):
        """

        std::string OgrWrapper::pcs_citation_geo_key() const
        When converting to the GEOTIFF header format used by VICAR, this is
        the value that the PCSCitationGeoKey should have.

        This is a short text description of the projection. 
        """
        return _ogr_coordinate.OgrWrapper__v_pcs_citation_geo_key(self)


    @property
    def pcs_citation_geo_key(self):
        return self._v_pcs_citation_geo_key()


    def _v_geogcs_name(self):
        """

        std::string OgrWrapper::geogcs_name() const
        The name of the GEOGCS. 
        """
        return _ogr_coordinate.OgrWrapper__v_geogcs_name(self)


    @property
    def geogcs_name(self):
        return self._v_geogcs_name()


    def _v_wkt(self):
        """

        std::string OgrWrapper::wkt() const
        Write out a the WKT (Well Known Text) for ogr_. 
        """
        return _ogr_coordinate.OgrWrapper__v_wkt(self)


    @property
    def wkt(self):
        return self._v_wkt()


    def _v_pretty_wkt(self):
        """

        std::string OgrWrapper::pretty_wkt() const
        Write out a prettified version of the WKT for ogr_. 
        """
        return _ogr_coordinate.OgrWrapper__v_pretty_wkt(self)


    @property
    def pretty_wkt(self):
        return self._v_pretty_wkt()


    def _v_naif_code(self):
        """

        int GeoCal::OgrWrapper::naif_code() const
        Return the NAIF code for the planet this coordinate is for. 
        """
        return _ogr_coordinate.OgrWrapper__v_naif_code(self)


    @property
    def naif_code(self):
        return self._v_naif_code()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ogr_coordinate.delete_OgrWrapper
OgrWrapper._v_ogr = new_instancemethod(_ogr_coordinate.OgrWrapper__v_ogr, None, OgrWrapper)
OgrWrapper._v_transform = new_instancemethod(_ogr_coordinate.OgrWrapper__v_transform, None, OgrWrapper)
OgrWrapper._v_inverse_transform = new_instancemethod(_ogr_coordinate.OgrWrapper__v_inverse_transform, None, OgrWrapper)
OgrWrapper._v_cf_transform = new_instancemethod(_ogr_coordinate.OgrWrapper__v_cf_transform, None, OgrWrapper)
OgrWrapper._v_cf_inverse_transform = new_instancemethod(_ogr_coordinate.OgrWrapper__v_cf_inverse_transform, None, OgrWrapper)
OgrWrapper._v_projected_cs_type_geo_key = new_instancemethod(_ogr_coordinate.OgrWrapper__v_projected_cs_type_geo_key, None, OgrWrapper)
OgrWrapper._v_pcs_citation_geo_key = new_instancemethod(_ogr_coordinate.OgrWrapper__v_pcs_citation_geo_key, None, OgrWrapper)
OgrWrapper._v_geogcs_name = new_instancemethod(_ogr_coordinate.OgrWrapper__v_geogcs_name, None, OgrWrapper)
OgrWrapper._v_wkt = new_instancemethod(_ogr_coordinate.OgrWrapper__v_wkt, None, OgrWrapper)
OgrWrapper._v_pretty_wkt = new_instancemethod(_ogr_coordinate.OgrWrapper__v_pretty_wkt, None, OgrWrapper)
OgrWrapper._v_naif_code = new_instancemethod(_ogr_coordinate.OgrWrapper__v_naif_code, None, OgrWrapper)
OgrWrapper.__str__ = new_instancemethod(_ogr_coordinate.OgrWrapper___str__, None, OgrWrapper)
OgrWrapper_swigregister = _ogr_coordinate.OgrWrapper_swigregister
OgrWrapper_swigregister(OgrWrapper)

def OgrWrapper_from_epsg(Epsg_id):
    """

    boost::shared_ptr< OgrWrapper > OgrWrapper::from_epsg(int Epsg_id)
    Create a OgrWrapper for a coordinate system given by the EPSG ID.

    You can look the EPSG code up for various coordinate systems
    athttp://www.epsg-registry.org 
    """
    return _ogr_coordinate.OgrWrapper_from_epsg(Epsg_id)

def OgrWrapper_from_proj4(Proj4_string):
    """

    boost::shared_ptr< OgrWrapper > OgrWrapper::from_proj4(const std::string &Proj4_string)
    Create a OgrWrapper for a coordinate system from a Proj 4 string. 
    """
    return _ogr_coordinate.OgrWrapper_from_proj4(Proj4_string)

class OgrCoordinate(geocal_swig.ground_coordinate.GroundCoordinate):
    """

    This is a ground coordinate, implemented by the OGRSpatialReference
    class supplied with GDAL.

    This class is particularly useful for working with
    GdalMapProjectedImage or VicarMapProjectedImage. The
    OGRSpatialReference can work with a number of ways of describing a
    coordinate system, including Well Known Text format (WKT) of the
    OpenGIS Transformation specification and the EPSG specification. Note
    in particular the EPSG specification is what is used by geotiff, the
    format used by the AFIDS system.

    You can see the documentation for OGRSpatialReference
    athttp://www.gdal.org/ogr/osr_tutorial.html.

    C++ includes: ogr_coordinate.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        OgrCoordinate::OgrCoordinate(const boost::shared_ptr< OgrWrapper > &Ogr, const GroundCoordinate
        &G)
        Convert from GroundCoordinate to the coordinate system given by Ogr.

        """
        _ogr_coordinate.OgrCoordinate_swiginit(self, _ogr_coordinate.new_OgrCoordinate(*args))

    def _v_ogr(self):
        """

        const boost::shared_ptr<OgrWrapper>& GeoCal::OgrCoordinate::ogr_ptr() const
        Underlying OgrWrapper. 
        """
        return _ogr_coordinate.OgrCoordinate__v_ogr(self)


    @property
    def ogr(self):
        return self._v_ogr()


    def _v_utm_zone(self):
        """

        int OgrCoordinate::utm_zone() const
        Return the UTM zone number for the coordinate (positive for North,
        negative for South).

        If this isn't actually in UTM, then this just returns 0. 
        """
        return _ogr_coordinate.OgrCoordinate__v_utm_zone(self)


    @property
    def utm_zone(self):
        return self._v_utm_zone()

    x = _swig_property(_ogr_coordinate.OgrCoordinate_x_get, _ogr_coordinate.OgrCoordinate_x_set)
    y = _swig_property(_ogr_coordinate.OgrCoordinate_y_get, _ogr_coordinate.OgrCoordinate_y_set)
    z = _swig_property(_ogr_coordinate.OgrCoordinate_z_get, _ogr_coordinate.OgrCoordinate_z_set)

    def to_utm(Gc, zone=-999):
        """

        OgrCoordinate OgrCoordinate::to_utm(const Geodetic &Gc, int zone=-999)
        Convert a GroundCoordinate to UTM.

        If desired, you can pass in the zone number (use negative number for
        southern zones), or if left blank we determine the appropriate zone to
        use. 
        """
        return _ogr_coordinate.OgrCoordinate_to_utm(Gc, zone)

    to_utm = staticmethod(to_utm)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ogr_coordinate.delete_OgrCoordinate
OgrCoordinate._v_ogr = new_instancemethod(_ogr_coordinate.OgrCoordinate__v_ogr, None, OgrCoordinate)
OgrCoordinate._v_utm_zone = new_instancemethod(_ogr_coordinate.OgrCoordinate__v_utm_zone, None, OgrCoordinate)
OgrCoordinate_swigregister = _ogr_coordinate.OgrCoordinate_swigregister
OgrCoordinate_swigregister(OgrCoordinate)

def OgrCoordinate_to_utm(Gc, zone=-999):
    """

    OgrCoordinate OgrCoordinate::to_utm(const Geodetic &Gc, int zone=-999)
    Convert a GroundCoordinate to UTM.

    If desired, you can pass in the zone number (use negative number for
    southern zones), or if left blank we determine the appropriate zone to
    use. 
    """
    return _ogr_coordinate.OgrCoordinate_to_utm(Gc, zone)

class OgrCoordinateConverter(geocal_swig.coordinate_converter.CoordinateConverter):
    """

    This is a CoordinateConverter for working with OgrCoordinates.

    C++ includes: ogr_coordinate.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Ogr):
        """

        GeoCal::OgrCoordinateConverter::OgrCoordinateConverter(const boost::shared_ptr< OgrWrapper > Ogr)
        Constructor. 
        """
        _ogr_coordinate.OgrCoordinateConverter_swiginit(self, _ogr_coordinate.new_OgrCoordinateConverter(Ogr))

    def _v_ogr(self):
        """

        const boost::shared_ptr<OgrWrapper>& GeoCal::OgrCoordinateConverter::ogr_ptr() const
        Underlying OgrWrapper. 
        """
        return _ogr_coordinate.OgrCoordinateConverter__v_ogr(self)


    @property
    def ogr(self):
        return self._v_ogr()


    def convert_from_coordinate(self, X, Y, Height=0):
        """

        virtual boost::shared_ptr<GroundCoordinate> GeoCal::OgrCoordinateConverter::convert_from_coordinate(double X, double Y, double Height=0) const
        Create a OgrCoordinate.

        X, Y, and Height are in whatever X, Y, and Z are for the underlying
        OgrWrapper projection. 
        """
        return _ogr_coordinate.OgrCoordinateConverter_convert_from_coordinate(self, X, Y, Height)


    def utm_converter(Zone):
        """

        boost::shared_ptr< OgrCoordinateConverter > OgrCoordinateConverter::utm_converter(int Zone)
        Create a converter for UTM.

        The zone number should be positive for north, negative for south. 
        """
        return _ogr_coordinate.OgrCoordinateConverter_utm_converter(Zone)

    utm_converter = staticmethod(utm_converter)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ogr_coordinate.delete_OgrCoordinateConverter
OgrCoordinateConverter._v_ogr = new_instancemethod(_ogr_coordinate.OgrCoordinateConverter__v_ogr, None, OgrCoordinateConverter)
OgrCoordinateConverter.convert_from_coordinate = new_instancemethod(_ogr_coordinate.OgrCoordinateConverter_convert_from_coordinate, None, OgrCoordinateConverter)
OgrCoordinateConverter_swigregister = _ogr_coordinate.OgrCoordinateConverter_swigregister
OgrCoordinateConverter_swigregister(OgrCoordinateConverter)

def OgrCoordinateConverter_utm_converter(Zone):
    """

    boost::shared_ptr< OgrCoordinateConverter > OgrCoordinateConverter::utm_converter(int Zone)
    Create a converter for UTM.

    The zone number should be positive for north, negative for south. 
    """
    return _ogr_coordinate.OgrCoordinateConverter_utm_converter(Zone)



