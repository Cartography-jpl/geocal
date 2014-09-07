# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _planet_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_planet_coordinate', [dirname(__file__)])
        except ImportError:
            import _planet_coordinate
            return _planet_coordinate
        if fp is not None:
            try:
                _mod = imp.load_module('_planet_coordinate', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _planet_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _planet_coordinate
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


SHARED_PTR_DISOWN = _planet_coordinate.SHARED_PTR_DISOWN
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

import geocal_swig.ground_coordinate
import geocal_swig.generic_object
import geocal_swig.look_vector
import geocal_swig.dem
class MarsConstant(object):
    """
    Constants for a planet.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def planet_a():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_a()

        """
        return _planet_coordinate.MarsConstant_planet_a()

    planet_a = staticmethod(planet_a)
    def planet_b():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_b()

        """
        return _planet_coordinate.MarsConstant_planet_b()

    planet_b = staticmethod(planet_b)
    def planet_esq():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_esq()

        """
        return _planet_coordinate.MarsConstant_planet_esq()

    planet_esq = staticmethod(planet_esq)
    def planet_name():
        """
        static std::string GeoCal::PlanetConstant< NCODE >::planet_name()

        """
        return _planet_coordinate.MarsConstant_planet_name()

    planet_name = staticmethod(planet_name)
    def __init__(self): 
        _planet_coordinate.MarsConstant_swiginit(self,_planet_coordinate.new_MarsConstant())
    __swig_destroy__ = _planet_coordinate.delete_MarsConstant
MarsConstant_swigregister = _planet_coordinate.MarsConstant_swigregister
MarsConstant_swigregister(MarsConstant)

def MarsConstant_planet_a():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_a()

    """
  return _planet_coordinate.MarsConstant_planet_a()

def MarsConstant_planet_b():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_b()

    """
  return _planet_coordinate.MarsConstant_planet_b()

def MarsConstant_planet_esq():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_esq()

    """
  return _planet_coordinate.MarsConstant_planet_esq()

def MarsConstant_planet_name():
  """
    static std::string GeoCal::PlanetConstant< NCODE >::planet_name()

    """
  return _planet_coordinate.MarsConstant_planet_name()

class MarsFixed(geocal_swig.ground_coordinate.CartesianFixed):
    """
    This is a ground coordinate, expressed in fixed Planet coordinates.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::PlanetFixed< NAIF_CODE >::PlanetFixed()
        Default constructor. 
        """
        _planet_coordinate.MarsFixed_swiginit(self,_planet_coordinate.new_MarsFixed(*args))
    def convert_to_planetocentric(self):
        """
        Planetocentric< NAIF_CODE > GeoCal::PlanetFixed< NAIF_CODE >::convert_to_planetocentric() const

        """
        return _planet_coordinate.MarsFixed_convert_to_planetocentric(self)

    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::PlanetFixed< NAIF_CODE >::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
        Height_reference_surface=0) const

        """
        return _planet_coordinate.MarsFixed_reference_surface_intersect_approximate(self, *args)

    def target_position(*args):
        """
        static PlanetFixed<NAIF_CODE> GeoCal::PlanetFixed< NAIF_CODE >::target_position(const std::string &Target_name, const Time &T)
        Use spice to determine the position of the given body at the given
        time. 
        """
        return _planet_coordinate.MarsFixed_target_position(*args)

    target_position = staticmethod(target_position)
    def orbit_data(*args):
        """
        static boost::shared_ptr<QuaternionOrbitData> GeoCal::PlanetFixed< NAIF_CODE >::orbit_data(const std::string &Target_name, const std::string
        &Spacecraft_reference_frame_name, const Time &T)
        Return orbit data for the given target and spacecraft reference frame.

        """
        return _planet_coordinate.MarsFixed_orbit_data(*args)

    orbit_data = staticmethod(orbit_data)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.position[0],self.position[1],self.position[2])

    __swig_destroy__ = _planet_coordinate.delete_MarsFixed
