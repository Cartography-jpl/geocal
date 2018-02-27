# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _quickbird_orbit.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
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
    pass  # Python < 2.2 doesn't have 'property'.


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


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



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
except:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _quickbird_orbit.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_quickbird_orbit.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_quickbird_orbit.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_quickbird_orbit.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_quickbird_orbit.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_quickbird_orbit.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_quickbird_orbit.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_quickbird_orbit.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_quickbird_orbit.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_quickbird_orbit.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_quickbird_orbit.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _quickbird_orbit.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_quickbird_orbit.SHARED_PTR_DISOWN_swigconstant(_quickbird_orbit)
SHARED_PTR_DISOWN = _quickbird_orbit.SHARED_PTR_DISOWN

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

import geocal_swig.generic_object
import geocal_swig.orbit
import geocal_swig.observer
import geocal_swig.with_parameter
class QuickBirdEphemeris(geocal_swig.generic_object.GenericObject):
    """

    This is a low level class that reads a Quickbird ephemeris file.

    You probably don't want to use this directly, but rather use the
    QuickBirdOrbit

    C++ includes: quickbird_orbit.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Fname):
        """

        GeoCal::QuickBirdEphemeris::QuickBirdEphemeris(const std::string &Fname)
        Read the quickbird ephemeris file. 
        """
        _quickbird_orbit.QuickBirdEphemeris_swiginit(self, _quickbird_orbit.new_QuickBirdEphemeris(Fname))

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


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdEphemeris
QuickBirdEphemeris._v_min_time = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_min_time, None, QuickBirdEphemeris)
QuickBirdEphemeris._v_max_time = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_max_time, None, QuickBirdEphemeris)
QuickBirdEphemeris._v_time_spacing = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_time_spacing, None, QuickBirdEphemeris)
QuickBirdEphemeris._v_ephemeris = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_ephemeris, None, QuickBirdEphemeris)
QuickBirdEphemeris._v_file_name = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris__v_file_name, None, QuickBirdEphemeris)
QuickBirdEphemeris.__str__ = new_instancemethod(_quickbird_orbit.QuickBirdEphemeris___str__, None, QuickBirdEphemeris)
QuickBirdEphemeris_swigregister = _quickbird_orbit.QuickBirdEphemeris_swigregister
QuickBirdEphemeris_swigregister(QuickBirdEphemeris)

class QuickBirdAttitude(geocal_swig.generic_object.GenericObject):
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

    def __init__(self, Fname):
        """

        GeoCal::QuickBirdAttitude::QuickBirdAttitude(const std::string &Fname)
        Read the quickbird attitude file. 
        """
        _quickbird_orbit.QuickBirdAttitude_swiginit(self, _quickbird_orbit.new_QuickBirdAttitude(Fname))

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


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdAttitude
QuickBirdAttitude._v_min_time = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_min_time, None, QuickBirdAttitude)
QuickBirdAttitude._v_max_time = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_max_time, None, QuickBirdAttitude)
QuickBirdAttitude._v_time_spacing = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_time_spacing, None, QuickBirdAttitude)
QuickBirdAttitude._v_attitude = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_attitude, None, QuickBirdAttitude)
QuickBirdAttitude._v_file_name = new_instancemethod(_quickbird_orbit.QuickBirdAttitude__v_file_name, None, QuickBirdAttitude)
QuickBirdAttitude.__str__ = new_instancemethod(_quickbird_orbit.QuickBirdAttitude___str__, None, QuickBirdAttitude)
QuickBirdAttitude_swigregister = _quickbird_orbit.QuickBirdAttitude_swigregister
QuickBirdAttitude_swigregister(QuickBirdAttitude)

class QuickBirdOrbit(geocal_swig.orbit.Orbit):
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

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > QuickBirdOrbit::orbit_data(const TimeWithDerivative &T) const

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


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _quickbird_orbit.delete_QuickBirdOrbit
QuickBirdOrbit.orbit_data = new_instancemethod(_quickbird_orbit.QuickBirdOrbit_orbit_data, None, QuickBirdOrbit)
QuickBirdOrbit._v_ephemeris_file_name = new_instancemethod(_quickbird_orbit.QuickBirdOrbit__v_ephemeris_file_name, None, QuickBirdOrbit)
QuickBirdOrbit._v_attitude_file_name = new_instancemethod(_quickbird_orbit.QuickBirdOrbit__v_attitude_file_name, None, QuickBirdOrbit)
QuickBirdOrbit_swigregister = _quickbird_orbit.QuickBirdOrbit_swigregister
QuickBirdOrbit_swigregister(QuickBirdOrbit)


__all__ = ["QuickBirdOrbit"]



