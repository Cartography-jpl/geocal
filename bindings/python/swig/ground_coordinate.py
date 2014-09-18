# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ground_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ground_coordinate', [dirname(__file__)])
        except ImportError:
            import _ground_coordinate
            return _ground_coordinate
        if fp is not None:
            try:
                _mod = imp.load_module('_ground_coordinate', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ground_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _ground_coordinate
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
    __swig_destroy__ = _ground_coordinate.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_ground_coordinate.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ground_coordinate.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ground_coordinate.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ground_coordinate.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ground_coordinate.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ground_coordinate.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ground_coordinate.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ground_coordinate.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ground_coordinate.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ground_coordinate.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ground_coordinate.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ground_coordinate.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ground_coordinate.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ground_coordinate.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ground_coordinate.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ground_coordinate.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _ground_coordinate.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ground_coordinate.SHARED_PTR_DISOWN
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
import geocal_swig.look_vector
class GroundCoordinate(geocal_swig.generic_object.GenericObject):
    """
    This represents a location on the ground.

    It is a base class, each of the derived classes represent different
    coordinate systems that can be used to express the ground location.
    Any ground coordinate can be converted to CartesianFixed coordinates.

    Because it is commonly needed, we also can convert to latitude,
    longitude and height_reference_surface. The default is just convert
    first to CartesianFixed, and then convert to latitude, longitude or
    height_reference_surface, but derived classes can supply more
    efficient versions of this if desired.

    C++ includes: ground_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def _v_latitude(self):
        """
        double GeoCal::GroundCoordinate::latitude() const
        Return latitude in degrees.

        By default we just convert to CartesianFixed and then to latitude, but
        derived classes can supply more efficient versions of these if needed.
        Latitude is -90 to 90. 
        """
        return _ground_coordinate.GroundCoordinate__v_latitude(self)

    @property
    def latitude(self):
        return self._v_latitude()

    def _v_longitude(self):
        """
        double GeoCal::GroundCoordinate::longitude() const
        Return longitude in degrees.

        By default we just convert to CartesianFixed and then to longitude,
        but derived classes can supply more efficient versions of these if
        needed. Longitude is -180 to 180. 
        """
        return _ground_coordinate.GroundCoordinate__v_longitude(self)

    @property
    def longitude(self):
        return self._v_longitude()

    def _v_height_reference_surface(self):
        """
        double GeoCal::GroundCoordinate::height_reference_surface() const
        Return height in meters above reference surface (e.g., WGS-84 for the
        earth).

        By default we just convert to CartesianFixed and then to height, but
        derived classes can supply more efficient versions of these if needed.

        """
        return _ground_coordinate.GroundCoordinate__v_height_reference_surface(self)

    @property
    def height_reference_surface(self):
        return self._v_height_reference_surface()

    def convert_to_cf(self):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::GroundCoordinate::convert_to_cf() const =0
        Convert to CartesianFixed. 
        """
        return _ground_coordinate.GroundCoordinate_convert_to_cf(self)

    __swig_destroy__ = _ground_coordinate.delete_GroundCoordinate
GroundCoordinate._v_latitude = new_instancemethod(_ground_coordinate.GroundCoordinate__v_latitude,None,GroundCoordinate)
GroundCoordinate._v_longitude = new_instancemethod(_ground_coordinate.GroundCoordinate__v_longitude,None,GroundCoordinate)
GroundCoordinate._v_height_reference_surface = new_instancemethod(_ground_coordinate.GroundCoordinate__v_height_reference_surface,None,GroundCoordinate)
GroundCoordinate.convert_to_cf = new_instancemethod(_ground_coordinate.GroundCoordinate_convert_to_cf,None,GroundCoordinate)
GroundCoordinate.__str__ = new_instancemethod(_ground_coordinate.GroundCoordinate___str__,None,GroundCoordinate)
GroundCoordinate_swigregister = _ground_coordinate.GroundCoordinate_swigregister
GroundCoordinate_swigregister(GroundCoordinate)

class CartesianInertial(geocal_swig.generic_object.GenericObject):
    """
    This gives the location in a Cartesian inertial coordinate system
    (.e.g., ECI).

    See CartesianFixed for a discussion on converting to and from
    CartesianFixed.

    C++ includes: ground_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def convert_to_cf(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianInertial::convert_to_cf(const Time &T) const =0
        Convert to a CartesianFixed. 
        """
        return _ground_coordinate.CartesianInertial_convert_to_cf(self, *args)

    def ci_to_cf(self, *args):
        """
        virtual void GeoCal::CartesianInertial::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const =0
        Calculate matrix that can be used to convert CartesianFixed to
        CartesianInertial at the given Time.

        The matrix for going from CartesianInertial to CartesianFixed is the
        transpose of this matrix. 
        """
        return _ground_coordinate.CartesianInertial_ci_to_cf(self, *args)

    def ci_to_cf_quat(self, *args):
        """
        boost::math::quaternion<double> GeoCal::CartesianInertial::ci_to_cf_quat(const Time &T) const
        Calculate quaternion that can be used to convert CartesianFixed to
        CartesianInertial at the given Time. 
        """
        return _ground_coordinate.CartesianInertial_ci_to_cf_quat(self, *args)

    def create(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianInertial::create(boost::array< double, 3 > P) const =0
        Create an instance of whatever type of CartesianInertial this is. 
        """
        return _ground_coordinate.CartesianInertial_create(self, *args)

    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianInertial::reference_surface_intersect_approximate(const CartesianInertialLookVector &Cl, double
        Height_reference_surface=0) const =0
        Find the intersection with the surface at the given height starting at
        this point and looking in the given direction.

        The intersection is done in an approximate but quicker manner, see the
        derived classes for the approximation used. 
        """
        return _ground_coordinate.CartesianInertial_reference_surface_intersect_approximate(self, *args)

    @property
    def position(self):
        return self._position()

    @position.setter
    def position(self, value):
      self._position(value)

    __swig_destroy__ = _ground_coordinate.delete_CartesianInertial
CartesianInertial.convert_to_cf = new_instancemethod(_ground_coordinate.CartesianInertial_convert_to_cf,None,CartesianInertial)
CartesianInertial.ci_to_cf = new_instancemethod(_ground_coordinate.CartesianInertial_ci_to_cf,None,CartesianInertial)
CartesianInertial.ci_to_cf_quat = new_instancemethod(_ground_coordinate.CartesianInertial_ci_to_cf_quat,None,CartesianInertial)
CartesianInertial.create = new_instancemethod(_ground_coordinate.CartesianInertial_create,None,CartesianInertial)
CartesianInertial.reference_surface_intersect_approximate = new_instancemethod(_ground_coordinate.CartesianInertial_reference_surface_intersect_approximate,None,CartesianInertial)
CartesianInertial.__str__ = new_instancemethod(_ground_coordinate.CartesianInertial___str__,None,CartesianInertial)
CartesianInertial._position = new_instancemethod(_ground_coordinate.CartesianInertial__position,None,CartesianInertial)
CartesianInertial_swigregister = _ground_coordinate.CartesianInertial_swigregister
CartesianInertial_swigregister(CartesianInertial)

class CartesianFixed(GroundCoordinate):
    """
    This gives the fixed Cartesian Coordinates for a ground coordinate.

    This is an abstract base class, there are CartesianFixed for the Earth
    (Ecr), Mars (MarsFixed), etc.

    We want to be able to convert to and from CartesianInertial
    coordinates. However, we want to abstract out the toolkit used to do
    this. We use the toolkit pointed to by
    CartesianFixed::toolkit_coordinate_interface. If either SPICE or SDP
    is available, then that toolkit is the one used for the conversion. If
    we have both available, then we default to SPICE. You can change the
    choice by updating the variable
    CartesianFixed::toolkit_coordinate_interface.

    C++ includes: ground_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def convert_to_ci(self, *args):
        """
        virtual boost::shared_ptr<CartesianInertial> GeoCal::CartesianFixed::convert_to_ci(const Time &T) const =0
        Convert to CartesianInertial. 
        """
        return _ground_coordinate.CartesianFixed_convert_to_ci(self, *args)

    def create(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::create(boost::array< double, 3 > P) const =0
        Create an instance of whatever type of CartesianFixed this is. 
        """
        return _ground_coordinate.CartesianFixed_create(self, *args)

    def ci_to_cf(self, *args):
        """
        virtual void GeoCal::CartesianFixed::ci_to_cf(const Time &T, double Ci_to_cf[3][3]) const =0
        Calculate matrix that can be used to convert CartesianFixed to
        CartesianInertial at the given Time.

        The matrix for going from CartesianInertial to CartesianFixed is the
        transpose of this matrix. 
        """
        return _ground_coordinate.CartesianFixed_ci_to_cf(self, *args)

    def ci_to_cf_quat(self, *args):
        """
        boost::math::quaternion<double> GeoCal::CartesianFixed::ci_to_cf_quat(const Time &T) const
        Calculate quaternion that can be used to convert CartesianFixed to
        CartesianInertial at the given Time. 
        """
        return _ground_coordinate.CartesianFixed_ci_to_cf_quat(self, *args)

    def reference_surface_intersect_approximate(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::CartesianFixed::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
        Height_reference_surface=0) const =0
        Find the intersection with the surface at the given height starting at
        this point and looking in the given direction.

        The intersection is done in an approximate but quicker manner, see the
        derived classes for the approximation used. 
        """
        return _ground_coordinate.CartesianFixed_reference_surface_intersect_approximate(self, *args)

    def _v_min_radius_reference_surface(self):
        """
        virtual double GeoCal::CartesianFixed::min_radius_reference_surface() const =0
        This is the radius of the largest sphere that fits completely into the
        reference surface (e.g., radius for a reference sphere, minimum of a
        and b for a ellipsoid). 
        """
        return _ground_coordinate.CartesianFixed__v_min_radius_reference_surface(self)

    @property
    def min_radius_reference_surface(self):
        return self._v_min_radius_reference_surface()

    @property
    def position(self):
        return self._position()

    @position.setter
    def position(self, value):
      self._position(value)

    __swig_destroy__ = _ground_coordinate.delete_CartesianFixed
CartesianFixed.convert_to_ci = new_instancemethod(_ground_coordinate.CartesianFixed_convert_to_ci,None,CartesianFixed)
CartesianFixed.create = new_instancemethod(_ground_coordinate.CartesianFixed_create,None,CartesianFixed)
CartesianFixed.ci_to_cf = new_instancemethod(_ground_coordinate.CartesianFixed_ci_to_cf,None,CartesianFixed)
CartesianFixed.ci_to_cf_quat = new_instancemethod(_ground_coordinate.CartesianFixed_ci_to_cf_quat,None,CartesianFixed)
CartesianFixed.reference_surface_intersect_approximate = new_instancemethod(_ground_coordinate.CartesianFixed_reference_surface_intersect_approximate,None,CartesianFixed)
CartesianFixed._v_min_radius_reference_surface = new_instancemethod(_ground_coordinate.CartesianFixed__v_min_radius_reference_surface,None,CartesianFixed)
CartesianFixed._position = new_instancemethod(_ground_coordinate.CartesianFixed__position,None,CartesianFixed)
CartesianFixed_swigregister = _ground_coordinate.CartesianFixed_swigregister
CartesianFixed_swigregister(CartesianFixed)

class LnLookVector(geocal_swig.look_vector.LookVectorDouble):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _ground_coordinate.LnLookVector_swiginit(self,_ground_coordinate.new_LnLookVector(*args))
    cf_to_enu = staticmethod(_ground_coordinate.LnLookVector_cf_to_enu)
    enu_to_cf = staticmethod(_ground_coordinate.LnLookVector_enu_to_cf)
    solar_look_vector = staticmethod(_ground_coordinate.LnLookVector_solar_look_vector)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.look_vector[0],self.look_vector[1],self.look_vector[2])

    __swig_destroy__ = _ground_coordinate.delete_LnLookVector
LnLookVector.to_cf = new_instancemethod(_ground_coordinate.LnLookVector_to_cf,None,LnLookVector)
LnLookVector.__str__ = new_instancemethod(_ground_coordinate.LnLookVector___str__,None,LnLookVector)
LnLookVector_swigregister = _ground_coordinate.LnLookVector_swigregister
LnLookVector_swigregister(LnLookVector)

def LnLookVector_cf_to_enu(*args):
  return _ground_coordinate.LnLookVector_cf_to_enu(*args)
LnLookVector_cf_to_enu = _ground_coordinate.LnLookVector_cf_to_enu

def LnLookVector_enu_to_cf(*args):
  return _ground_coordinate.LnLookVector_enu_to_cf(*args)
LnLookVector_enu_to_cf = _ground_coordinate.LnLookVector_enu_to_cf

def LnLookVector_solar_look_vector(*args):
  return _ground_coordinate.LnLookVector_solar_look_vector(*args)
LnLookVector_solar_look_vector = _ground_coordinate.LnLookVector_solar_look_vector

class CartesianFixedLookVector(geocal_swig.look_vector.LookVectorDouble):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        _ground_coordinate.CartesianFixedLookVector_swiginit(self,_ground_coordinate.new_CartesianFixedLookVector(*args))
    solar_look_vector = staticmethod(_ground_coordinate.CartesianFixedLookVector_solar_look_vector)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.look_vector[0],self.look_vector[1],self.look_vector[2])

    __swig_destroy__ = _ground_coordinate.delete_CartesianFixedLookVector
CartesianFixedLookVector.__str__ = new_instancemethod(_ground_coordinate.CartesianFixedLookVector___str__,None,CartesianFixedLookVector)
CartesianFixedLookVector_swigregister = _ground_coordinate.CartesianFixedLookVector_swigregister
CartesianFixedLookVector_swigregister(CartesianFixedLookVector)

def CartesianFixedLookVector_solar_look_vector(*args):
  return _ground_coordinate.CartesianFixedLookVector_solar_look_vector(*args)
CartesianFixedLookVector_solar_look_vector = _ground_coordinate.CartesianFixedLookVector_solar_look_vector


def distance(*args):
  """
    double GeoCal::distance(const GroundCoordinate &G1, const GroundCoordinate &G2)
    Distance in meters between ground coordinates.

    This is the distance of the points in 3 space, i.e. it is not distance
    along a geodesic or anything like that. 
    """
  return _ground_coordinate.distance(*args)
class Vector_GroundCoordinate(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _ground_coordinate.Vector_GroundCoordinate_swiginit(self,_ground_coordinate.new_Vector_GroundCoordinate(*args))
    __swig_destroy__ = _ground_coordinate.delete_Vector_GroundCoordinate
Vector_GroundCoordinate.iterator = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_iterator,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__nonzero__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___nonzero__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__bool__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___bool__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__len__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___len__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.pop = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_pop,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__getslice__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___getslice__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__setslice__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___setslice__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__delslice__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___delslice__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__delitem__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___delitem__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__getitem__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___getitem__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.__setitem__ = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate___setitem__,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.append = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_append,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.empty = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_empty,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.size = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_size,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.clear = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_clear,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.swap = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_swap,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.get_allocator = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_get_allocator,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.begin = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_begin,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.end = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_end,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.rbegin = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_rbegin,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.rend = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_rend,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.pop_back = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_pop_back,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.erase = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_erase,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.push_back = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_push_back,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.front = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_front,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.back = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_back,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.assign = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_assign,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.resize = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_resize,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.insert = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_insert,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.reserve = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_reserve,None,Vector_GroundCoordinate)
Vector_GroundCoordinate.capacity = new_instancemethod(_ground_coordinate.Vector_GroundCoordinate_capacity,None,Vector_GroundCoordinate)
Vector_GroundCoordinate_swigregister = _ground_coordinate.Vector_GroundCoordinate_swigregister
Vector_GroundCoordinate_swigregister(Vector_GroundCoordinate)



