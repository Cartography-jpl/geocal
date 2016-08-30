# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _ipi.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ipi', [dirname(__file__)])
        except ImportError:
            import _ipi
            return _ipi
        if fp is not None:
            try:
                _mod = imp.load_module('_ipi', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ipi = swig_import_helper()
    del swig_import_helper
else:
    import _ipi
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
    __swig_destroy__ = _ipi.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_ipi.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ipi.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ipi.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ipi.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ipi.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ipi.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ipi.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ipi.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ipi.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ipi.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ipi.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ipi.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ipi.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ipi.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ipi.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ipi.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _ipi.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_ipi.SHARED_PTR_DISOWN_swigconstant(_ipi)
SHARED_PTR_DISOWN = _ipi.SHARED_PTR_DISOWN

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
import geocal_swig.observer
import geocal_swig.with_parameter
class Ipi(geocal_swig.generic_object.GenericObject):
    """

    This performs an image point intersection (IPI), finding the time or
    image coordinates that a given ground point is seen by a camera.

    This solves the collinearity equation in order to find the time and
    frame coordinates that a ground point is seen. Note however that the
    collinearity equation can give "false" solutions, because it is
    perfectly happy to find points seen by looking straight through the
    earth (since it does not account for an obstructed view).

    These "false" solutions have nothing to do with the obscuration
    "looking being a mountain" sorts of problems. This actually is a
    different problem. When you run the IPI you can get solutions that are
    on the other side of earth, so looking at when a ground point in the
    United State is seen you can get a solution that returns a time for
    when the spacecraft is flying over China. This is not the "behind a
    mountain" issue, but the fact that a line of sight intersects a
    sphere twice on opposite sides of the Earth. The "false solutions"
    tries to throw these "other side of the earth" solutions out. But
    the IPI does nothing for simple obscuration of the "behind a
    mountain" sorts. The IPI assumes that the point it is given is not
    obscured by terrain. If you need to calculate obscuration, you should
    use a full ray caster (e.g., IgcRayCaster).

    We also get "false" solutions if the camera model diverges (i.e, we
    pass in a look vector parallel to the camera CCD plane, so the point
    is not seen by the camera). This is because the root finder doesn't
    actually look for a zero, but rather a change in the sign of the
    equation. When the camera model diverges, we go from negative infinity
    to positive infinity. We recognize these "false" solutions by
    comparing the collinearity equation results to a user supplied
    tolerance.

    We find all the solutions to the collinearity equation, in the range
    Tmin to Tmax. The solutions found must be seperated by a time larger
    then Root_min_separation.

    We then inspect the list of roots, and reject those that are "false"
    solutions. If 1 solution is left, we return it with the success flag
    set to true. Otherwise, we return with the success flag set to false.

    It is very common to call the IPI with a ground point near the last
    call. As a performance improvement, we remember the time results from
    the previous IPI calculation. We first look in a local time range
    around the previous time result. If no solution is found, we then look
    over the entire time range.

    C++ includes: ipi.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        Ipi::Ipi(const boost::shared_ptr< Orbit > &Orb, const boost::shared_ptr<
        Camera > &Cam, int Band, Time Tmin, Time Tmax, const
        boost::shared_ptr< TimeTable > &Tt=boost::shared_ptr< TimeTable >(),
        double Local_time_window_size=5.0, double Root_min_separation=30.0,
        double Time_tolerance=1e-6)
        Constructor.

        If you only want to get the Time from the Ipi and not ImageCoordinate,
        it is ok for Tt to be null.

        We find solutions to the Collinearity equations over the full time
        range Tmin to Tmax. However, as a performance enhancement we first
        look around the given Local_time_window_size in seconds around the
        solution from the last call to time() or image_coordinate(). We look
        for all solutions that are Root_min_separation apart. We find time
        with a accuracy given by Time_tolerance. 
        """
        _ipi.Ipi_swiginit(self, _ipi.new_Ipi(*args))

    def image_coordinate(self, Gp):
        """

        void Ipi::image_coordinate(const GroundCoordinate &Gp, ImageCoordinate &Ic, bool &Success) const
        Look for the image location that a point is seen.

        Note that unlike time, if a point is seen only outside of the sample
        range of the image then we consider the Ipi a failure. If Success is
        true, then Ic will have a line and sample number inside of the image
        given by the Camera and TimeTable. 
        """
        return _ipi.Ipi_image_coordinate(self, Gp)


    def image_coordinate_with_derivative(self, *args):
        """

        void Ipi::image_coordinate_with_derivative(const GroundCoordinate &Gp, const boost::array< AutoDerivative<
        double >, 3 > &Gp_with_der, ImageCoordinateWithDerivative &Ic, bool
        &Success) const

        """
        return _ipi.Ipi_image_coordinate_with_derivative(self, *args)


    def image_coordinate_with_derivative_extended(self, *args):
        """

        void Ipi::image_coordinate_with_derivative_extended(const GroundCoordinate &Gp, const boost::array< AutoDerivative<
        double >, 3 > &Gp_with_der, ImageCoordinateWithDerivative &Ic, bool
        &Success) const

        """
        return _ipi.Ipi_image_coordinate_with_derivative_extended(self, *args)


    def time(self, Gp):
        """

        void Ipi::time(const GroundCoordinate &Gp, Time &Tres, FrameCoordinate &Fres, bool
        &Success) const
        Look for solution for when the GroundCoordinate Gp is seen.

        If we don't find a solution, then Success will be false.

        Note that we may return a solution where Fres has a sample number < 0
        or > cam.number_sample(). This means that a point is not actually seen
        in the image, but would be if we had an infinite extent camera. Often
        this is useful information, however depending on the application you
        may want to reject points with Fres outside of the image range.

        If it is more useful to you, image_coordinate has the opposite
        behavior, it rejects any points that are outside of the actual image.

        """
        return _ipi.Ipi_time(self, Gp)


    def time_with_derivative(self, *args):
        """

        void Ipi::time_with_derivative(const GroundCoordinate &Gp, const boost::array< AutoDerivative<
        double >, 3 > &Gp_with_der, TimeWithDerivative &Tres,
        FrameCoordinateWithDerivative &Fres, bool &Success) const

        """
        return _ipi.Ipi_time_with_derivative(self, *args)


    def footprint(self, D):
        """

        std::vector< boost::shared_ptr< GroundCoordinate > > Ipi::footprint(const Dem &D) const
        Return the footprint of the data on the ground.

        This is just the footprint of the orbit data at the start and end of
        the time range. 
        """
        return _ipi.Ipi_footprint(self, D)


    def _v_resolution_meter(self):
        """

        double Ipi::resolution_meter() const
        Give the approximate ground resolution in meters.

        This is for the center pixel in the area covered by the Ipi. 
        """
        return _ipi.Ipi__v_resolution_meter(self)


    @property
    def resolution_meter(self):
        return self._v_resolution_meter()


    def _v_orbit(self, *args):
        """

        void GeoCal::Ipi::orbit_ptr(const boost::shared_ptr< Orbit > &Orb)

        """
        return _ipi.Ipi__v_orbit(self, *args)


    @property
    def orbit(self):
        return self._v_orbit()

    @orbit.setter
    def orbit(self, value):
      self._v_orbit(value)


    def _v_camera(self, *args):
        """

        void GeoCal::Ipi::camera_ptr(const boost::shared_ptr< Camera > &Cam)

        """
        return _ipi.Ipi__v_camera(self, *args)


    @property
    def camera(self):
        return self._v_camera()

    @camera.setter
    def camera(self, value):
      self._v_camera(value)


    def _v_time_table(self):
        """

        boost::shared_ptr<TimeTable> GeoCal::Ipi::time_table_ptr() const

        """
        return _ipi.Ipi__v_time_table(self)


    @property
    def time_table(self):
        return self._v_time_table()


    def _v_band(self, *args):
        """

        void GeoCal::Ipi::band(int B)
        Set band that we are using. 
        """
        return _ipi.Ipi__v_band(self, *args)


    @property
    def band(self):
        return self._v_band()

    @band.setter
    def band(self, value):
      self._v_band(value)


    def _v_min_time(self):
        """

        Time GeoCal::Ipi::min_time() const
        Minimum time that we are using. 
        """
        return _ipi.Ipi__v_min_time(self)


    @property
    def min_time(self):
        return self._v_min_time()


    def _v_max_time(self):
        """

        Time GeoCal::Ipi::max_time() const
        Maximum time that we are using. 
        """
        return _ipi.Ipi__v_max_time(self)


    @property
    def max_time(self):
        return self._v_max_time()


    def _v_local_time_window_size(self):
        """

        double GeoCal::Ipi::local_time_window_size() const
        How large of a window to search around last solution, in seconds. 
        """
        return _ipi.Ipi__v_local_time_window_size(self)


    @property
    def local_time_window_size(self):
        return self._v_local_time_window_size()


    def _v_root_min_separation(self):
        """

        double GeoCal::Ipi::root_min_separation() const
        How far apart in seconds we look for solutions to the collinearity
        equation. 
        """
        return _ipi.Ipi__v_root_min_separation(self)


    @property
    def root_min_separation(self):
        return self._v_root_min_separation()


    def _v_time_tolerance(self):
        """

        double GeoCal::Ipi::time_tolerance() const
        How accurate we find the time. 
        """
        return _ipi.Ipi__v_time_tolerance(self)


    @property
    def time_tolerance(self):
        return self._v_time_tolerance()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ipi.delete_Ipi
Ipi.image_coordinate = new_instancemethod(_ipi.Ipi_image_coordinate, None, Ipi)
Ipi.image_coordinate_with_derivative = new_instancemethod(_ipi.Ipi_image_coordinate_with_derivative, None, Ipi)
Ipi.image_coordinate_with_derivative_extended = new_instancemethod(_ipi.Ipi_image_coordinate_with_derivative_extended, None, Ipi)
Ipi.time = new_instancemethod(_ipi.Ipi_time, None, Ipi)
Ipi.time_with_derivative = new_instancemethod(_ipi.Ipi_time_with_derivative, None, Ipi)
Ipi.footprint = new_instancemethod(_ipi.Ipi_footprint, None, Ipi)
Ipi._v_resolution_meter = new_instancemethod(_ipi.Ipi__v_resolution_meter, None, Ipi)
Ipi.__str__ = new_instancemethod(_ipi.Ipi___str__, None, Ipi)
Ipi._v_orbit = new_instancemethod(_ipi.Ipi__v_orbit, None, Ipi)
Ipi._v_camera = new_instancemethod(_ipi.Ipi__v_camera, None, Ipi)
Ipi._v_time_table = new_instancemethod(_ipi.Ipi__v_time_table, None, Ipi)
Ipi._v_band = new_instancemethod(_ipi.Ipi__v_band, None, Ipi)
Ipi._v_min_time = new_instancemethod(_ipi.Ipi__v_min_time, None, Ipi)
Ipi._v_max_time = new_instancemethod(_ipi.Ipi__v_max_time, None, Ipi)
Ipi._v_local_time_window_size = new_instancemethod(_ipi.Ipi__v_local_time_window_size, None, Ipi)
Ipi._v_root_min_separation = new_instancemethod(_ipi.Ipi__v_root_min_separation, None, Ipi)
Ipi._v_time_tolerance = new_instancemethod(_ipi.Ipi__v_time_tolerance, None, Ipi)
Ipi_swigregister = _ipi.Ipi_swigregister
Ipi_swigregister(Ipi)



