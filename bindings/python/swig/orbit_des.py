# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_des.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_orbit_des')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_orbit_des')
    _orbit_des = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_des', [dirname(__file__)])
        except ImportError:
            import _orbit_des
            return _orbit_des
        try:
            _mod = imp.load_module('_orbit_des', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _orbit_des = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_des
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
    __swig_destroy__ = _orbit_des.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_des.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_des.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_des.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_des.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_des.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_des.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_des.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_des.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_des.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_des.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_des.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_des.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_des.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_des.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_des.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_des.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_des.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_des.SHARED_PTR_DISOWN

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
class PosCsephb(geocal_swig.generic_object.GenericObject):
    """

    This handles position reading, writing, and interpolation.

    This uses the NITF DES CSEPHB (See the SNIP documentation).

    Note that this class doesn't read and write the full DES, only the
    data portion. It works with the python code found in
    geocal_des_extension.py.

    The CSEPHB DES doens't contain velocity. We calculate the velocity
    from the positions.

    Note that the the CSEPHB data is like a NITF TRE. But because it is a
    DES, it is potentially much larger. For efficiency, we read and write
    the data as istream and ostream rather than return strings as we
    typically do for TREs. On the python side, this can be mapped from a
    io object like FileHandle or BytesIO.

    C++ includes: orbit_des.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    EPHEMERIS_QUALITY_SUSPECT = _orbit_des.PosCsephb_EPHEMERIS_QUALITY_SUSPECT
    EPHEMERIS_QUALITY_GOOD = _orbit_des.PosCsephb_EPHEMERIS_QUALITY_GOOD
    NEAREST_NEIGHBOR = _orbit_des.PosCsephb_NEAREST_NEIGHBOR
    LINEAR = _orbit_des.PosCsephb_LINEAR
    LAGRANGE = _orbit_des.PosCsephb_LAGRANGE
    NO_LAGRANGE = _orbit_des.PosCsephb_NO_LAGRANGE
    LAGRANGE_1 = _orbit_des.PosCsephb_LAGRANGE_1
    LAGRANGE_3 = _orbit_des.PosCsephb_LAGRANGE_3
    LAGRANGE_5 = _orbit_des.PosCsephb_LAGRANGE_5
    LAGRANGE_7 = _orbit_des.PosCsephb_LAGRANGE_7
    PREDICTED = _orbit_des.PosCsephb_PREDICTED
    ACTUAL = _orbit_des.PosCsephb_ACTUAL
    REFINED = _orbit_des.PosCsephb_REFINED

    def __init__(self, *args):
        """

        PosCsephb::PosCsephb(const Orbit &Orb, const Time &Min_time, const Time &Max_time, double
        Tstep, InterpolationType Itype=LINEAR, LagrangeOrder
        Lagrange_order=NO_LAGRANGE, EphemerisDataQuality
        E_quality=EPHEMERIS_QUALITY_GOOD, EphemerisSource E_source=ACTUAL)
        Constructor.

        We sample the position of the given Orbit at fixed spaces times. This
        version goes from the Min_time, up to Max_time (or more accurately,
        the largest time Min_time i * Tstep that is <= Max_time).w 
        """
        _orbit_des.PosCsephb_swiginit(self, _orbit_des.new_PosCsephb(*args))

    def pos_vel(self, *args):
        """

        blitz::Array< AutoDerivative< double >, 1 > PosCsephb::pos_vel(const TimeWithDerivative &T) const
        Return position and velocity for the given time. 
        """
        return _orbit_des.PosCsephb_pos_vel(self, *args)


    def _v_is_cf(self):
        """

        bool GeoCal::PosCsephb::is_cf() const
        True if data is CartesianFixed.

        If false, then data is CartesianInertial. 
        """
        return _orbit_des.PosCsephb__v_is_cf(self)


    @property
    def is_cf(self):
        return self._v_is_cf()


    def _v_min_time(self):
        """

        const Time& GeoCal::PosCsephb::min_time() const
        Minimum time we have data for. 
        """
        return _orbit_des.PosCsephb__v_min_time(self)


    @property
    def min_time(self):
        return self._v_min_time()


    def _v_max_time(self):
        """

        Time GeoCal::PosCsephb::max_time() const
        Maximum time we have data for. 
        """
        return _orbit_des.PosCsephb__v_max_time(self)


    @property
    def max_time(self):
        return self._v_max_time()


    def _v_time_step(self):
        """

        double GeoCal::PosCsephb::time_step() const
        Time step between position data, in seconds. 
        """
        return _orbit_des.PosCsephb__v_time_step(self)


    @property
    def time_step(self):
        return self._v_time_step()


    def _v_interpolation_type(self, *args):
        """

        void GeoCal::PosCsephb::interpolation_type(InterpolationType Itype)

        """
        return _orbit_des.PosCsephb__v_interpolation_type(self, *args)


    @property
    def interpolation_type(self):
        return self._v_interpolation_type()

    @interpolation_type.setter
    def interpolation_type(self, value):
      self._v_interpolation_type(value)


    def _v_ephemeris_data_quality(self, *args):
        """

        void GeoCal::PosCsephb::ephemeris_data_quality(EphemerisDataQuality E_quality)

        """
        return _orbit_des.PosCsephb__v_ephemeris_data_quality(self, *args)


    @property
    def ephemeris_data_quality(self):
        return self._v_ephemeris_data_quality()

    @ephemeris_data_quality.setter
    def ephemeris_data_quality(self, value):
      self._v_ephemeris_data_quality(value)


    def _v_ephemeris_source(self, *args):
        """

        void GeoCal::PosCsephb::ephemeris_source(EphemerisSource E_source)

        """
        return _orbit_des.PosCsephb__v_ephemeris_source(self, *args)


    @property
    def ephemeris_source(self):
        return self._v_ephemeris_source()

    @ephemeris_source.setter
    def ephemeris_source(self, value):
      self._v_ephemeris_source(value)


    def _v_lagrange_order(self, *args):
        """

        void GeoCal::PosCsephb::lagrange_order(LagrangeOrder Lagrange_order)

        """
        return _orbit_des.PosCsephb__v_lagrange_order(self, *args)


    @property
    def lagrange_order(self):
        return self._v_lagrange_order()

    @lagrange_order.setter
    def lagrange_order(self, value):
      self._v_lagrange_order(value)


    def _v_position_data(self):
        """

        const blitz::Array<double, 2>& GeoCal::PosCsephb::position_data() const
        Raw data. 
        """
        return _orbit_des.PosCsephb__v_position_data(self)


    @property
    def position_data(self):
        return self._v_position_data()


    def min_time_split(self):
        """

        void PosCsephb::min_time_split(std::string &d_mtime, std::string &t_mtime) const
        Return min_time split into the component pieces the DES requires. 
        """
        return _orbit_des.PosCsephb_min_time_split(self)


    @property
    def qual_flag_eph(self):
        return self.ephemeris_data_quality

    @property
    def interp_type_eph(self):
        return self.interpolation_type

    @property
    def interp_order_eph(self):
        return self.lagrange_order

    @property
    def ephem_flag(self):  
        return self.ephemeris_source

    @property
    def eci_ecf_ephem(self):
        return (1 if self.is_cf else 0)

    @property
    def dt_ephem(self):
        return self.time_step

    @property
    def date_ephem(self):
        return int(self.min_time_split()[0])

    @property
    def t0_ephem(self):
        return float(self.min_time_split()[1])

    @property
    def reserved_len(self):
        return 0

    @property
    def num_ephem(self):
        return self.position_data.shape[0]

    @property
    def ephem_x(self):
        return self.position_data[:,0]

    @property
    def ephem_y(self):
        return self.position_data[:,1]

    @property
    def ephem_z(self):
        return self.position_data[:,2]


    def des_write(self, Os):
        """

        void PosCsephb::des_write(std::ostream &Os) const
        Write out the DES data to the given stream. 
        """
        return _orbit_des.PosCsephb_des_write(self, Os)


    def des_read(In):
        """

        boost::shared_ptr< PosCsephb > PosCsephb::des_read(std::istream &In)
        Read the DES data the given stream. 
        """
        return _orbit_des.PosCsephb_des_read(In)

    des_read = staticmethod(des_read)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_des.delete_PosCsephb
PosCsephb.pos_vel = new_instancemethod(_orbit_des.PosCsephb_pos_vel, None, PosCsephb)
PosCsephb._v_is_cf = new_instancemethod(_orbit_des.PosCsephb__v_is_cf, None, PosCsephb)
PosCsephb._v_min_time = new_instancemethod(_orbit_des.PosCsephb__v_min_time, None, PosCsephb)
PosCsephb._v_max_time = new_instancemethod(_orbit_des.PosCsephb__v_max_time, None, PosCsephb)
PosCsephb._v_time_step = new_instancemethod(_orbit_des.PosCsephb__v_time_step, None, PosCsephb)
PosCsephb._v_interpolation_type = new_instancemethod(_orbit_des.PosCsephb__v_interpolation_type, None, PosCsephb)
PosCsephb._v_ephemeris_data_quality = new_instancemethod(_orbit_des.PosCsephb__v_ephemeris_data_quality, None, PosCsephb)
PosCsephb._v_ephemeris_source = new_instancemethod(_orbit_des.PosCsephb__v_ephemeris_source, None, PosCsephb)
PosCsephb._v_lagrange_order = new_instancemethod(_orbit_des.PosCsephb__v_lagrange_order, None, PosCsephb)
PosCsephb._v_position_data = new_instancemethod(_orbit_des.PosCsephb__v_position_data, None, PosCsephb)
PosCsephb.min_time_split = new_instancemethod(_orbit_des.PosCsephb_min_time_split, None, PosCsephb)
PosCsephb.des_write = new_instancemethod(_orbit_des.PosCsephb_des_write, None, PosCsephb)
PosCsephb.__str__ = new_instancemethod(_orbit_des.PosCsephb___str__, None, PosCsephb)
PosCsephb_swigregister = _orbit_des.PosCsephb_swigregister
PosCsephb_swigregister(PosCsephb)

def PosCsephb_des_read(In):
    """

    boost::shared_ptr< PosCsephb > PosCsephb::des_read(std::istream &In)
    Read the DES data the given stream. 
    """
    return _orbit_des.PosCsephb_des_read(In)

class AttCsattb(geocal_swig.generic_object.GenericObject):
    """

    This handles attitude reading, writing, and interpolation.

    This uses the NITF DES CSEATTB (See the SNIP documentation).

    Note that this class doesn't read and write the full DES, only the
    data portion. It works with the python code found in
    geocal_des_extension.py.

    Note that the the CSATTB data is like a NITF TRE. But because it is a
    DES, it is potentially much larger. For efficiency, we read and write
    the data as istream and ostream rather than return strings as we
    typically do for TREs. On the python side, this can be mapped from a
    io object like FileHandle or BytesIO.

    Note that NITF has different convention for quaternions than we use
    internally with OrbitData. We use the functions nitf_to_quaternion and
    quaternion_to_nitf to map back and forth, look at the documentation of
    those functions to see what the mapping does.

    C++ includes: orbit_des.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    ATTITUDE_QUALITY_SUSPECT = _orbit_des.AttCsattb_ATTITUDE_QUALITY_SUSPECT
    ATTITUDE_QUALITY_GOOD = _orbit_des.AttCsattb_ATTITUDE_QUALITY_GOOD
    NEAREST_NEIGHBOR = _orbit_des.AttCsattb_NEAREST_NEIGHBOR
    LINEAR = _orbit_des.AttCsattb_LINEAR
    LAGRANGE = _orbit_des.AttCsattb_LAGRANGE
    NO_LAGRANGE = _orbit_des.AttCsattb_NO_LAGRANGE
    LAGRANGE_1 = _orbit_des.AttCsattb_LAGRANGE_1
    LAGRANGE_3 = _orbit_des.AttCsattb_LAGRANGE_3
    LAGRANGE_5 = _orbit_des.AttCsattb_LAGRANGE_5
    LAGRANGE_7 = _orbit_des.AttCsattb_LAGRANGE_7
    PREDICTED = _orbit_des.AttCsattb_PREDICTED
    ACTUAL = _orbit_des.AttCsattb_ACTUAL
    REFINED = _orbit_des.AttCsattb_REFINED

    def __init__(self, *args):
        """

        AttCsattb::AttCsattb(const Orbit &Orb, const Time &Min_time, const Time &Max_time, double
        Tstep, InterpolationType Itype=LINEAR, LagrangeOrder
        Lagrange_order=NO_LAGRANGE, AttitudeDataQuality
        A_quality=ATTITUDE_QUALITY_GOOD, AttitudeSource A_source=ACTUAL)
        Constructor.

        We sample the attitude of the given Orbit at fixed spaces times. This
        version goes from the Min_time, up to Max_time (or more accurately,
        the largest time Min_time i * Tstep that is <= Max_time).w 
        """
        _orbit_des.AttCsattb_swiginit(self, _orbit_des.new_AttCsattb(*args))

    def _v_is_cf(self):
        """

        bool GeoCal::AttCsattb::is_cf() const
        True if data is CartesianFixed.

        If false, then data is CartesianInertial. 
        """
        return _orbit_des.AttCsattb__v_is_cf(self)


    @property
    def is_cf(self):
        return self._v_is_cf()


    def _v_min_time(self):
        """

        const Time& GeoCal::AttCsattb::min_time() const
        Minimum time we have data for. 
        """
        return _orbit_des.AttCsattb__v_min_time(self)


    @property
    def min_time(self):
        return self._v_min_time()


    def _v_max_time(self):
        """

        Time GeoCal::AttCsattb::max_time() const
        Maximum time we have data for. 
        """
        return _orbit_des.AttCsattb__v_max_time(self)


    @property
    def max_time(self):
        return self._v_max_time()


    def _v_time_step(self):
        """

        double GeoCal::AttCsattb::time_step() const
        Time step between attitude data, in seconds. 
        """
        return _orbit_des.AttCsattb__v_time_step(self)


    @property
    def time_step(self):
        return self._v_time_step()


    def _v_interpolation_type(self, *args):
        """

        void GeoCal::AttCsattb::interpolation_type(InterpolationType Itype)

        """
        return _orbit_des.AttCsattb__v_interpolation_type(self, *args)


    @property
    def interpolation_type(self):
        return self._v_interpolation_type()

    @interpolation_type.setter
    def interpolation_type(self, value):
      self._v_interpolation_type(value)


    def _v_attitude_data_quality(self, *args):
        """

        void GeoCal::AttCsattb::attitude_data_quality(AttitudeDataQuality A_quality)

        """
        return _orbit_des.AttCsattb__v_attitude_data_quality(self, *args)


    @property
    def attitude_data_quality(self):
        return self._v_attitude_data_quality()

    @attitude_data_quality.setter
    def attitude_data_quality(self, value):
      self._v_attitude_data_quality(value)


    def _v_attitude_source(self, *args):
        """

        void GeoCal::AttCsattb::attitude_source(AttitudeSource A_source)

        """
        return _orbit_des.AttCsattb__v_attitude_source(self, *args)


    @property
    def attitude_source(self):
        return self._v_attitude_source()

    @attitude_source.setter
    def attitude_source(self, value):
      self._v_attitude_source(value)


    def _v_lagrange_order(self, *args):
        """

        void GeoCal::AttCsattb::lagrange_order(LagrangeOrder Lagrange_order)

        """
        return _orbit_des.AttCsattb__v_lagrange_order(self, *args)


    @property
    def lagrange_order(self):
        return self._v_lagrange_order()

    @lagrange_order.setter
    def lagrange_order(self, value):
      self._v_lagrange_order(value)


    def _v_attitude_data(self):
        """

        const blitz::Array<double, 2>& GeoCal::AttCsattb::attitude_data() const
        Raw data. 
        """
        return _orbit_des.AttCsattb__v_attitude_data(self)


    @property
    def attitude_data(self):
        return self._v_attitude_data()


    def att_q(self, *args):
        """

        boost::math::quaternion< AutoDerivative< double > > AttCsattb::att_q(const TimeWithDerivative &T) const
        Return attitude quaternion for the given time. 
        """
        return _orbit_des.AttCsattb_att_q(self, *args)


    def min_time_split(self):
        """

        void AttCsattb::min_time_split(std::string &d_mtime, std::string &t_mtime) const
        Return min_time split into the component pieces the DES requires. 
        """
        return _orbit_des.AttCsattb_min_time_split(self)


    @property
    def qual_flag_att(self):
        return self.attitude_data_quality

    @property
    def interp_type_att(self):
        return self.interpolation_type

    @property
    def interp_order_att(self):
        return self.lagrange_order

    @property
    def att_type(self):  
        return self.attitude_source

    @property
    def eci_ecf_att(self):
        return (1 if self.is_cf else 0)

    @property
    def dt_att(self):
        return self.time_step

    @property
    def date_att(self):
        return int(self.min_time_split()[0])

    @property
    def t0_att(self):
        return float(self.min_time_split()[1])

    @property
    def reserved_len(self):
        return 0

    @property
    def num_att(self):
        return self.attitude_data.shape[0]

    @property
    def q1(self):
        return self.attitude_data[:,0]

    @property
    def q2(self):
        return self.attitude_data[:,1]

    @property
    def q3(self):
        return self.attitude_data[:,2]

    @property
    def q4(self):
        return self.attitude_data[:,3]



    def des_write(self, Os):
        """

        void AttCsattb::des_write(std::ostream &Os) const
        Write out the DES data to the given stream. 
        """
        return _orbit_des.AttCsattb_des_write(self, Os)


    def des_read(In):
        """

        boost::shared_ptr< AttCsattb > AttCsattb::des_read(std::istream &In)
        Read the DES data the given stream. 
        """
        return _orbit_des.AttCsattb_des_read(In)

    des_read = staticmethod(des_read)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_des.delete_AttCsattb
AttCsattb._v_is_cf = new_instancemethod(_orbit_des.AttCsattb__v_is_cf, None, AttCsattb)
AttCsattb._v_min_time = new_instancemethod(_orbit_des.AttCsattb__v_min_time, None, AttCsattb)
AttCsattb._v_max_time = new_instancemethod(_orbit_des.AttCsattb__v_max_time, None, AttCsattb)
AttCsattb._v_time_step = new_instancemethod(_orbit_des.AttCsattb__v_time_step, None, AttCsattb)
AttCsattb._v_interpolation_type = new_instancemethod(_orbit_des.AttCsattb__v_interpolation_type, None, AttCsattb)
AttCsattb._v_attitude_data_quality = new_instancemethod(_orbit_des.AttCsattb__v_attitude_data_quality, None, AttCsattb)
AttCsattb._v_attitude_source = new_instancemethod(_orbit_des.AttCsattb__v_attitude_source, None, AttCsattb)
AttCsattb._v_lagrange_order = new_instancemethod(_orbit_des.AttCsattb__v_lagrange_order, None, AttCsattb)
AttCsattb._v_attitude_data = new_instancemethod(_orbit_des.AttCsattb__v_attitude_data, None, AttCsattb)
AttCsattb.att_q = new_instancemethod(_orbit_des.AttCsattb_att_q, None, AttCsattb)
AttCsattb.min_time_split = new_instancemethod(_orbit_des.AttCsattb_min_time_split, None, AttCsattb)
AttCsattb.des_write = new_instancemethod(_orbit_des.AttCsattb_des_write, None, AttCsattb)
AttCsattb.__str__ = new_instancemethod(_orbit_des.AttCsattb___str__, None, AttCsattb)
AttCsattb_swigregister = _orbit_des.AttCsattb_swigregister
AttCsattb_swigregister(AttCsattb)

def AttCsattb_des_read(In):
    """

    boost::shared_ptr< AttCsattb > AttCsattb::des_read(std::istream &In)
    Read the DES data the given stream. 
    """
    return _orbit_des.AttCsattb_des_read(In)

class OrbitDes(geocal_swig.orbit.Orbit):
    """

    This combines a PosCsephb and AttCsattb into an Orbit.

    Note that we calculate things for every time point. If you are using
    this a bit you may want to create a OrbitQuaternionList from this
    Orbit to speed things up.

    Note that this orbit uses a different convention for the ScLookVector
    coordinates than we use in for example KeplerOrbit based on the
    EOS/MISR convention. We have +y in the line/along track direction and
    +x in the sample/cross track direction. The other convention we use in
    other orbits (e.g., MISR) has +x in the line direction and +y in the
    sample direction.

    C++ includes: orbit_des.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Pos, Att):
        """

        OrbitDes::OrbitDes(const boost::shared_ptr< PosCsephb > &Pos, const boost::shared_ptr<
        AttCsattb > &Att)
        Constructor. 
        """
        _orbit_des.OrbitDes_swiginit(self, _orbit_des.new_OrbitDes(Pos, Att))

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > OrbitDes::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_des.OrbitDes_orbit_data(self, *args)


    def _v_pos_csephb(self):
        """

        const boost::shared_ptr<PosCsephb>& GeoCal::OrbitDes::pos_csephb() const

        """
        return _orbit_des.OrbitDes__v_pos_csephb(self)


    @property
    def pos_csephb(self):
        return self._v_pos_csephb()


    def _v_att_csattb(self):
        """

        const boost::shared_ptr<AttCsattb>& GeoCal::OrbitDes::att_csattb() const

        """
        return _orbit_des.OrbitDes__v_att_csattb(self)


    @property
    def att_csattb(self):
        return self._v_att_csattb()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_des.delete_OrbitDes
OrbitDes.orbit_data = new_instancemethod(_orbit_des.OrbitDes_orbit_data, None, OrbitDes)
OrbitDes._v_pos_csephb = new_instancemethod(_orbit_des.OrbitDes__v_pos_csephb, None, OrbitDes)
OrbitDes._v_att_csattb = new_instancemethod(_orbit_des.OrbitDes__v_att_csattb, None, OrbitDes)
OrbitDes_swigregister = _orbit_des.OrbitDes_swigregister
OrbitDes_swigregister(OrbitDes)


__all__ = ["PosCsephb","AttCsattb","OrbitDes"]



