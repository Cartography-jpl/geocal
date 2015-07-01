# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _orbit_offset_correction.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_offset_correction', [dirname(__file__)])
        except ImportError:
            import _orbit_offset_correction
            return _orbit_offset_correction
        if fp is not None:
            try:
                _mod = imp.load_module('_orbit_offset_correction', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _orbit_offset_correction = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_offset_correction
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


SHARED_PTR_DISOWN = _orbit_offset_correction.SHARED_PTR_DISOWN
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
class OrbitOffsetCorrection(geocal_swig.orbit.Orbit):
    """
    This class gives an orbit that tries to correct errors in another
    underlying orbit.

    This uses a simple error model which captures a common set of orbit
    errors.

    A time dependent correction is added to the position of the orbit.
    This correction supplies a correction at fixed time values. For times
    in between, we interpolate the position correction.

    A time dependent correction is added to the spacecraft to Cartesian
    inertial system. This correction supplies a yaw, pitch, and roll
    correction at fixed time values. For times in between we interpolate
    the quaternion correction. Note that in general the time points used
    in the attitude correction do not match the time points used in the
    position correction.

    The position offset is in meters. Right now, the attitude correction
    is in arcseconds. We may change that.

    The velocity is left unchanged.

    The underlying orbit should return a QuaternionOrbitData orbit data,
    since this is currently the only type supported.

    C++ includes: orbit_offset_correction.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        OrbitOffsetCorrection::OrbitOffsetCorrection(const boost::shared_ptr< Orbit > Orb_uncorr, bool
        Outside_is_error=false, bool Use_local_north_coordinate=false, bool
        Fit_position_x=true, bool Fit_position_y=true, bool
        Fit_position_z=true, bool Fit_yaw=true, bool Fit_pitch=true, bool
        Fit_roll=true)
        Constructor.

        This has no time points for doing corrections, but you can add those
        using insert_time_point. 
        """
        _orbit_offset_correction.OrbitOffsetCorrection_swiginit(self,_orbit_offset_correction.new_OrbitOffsetCorrection(*args))
    def _v_outside_is_error(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::outside_is_error(bool Outside_is_error)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_outside_is_error(self, *args)

    @property
    def outside_is_error(self):
        return self._v_outside_is_error()

    @outside_is_error.setter
    def outside_is_error(self, value):
      self._v_outside_is_error(value)

    def _v_fit_position_x(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_x(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_x(self, *args)

    @property
    def fit_position_x(self):
        return self._v_fit_position_x()

    @fit_position_x.setter
    def fit_position_x(self, value):
      self._v_fit_position_x(value)

    def _v_fit_position_y(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_y(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_y(self, *args)

    @property
    def fit_position_y(self):
        return self._v_fit_position_y()

    @fit_position_y.setter
    def fit_position_y(self, value):
      self._v_fit_position_y(value)

    def _v_fit_position_z(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_z(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_z(self, *args)

    @property
    def fit_position_z(self):
        return self._v_fit_position_z()

    @fit_position_z.setter
    def fit_position_z(self, value):
      self._v_fit_position_z(value)

    def _v_fit_position_e(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_e(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_e(self, *args)

    @property
    def fit_position_e(self):
        return self._v_fit_position_e()

    @fit_position_e.setter
    def fit_position_e(self, value):
      self._v_fit_position_e(value)

    def _v_fit_position_n(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_n(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_n(self, *args)

    @property
    def fit_position_n(self):
        return self._v_fit_position_n()

    @fit_position_n.setter
    def fit_position_n(self, value):
      self._v_fit_position_n(value)

    def _v_fit_position_u(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_position_u(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_u(self, *args)

    @property
    def fit_position_u(self):
        return self._v_fit_position_u()

    @fit_position_u.setter
    def fit_position_u(self, value):
      self._v_fit_position_u(value)

    def _v_use_local_north_coordinate(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::use_local_north_coordinate(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_use_local_north_coordinate(self, *args)

    @property
    def use_local_north_coordinate(self):
        return self._v_use_local_north_coordinate()

    @use_local_north_coordinate.setter
    def use_local_north_coordinate(self, value):
      self._v_use_local_north_coordinate(value)

    def _v_fit_yaw(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_yaw(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_yaw(self, *args)

    @property
    def fit_yaw(self):
        return self._v_fit_yaw()

    @fit_yaw.setter
    def fit_yaw(self, value):
      self._v_fit_yaw(value)

    def _v_fit_pitch(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_pitch(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_pitch(self, *args)

    @property
    def fit_pitch(self):
        return self._v_fit_pitch()

    @fit_pitch.setter
    def fit_pitch(self, value):
      self._v_fit_pitch(value)

    def _v_fit_roll(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::fit_roll(bool V)

        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_fit_roll(self, *args)

    @property
    def fit_roll(self):
        return self._v_fit_roll()

    @fit_roll.setter
    def fit_roll(self, value):
      self._v_fit_roll(value)

    def _v_orbit_uncorrected(self):
        """
        boost::shared_ptr<Orbit> GeoCal::OrbitOffsetCorrection::orbit_uncorrected() const
        The uncorrected orbit. 
        """
        return _orbit_offset_correction.OrbitOffsetCorrection__v_orbit_uncorrected(self)

    @property
    def orbit_uncorrected(self):
        return self._v_orbit_uncorrected()

    def update_quaterion(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::update_quaterion(int Ind, const boost::math::quaternion< double > &Q)
        Directly update the quaternion at time_point i.

        This is occasionally more convenient that updating the parameters. 
        """
        return _orbit_offset_correction.OrbitOffsetCorrection_update_quaterion(self, *args)

    def insert_attitude_time_point(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::insert_attitude_time_point(Time T_pt)
        Add a time point where we are going to do an attitude correction. 
        """
        return _orbit_offset_correction.OrbitOffsetCorrection_insert_attitude_time_point(self, *args)

    def insert_position_time_point(self, *args):
        """
        void GeoCal::OrbitOffsetCorrection::insert_position_time_point(Time T_pt)
        Add a time point where we are going to do a position correction. 
        """
        return _orbit_offset_correction.OrbitOffsetCorrection_insert_position_time_point(self, *args)

    def orbit_data(self, *args):
        """
        boost::shared_ptr< OrbitData > OrbitOffsetCorrection::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_offset_correction.OrbitOffsetCorrection_orbit_data(self, *args)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_offset_correction.delete_OrbitOffsetCorrection
OrbitOffsetCorrection._v_outside_is_error = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_outside_is_error,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_x = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_x,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_y = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_y,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_z = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_z,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_e = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_e,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_n = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_n,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_position_u = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_position_u,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_use_local_north_coordinate = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_use_local_north_coordinate,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_yaw = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_yaw,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_pitch = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_pitch,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_fit_roll = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_fit_roll,None,OrbitOffsetCorrection)
OrbitOffsetCorrection._v_orbit_uncorrected = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection__v_orbit_uncorrected,None,OrbitOffsetCorrection)
OrbitOffsetCorrection.update_quaterion = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection_update_quaterion,None,OrbitOffsetCorrection)
OrbitOffsetCorrection.insert_attitude_time_point = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection_insert_attitude_time_point,None,OrbitOffsetCorrection)
OrbitOffsetCorrection.insert_position_time_point = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection_insert_position_time_point,None,OrbitOffsetCorrection)
OrbitOffsetCorrection.orbit_data = new_instancemethod(_orbit_offset_correction.OrbitOffsetCorrection_orbit_data,None,OrbitOffsetCorrection)
OrbitOffsetCorrection_swigregister = _orbit_offset_correction.OrbitOffsetCorrection_swigregister
OrbitOffsetCorrection_swigregister(OrbitOffsetCorrection)



