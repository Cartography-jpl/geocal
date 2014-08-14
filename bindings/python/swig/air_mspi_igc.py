# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _air_mspi_igc.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_air_mspi_igc', [dirname(__file__)])
        except ImportError:
            import _air_mspi_igc
            return _air_mspi_igc
        if fp is not None:
            try:
                _mod = imp.load_module('_air_mspi_igc', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _air_mspi_igc = swig_import_helper()
    del swig_import_helper
else:
    import _air_mspi_igc
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


SHARED_PTR_DISOWN = _air_mspi_igc.SHARED_PTR_DISOWN
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

import geocal_swig.ipi_image_ground_connection
import geocal_swig.image_ground_connection
import geocal_swig.generic_object
import geocal_swig.geocal_exception
import geocal_swig.look_vector
import geocal_swig.quaternion_camera
class AirMspiIgc(geocal_swig.ipi_image_ground_connection.IpiImageGroundConnection):
    """
    This is an ImageGroundConnection for AirMspi.

    C++ includes: air_mspi_igc.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        AirMspiIgc::AirMspiIgc(const std::string &Master_config_file, const std::string
        &Orbit_file_name, const std::string &L1b1_file_name, int Band, const
        std::string &Base_directory=".")
        Constructor.

        This takes the master config file and uses it to create a AirMspiIgc.

        You can optionally add the base directory that file names in the
        Master_config_file are relative to. The default is the current
        directory. 
        """
        _air_mspi_igc.AirMspiIgc_swiginit(self,_air_mspi_igc.new_AirMspiIgc(*args))
    def _v_band(self, *args):
        """
        void GeoCal::AirMspiIgc::band(int B)
        Set band that we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_band(self, *args)

    @property
    def band(self):
        return self._v_band()

    @band.setter
    def band(self, value):
      self._v_band(value)

    def _v_base_directory(self):
        """
        const std::string& GeoCal::AirMspiIgc::base_directory() const
        The base directory that various files in the master config file are
        relative to. 
        """
        return _air_mspi_igc.AirMspiIgc__v_base_directory(self)

    @property
    def base_directory(self):
        return self._v_base_directory()

    def _v_master_config_file(self):
        """
        const std::string& GeoCal::AirMspiIgc::master_config_file() const
        The name of the master config file used to create this object. 
        """
        return _air_mspi_igc.AirMspiIgc__v_master_config_file(self)

    @property
    def master_config_file(self):
        return self._v_master_config_file()

    def _v_orbit(self):
        """
        boost::shared_ptr<AirMspiOrbit> GeoCal::AirMspiIgc::orbit() const
        Orbit we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_orbit(self)

    @property
    def orbit(self):
        return self._v_orbit()

    def _v_time_table(self):
        """
        boost::shared_ptr<TimeTable> GeoCal::AirMspiIgc::time_table() const
        TimeTable we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_time_table(self)

    @property
    def time_table(self):
        return self._v_time_table()

    def _v_camera(self):
        """
        boost::shared_ptr<MspiCamera> GeoCal::AirMspiIgc::camera() const
        Camera we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_camera(self)

    @property
    def camera(self):
        return self._v_camera()

    def _v_orbit_file_name(self):
        """
        std::string GeoCal::AirMspiIgc::orbit_file_name() const
        The name of the orbit file. 
        """
        return _air_mspi_igc.AirMspiIgc__v_orbit_file_name(self)

    @property
    def orbit_file_name(self):
        return self._v_orbit_file_name()

    def _v_l1b1_file_name(self):
        """
        std::string GeoCal::AirMspiIgc::l1b1_file_name() const
        The name of the l1b1 file. 
        """
        return _air_mspi_igc.AirMspiIgc__v_l1b1_file_name(self)

    @property
    def l1b1_file_name(self):
        return self._v_l1b1_file_name()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.master_config_file,self.orbit_file_name,self.l1b1_file_name,self.band,self.base_directory)

    __swig_destroy__ = _air_mspi_igc.delete_AirMspiIgc
AirMspiIgc._v_band = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_band,None,AirMspiIgc)
AirMspiIgc._v_base_directory = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_base_directory,None,AirMspiIgc)
AirMspiIgc._v_master_config_file = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_master_config_file,None,AirMspiIgc)
AirMspiIgc._v_orbit = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_orbit,None,AirMspiIgc)
AirMspiIgc._v_time_table = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_time_table,None,AirMspiIgc)
AirMspiIgc._v_camera = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_camera,None,AirMspiIgc)
AirMspiIgc._v_orbit_file_name = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_orbit_file_name,None,AirMspiIgc)
AirMspiIgc._v_l1b1_file_name = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_l1b1_file_name,None,AirMspiIgc)
AirMspiIgc_swigregister = _air_mspi_igc.AirMspiIgc_swigregister
AirMspiIgc_swigregister(AirMspiIgc)



