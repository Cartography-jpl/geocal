# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _mspi_camera.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_mspi_camera', [dirname(__file__)])
        except ImportError:
            import _mspi_camera
            return _mspi_camera
        if fp is not None:
            try:
                _mod = imp.load_module('_mspi_camera', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _mspi_camera = swig_import_helper()
    del swig_import_helper
else:
    import _mspi_camera
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


SHARED_PTR_DISOWN = _mspi_camera.SHARED_PTR_DISOWN
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

import geocal_swig.quaternion_camera
import geocal_swig.camera
import geocal_swig.generic_object
class MspiCamera(geocal_swig.quaternion_camera.QuaternionCamera):
    """
    This is the MSPI camera model.

    References: [1] Veljko Jovanovic, "Algorithm Theoretical Basis and
    processing software design consideration for GroundMSPI L1B2
    process", Rev A.1, April 4, 2012.

    C++ includes: mspi_camera.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::MspiCamera::MspiCamera(const std::string &File_name, const blitz::Array< double, 1 > &Parm)
        Constructor, which creates a MspiCamera from the given configuration
        file and then updates the parameters to the given value. 
        """
        _mspi_camera.MspiCamera_swiginit(self,_mspi_camera.new_MspiCamera(*args))
    def read_config_file(self, *args):
        """
        void MspiCamera::read_config_file(const std::string &File_name)
        Read in the given configuration file.

        Get mapping from band to row number 
        """
        return _mspi_camera.MspiCamera_read_config_file(self, *args)

    def _v_file_name(self):
        """
        const std::string& GeoCal::MspiCamera::file_name() const
        File name for MspiConfigFile. 
        """
        return _mspi_camera.MspiCamera__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def _v_granule_id(self):
        """
        const std::string& GeoCal::MspiCamera::granule_id() const
        Granule ID. This is metadata, found in the configuration file. 
        """
        return _mspi_camera.MspiCamera__v_granule_id(self)

    @property
    def granule_id(self):
        return self._v_granule_id()

    def _v_epsilon(self):
        """
        double GeoCal::MspiCamera::epsilon() const
        Epsilon angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_epsilon(self)

    @property
    def epsilon(self):
        return self._v_epsilon()

    def _v_psi(self):
        """
        double GeoCal::MspiCamera::psi() const
        Psi angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_psi(self)

    @property
    def psi(self):
        return self._v_psi()

    def _v_theta(self):
        """
        double GeoCal::MspiCamera::theta() const
        Theta angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_theta(self)

    @property
    def theta(self):
        return self._v_theta()

    def _v_yaw(self):
        """
        double GeoCal::MspiCamera::yaw() const
        Yaw angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_yaw(self)

    @property
    def yaw(self):
        return self._v_yaw()

    def _v_pitch(self):
        """
        double GeoCal::MspiCamera::pitch() const
        Pitch angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_pitch(self)

    @property
    def pitch(self):
        return self._v_pitch()

    def _v_roll(self):
        """
        double GeoCal::MspiCamera::roll() const
        Roll angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_roll(self)

    @property
    def roll(self):
        return self._v_roll()

    def _v_boresight_angle(self):
        """
        double GeoCal::MspiCamera::boresight_angle() const
        Boresight angle, in radians. 
        """
        return _mspi_camera.MspiCamera__v_boresight_angle(self)

    @property
    def boresight_angle(self):
        return self._v_boresight_angle()

    def _v_inversion(self):
        """
        bool GeoCal::MspiCamera::inversion() const
        Indicate if the camera has "inversion" indicated in the
        configuration. 
        """
        return _mspi_camera.MspiCamera__v_inversion(self)

    @property
    def inversion(self):
        return self._v_inversion()

    def angular_separation(self, *args):
        """
        double MspiCamera::angular_separation(int Reference_band, int Target_band) const
        Return angular separation (in radians) between the given reference and
        target bands in the real focal plane.

        (MSPI L1B2 ATB equation 14 modified to use Forigin in place of b0) 
        """
        return _mspi_camera.MspiCamera_angular_separation(self, *args)

    def row_number(self, *args):
        """
        int GeoCal::MspiCamera::row_number(int Band) const
        Return the camera row number for the given band.

        This ends up being used in other places (e.g., the "Row Table" in
        L1B1 files), so we make this available. 
        """
        return _mspi_camera.MspiCamera_row_number(self, *args)

    def band_number(self, *args):
        """
        int MspiCamera::band_number(int Row_number) const
        Return the band number for the given row. 
        """
        return _mspi_camera.MspiCamera_band_number(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.parameter)

    __swig_destroy__ = _mspi_camera.delete_MspiCamera
MspiCamera.read_config_file = new_instancemethod(_mspi_camera.MspiCamera_read_config_file,None,MspiCamera)
MspiCamera._v_file_name = new_instancemethod(_mspi_camera.MspiCamera__v_file_name,None,MspiCamera)
MspiCamera._v_granule_id = new_instancemethod(_mspi_camera.MspiCamera__v_granule_id,None,MspiCamera)
MspiCamera._v_epsilon = new_instancemethod(_mspi_camera.MspiCamera__v_epsilon,None,MspiCamera)
MspiCamera._v_psi = new_instancemethod(_mspi_camera.MspiCamera__v_psi,None,MspiCamera)
MspiCamera._v_theta = new_instancemethod(_mspi_camera.MspiCamera__v_theta,None,MspiCamera)
MspiCamera._v_yaw = new_instancemethod(_mspi_camera.MspiCamera__v_yaw,None,MspiCamera)
MspiCamera._v_pitch = new_instancemethod(_mspi_camera.MspiCamera__v_pitch,None,MspiCamera)
MspiCamera._v_roll = new_instancemethod(_mspi_camera.MspiCamera__v_roll,None,MspiCamera)
MspiCamera._v_boresight_angle = new_instancemethod(_mspi_camera.MspiCamera__v_boresight_angle,None,MspiCamera)
MspiCamera._v_inversion = new_instancemethod(_mspi_camera.MspiCamera__v_inversion,None,MspiCamera)
MspiCamera.angular_separation = new_instancemethod(_mspi_camera.MspiCamera_angular_separation,None,MspiCamera)
MspiCamera.row_number = new_instancemethod(_mspi_camera.MspiCamera_row_number,None,MspiCamera)
MspiCamera.band_number = new_instancemethod(_mspi_camera.MspiCamera_band_number,None,MspiCamera)
MspiCamera.__str__ = new_instancemethod(_mspi_camera.MspiCamera___str__,None,MspiCamera)
MspiCamera_swigregister = _mspi_camera.MspiCamera_swigregister
MspiCamera_swigregister(MspiCamera)



