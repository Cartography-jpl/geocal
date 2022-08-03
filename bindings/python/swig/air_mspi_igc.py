# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _air_mspi_igc.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_air_mspi_igc')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_air_mspi_igc')
    _air_mspi_igc = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_air_mspi_igc', [dirname(__file__)])
        except ImportError:
            import _air_mspi_igc
            return _air_mspi_igc
        try:
            _mod = imp.load_module('_air_mspi_igc', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _air_mspi_igc = swig_import_helper()
    del swig_import_helper
else:
    import _air_mspi_igc
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


try:
    import weakref
    weakref_proxy = weakref.proxy
except __builtin__.Exception:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _air_mspi_igc.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_air_mspi_igc.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_air_mspi_igc.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_air_mspi_igc.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_air_mspi_igc.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_air_mspi_igc.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_air_mspi_igc.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_air_mspi_igc.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_air_mspi_igc.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_air_mspi_igc.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_air_mspi_igc.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _air_mspi_igc.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _air_mspi_igc.SHARED_PTR_DISOWN

import os

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

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
import geocal_swig.observer
import geocal_swig.raster_image_tiled_file
import geocal_swig.raster_image_variable
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

        AirMspiIgc::AirMspiIgc(const boost::shared_ptr< Orbit > &Orb, const boost::shared_ptr<
        MspiCamera > &Cam, const boost::shared_ptr< MspiGimbal > &Gim, const
        boost::shared_ptr< Dem > &Dem, const std::string &L1b1_file_name, int
        Min_l1b1_line, int Max_l1b1_line, const std::string
        &Swath_to_use="660-I", const std::string &Title="Image", int
        Dem_resolution=10, int Tile_number_line=-1, int Tile_number_sample=-1,
        unsigned int Number_tile=4)
        Constructor.

        This variation of the constructor directly takes the information
        needed to construct the object, rather than reading this from the
        master configuration file. 
        """
        _air_mspi_igc.AirMspiIgc_swiginit(self, _air_mspi_igc.new_AirMspiIgc(*args))

    def _v_band(self, *args):
        """

        void AirMspiIgc::band(int B)
        Set band that we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_band(self, *args)


    @property
    def band(self):
        return self._v_band()

    @band.setter
    def band(self, value):
      self._v_band(value)


    def _v_orbit(self):
        """

        boost::shared_ptr<Orbit> GeoCal::AirMspiIgc::orbit() const
        Orbit we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_orbit(self)


    @property
    def orbit(self):
        return self._v_orbit()


    def _v_time_table(self):
        """

        boost::shared_ptr<AirMspiTimeTable> GeoCal::AirMspiIgc::time_table() const
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


    def _v_gimbal(self):
        """

        boost::shared_ptr<MspiGimbal> GeoCal::AirMspiIgc::gimbal() const
        Gimbal we are using. 
        """
        return _air_mspi_igc.AirMspiIgc__v_gimbal(self)


    @property
    def gimbal(self):
        return self._v_gimbal()


    def _v_l1b1_file_name(self):
        """

        std::string GeoCal::AirMspiIgc::l1b1_file_name() const
        The name of the l1b1 file. 
        """
        return _air_mspi_igc.AirMspiIgc__v_l1b1_file_name(self)


    @property
    def l1b1_file_name(self):
        return self._v_l1b1_file_name()


    def __reduce__(self):
      return _new_from_serialization_dir, (os.getcwd(), geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _air_mspi_igc.delete_AirMspiIgc
AirMspiIgc._v_band = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_band, None, AirMspiIgc)
AirMspiIgc._v_orbit = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_orbit, None, AirMspiIgc)
AirMspiIgc._v_time_table = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_time_table, None, AirMspiIgc)
AirMspiIgc._v_camera = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_camera, None, AirMspiIgc)
AirMspiIgc._v_gimbal = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_gimbal, None, AirMspiIgc)
AirMspiIgc._v_l1b1_file_name = new_instancemethod(_air_mspi_igc.AirMspiIgc__v_l1b1_file_name, None, AirMspiIgc)
AirMspiIgc_swigregister = _air_mspi_igc.AirMspiIgc_swigregister
AirMspiIgc_swigregister(AirMspiIgc)


__all__ = ["AirMspiIgc"]



