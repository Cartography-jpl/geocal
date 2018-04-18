# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _igc_rolling_shutter.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_rolling_shutter', [dirname(__file__)])
        except ImportError:
            import _igc_rolling_shutter
            return _igc_rolling_shutter
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_rolling_shutter', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_rolling_shutter = swig_import_helper()
    del swig_import_helper
else:
    import _igc_rolling_shutter
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
    __swig_destroy__ = _igc_rolling_shutter.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_igc_rolling_shutter.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_igc_rolling_shutter.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _igc_rolling_shutter.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_igc_rolling_shutter.SHARED_PTR_DISOWN_swigconstant(_igc_rolling_shutter)
SHARED_PTR_DISOWN = _igc_rolling_shutter.SHARED_PTR_DISOWN

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

import geocal_swig.image_ground_connection
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
import geocal_swig.observer
class IgcRollingShutter(geocal_swig.image_ground_connection.ImageGroundConnection):
    """

    This is a ImageGroundConnection where the connection is made by
    OrbitData and a Camera.

    This is similar to OrbitDataImageGroundConnection, however this is
    intended for use with a rolling shutter camera.

    A rolling shutter is common with CMOS sensors. Rather than recording
    an image at a single instance, the image is recorded by scanning over
    the camera rapidly in either the line or sample direction.

    If the camera is moving while the image is taken (e.g., it is on an
    aircraft), then significant differences can occur between modeling
    this as close a frame camera vs. modeling the rolling shutter.

    We currently only support the line roll direction, although we have
    some interface support for sample roll direction (just in case we need
    to expand this in the future).

    Note that this class assumes that the orbit data varies smoothly over
    the time that the rolling shutter operates. We speed up the class by
    taking the orbit data at the start and end of the rolling shutter and
    interpolating. If this is not true of the orbit data, then there will
    be significant errors in the calculations done by this class.

    C++ includes: igc_rolling_shutter.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    ROLL_LINE_DIRECTION = _igc_rolling_shutter.IgcRollingShutter_ROLL_LINE_DIRECTION
    ROLL_SAMPLE_DIRECTION = _igc_rolling_shutter.IgcRollingShutter_ROLL_SAMPLE_DIRECTION

    def __init__(self, *args):
        """

        GeoCal::IgcRollingShutter::IgcRollingShutter(const boost::shared_ptr< Orbit > &Orb, const boost::shared_ptr<
        TimeTable > &Time_table, const boost::shared_ptr< Camera > &Cam, const
        boost::shared_ptr< Dem > &D, const boost::shared_ptr< RasterImage >
        &Img, RollDirection Roll_direction=ROLL_LINE_DIRECTION, const
        std::string Title="", const boost::shared_ptr< Refraction >
        &Ref=boost::shared_ptr< Refraction >(), double Resolution=30, int
        Band=0, double Max_height=9000)
        Constructor that takes a Orbit and a Time_table.

        If the roll direction is in the line direction we assume that all the
        samples for a particular line are acquired at the same time, and if
        the roll direction is in the sample direction we assume that all the
        lines for a particular sample are acquired at the same time.

        You can optionally include a approximate refraction correction, the
        default is not to. 
        """
        _igc_rolling_shutter.IgcRollingShutter_swiginit(self, _igc_rolling_shutter.new_IgcRollingShutter(*args))

    def cf_look_vector(self, Ic):
        """

        void IgcRollingShutter::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
        boost::shared_ptr< CartesianFixed > &P) const

        """
        return _igc_rolling_shutter.IgcRollingShutter_cf_look_vector(self, Ic)


    def _v_orbit(self, *args):
        """

        void IgcRollingShutter::orbit(const boost::shared_ptr< Orbit > &Orb)
        Set orbit. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_orbit(self, *args)


    @property
    def orbit(self):
        return self._v_orbit()

    @orbit.setter
    def orbit(self, value):
      self._v_orbit(value)


    def _v_time_table(self, *args):
        """

        void GeoCal::IgcRollingShutter::time_table(const boost::shared_ptr< TimeTable > &Tt)
        Set time table. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_time_table(self, *args)


    @property
    def time_table(self):
        return self._v_time_table()

    @time_table.setter
    def time_table(self, value):
      self._v_time_table(value)


    def _v_camera(self, *args):
        """

        void IgcRollingShutter::camera(const boost::shared_ptr< Camera > &C)
        Set Camera that we are using. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_camera(self, *args)


    @property
    def camera(self):
        return self._v_camera()

    @camera.setter
    def camera(self, value):
      self._v_camera(value)


    def _v_resolution(self, *args):
        """

        void GeoCal::IgcRollingShutter::resolution(double R)
        Set resolution in meters that we examine Dem out.

        This affects how long ground_coordinate takes to figure out. It should
        be about the resolution of the Dem 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_resolution(self, *args)


    @property
    def resolution(self):
        return self._v_resolution()

    @resolution.setter
    def resolution(self, value):
      self._v_resolution(value)


    def _v_refraction(self, *args):
        """

        void GeoCal::IgcRollingShutter::refraction(const boost::shared_ptr< Refraction > &Ref)
        Set refraction object we are using.

        May be null if we aren't including refraction. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_refraction(self, *args)


    @property
    def refraction(self):
        return self._v_refraction()

    @refraction.setter
    def refraction(self, value):
      self._v_refraction(value)


    def _v_band(self, *args):
        """

        void GeoCal::IgcRollingShutter::band(int B)
        Set camera band we are using. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_band(self, *args)


    @property
    def band(self):
        return self._v_band()

    @band.setter
    def band(self, value):
      self._v_band(value)


    def _v_max_height(self, *args):
        """

        void GeoCal::IgcRollingShutter::max_height(double Max_h)
        Set Maximum height that we expect to see in the Dem. 
        """
        return _igc_rolling_shutter.IgcRollingShutter__v_max_height(self, *args)


    @property
    def max_height(self):
        return self._v_max_height()

    @max_height.setter
    def max_height(self, value):
      self._v_max_height(value)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _igc_rolling_shutter.delete_IgcRollingShutter
IgcRollingShutter.cf_look_vector = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter_cf_look_vector, None, IgcRollingShutter)
IgcRollingShutter._v_orbit = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_orbit, None, IgcRollingShutter)
IgcRollingShutter._v_time_table = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_time_table, None, IgcRollingShutter)
IgcRollingShutter._v_camera = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_camera, None, IgcRollingShutter)
IgcRollingShutter._v_resolution = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_resolution, None, IgcRollingShutter)
IgcRollingShutter._v_refraction = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_refraction, None, IgcRollingShutter)
IgcRollingShutter._v_band = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_band, None, IgcRollingShutter)
IgcRollingShutter._v_max_height = new_instancemethod(_igc_rolling_shutter.IgcRollingShutter__v_max_height, None, IgcRollingShutter)
IgcRollingShutter_swigregister = _igc_rolling_shutter.IgcRollingShutter_swigregister
IgcRollingShutter_swigregister(IgcRollingShutter)



