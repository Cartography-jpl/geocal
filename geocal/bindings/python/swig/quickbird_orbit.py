# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _quickbird_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_quickbird_orbit', [dirname(__file__)])
        except ImportError:
            import _quickbird_orbit
            return _quickbird_orbit
        if fp is not None:
            try:
                _mod = imp.load_module('_quickbird_orbit', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _quickbird_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _quickbird_orbit
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


SHARED_PTR_DISOWN = _quickbird_orbit.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
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

import geocal.generic_object
import geocal.orbit
class QuickBirdEphemeris(geocal.generic_object.GenericObject):
    """
    This is a low level class that reads a Quickbird ephemeris file.

    You probably don't want to use this directly, but rather use the
    QuickBirdOrbit

    C++ includes: quickbird_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::QuickBirdEphemeris::QuickBirdEphemeris(const std::string &Fname)
        Read the quickbird ephemeris file. 
        """
        _quickbird_orbit.QuickBirdEphemeris_swiginit(self,_quickbird_orbit.new_QuickBirdEphemeris(*args))
    def _v_min_time(self):
        """
        Time GeoCal::QuickBirdFile< D >::min_time() const
        Start time of data. 
        """
        return _quickbird_orbit.QuickBirdEphemeris__v_min_time(self)

    @property
    def min_time(self):
        return self._v_min_time()

    def _v_max_time(self):
        """
        Time GeoCal::QuickBirdFile< D >::max_time() const
        End time of data. 
        """
        return _quickbird_orbit.QuickBirdEphemeris__v_max_time(self)

    @property
    def max_time(self):
        return self._v_max_time()

    def _v_time_spacing(self):
        """
        double GeoCal::QuickBirdFile< D >::time_spacing() const
        Spacing in seconds between points. 
        """
        return _quickbird_orbit.QuickBirdEphemeris__v_time_spacing(self)

    @property
    def time_spacing(self):
        return self._v_time_spacing()

    def _v_ephemeris(self):
        """
        const std::vector<boost::array<double, 12> >& GeoCal::QuickBirdEphemeris::ephemeris() const
        Ephemeris data.

        This as 12 numbers in each entry. The first 3 are the X, Y, and Z
        position in meters, in ECF (or ECR?) coordinate system. The next 3 are
        the velocity in m/s. The final 6 are the upper right coordinates of
        the position covariance matrix (so order is (1, 1), (1, 2), (1, 3),
        (2, 2), (2,3), (3, 3)). 
        """
        return _quickbird_orbit.QuickBirdEphemeris__v_ephemeris(self)

    @property
    def ephemeris(self):
        return self._v_ephemeris()

    def _v_file_name(self):
        """
        const std::string& GeoCal::QuickBirdFile< D >::file_name() const
        File name we are reading. 
        """
        return _quickbird_orbit.QuickBirdEphemeris__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdEphemeris
QuickBirdEphemeris._v_min_time = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_min_time,None,QuickBirdEphemeris)
QuickBirdEphemeris._v_max_time = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_max_time,None,QuickBirdEphemeris)
QuickBirdEphemeris._v_time_spacing = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_time_spacing,None,QuickBirdEphemeris)
QuickBirdEphemeris._v_ephemeris = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_ephemeris,None,QuickBirdEphemeris)
QuickBirdEphemeris._v_file_name = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_file_name,None,QuickBirdEphemeris)
QuickBirdEphemeris.__str__ = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris___str__,None,QuickBirdEphemeris)
QuickBirdEphemeris_swigregister = _quickbird_orbit.QuickBirdEphemeris_swigregister
QuickBirdEphemeris_swigregister(QuickBirdEphemeris)

