# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _igc_ray_caster.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_igc_ray_caster')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_igc_ray_caster')
    _igc_ray_caster = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_ray_caster', [dirname(__file__)])
        except ImportError:
            import _igc_ray_caster
            return _igc_ray_caster
        try:
            _mod = imp.load_module('_igc_ray_caster', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _igc_ray_caster = swig_import_helper()
    del swig_import_helper
else:
    import _igc_ray_caster
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
    __swig_destroy__ = _igc_ray_caster.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_igc_ray_caster.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_igc_ray_caster.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_igc_ray_caster.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_igc_ray_caster.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_igc_ray_caster.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_igc_ray_caster.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_igc_ray_caster.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_igc_ray_caster.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_igc_ray_caster.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_igc_ray_caster.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _igc_ray_caster.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _igc_ray_caster.SHARED_PTR_DISOWN

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

import geocal_swig.ray_caster
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class IgcRayCaster(geocal_swig.ray_caster.RayCaster):
    """

    This class implements a RayCaster by using a ImageGroundConnection.

    We get a significant speed increase by having a good initial guess at
    the starting length of each ray when finding the intersection with the
    surface.

    For a pushbroom camera, we can get a pretty good guess by making use
    of the fact that the rays form the same camera line/sample are nearly
    parallel from on orbit position to the next. For a forward pointing
    camera, the rays from time t + epsilon will be above the rays from
    time t. This means that we can get a good guess at the length of the
    intersection of the ray at time t + epsilon by using the distance from
    the camera position at time t + epsilon & the intersection at time t.

    We can improve the guess a bit by using not just one ray. For the
    subpixel (sub_l, sub_s) of camera pixel (cam_l, cam_s), we get the
    guess at the ray length by:

    ray_length = min(distance for pos(t + eps) from intersection at
    (cam_l, cam_s - 1, sub_l, sub_s, t) (cam_l, cam_s , sub_l, sub_s, t)
    (cam_l, cam_s + 1, sub_l, sub_s, t) (cam_l, cam_s , sub_l, sub_s, t +
    eps)

    There are a few complications: What do we do for the first line of
    data, when data from time t has not been calculated.

    What do we do at the egdes of the camera, when cam_s - 1 or cam_s + 1
    has not been calculated.

    For (1) we use the length of the ray that intersectes the WGS84 +
    Max_height where Max_height is greater than any height we encounter in
    the dem. This is guaranteed to give a ray length that will be above
    the surface.

    For(2), we just use the neighbors that are available.

    For aftward cameras, we just progress through the orbit in the
    opposite order, starting with the last orbit position and working
    backwards.

    We assume that this holds for the ImageGroundConnection, which might
    not be a push broom camera. We may need to generalize this class at
    some point, but for now this is the assumption made.

    We determine if we are forward or aftward by looking at the first
    pixel of the first 2 lines. We do a intersection with the reference
    ellipsoid, and if the point for line 1 is farther away from the
    position and line 0 then we have a forward looking camera, otherwise
    we have an aftward camera.

    Note that this assumes that we continue pointing in the same
    direction. This is not the case for something like AirMSPI running in
    sweep mode (where we step through a number of camera angles), but for
    now we just assume that whatever the first pointing is that we use is
    the pointing that will be used for all lines.

    Note that we could relax the assumption that we are always going
    forward or aftward in the future by doing a calculation like we do for
    the initial determination of forward/aftward for each line. We could
    then break the position up into segments one direction of the other,
    going one way through forward and the other through aftward. But we
    have not implemented this yet.

    We break the pixels up into the number of subpixels needed to cover
    the desired Resolution of the results. This determination is done for
    the first line, and we assume it holds for the full orbit. Again, we
    could relax this if desired and calculate this for every line. But we
    don't do that right now.

    C++ includes: igc_ray_caster.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Igc, Start_line=0, Number_line=-1, Number_integration_step=2, Resolution=100, Max_height=10e3, Start_sample=0, Number_sample=-1, Include_path_distance=False):
        """

        IgcRayCaster::IgcRayCaster(const boost::shared_ptr< ImageGroundConnection > &Igc, int
        Start_line=0, int Number_line=-1, int Number_integration_step=2,
        double Resolution=100, double Max_height=10e3, int Start_sample=0, int
        Number_sample=-1, bool Include_path_distance=false)
        Constructor.

        You can pass the starting line to use and the number of lines to
        process, the default is to do the full range covered by the Igc. The
        resolution is the desired accuracy, we use this both to figure out the
        number of subpixels to use and the accuracy that we do the
        intersection with the DEM. The Max_height should be larger than the
        greatest height we will encounter in the Dem belonging to the Igc.

        For larger cameras, it might be more convenient to pass in start
        sample and number of samples to process, the default is to do the full
        camera.

        If you have Include_path_distance set to true, then we return an extra
        entry in the result array that is the path distance. 
        """
        _igc_ray_caster.IgcRayCaster_swiginit(self, _igc_ray_caster.new_IgcRayCaster(Igc, Start_line, Number_line, Number_integration_step, Resolution, Max_height, Start_sample, Number_sample, Include_path_distance))

    def _v_start_sample(self):
        """

        int GeoCal::IgcRayCaster::start_sample() const

        """
        return _igc_ray_caster.IgcRayCaster__v_start_sample(self)


    @property
    def start_sample(self):
        return self._v_start_sample()


    def _v_number_sample(self):
        """

        int GeoCal::IgcRayCaster::number_sample() const

        """
        return _igc_ray_caster.IgcRayCaster__v_number_sample(self)


    @property
    def number_sample(self):
        return self._v_number_sample()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _igc_ray_caster.delete_IgcRayCaster
IgcRayCaster._v_start_sample = new_instancemethod(_igc_ray_caster.IgcRayCaster__v_start_sample, None, IgcRayCaster)
IgcRayCaster._v_number_sample = new_instancemethod(_igc_ray_caster.IgcRayCaster__v_number_sample, None, IgcRayCaster)
IgcRayCaster_swigregister = _igc_ray_caster.IgcRayCaster_swigregister
IgcRayCaster_swigregister(IgcRayCaster)


__all__ = ["IgcRayCaster"]



