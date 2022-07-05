# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _mspi_gimbal.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_mspi_gimbal')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_mspi_gimbal')
    _mspi_gimbal = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_mspi_gimbal', [dirname(__file__)])
        except ImportError:
            import _mspi_gimbal
            return _mspi_gimbal
        try:
            _mod = imp.load_module('_mspi_gimbal', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _mspi_gimbal = swig_import_helper()
    del swig_import_helper
else:
    import _mspi_gimbal
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
    __swig_destroy__ = _mspi_gimbal.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_mspi_gimbal.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_mspi_gimbal.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_mspi_gimbal.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_mspi_gimbal.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_mspi_gimbal.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_mspi_gimbal.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_mspi_gimbal.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_mspi_gimbal.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_mspi_gimbal.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_mspi_gimbal.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _mspi_gimbal.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _mspi_gimbal.SHARED_PTR_DISOWN

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

import geocal_swig.with_parameter
import geocal_swig.generic_object
import geocal_swig.observer
class ObservableMspiGimbal(geocal_swig.generic_object.GenericObject):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _mspi_gimbal.delete_ObservableMspiGimbal
ObservableMspiGimbal.add_observer_and_keep_reference = new_instancemethod(_mspi_gimbal.ObservableMspiGimbal_add_observer_and_keep_reference, None, ObservableMspiGimbal)
ObservableMspiGimbal.add_observer = new_instancemethod(_mspi_gimbal.ObservableMspiGimbal_add_observer, None, ObservableMspiGimbal)
ObservableMspiGimbal.remove_observer = new_instancemethod(_mspi_gimbal.ObservableMspiGimbal_remove_observer, None, ObservableMspiGimbal)
ObservableMspiGimbal_swigregister = _mspi_gimbal.ObservableMspiGimbal_swigregister
ObservableMspiGimbal_swigregister(ObservableMspiGimbal)

class ObserverMspiGimbal(geocal_swig.generic_object.GenericObject):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        _mspi_gimbal.ObserverMspiGimbal_swiginit(self, _mspi_gimbal.new_ObserverMspiGimbal())
    __swig_destroy__ = _mspi_gimbal.delete_ObserverMspiGimbal
ObserverMspiGimbal.notify_update = new_instancemethod(_mspi_gimbal.ObserverMspiGimbal_notify_update, None, ObserverMspiGimbal)
ObserverMspiGimbal.notify_add = new_instancemethod(_mspi_gimbal.ObserverMspiGimbal_notify_add, None, ObserverMspiGimbal)
ObserverMspiGimbal.notify_remove = new_instancemethod(_mspi_gimbal.ObserverMspiGimbal_notify_remove, None, ObserverMspiGimbal)
ObserverMspiGimbal_swigregister = _mspi_gimbal.ObserverMspiGimbal_swigregister
ObserverMspiGimbal_swigregister(ObserverMspiGimbal)

class MspiGimbal(geocal_swig.with_parameter.WithParameter, ObservableMspiGimbal):
    """

    This handles the AirMSPI gimbal calculations.

    C++ includes: mspi_gimbal.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::MspiGimbal::MspiGimbal(double Epsilon, double Psi, double Theta)
        Constructor. The angles should be in degrees. 
        """
        _mspi_gimbal.MspiGimbal_swiginit(self, _mspi_gimbal.new_MspiGimbal(*args))

    def read_config_file(self, *args):
        """

        void MspiGimbal::read_config_file(const std::string &File_name, const std::string
        &Extra_config_file="")
        Read in the given configuration file. 
        """
        return _mspi_gimbal.MspiGimbal_read_config_file(self, *args)


    def _v_epsilon(self):
        """

        double GeoCal::MspiGimbal::epsilon() const
        Gimbal epsilon in degrees. 
        """
        return _mspi_gimbal.MspiGimbal__v_epsilon(self)


    @property
    def epsilon(self):
        return self._v_epsilon()


    def _v_epsilon_with_derivative(self):
        """

        AutoDerivative<double> GeoCal::MspiGimbal::epsilon_with_derivative() const

        """
        return _mspi_gimbal.MspiGimbal__v_epsilon_with_derivative(self)


    @property
    def epsilon_with_derivative(self):
        return self._v_epsilon_with_derivative()


    def _v_psi(self):
        """

        double GeoCal::MspiGimbal::psi() const
        Gimbal psi in degrees.

        We don't actually include the gimbal angles in the camera model,
        instead this is handled by AirMspiOrbit. However the gimbal parameters
        are recorded in the camera configuration file, so it make sense to
        read and report them here. 
        """
        return _mspi_gimbal.MspiGimbal__v_psi(self)


    @property
    def psi(self):
        return self._v_psi()


    def _v_psi_with_derivative(self):
        """

        AutoDerivative<double> GeoCal::MspiGimbal::psi_with_derivative() const

        """
        return _mspi_gimbal.MspiGimbal__v_psi_with_derivative(self)


    @property
    def psi_with_derivative(self):
        return self._v_psi_with_derivative()


    def _v_theta(self):
        """

        double GeoCal::MspiGimbal::theta() const
        Gimbal theta in degrees.

        We don't actually include the gimbal angles in the camera model,
        instead this is handled by AirMspiOrbit. However the gimbal parameters
        are recorded in the camera configuration file, so it make sense to
        read and report them here. 
        """
        return _mspi_gimbal.MspiGimbal__v_theta(self)


    @property
    def theta(self):
        return self._v_theta()


    def _v_theta_with_derivative(self):
        """

        AutoDerivative<double> GeoCal::MspiGimbal::theta_with_derivative() const

        """
        return _mspi_gimbal.MspiGimbal__v_theta_with_derivative(self)


    @property
    def theta_with_derivative(self):
        return self._v_theta_with_derivative()


    def _v_parameter_mask(self, *args):
        """

        void MspiGimbal::parameter_mask(const blitz::Array< bool, 1 > &Pm)

        """
        return _mspi_gimbal.MspiGimbal__v_parameter_mask(self, *args)


    @property
    def parameter_mask(self):
        return self._v_parameter_mask()

    @parameter_mask.setter
    def parameter_mask(self, value):
      self._v_parameter_mask(value)


    def station_to_sc_with_derivative(self, Gimbal_pos):
        """

        boost::math::quaternion< AutoDerivative< double > > MspiGimbal::station_to_sc_with_derivative(const AutoDerivative< double > &Gimbal_pos) const
        Return the station to spacecraft transformation.

        The gimbal position should be in radians. This is inconsistent with
        the other angles used by this class that are in degrees, but this
        matches what AirMSPI navigation data is stored as. 
        """
        return _mspi_gimbal.MspiGimbal_station_to_sc_with_derivative(self, Gimbal_pos)


    def station_to_sc(self, Gimbal_pos):
        """

        boost::math::quaternion< double > MspiGimbal::station_to_sc(double Gimbal_pos) const

        """
        return _mspi_gimbal.MspiGimbal_station_to_sc(self, Gimbal_pos)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _mspi_gimbal.delete_MspiGimbal
MspiGimbal.read_config_file = new_instancemethod(_mspi_gimbal.MspiGimbal_read_config_file, None, MspiGimbal)
MspiGimbal._v_epsilon = new_instancemethod(_mspi_gimbal.MspiGimbal__v_epsilon, None, MspiGimbal)
MspiGimbal._v_epsilon_with_derivative = new_instancemethod(_mspi_gimbal.MspiGimbal__v_epsilon_with_derivative, None, MspiGimbal)
MspiGimbal._v_psi = new_instancemethod(_mspi_gimbal.MspiGimbal__v_psi, None, MspiGimbal)
MspiGimbal._v_psi_with_derivative = new_instancemethod(_mspi_gimbal.MspiGimbal__v_psi_with_derivative, None, MspiGimbal)
MspiGimbal._v_theta = new_instancemethod(_mspi_gimbal.MspiGimbal__v_theta, None, MspiGimbal)
MspiGimbal._v_theta_with_derivative = new_instancemethod(_mspi_gimbal.MspiGimbal__v_theta_with_derivative, None, MspiGimbal)
MspiGimbal._v_parameter_mask = new_instancemethod(_mspi_gimbal.MspiGimbal__v_parameter_mask, None, MspiGimbal)
MspiGimbal.station_to_sc_with_derivative = new_instancemethod(_mspi_gimbal.MspiGimbal_station_to_sc_with_derivative, None, MspiGimbal)
MspiGimbal.station_to_sc = new_instancemethod(_mspi_gimbal.MspiGimbal_station_to_sc, None, MspiGimbal)
MspiGimbal.__str__ = new_instancemethod(_mspi_gimbal.MspiGimbal___str__, None, MspiGimbal)
MspiGimbal_swigregister = _mspi_gimbal.MspiGimbal_swigregister
MspiGimbal_swigregister(MspiGimbal)


__all__ = ["MspiGimbal","ObserverMspiGimbal","ObservableMspiGimbal"]