MarsFixed.convert_to_planetocentric = new_instancemethod(_planet_coordinate.MarsFixed_convert_to_planetocentric,None,MarsFixed)
MarsFixed.reference_surface_intersect_approximate = new_instancemethod(_planet_coordinate.MarsFixed_reference_surface_intersect_approximate,None,MarsFixed)
MarsFixed_swigregister = _planet_coordinate.MarsFixed_swigregister
MarsFixed_swigregister(MarsFixed)

def MarsFixed_target_position(*args):
  """
    static PlanetFixed<NAIF_CODE> GeoCal::PlanetFixed< NAIF_CODE >::target_position(const std::string &Target_name, const Time &T)
    Use spice to determine the position of the given body at the given
    time. 
    """
  return _planet_coordinate.MarsFixed_target_position(*args)

def MarsFixed_orbit_data(*args):
  """
    static boost::shared_ptr<QuaternionOrbitData> GeoCal::PlanetFixed< NAIF_CODE >::orbit_data(const std::string &Target_name, const std::string
    &Spacecraft_reference_frame_name, const Time &T)
    Return orbit data for the given target and spacecraft reference frame.

    """
  return _planet_coordinate.MarsFixed_orbit_data(*args)

class MarsInertial(geocal_swig.ground_coordinate.CartesianInertial):
    """
    This is a Planet Intertial coordinate.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::PlanetInertial< NAIF_CODE >::PlanetInertial(const boost::array< double, 3 > &Pos)
        Create an PlanetInertial with the given position in meters. 
        """
        _planet_coordinate.MarsInertial_swiginit(self,_planet_coordinate.new_MarsInertial(*args))
    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::PlanetInertial< NAIF_CODE >::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
        Height_reference_surface=0) const

        """
        return _planet_coordinate.MarsInertial_reference_surface_intersect_approximate(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.position[0],self.position[1],self.position[2])

    __swig_destroy__ = _planet_coordinate.delete_MarsInertial
MarsInertial.reference_surface_intersect_approximate = new_instancemethod(_planet_coordinate.MarsInertial_reference_surface_intersect_approximate,None,MarsInertial)
MarsInertial_swigregister = _planet_coordinate.MarsInertial_swigregister
MarsInertial_swigregister(MarsInertial)

class MarsPlanetocentric(geocal_swig.ground_coordinate.GroundCoordinate):
    """
    This is Planet coordinates as Planetocentric latitude, longitude, and
    height above the reference ellipsoid.

    This is the planet equivalent of Geocentric ( not Geodetic). Height is
    relative to the ellipsoid, but latitude is relative to center of
    planet rather than normal of ellipsoid.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::Planetocentric< NAIF_CODE >::Planetocentric()
        Default constructor. 
        """
        _planet_coordinate.MarsPlanetocentric_swiginit(self,_planet_coordinate.new_MarsPlanetocentric(*args))
    @property
    def height_reference_surface(self):
        return self._v_height_reference_surface()

    @property
    def latitude(self):
        return self._v_latitude()

    @property
    def longitude(self):
        return self._v_longitude()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.latitude,self.longitude,self.height_reference_surface)

    __swig_destroy__ = _planet_coordinate.delete_MarsPlanetocentric
MarsPlanetocentric_swigregister = _planet_coordinate.MarsPlanetocentric_swigregister
MarsPlanetocentric_swigregister(MarsPlanetocentric)

class MarsSimpleDem(geocal_swig.dem.Dem):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, H=0): 
        _planet_coordinate.MarsSimpleDem_swiginit(self,_planet_coordinate.new_MarsSimpleDem(H))
    @property
    def h(self):
        return self._v_h()

    @h.setter
    def h(self, value):
      self._v_h(value)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.h)

    __swig_destroy__ = _planet_coordinate.delete_MarsSimpleDem
MarsSimpleDem._v_h = new_instancemethod(_planet_coordinate.MarsSimpleDem__v_h,None,MarsSimpleDem)
MarsSimpleDem_swigregister = _planet_coordinate.MarsSimpleDem_swigregister
MarsSimpleDem_swigregister(MarsSimpleDem)

class EuropaConstant(object):
    """
    Constants for a planet.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def planet_a():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_a()

        """
        return _planet_coordinate.EuropaConstant_planet_a()

    planet_a = staticmethod(planet_a)
    def planet_b():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_b()

        """
        return _planet_coordinate.EuropaConstant_planet_b()

    planet_b = staticmethod(planet_b)
    def planet_esq():
        """
        static double GeoCal::PlanetConstant< NCODE >::planet_esq()

        """
        return _planet_coordinate.EuropaConstant_planet_esq()

    planet_esq = staticmethod(planet_esq)
    def planet_name():
        """
        static std::string GeoCal::PlanetConstant< NCODE >::planet_name()

        """
        return _planet_coordinate.EuropaConstant_planet_name()

    planet_name = staticmethod(planet_name)
    def __init__(self): 
        _planet_coordinate.EuropaConstant_swiginit(self,_planet_coordinate.new_EuropaConstant())
    __swig_destroy__ = _planet_coordinate.delete_EuropaConstant
