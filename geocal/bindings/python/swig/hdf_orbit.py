# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _hdf_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_hdf_orbit', [dirname(__file__)])
        except ImportError:
            import _hdf_orbit
            return _hdf_orbit
        if fp is not None:
            try:
                _mod = imp.load_module('_hdf_orbit', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _hdf_orbit = swig_import_helper()
    del swig_import_helper
else:
    import _hdf_orbit
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


SHARED_PTR_DISOWN = _hdf_orbit.SHARED_PTR_DISOWN
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

import geocal.orbit
import geocal.generic_object
class HdfOrbit_EciTod_TimeAcs(geocal.orbit.Orbit):
    """
    This is an implementation of an Orbit that reads position, velocity,
    and attitude quaternion from an HDF file.

    For times that fall between the values given in the file, we
    interpolate to get the OrbitData.

    The file should have the following fields:

    <Base group>="">/Attitude/Time - natt in size, time of attitude
    measurement as doubles. <Base group>="">/Attitude/Quaternion - natt
    x 4 in size, quaternion to take same coordinate system as Position
    (e.g., ECI) <Base group>="">/Ephemeris/Time - neph in size, time of
    ephemeris measurement as doubles <Base group>="">/Ephemeris/Position
    - neph x 3 in size, position measurement as doubles in meters <Base
    group>="">/Ephemeris/Velocity - neph x 3 in size, velocity
    measurement as doubles in meter/second

    Because it is useful, we allow the type of position measurement and
    time to be changed. The measurement class is passed in, e.g., Eci, as
    is a small wrapper to give the conversion to Time.

    C++ includes: hdf_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::HdfOrbit< PositionType, TimeCreatorType >::HdfOrbit(const std::string &Fname, const std::string &Base_group="Orbit")
        Read the given orbit data file.

        You can optional pass the base group of the HDF file, the default is
        "/Orbit" 
        """
        _hdf_orbit.HdfOrbit_EciTod_TimeAcs_swiginit(self,_hdf_orbit.new_HdfOrbit_EciTod_TimeAcs(*args))
    def _v_file_name(self):
        """
        const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::file_name() const
        Return the file name. 
        """
        return _hdf_orbit.HdfOrbit_EciTod_TimeAcs__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_base_group(self):
        """
        const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::base_group() const
        Return the base group. 
        """
        return _hdf_orbit.HdfOrbit_EciTod_TimeAcs__v_base_group(self)

    @property
    def base_group(self):
        return self._v_base_group()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.base_group)

    __swig_destroy__ = _hdf_orbit.delete_HdfOrbit_EciTod_TimeAcs
HdfOrbit_EciTod_TimeAcs._v_file_name = new_instancemethod(_hdf_orbit.HdfOrbit_EciTod_TimeAcs__v_file_name,None,HdfOrbit_EciTod_TimeAcs)
HdfOrbit_EciTod_TimeAcs._v_base_group = new_instancemethod(_hdf_orbit.HdfOrbit_EciTod_TimeAcs__v_base_group,None,HdfOrbit_EciTod_TimeAcs)
HdfOrbit_EciTod_TimeAcs_swigregister = _hdf_orbit.HdfOrbit_EciTod_TimeAcs_swigregister
HdfOrbit_EciTod_TimeAcs_swigregister(HdfOrbit_EciTod_TimeAcs)

class HdfOrbit_EciTodBurl_TimeAcs(geocal.orbit.Orbit):
    """
    This is an implementation of an Orbit that reads position, velocity,
    and attitude quaternion from an HDF file.

    For times that fall between the values given in the file, we
    interpolate to get the OrbitData.

    The file should have the following fields:

    <Base group>="">/Attitude/Time - natt in size, time of attitude
    measurement as doubles. <Base group>="">/Attitude/Quaternion - natt
    x 4 in size, quaternion to take same coordinate system as Position
    (e.g., ECI) <Base group>="">/Ephemeris/Time - neph in size, time of
    ephemeris measurement as doubles <Base group>="">/Ephemeris/Position
    - neph x 3 in size, position measurement as doubles in meters <Base
    group>="">/Ephemeris/Velocity - neph x 3 in size, velocity
    measurement as doubles in meter/second

    Because it is useful, we allow the type of position measurement and
    time to be changed. The measurement class is passed in, e.g., Eci, as
    is a small wrapper to give the conversion to Time.

    C++ includes: hdf_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::HdfOrbit< PositionType, TimeCreatorType >::HdfOrbit(const std::string &Fname, const std::string &Base_group="Orbit")
        Read the given orbit data file.

        You can optional pass the base group of the HDF file, the default is
        "/Orbit" 
        """
        _hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs_swiginit(self,_hdf_orbit.new_HdfOrbit_EciTodBurl_TimeAcs(*args))
    def _v_file_name(self):
        """
        const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::file_name() const
        Return the file name. 
        """
        return _hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_base_group(self):
        """
        const std::string& GeoCal::HdfOrbit< PositionType, TimeCreatorType >::base_group() const
        Return the base group. 
        """
        return _hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs__v_base_group(self)

    @property
    def base_group(self):
        return self._v_base_group()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.base_group)

    __swig_destroy__ = _hdf_orbit.delete_HdfOrbit_EciTodBurl_TimeAcs
HdfOrbit_EciTodBurl_TimeAcs._v_file_name = new_instancemethod(_hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs__v_file_name,None,HdfOrbit_EciTodBurl_TimeAcs)
HdfOrbit_EciTodBurl_TimeAcs._v_base_group = new_instancemethod(_hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs__v_base_group,None,HdfOrbit_EciTodBurl_TimeAcs)
HdfOrbit_EciTodBurl_TimeAcs_swigregister = _hdf_orbit.HdfOrbit_EciTodBurl_TimeAcs_swigregister
HdfOrbit_EciTodBurl_TimeAcs_swigregister(HdfOrbit_EciTodBurl_TimeAcs)



