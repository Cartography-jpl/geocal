# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit', [dirname(__file__)])
        except ImportError:
            import _orbit
            return _orbit
        if fp is not None:
            try:
                _mod = imp.load_module('_orbit', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _orbit = swig_import_helper()
    del swig_import_helper
else:
    import _orbit
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
    __swig_destroy__ = _orbit.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_orbit.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit.SHARED_PTR_DISOWN
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

import geocal_swig.generic_object
class OrbitData(geocal_swig.generic_object.GenericObject):
    """
    This class is used to convert ScLookVector,
    CartesianInertialLookVector and CartesianFixedLookVector to and from
    each other at a given time.

    C++ includes: orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def resolution_meter(self, *args):
        """
        double OrbitData::resolution_meter(const Camera &C, const FrameCoordinate &Fc, int Band=0) const
        Calculate the approximate resolution on the ground of a given Camera
        for this OrbitData.

        This finds the intersection with the reference surface for the given
        pixel of the camera, + 1 in the line and sample direction. We find the
        difference in meters between these points, and select the maximum
        value. 
        """
        return _orbit.OrbitData_resolution_meter(self, *args)

    def ci_look_vector(self, *args):
        """
        virtual CartesianInertialLookVector GeoCal::OrbitData::ci_look_vector(const ScLookVector &Sl) const =0
        Convert from ScLookVector to CartesianInertialLookVector. 
        """
        return _orbit.OrbitData_ci_look_vector(self, *args)

    def cf_look_vector(self, *args):
        """
        virtual CartesianFixedLookVector GeoCal::OrbitData::cf_look_vector(const ScLookVector &Sl) const =0
        Convert from ScLookVector to CartesianFixedLookVector. 
        """
        return _orbit.OrbitData_cf_look_vector(self, *args)

    def sc_look_vector(self, *args):
        """
        virtual ScLookVector GeoCal::OrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const =0
        Convert from CartesianFixedLookVector to ScLookVector. 
        """
        return _orbit.OrbitData_sc_look_vector(self, *args)

    def frame_coordinate(self, *args):
        """
        FrameCoordinate OrbitData::frame_coordinate(const GroundCoordinate &Gc, const Camera &C, int Band=0) const
        Give the frame coordinates that a particular point on the ground is
        seen. 
        """
        return _orbit.OrbitData_frame_coordinate(self, *args)

    def reference_surface_intersect_approximate(self, *args):
        """
        boost::shared_ptr< CartesianFixed > OrbitData::reference_surface_intersect_approximate(const Camera &C, const FrameCoordinate &Fc, int Band=0, double
        Height_reference_surface=0.0) const
        Return location on the reference surface that a particular frame
        coordinate is seen.

        This is approximate, in the same way
        CartesianFixed::reference_intersect_approximate is approximate. 
        """
        return _orbit.OrbitData_reference_surface_intersect_approximate(self, *args)

    def _v_position_ci(self):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::OrbitData::position_ci() const =0
        Return position as a pointer. 
        """
        return _orbit.OrbitData__v_position_ci(self)

    @property
    def position_ci(self):
        return self._v_position_ci()

    def _v_position_cf(self):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::OrbitData::position_cf() const =0
        Return position as a pointer. 
        """
        return _orbit.OrbitData__v_position_cf(self)

    @property
    def position_cf(self):
        return self._v_position_cf()

    def footprint(self, *args):
        """
        std::vector< boost::shared_ptr< GroundCoordinate > > OrbitData::footprint(const Camera &C, const Dem &D, double Resolution=30, int Band=0,
        double Max_height=9000) const
        Return the footprint on the ground for the given camera and dem.

        This just calls surface_intersect for the four corner points. 
        """
        return _orbit.OrbitData_footprint(self, *args)

    def surface_intersect(self, *args):
        """
        boost::shared_ptr< CartesianFixed > OrbitData::surface_intersect(const Camera &C, const FrameCoordinate &Fc, const Dem &D, double
        Resolution=30, int Band=0, double Max_height=9000) const
        Calculate the intersection with the surface.

        Resolution is the dem postings in meters, you usually don't want this
        much more accurate than the intrinsic accuracy of the Dem (e.g., the
        Did is 30 meter, so resolution shouldn't be better than 30). 
        """
        return _orbit.OrbitData_surface_intersect(self, *args)

    @property
    def velocity_ci(self):
        return self._velocity_ci()
      
    def _v_time(self):
        """
        virtual Time GeoCal::OrbitData::time() const =0
        Return Time of OrbitData. 
        """
        return _orbit.OrbitData__v_time(self)

    @property
    def time(self):
        return self._v_time()

    __swig_destroy__ = _orbit.delete_OrbitData
OrbitData.resolution_meter = new_instancemethod(_orbit.OrbitData_resolution_meter,None,OrbitData)
OrbitData.ci_look_vector = new_instancemethod(_orbit.OrbitData_ci_look_vector,None,OrbitData)
OrbitData.cf_look_vector = new_instancemethod(_orbit.OrbitData_cf_look_vector,None,OrbitData)
OrbitData.sc_look_vector = new_instancemethod(_orbit.OrbitData_sc_look_vector,None,OrbitData)
OrbitData.frame_coordinate = new_instancemethod(_orbit.OrbitData_frame_coordinate,None,OrbitData)
OrbitData.reference_surface_intersect_approximate = new_instancemethod(_orbit.OrbitData_reference_surface_intersect_approximate,None,OrbitData)
OrbitData._v_position_ci = new_instancemethod(_orbit.OrbitData__v_position_ci,None,OrbitData)
OrbitData._v_position_cf = new_instancemethod(_orbit.OrbitData__v_position_cf,None,OrbitData)
OrbitData.footprint = new_instancemethod(_orbit.OrbitData_footprint,None,OrbitData)
OrbitData.surface_intersect = new_instancemethod(_orbit.OrbitData_surface_intersect,None,OrbitData)
OrbitData._velocity_ci = new_instancemethod(_orbit.OrbitData__velocity_ci,None,OrbitData)
OrbitData._v_time = new_instancemethod(_orbit.OrbitData__v_time,None,OrbitData)
OrbitData.__str__ = new_instancemethod(_orbit.OrbitData___str__,None,OrbitData)
OrbitData_swigregister = _orbit.OrbitData_swigregister
OrbitData_swigregister(OrbitData)

class QuaternionOrbitData(OrbitData):
    """
    This class implements the most common way of doing OrbitData
    conversions, which just uses fixed quaternions.

    This accounts for both the orientation of the spacecraft relative to
    the planet, as well as the aberration of light (a small correction due
    to the movement of the spacecraft relative to the planet).

    This does *not* account for atmospheric refraction. Depending on the
    zenith angle, this can be somewhat important for satellites. From the
    approximate atmospheric model described in "Theoretical Basis of the
    SDP Toolkit Geolocation package for the ECS", Table 6-5 the linear
    displacement for a zenith angle of 10 is 0.549 meters, 20 degrees is
    1.223 meters, and 30 degrees is 2.221.

    We may want to add a atmospheric refraction correction in the future,
    but this hasn't been done yet.

    We need to have one of the toolkit available if we want to convert for
    the CartesianFixed coordinates used by this class to
    CartesianInertial. If you stick to working with CartesianFixed only,
    you can avoid the need of using one of these toolkits.

    C++ includes: orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        QuaternionOrbitData::QuaternionOrbitData(Time Tm, const boost::shared_ptr< CartesianInertial > &pos_ci, const
        boost::array< double, 3 > &vel_inertial, const
        boost::math::quaternion< double > &sc_to_ci_q)
        Construct QuaternionOrbitData.

        This takes data in a CartesianInertial coordinate system (e.g., Eci
        coordinates). 
        """
        _orbit.QuaternionOrbitData_swiginit(self,_orbit.new_QuaternionOrbitData(*args))
    def sc_look_vector(self, *args):
        """
        ScLookVector QuaternionOrbitData::sc_look_vector(const CartesianFixedLookVector &Cf) const
        Convert to ScLookVector. 
        """
        return _orbit.QuaternionOrbitData_sc_look_vector(self, *args)

    def _v_sc_to_ci(self):
        """
        boost::math::quaternion<double> GeoCal::QuaternionOrbitData::sc_to_ci() const
        Return the quaternion used to go from spacecraft to cartesian
        inertial. 
        """
        return _orbit.QuaternionOrbitData__v_sc_to_ci(self)

    @property
    def sc_to_ci(self):
        return self._v_sc_to_ci()

    def _v_sc_to_cf(self, *args):
        """
        void GeoCal::QuaternionOrbitData::sc_to_cf(const boost::math::quaternion< double > &Sc_to_cf)
        Set the quaternion used to go from spacecraft to cartesian fixed. 
        """
        return _orbit.QuaternionOrbitData__v_sc_to_cf(self, *args)

    @property
    def sc_to_cf(self):
        return self._v_sc_to_cf()

    @sc_to_cf.setter
    def sc_to_cf(self, value):
      self._v_sc_to_cf(value)

    def _v_from_cf(self):
        """
        bool GeoCal::QuaternionOrbitData::from_cf() const
        Was this created form the cartesian fixed version of the constructor?
        This is intended for use by python when we pickle this object so we
        know which constructor to call.

        It probably isn't of interest to anything else. 
        """
        return _orbit.QuaternionOrbitData__v_from_cf(self)

    @property
    def from_cf(self):
        return self._v_from_cf()

    @property
    def velocity_cf(self):
        return self._velocity_cf()
      
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      if(self.from_cf):
        return _new_from_init, (self.__class__, 1, self.time, self.position_cf, 
    			    self.velocity_cf, self.sc_to_cf)
      else:
        return _new_from_init, (self.__class__, 1, self.time, self.position_ci, 
    			    self.velocity_ci, self.sc_to_ci)


    __swig_destroy__ = _orbit.delete_QuaternionOrbitData
QuaternionOrbitData.sc_look_vector = new_instancemethod(_orbit.QuaternionOrbitData_sc_look_vector,None,QuaternionOrbitData)
QuaternionOrbitData._v_sc_to_ci = new_instancemethod(_orbit.QuaternionOrbitData__v_sc_to_ci,None,QuaternionOrbitData)
QuaternionOrbitData._v_sc_to_cf = new_instancemethod(_orbit.QuaternionOrbitData__v_sc_to_cf,None,QuaternionOrbitData)
QuaternionOrbitData._v_from_cf = new_instancemethod(_orbit.QuaternionOrbitData__v_from_cf,None,QuaternionOrbitData)
QuaternionOrbitData._velocity_cf = new_instancemethod(_orbit.QuaternionOrbitData__velocity_cf,None,QuaternionOrbitData)
QuaternionOrbitData_swigregister = _orbit.QuaternionOrbitData_swigregister
QuaternionOrbitData_swigregister(QuaternionOrbitData)

class Orbit(geocal_swig.generic_object.GenericObject):
    """
    This class is used to model orbit data, allowing conversions from
    spacecraft coordinates to CartesianInertial and CartesianFixed
    coordinates.

    This class is used to return orbit data at a given time. OrbitData is
    a class that is able to convert from spacecraft coordinates to
    CartesianInertial coordinates and vice-versa at a given time, as well
    as giving the platforms position.

    As an optimization, methods for direct conversion from spacecraft
    coordinates to CartesianInertial and vice-versa, which don't use the
    intermediate OrbitData classes, are supplied. The default methods just
    use the OrbitData methods, but derived classes can supply more
    optimized versions of these methods.

    An orbit has a min_time() and a max_time() that orbit data is
    available for. Requesting data outside of this range will cause an
    exception to be thrown. For Time T, we must have min_time() <= T <
    max_time().

    C++ includes: orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::Orbit::Orbit(Time Min_time=Time::min_valid_time, Time
        Max_time=Time::max_valid_time)
        Constructor.

        The Orbit is valid for the given range of minimum to maximum time. 
        """
        if self.__class__ == Orbit:
            _self = None
        else:
            _self = self
        _orbit.Orbit_swiginit(self,_orbit.new_Orbit(_self, *args))
    __swig_destroy__ = _orbit.delete_Orbit
    def ci_look_vector(self, *args):
        """
        virtual CartesianInertialLookVector GeoCal::Orbit::ci_look_vector(Time T, const ScLookVector &Sl) const
        Convert from ScLookVector to CartesianInertialLookVector for the given
        time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_ci_look_vector(self, *args)

    def cf_look_vector(self, *args):
        """
        virtual CartesianFixedLookVector GeoCal::Orbit::cf_look_vector(Time T, const ScLookVector &Sl) const
        Convert from ScLookVector to CartesianFixedLookVector for the given
        time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_cf_look_vector(self, *args)

    def frame_coordinate(self, *args):
        """
        FrameCoordinate GeoCal::Orbit::frame_coordinate(Time T, const GroundCoordinate &Gc, const Camera &C, int Band=0)
        const
        Give the frame coordinates that a particular point on the ground is
        seen. 
        """
        return _orbit.Orbit_frame_coordinate(self, *args)

    def reference_surface_intersect_approximate(self, *args):
        """
        boost::shared_ptr<CartesianFixed> GeoCal::Orbit::reference_surface_intersect_approximate(Time T, const Camera &C, const FrameCoordinate &Fc, int Band=0,
        double Height_reference_surface=0.0) const
        Return location on the reference surface that a particular frame
        coordinate is seen.

        This is approximate, in the same way
        CartesianFixed::reference_intersect_approximate is approximate. 
        """
        return _orbit.Orbit_reference_surface_intersect_approximate(self, *args)

    def sc_look_vector(self, *args):
        """
        virtual ScLookVector GeoCal::Orbit::sc_look_vector(Time T, const CartesianFixedLookVector &Cf) const
        Convert from CartesianFixedLookVector to ScLookVector for the given
        time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_sc_look_vector(self, *args)

    def position_ci(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::Orbit::position_ci(Time T) const
        Return position at given time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_position_ci(self, *args)

    def position_cf(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::Orbit::position_cf(Time T) const
        Return position at given time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_position_cf(self, *args)

    def _v_min_time(self):
        """
        Time GeoCal::Orbit::min_time() const
        Minimum time that we can return OrbitData for. 
        """
        return _orbit.Orbit__v_min_time(self)

    @property
    def min_time(self):
        return self._v_min_time()

    def _v_max_time(self):
        """
        Time GeoCal::Orbit::max_time() const
        Maximum time that we can return OrbitData for. 
        """
        return _orbit.Orbit__v_max_time(self)

    @property
    def max_time(self):
        return self._v_max_time()

    def orbit_data(self, *args):
        """
        virtual boost::shared_ptr<OrbitData> GeoCal::Orbit::orbit_data(Time T) const =0
        Return OrbitData for the given time.

        We should have min_time() <= T < max_time(). 
        """
        return _orbit.Orbit_orbit_data(self, *args)

    def interpolate(self, *args):
        """
        boost::math::quaternion<double> interpolate(const boost::math::quaternion< double > &Q1, const
        boost::math::quaternion< double > &Q2, double toffset, double tspace)
        This is a utility function for use by derived classes.

        A common way of getting orbit data is to have discrete measurements of
        the quaternion describing the rotation of the spacecraft. For a time t
        between t1 and t2, we have Q1 as the quaternion at time t1, Q2 the
        quaternion at time t2, tspace = t2 - t1, toffset = t - t1. This
        function then returns Qres. We calculate this by determining the axis
        and angle rotation that takes use from Q1 to Q2, and then do a linear
        interpolation of that angle for the given time. 
        """
        return _orbit.Orbit_interpolate(self, *args)

    def __disown__(self):
        self.this.disown()
        _orbit.disown_Orbit(self)
        return weakref_proxy(self)
Orbit.ci_look_vector = new_instancemethod(_orbit.Orbit_ci_look_vector,None,Orbit)
Orbit.cf_look_vector = new_instancemethod(_orbit.Orbit_cf_look_vector,None,Orbit)
Orbit.frame_coordinate = new_instancemethod(_orbit.Orbit_frame_coordinate,None,Orbit)
Orbit.reference_surface_intersect_approximate = new_instancemethod(_orbit.Orbit_reference_surface_intersect_approximate,None,Orbit)
Orbit.sc_look_vector = new_instancemethod(_orbit.Orbit_sc_look_vector,None,Orbit)
Orbit.position_ci = new_instancemethod(_orbit.Orbit_position_ci,None,Orbit)
Orbit.position_cf = new_instancemethod(_orbit.Orbit_position_cf,None,Orbit)
Orbit._v_min_time = new_instancemethod(_orbit.Orbit__v_min_time,None,Orbit)
Orbit._v_max_time = new_instancemethod(_orbit.Orbit__v_max_time,None,Orbit)
Orbit.orbit_data = new_instancemethod(_orbit.Orbit_orbit_data,None,Orbit)
Orbit.__str__ = new_instancemethod(_orbit.Orbit___str__,None,Orbit)
Orbit.interpolate = new_instancemethod(_orbit.Orbit_interpolate,None,Orbit)
Orbit_swigregister = _orbit.Orbit_swigregister
Orbit_swigregister(Orbit)

class KeplerOrbit(Orbit):
    """
    This is a simple implementation of an Orbit.

    It just uses Kepler's equations. This is intended primarily for easy
    testing of other classes that need an Orbit, rather than being a
    realistic orbit simulation for real use.

    The default parameters are for a nominal MISR orbit.

    C++ includes: orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        KeplerOrbit::KeplerOrbit(Time Min_time=Time::min_valid_time, Time
        Max_time=Time::max_valid_time, Time
        Epoch=Time::time_pgs(173357492.32), double Semimajor_axis=7086930,
        double Eccentricity=0.001281620, double Inclination=98.199990, double
        Ra_ascending_node=255.355971130, double Ap_at_epoch=69.086962170,
        double Mean_anomaly_at_epoch=290.912925280)
        Create a Kepler orbit with the given elements, valid over the given
        time range.

        Distances are in meters and angles are in degrees. The Epoch gives the
        Time that the rest of the data is valid for. The default values are a
        nominal orbit for MISR. 
        """
        _orbit.KeplerOrbit_swiginit(self,_orbit.new_KeplerOrbit(*args))
    def _v_epoch(self, *args):
        """
        void GeoCal::KeplerOrbit::epoch(const Time &Epoch)
        Set epoch that rest of data is for. 
        """
        return _orbit.KeplerOrbit__v_epoch(self, *args)

    @property
    def epoch(self):
        return self._v_epoch()

    @epoch.setter
    def epoch(self, value):
      self._v_epoch(value)

    def _v_semimajor_axis(self, *args):
        """
        void GeoCal::KeplerOrbit::semimajor_axis(double Semimajor_axis)
        Set semimajor axis in meters. 
        """
        return _orbit.KeplerOrbit__v_semimajor_axis(self, *args)

    @property
    def semimajor_axis(self):
        return self._v_semimajor_axis()

    @semimajor_axis.setter
    def semimajor_axis(self, value):
      self._v_semimajor_axis(value)

    def _v_argument_of_perigee(self, *args):
        """
        void GeoCal::KeplerOrbit::argument_of_perigee(double Ap_at_epoch)
        Set Argument of perigee at epoch, in degrees. 
        """
        return _orbit.KeplerOrbit__v_argument_of_perigee(self, *args)

    @property
    def argument_of_perigee(self):
        return self._v_argument_of_perigee()

    @argument_of_perigee.setter
    def argument_of_perigee(self, value):
      self._v_argument_of_perigee(value)

    def _v_eccentricity(self, *args):
        """
        void GeoCal::KeplerOrbit::eccentricity(double Eccentricity)
        Set eccentricity of orbit. 
        """
        return _orbit.KeplerOrbit__v_eccentricity(self, *args)

    @property
    def eccentricity(self):
        return self._v_eccentricity()

    @eccentricity.setter
    def eccentricity(self, value):
      self._v_eccentricity(value)

    def _v_mean_anomoly(self, *args):
        """
        void GeoCal::KeplerOrbit::mean_anomoly(double Mean_anomaly_at_epoch)
        Set mean anomoly at epoch, in degrees. 
        """
        return _orbit.KeplerOrbit__v_mean_anomoly(self, *args)

    @property
    def mean_anomoly(self):
        return self._v_mean_anomoly()

    @mean_anomoly.setter
    def mean_anomoly(self, value):
      self._v_mean_anomoly(value)

    def _v_inclination(self, *args):
        """
        void GeoCal::KeplerOrbit::inclination(double Inclination)
        Set inclination of orbit, in degrees. 
        """
        return _orbit.KeplerOrbit__v_inclination(self, *args)

    @property
    def inclination(self):
        return self._v_inclination()

    @inclination.setter
    def inclination(self, value):
      self._v_inclination(value)

    def _v_right_ascension(self, *args):
        """
        void GeoCal::KeplerOrbit::right_ascension(double Ra_ascending_node)
        Set right ascension of ascending node, in degrees. 
        """
        return _orbit.KeplerOrbit__v_right_ascension(self, *args)

    @property
    def right_ascension(self):
        return self._v_right_ascension()

    @right_ascension.setter
    def right_ascension(self, value):
      self._v_right_ascension(value)

    def _v_period(self):
        """
        double GeoCal::KeplerOrbit::period() const
        Period in seconds. 
        """
        return _orbit.KeplerOrbit__v_period(self)

    @property
    def period(self):
        return self._v_period()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.min_time,self.max_time,self.epoch,self.semimajor_axis,self.eccentricity,self.inclination,self.right_ascension,self.argument_of_perigee,self.mean_anomoly)

    __swig_destroy__ = _orbit.delete_KeplerOrbit
KeplerOrbit._v_epoch = new_instancemethod(_orbit.KeplerOrbit__v_epoch,None,KeplerOrbit)
KeplerOrbit._v_semimajor_axis = new_instancemethod(_orbit.KeplerOrbit__v_semimajor_axis,None,KeplerOrbit)
KeplerOrbit._v_argument_of_perigee = new_instancemethod(_orbit.KeplerOrbit__v_argument_of_perigee,None,KeplerOrbit)
KeplerOrbit._v_eccentricity = new_instancemethod(_orbit.KeplerOrbit__v_eccentricity,None,KeplerOrbit)
KeplerOrbit._v_mean_anomoly = new_instancemethod(_orbit.KeplerOrbit__v_mean_anomoly,None,KeplerOrbit)
KeplerOrbit._v_inclination = new_instancemethod(_orbit.KeplerOrbit__v_inclination,None,KeplerOrbit)
KeplerOrbit._v_right_ascension = new_instancemethod(_orbit.KeplerOrbit__v_right_ascension,None,KeplerOrbit)
KeplerOrbit._v_period = new_instancemethod(_orbit.KeplerOrbit__v_period,None,KeplerOrbit)
KeplerOrbit_swigregister = _orbit.KeplerOrbit_swigregister
KeplerOrbit_swigregister(KeplerOrbit)

class Vector_QuaternionOrbitData(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _orbit.Vector_QuaternionOrbitData_swiginit(self,_orbit.new_Vector_QuaternionOrbitData(*args))
    @classmethod
    def pickle_format_version(cls):
      return 1

    def to_list(self):
       res = []
       for i in range(self.size()):
          res.append(self[i])
       return res

    def __reduce__(self):
      return _new_vector, (self.__class__, 1, self.to_list())

    __swig_destroy__ = _orbit.delete_Vector_QuaternionOrbitData
Vector_QuaternionOrbitData.iterator = new_instancemethod(_orbit.Vector_QuaternionOrbitData_iterator,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__nonzero__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___nonzero__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__bool__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___bool__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__len__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___len__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.pop = new_instancemethod(_orbit.Vector_QuaternionOrbitData_pop,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__getslice__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___getslice__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__setslice__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___setslice__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__delslice__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___delslice__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__delitem__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___delitem__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__getitem__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___getitem__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.__setitem__ = new_instancemethod(_orbit.Vector_QuaternionOrbitData___setitem__,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.append = new_instancemethod(_orbit.Vector_QuaternionOrbitData_append,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.empty = new_instancemethod(_orbit.Vector_QuaternionOrbitData_empty,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.size = new_instancemethod(_orbit.Vector_QuaternionOrbitData_size,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.clear = new_instancemethod(_orbit.Vector_QuaternionOrbitData_clear,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.swap = new_instancemethod(_orbit.Vector_QuaternionOrbitData_swap,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.get_allocator = new_instancemethod(_orbit.Vector_QuaternionOrbitData_get_allocator,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.begin = new_instancemethod(_orbit.Vector_QuaternionOrbitData_begin,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.end = new_instancemethod(_orbit.Vector_QuaternionOrbitData_end,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.rbegin = new_instancemethod(_orbit.Vector_QuaternionOrbitData_rbegin,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.rend = new_instancemethod(_orbit.Vector_QuaternionOrbitData_rend,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.pop_back = new_instancemethod(_orbit.Vector_QuaternionOrbitData_pop_back,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.erase = new_instancemethod(_orbit.Vector_QuaternionOrbitData_erase,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.push_back = new_instancemethod(_orbit.Vector_QuaternionOrbitData_push_back,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.front = new_instancemethod(_orbit.Vector_QuaternionOrbitData_front,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.back = new_instancemethod(_orbit.Vector_QuaternionOrbitData_back,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.assign = new_instancemethod(_orbit.Vector_QuaternionOrbitData_assign,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.resize = new_instancemethod(_orbit.Vector_QuaternionOrbitData_resize,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.insert = new_instancemethod(_orbit.Vector_QuaternionOrbitData_insert,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.reserve = new_instancemethod(_orbit.Vector_QuaternionOrbitData_reserve,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData.capacity = new_instancemethod(_orbit.Vector_QuaternionOrbitData_capacity,None,Vector_QuaternionOrbitData)
Vector_QuaternionOrbitData_swigregister = _orbit.Vector_QuaternionOrbitData_swigregister
Vector_QuaternionOrbitData_swigregister(Vector_QuaternionOrbitData)