EuropaConstant_swigregister = _planet_coordinate.EuropaConstant_swigregister
EuropaConstant_swigregister(EuropaConstant)

def EuropaConstant_planet_a():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_a()

    """
  return _planet_coordinate.EuropaConstant_planet_a()

def EuropaConstant_planet_b():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_b()

    """
  return _planet_coordinate.EuropaConstant_planet_b()

def EuropaConstant_planet_esq():
  """
    static double GeoCal::PlanetConstant< NCODE >::planet_esq()

    """
  return _planet_coordinate.EuropaConstant_planet_esq()

def EuropaConstant_planet_name():
  """
    static std::string GeoCal::PlanetConstant< NCODE >::planet_name()

    """
  return _planet_coordinate.EuropaConstant_planet_name()

class EuropaFixed(geocal_swig.ground_coordinate.CartesianFixed):
    """
    This is a ground coordinate, expressed in fixed Planet coordinates.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::PlanetFixed< NAIF_CODE >::PlanetFixed()
        Default constructor. 
        """
        _planet_coordinate.EuropaFixed_swiginit(self,_planet_coordinate.new_EuropaFixed(*args))
    def convert_to_planetocentric(self):
        """
        Planetocentric< NAIF_CODE > GeoCal::PlanetFixed< NAIF_CODE >::convert_to_planetocentric() const

        """
        return _planet_coordinate.EuropaFixed_convert_to_planetocentric(self)

    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::PlanetFixed< NAIF_CODE >::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
        Height_reference_surface=0) const

        """
        return _planet_coordinate.EuropaFixed_reference_surface_intersect_approximate(self, *args)

    def target_position(*args):
        """
        static PlanetFixed<NAIF_CODE> GeoCal::PlanetFixed< NAIF_CODE >::target_position(const std::string &Target_name, const Time &T)
        Use spice to determine the position of the given body at the given
        time. 
        """
        return _planet_coordinate.EuropaFixed_target_position(*args)

    target_position = staticmethod(target_position)
    def orbit_data(*args):
        """
        static boost::shared_ptr<QuaternionOrbitData> GeoCal::PlanetFixed< NAIF_CODE >::orbit_data(const std::string &Target_name, const std::string
        &Spacecraft_reference_frame_name, const Time &T)
        Return orbit data for the given target and spacecraft reference frame.

        """
        return _planet_coordinate.EuropaFixed_orbit_data(*args)

    orbit_data = staticmethod(orbit_data)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.position[0],self.position[1],self.position[2])

    __swig_destroy__ = _planet_coordinate.delete_EuropaFixed
EuropaFixed.convert_to_planetocentric = new_instancemethod(_planet_coordinate.EuropaFixed_convert_to_planetocentric,None,EuropaFixed)
EuropaFixed.reference_surface_intersect_approximate = new_instancemethod(_planet_coordinate.EuropaFixed_reference_surface_intersect_approximate,None,EuropaFixed)
EuropaFixed_swigregister = _planet_coordinate.EuropaFixed_swigregister
EuropaFixed_swigregister(EuropaFixed)

def EuropaFixed_target_position(*args):
  """
    static PlanetFixed<NAIF_CODE> GeoCal::PlanetFixed< NAIF_CODE >::target_position(const std::string &Target_name, const Time &T)
    Use spice to determine the position of the given body at the given
    time. 
    """
  return _planet_coordinate.EuropaFixed_target_position(*args)

def EuropaFixed_orbit_data(*args):
  """
    static boost::shared_ptr<QuaternionOrbitData> GeoCal::PlanetFixed< NAIF_CODE >::orbit_data(const std::string &Target_name, const std::string
    &Spacecraft_reference_frame_name, const Time &T)
    Return orbit data for the given target and spacecraft reference frame.

    """
  return _planet_coordinate.EuropaFixed_orbit_data(*args)

class EuropaInertial(geocal_swig.ground_coordinate.CartesianInertial):
    """
    This is a Planet Intertial coordinate.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::PlanetInertial< NAIF_CODE >::PlanetInertial(const boost::array< double, 3 > &Pos)
        Create an PlanetInertial with the given position in meters. 
        """
        _planet_coordinate.EuropaInertial_swiginit(self,_planet_coordinate.new_EuropaInertial(*args))
    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::PlanetInertial< NAIF_CODE >::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
        Height_reference_surface=0) const

        """
        return _planet_coordinate.EuropaInertial_reference_surface_intersect_approximate(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.position[0],self.position[1],self.position[2])

    __swig_destroy__ = _planet_coordinate.delete_EuropaInertial
