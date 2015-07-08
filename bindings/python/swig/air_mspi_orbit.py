# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _air_mspi_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_air_mspi_orbit', [dirname(__file__)])
        except ImportError:
            import _air_mspi_orbit
            return _air_mspi_orbit
        if fp is not None:
            try:
                _mod = imp.load_module('_air_mspi_orbit', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _air_mspi_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _air_mspi_orbit
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


SHARED_PTR_DISOWN = _air_mspi_orbit.SHARED_PTR_DISOWN
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

import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class AirMspiNavData(geocal_swig.generic_object.GenericObject):
    """
    Simple structure that describes navigation data at a point in time.

    C++ includes: air_mspi_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AirMspiNavData::AirMspiNavData(const blitz::Array< double, 1 > &Raw_data, const Datum &datum, bool
        Old_format)
        Constructor, that takes raw data and create AirMspiNavData from this.

        Note that I'm not sure about the datum here, the height might be
        relative to WGS-84 already. I'll check with Mike Bull. 
        """
        _air_mspi_orbit.AirMspiNavData_swiginit(self,_air_mspi_orbit.new_AirMspiNavData(*args))
    position = _swig_property(_air_mspi_orbit.AirMspiNavData_position_get, _air_mspi_orbit.AirMspiNavData_position_set)
    velocity = _swig_property(_air_mspi_orbit.AirMspiNavData_velocity_get, _air_mspi_orbit.AirMspiNavData_velocity_set)
    ypr = _swig_property(_air_mspi_orbit.AirMspiNavData_ypr_get, _air_mspi_orbit.AirMspiNavData_ypr_set)
    ypr_rate = _swig_property(_air_mspi_orbit.AirMspiNavData_ypr_rate_get, _air_mspi_orbit.AirMspiNavData_ypr_rate_set)
    gimbal_pos = _swig_property(_air_mspi_orbit.AirMspiNavData_gimbal_pos_get, _air_mspi_orbit.AirMspiNavData_gimbal_pos_set)
    gimbal_vel = _swig_property(_air_mspi_orbit.AirMspiNavData_gimbal_vel_get, _air_mspi_orbit.AirMspiNavData_gimbal_vel_set)
    def interpolate(*args):
        """
        AirMspiNavData AirMspiNavData::interpolate(const AirMspiNavData &N1, const AirMspiNavData &N2, double f)

        """
        return _air_mspi_orbit.AirMspiNavData_interpolate(*args)

    interpolate = staticmethod(interpolate)
    __swig_destroy__ = _air_mspi_orbit.delete_AirMspiNavData
AirMspiNavData.__str__ = new_instancemethod(_air_mspi_orbit.AirMspiNavData___str__,None,AirMspiNavData)
AirMspiNavData_swigregister = _air_mspi_orbit.AirMspiNavData_swigregister
AirMspiNavData_swigregister(AirMspiNavData)

def AirMspiNavData_interpolate(*args):
  """
    AirMspiNavData AirMspiNavData::interpolate(const AirMspiNavData &N1, const AirMspiNavData &N2, double f)

    """
  return _air_mspi_orbit.AirMspiNavData_interpolate(*args)

class AirMspiOrbit(geocal_swig.orbit.Orbit):
    """
    This reads the AirMSPI orbit file.

    As an implementation detail, we use GDAL to read the data. This isn't
    really an image, but by doing this we don't need to bother developing
    all the mechanism for reading/writing HDF files. May want to revisit
    this if we actually do develop that, since there is no reason other
    than convenience to use GDAL here.

    C++ includes: air_mspi_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AirMspiOrbit::AirMspiOrbit(const std::string &Fname, const boost::shared_ptr< MspiGimbal > &Gim,
        const boost::shared_ptr< Datum > &D=boost::shared_ptr< Datum >(new
        NoDatum()), AircraftOrbitData::VerticalDefinition
        Def=AircraftOrbitData::GEODETIC_VERTICAL)
        Read the given file. 
        """
        _air_mspi_orbit.AirMspiOrbit_swiginit(self,_air_mspi_orbit.new_AirMspiOrbit(*args))
    def _v_data_version(self):
        """
        std::string GeoCal::AirMspiOrbit::data_version() const
        The data version. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_data_version(self)

    @property
    def data_version(self):
        return self._v_data_version()

    def _v_file_name(self):
        """
        std::string GeoCal::AirMspiOrbit::file_name() const
        File name for data. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_datum(self):
        """
        const boost::shared_ptr<Datum>& GeoCal::AirMspiOrbit::datum() const
        Datum the raw height is relative to. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_datum(self)

    @property
    def datum(self):
        return self._v_datum()

    def _v_flight_description(self):
        """
        std::string GeoCal::AirMspiOrbit::flight_description() const
        Flight description. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_flight_description(self)

    @property
    def flight_description(self):
        return self._v_flight_description()

    def _v_time_spacing(self):
        """
        double GeoCal::AirMspiOrbit::time_spacing() const
        Spacing in time between measurements. This is in seconds. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_time_spacing(self)

    @property
    def time_spacing(self):
        return self._v_time_spacing()

    def _v_vertical_def(self):
        """
        AircraftOrbitData::VerticalDefinition GeoCal::AirMspiOrbit::vertical_def() const
        Definition of vertical.

        This should almost always be GEODETIC_VERTICAL, but we have this in
        place to be able to match old test data. 
        """
        return _air_mspi_orbit.AirMspiOrbit__v_vertical_def(self)

    @property
    def vertical_def(self):
        return self._v_vertical_def()

    def gimbal_position(self, *args):
        """
        double GeoCal::AirMspiOrbit::gimbal_position(Time T) const
        Return the gimbal position at time T in degrees. 
        """
        return _air_mspi_orbit.AirMspiOrbit_gimbal_position(self, *args)

    def nav_data(self, *args):
        """
        AirMspiNavData AirMspiOrbit::nav_data(Time T) const
        Nav data at the given time.

        Note that we interpolate the nav data, in a way different than we do
        for the actual orbit_data. This is meant for use in various reporting
        tools Mike Bull uses, not for actually calculating the orbit data
        with. 
        """
        return _air_mspi_orbit.AirMspiOrbit_nav_data(self, *args)

    def orbit_data_index(self, *args):
        """
        boost::shared_ptr< QuaternionOrbitData > AirMspiOrbit::orbit_data_index(int Index) const

        """
        return _air_mspi_orbit.AirMspiOrbit_orbit_data_index(self, *args)

    def orbit_data(self, *args):
        """
        boost::shared_ptr< OrbitData > AirMspiOrbit::orbit_data(const TimeWithDerivative &T) const

        """
        return _air_mspi_orbit.AirMspiOrbit_orbit_data(self, *args)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _air_mspi_orbit.delete_AirMspiOrbit
AirMspiOrbit._v_data_version = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_data_version,None,AirMspiOrbit)
AirMspiOrbit._v_file_name = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_file_name,None,AirMspiOrbit)
AirMspiOrbit._v_datum = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_datum,None,AirMspiOrbit)
AirMspiOrbit._v_flight_description = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_flight_description,None,AirMspiOrbit)
AirMspiOrbit._v_time_spacing = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_time_spacing,None,AirMspiOrbit)
AirMspiOrbit._v_vertical_def = new_instancemethod(_air_mspi_orbit.AirMspiOrbit__v_vertical_def,None,AirMspiOrbit)
AirMspiOrbit.gimbal_position = new_instancemethod(_air_mspi_orbit.AirMspiOrbit_gimbal_position,None,AirMspiOrbit)
AirMspiOrbit.nav_data = new_instancemethod(_air_mspi_orbit.AirMspiOrbit_nav_data,None,AirMspiOrbit)
AirMspiOrbit.orbit_data_index = new_instancemethod(_air_mspi_orbit.AirMspiOrbit_orbit_data_index,None,AirMspiOrbit)
AirMspiOrbit.orbit_data = new_instancemethod(_air_mspi_orbit.AirMspiOrbit_orbit_data,None,AirMspiOrbit)
AirMspiOrbit_swigregister = _air_mspi_orbit.AirMspiOrbit_swigregister
AirMspiOrbit_swigregister(AirMspiOrbit)



