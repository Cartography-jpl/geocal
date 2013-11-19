# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _refraction.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_refraction', [dirname(__file__)])
        except ImportError:
            import _refraction
            return _refraction
        if fp is not None:
            try:
                _mod = imp.load_module('_refraction', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _refraction = swig_import_helper()
    del swig_import_helper
else:
    import _refraction
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


SHARED_PTR_DISOWN = _refraction.SHARED_PTR_DISOWN
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

import geocal.generic_object
class Refraction(geocal.generic_object.GenericObject):
    """
    This calculates refraction.

    The algorithm used was take from the SDP toolkit. The original code
    was written by Peter Noerdlinger as the SDP toolkit function
    PGS_CSC_SpaceRefract. The algorithm is described in detail in
    "Theoretical Basis of the SDP Toolkit Geolocation Package for the
    ECS".

    This depends on the index of refraction of air at the surface. We
    don't actually know this. There are various approximations, and the
    toolkit uses one described in the "Theoretical Basis of the SDP
    Toolkit Geolocation Package for the ECS".

    This class allows the index of refraction to simple be given. This
    allows us to just fit for this unknown using something like a
    simultanous bundle adjustment. Note that a reasonable range of values
    is something like 1.00026 to 1.00029, so you can vary from the initial
    prediction by a few 1e-5.

    C++ includes: refraction.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        Refraction::Refraction(double Altitude, double Latitude, double Index_refraction_surface=-1)
        This sets up for a Refraction calculation.

        A representative altitude (in meters) and latitude (in degrees) is
        given. This doesn't depend very strongly on altitude and latitude, so
        unless you using widely varying points just using the center of you
        scene should be pretty much sufficient.

        The index of refraction at the surface can be supplied if desired,
        otherwise we calculate an approximate value. 
        """
        _refraction.Refraction_swiginit(self,_refraction.new_Refraction(*args))
    def _v_altitude(self):
        """
        double GeoCal::Refraction::altitude() const
        Altitude of reference point, in meters. 
        """
        return _refraction.Refraction__v_altitude(self)

    @property
    def altitude(self):
        return self._v_altitude()

    def _v_latitude(self):
        """
        double GeoCal::Refraction::latitude() const
        Latitude of reference point, in degrees. 
        """
        return _refraction.Refraction__v_latitude(self)

    @property
    def latitude(self):
        return self._v_latitude()

    def _v_index_refraction_surface(self, *args):
        """
        void GeoCal::Refraction::index_refraction_surface(double Index_refraction_surface)
        Set the index of refraction. 
        """
        return _refraction.Refraction__v_index_refraction_surface(self, *args)

    @property
    def index_refraction_surface(self):
        return self._v_index_refraction_surface()

    @index_refraction_surface.setter
    def index_refraction_surface(self, value):
      self._v_index_refraction_surface(value)

    def surface_zenith(self, *args):
        """
        double GeoCal::Refraction::surface_zenith(double Space_zenith) const
        Surface zenith angle in radians, given the space zenith angles. 
        """
        return _refraction.Refraction_surface_zenith(self, *args)

    def displacement(self, *args):
        """
        double Refraction::displacement(double Space_zenith) const
        Displacement in meters for given space zenith angle in radians. 
        """
        return _refraction.Refraction_displacement(self, *args)

    def refraction_apply(self, *args):
        """
        boost::shared_ptr< GroundCoordinate > Refraction::refraction_apply(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
        &Gc_no_refraction) const
        Apply refraction.

        We take the position of the spacecraft and the ground coordinates
        before apply refraction. This return the ground coordinates accounting
        for refraction. 
        """
        return _refraction.Refraction_refraction_apply(self, *args)

    def refraction_reverse(self, *args):
        """
        boost::shared_ptr< GroundCoordinate > Refraction::refraction_reverse(const GroundCoordinate &Spacecraft_pos, const GroundCoordinate
        &Gc_with_refraction) const
        Reverse the refraction calculation.

        This returns a ground coordinate so that when we feed it to
        refraction_apply we get back the original ground coordinate. This is
        useful when trying to go the other way, from the ground to the
        spacecraft, so we remove the effects of refraction and then continue
        with a normal orbit/camera model.

        Note for speed we don't calculate the exact reverse of
        refraction_apply, but a close approximation. For a MISR DF camera, the
        difference is only 2 cm. For most applications, the difference
        shouldn't matter. But if we have an application where this does, we'll
        want to replace this with a more exact calculation. 
        """
        return _refraction.Refraction_refraction_reverse(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.altitude,self.latitude,self.index_refraction_surface)

    __swig_destroy__ = _refraction.delete_Refraction
Refraction._v_altitude = new_instancemethod(_refraction.Refraction__v_altitude,None,Refraction)
Refraction._v_latitude = new_instancemethod(_refraction.Refraction__v_latitude,None,Refraction)
Refraction._v_index_refraction_surface = new_instancemethod(_refraction.Refraction__v_index_refraction_surface,None,Refraction)
Refraction.surface_zenith = new_instancemethod(_refraction.Refraction_surface_zenith,None,Refraction)
Refraction.displacement = new_instancemethod(_refraction.Refraction_displacement,None,Refraction)
Refraction.refraction_apply = new_instancemethod(_refraction.Refraction_refraction_apply,None,Refraction)
Refraction.refraction_reverse = new_instancemethod(_refraction.Refraction_refraction_reverse,None,Refraction)
Refraction.__str__ = new_instancemethod(_refraction.Refraction___str__,None,Refraction)
Refraction_swigregister = _refraction.Refraction_swigregister
Refraction_swigregister(Refraction)