EuropaInertial.reference_surface_intersect_approximate = new_instancemethod(_planet_coordinate.EuropaInertial_reference_surface_intersect_approximate,None,EuropaInertial)
EuropaInertial_swigregister = _planet_coordinate.EuropaInertial_swigregister
EuropaInertial_swigregister(EuropaInertial)

class EuropaPlanetocentric(geocal_swig.ground_coordinate.GroundCoordinate):
    """
    This is Planet coordinates as Planetocentric latitude, longitude, and
    height above the reference ellipsoid.

    This is the planet equivalent of Geocentric ( not Geodetic). Height is
    relative to the ellipsoid, but latitude is relative to center of
    planet rather than normal of ellipsoid.

    Note that "Planet" also includes "Moon of planet", basically
    anything with a NAIF_CODE

    C++ includes: planet_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::Planetocentric< NAIF_CODE >::Planetocentric()
        Default constructor. 
        """
        _planet_coordinate.EuropaPlanetocentric_swiginit(self,_planet_coordinate.new_EuropaPlanetocentric(*args))
    @property
    def height_reference_surface(self):
        return self._v_height_reference_surface()

    @property
    def latitude(self):
        return self._v_latitude()

    @property
    def longitude(self):
        return self._v_longitude()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.latitude,self.longitude,self.height_reference_surface)

    __swig_destroy__ = _planet_coordinate.delete_EuropaPlanetocentric
EuropaPlanetocentric_swigregister = _planet_coordinate.EuropaPlanetocentric_swigregister
EuropaPlanetocentric_swigregister(EuropaPlanetocentric)

class EuropaSimpleDem(geocal_swig.dem.Dem):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, H=0): 
        _planet_coordinate.EuropaSimpleDem_swiginit(self,_planet_coordinate.new_EuropaSimpleDem(H))
    @property
    def h(self):
        return self._v_h()

    @h.setter
    def h(self, value):
      self._v_h(value)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.h)

    __swig_destroy__ = _planet_coordinate.delete_EuropaSimpleDem
EuropaSimpleDem._v_h = new_instancemethod(_planet_coordinate.EuropaSimpleDem__v_h,None,EuropaSimpleDem)
EuropaSimpleDem_swigregister = _planet_coordinate.EuropaSimpleDem_swigregister
EuropaSimpleDem_swigregister(EuropaSimpleDem)