class QuickBirdAttitude(geocal.generic_object.GenericObject):
    """
    This is a low level class that reads a Quickbird attitude file.

    You probably don't want to use this directly, but rather use the
    QuickBirdOrbit.

    Note a possible source of confusion. There are a few different
    conventions about the ordering of the quaternion coefficients. The
    boost library places the real part at the front, so we have a + b i c
    j + d k and the quaternion is 4-tuple (a, b, c, d). The convention
    used by quickbird data is q1 i + q2 j + q3 k + q4 with the 4-tuple is
    (q1, q2, q3, q4). That means when we bring this over to the boost
    library, we need to reorder this to the 4-tuple (q4, q1, q2, q3).

    The code in QuickBirdOrbit accounts for these different conventions,
    but if you are using this class directly you need to be aware of this
    difference.

    C++ includes: quickbird_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::QuickBirdAttitude::QuickBirdAttitude(const std::string &Fname)
        Read the quickbird attitude file. 
        """
        _quickbird_orbit.QuickBirdAttitude_swiginit(self,_quickbird_orbit.new_QuickBirdAttitude(*args))
    def _v_min_time(self):
        """
        Time GeoCal::QuickBirdFile< D >::min_time() const
        Start time of data. 
        """
        return _quickbird_orbit.QuickBirdAttitude__v_min_time(self)

    @property
    def min_time(self):
        return self._v_min_time()

    def _v_max_time(self):
        """
        Time GeoCal::QuickBirdFile< D >::max_time() const
        End time of data. 
        """
        return _quickbird_orbit.QuickBirdAttitude__v_max_time(self)

    @property
    def max_time(self):
        return self._v_max_time()

    def _v_time_spacing(self):
        """
        double GeoCal::QuickBirdFile< D >::time_spacing() const
        Spacing in seconds between points. 
        """
        return _quickbird_orbit.QuickBirdAttitude__v_time_spacing(self)

    @property
    def time_spacing(self):
        return self._v_time_spacing()

    def _v_attitude(self):
        """
        const std::vector<boost::array<double, 14> >& GeoCal::QuickBirdAttitude::attitude() const
        Attitude data.

        The first 4 parameters are the quaternion parameters (q1, q2, q3, q4).
        The next 10 are the upper right elements of the attitude quaternion
        covariance matrix.

        Make sure to see the class notes for QuickBirdAttitude to see a
        discussion about the difference between the boost and Quickbird
        quaternion conventions. 
        """
        return _quickbird_orbit.QuickBirdAttitude__v_attitude(self)

    @property
    def attitude(self):
        return self._v_attitude()

    def _v_file_name(self):
        """
        const std::string& GeoCal::QuickBirdFile< D >::file_name() const
        File name we are reading. 
        """
        return _quickbird_orbit.QuickBirdAttitude__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdAttitude
QuickBirdAttitude._v_min_time = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_min_time,None,QuickBirdAttitude)
QuickBirdAttitude._v_max_time = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_max_time,None,QuickBirdAttitude)
QuickBirdAttitude._v_time_spacing = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_time_spacing,None,QuickBirdAttitude)
QuickBirdAttitude._v_attitude = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_attitude,None,QuickBirdAttitude)
QuickBirdAttitude._v_file_name = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_file_name,None,QuickBirdAttitude)
QuickBirdAttitude.__str__ = new_instancemethod(_quickbird_orbit.QuickBirdAttitude___str__,None,QuickBirdAttitude)
QuickBirdAttitude_swigregister = _quickbird_orbit.QuickBirdAttitude_swigregister
QuickBirdAttitude_swigregister(QuickBirdAttitude)

class QuickBirdOrbit(geocal.orbit.Orbit):
    """
    This is a Quickbird Orbit.

    This can be used for a rigorous model of Quickbird.

    Note an important limitation of the rigorous model vs. RPC. We don't
    currently account for atmospheric refraction, while the RPC does.
    Depending on the zenith angle, this can be somewhat important. From
    the approximate atmospheric model described in "Theoretical Basis of
    the SDP Toolkit Geolocation package for the ECS", Table 6-5 the
    linear displacement for a zenith angle of 10 is 0.549 meters, 20
    degress is 1.223 meters, and 30 degrees is 2.221. The typical
    Quickbird scene has something like 10 to 20 degree zenith angles, so
    this is a correction of 1 or 2 pixels.

    We will need to add atmospheric refraction in the future, but this
    hasn't been done yet.

    C++ includes: quickbird_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def orbit_data(self, *args):
        """
        boost::shared_ptr< OrbitData > QuickBirdOrbit::orbit_data(Time T) const
        Return OrbitData for the given time.

        We should have min_time() <= T < max_time(). 
        """
        return _quickbird_orbit.QuickBirdOrbit_orbit_data(self, *args)

    def _v_ephemeris_file_name(self):
        """
        std::string GeoCal::QuickBirdOrbit::ephemeris_file_name() const

        """
        return _quickbird_orbit.QuickBirdOrbit__v_ephemeris_file_name(self)

    @property
    def ephemeris_file_name(self):
        return self._v_ephemeris_file_name()

    def _v_attitude_file_name(self):
        """
        std::string GeoCal::QuickBirdOrbit::attitude_file_name() const

        """
        return _quickbird_orbit.QuickBirdOrbit__v_attitude_file_name(self)

    @property
    def attitude_file_name(self):
        return self._v_attitude_file_name()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.ephemeris_file_name,self.attitude_file_name)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdOrbit
QuickBirdOrbit.orbit_data = new_instancemethod(_quickbird_orbit.QuickBirdOrbit_orbit_data,None,QuickBirdOrbit)
QuickBirdOrbit._v_ephemeris_file_name = new_instancemethod(_quickbird_orbit.QuickBirdOrbit__v_ephemeris_file_name,None,QuickBirdOrbit)
QuickBirdOrbit._v_attitude_file_name = new_instancemethod(_quickbird_orbit.QuickBirdOrbit__v_attitude_file_name,None,QuickBirdOrbit)
QuickBirdOrbit_swigregister = _quickbird_orbit.QuickBirdOrbit_swigregister
QuickBirdOrbit_swigregister(QuickBirdOrbit)



