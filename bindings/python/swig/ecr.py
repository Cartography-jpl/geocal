# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ecr.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ecr', [dirname(__file__)])
        except ImportError:
            import _ecr
            return _ecr
        if fp is not None:
            try:
                _mod = imp.load_module('_ecr', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ecr = swig_import_helper()
    del swig_import_helper
else:
    import _ecr
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


SHARED_PTR_DISOWN = _ecr.SHARED_PTR_DISOWN
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

import geocal_swig.generic_object
import geocal_swig.ground_coordinate
class Ecr(geocal_swig.ground_coordinate.CartesianFixed):
    """
    This is a ground coordinate, represented in ECR coordinates.

    C++ includes: ecr.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::Ecr::Ecr()
        Default constructor. 
        """
        _ecr.Ecr_swiginit(self,_ecr.new_Ecr(*args))
    def convert_to_geodetic(self):
        """
        Geodetic Ecr::convert_to_geodetic() const
        Convert to Geodetic coordinates. 
        """
        return _ecr.Ecr_convert_to_geodetic(self)

    def reference_surface_intersect_approximate(self, *args):
        """
        boost::shared_ptr< CartesianFixed > Ecr::reference_surface_intersect_approximate(const CartesianFixedLookVector &Cl, double
        Height_reference_surface=0) const
        Find the intersection with the surface at the given height starting at
        this point and looking in the given direction.

        The intersection is done in an approximate but quicker manner.

        We find the intersection with the ellipsoid that has the given height
        added to the equatorial and polar axis. This is not the same as the
        figure with a height h above the ellipsoid (which isn't an ellipsoid),
        but for many purposes this is close enough. 
        """
        return _ecr.Ecr_reference_surface_intersect_approximate(self, *args)

    def sub_solar_point(*args):
        """
        Ecr Ecr::sub_solar_point(const Time &T)
        Return the subsolar point for the given time.

        This is the point on the surface of the reference ellipsoid that lies
        on the line from the center of the earth to the sun. 
        """
        return _ecr.Ecr_sub_solar_point(*args)

    sub_solar_point = staticmethod(sub_solar_point)
    def solar_distance(*args):
        """
        double Ecr::solar_distance(const Time &T)
        Solar distance at given time.

        This is AU (which is defined to be exactly 149597870700 meter). 
        """
        return _ecr.Ecr_solar_distance(*args)

    solar_distance = staticmethod(solar_distance)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.position[0],self.position[1],self.position[2])

    __swig_destroy__ = _ecr.delete_Ecr
Ecr.convert_to_geodetic = new_instancemethod(_ecr.Ecr_convert_to_geodetic,None,Ecr)
Ecr.reference_surface_intersect_approximate = new_instancemethod(_ecr.Ecr_reference_surface_intersect_approximate,None,Ecr)
Ecr_swigregister = _ecr.Ecr_swigregister
Ecr_swigregister(Ecr)

def Ecr_sub_solar_point(*args):
  """
    Ecr Ecr::sub_solar_point(const Time &T)
    Return the subsolar point for the given time.

    This is the point on the surface of the reference ellipsoid that lies
    on the line from the center of the earth to the sun. 
    """
  return _ecr.Ecr_sub_solar_point(*args)

def Ecr_solar_distance(*args):
  """
    double Ecr::solar_distance(const Time &T)
    Solar distance at given time.

    This is AU (which is defined to be exactly 149597870700 meter). 
    """
  return _ecr.Ecr_solar_distance(*args)



