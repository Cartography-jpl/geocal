# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _igc_simulated_ray_caster

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _igc_simulated_ray_caster.SWIG_PyInstanceMethod_New
_swig_new_static_method = _igc_simulated_ray_caster.SWIG_PyStaticMethod_New

def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


def _swig_setattr_nondynamic_instance_variable(set):
    def set_instance_attr(self, name, value):
        if name == "this":
            set(self, name, value)
        elif name == "thisown":
            self.this.own(value)
        elif hasattr(self, name) and isinstance(getattr(type(self), name), property):
            set(self, name, value)
        else:
            raise AttributeError("You cannot add instance attributes to %s" % self)
    return set_instance_attr


def _swig_setattr_nondynamic_class_variable(set):
    def set_class_attr(cls, name, value):
        if hasattr(cls, name) and not isinstance(getattr(cls, name), property):
            set(cls, name, value)
        else:
            raise AttributeError("You cannot add class attributes to %s" % cls)
    return set_class_attr


def _swig_add_metaclass(metaclass):
    """Class decorator for adding a metaclass to a SWIG wrapped class - a slimmed down version of six.add_metaclass"""
    def wrapper(cls):
        return metaclass(cls.__name__, cls.__bases__, cls.__dict__.copy())
    return wrapper


class _SwigNonDynamicMeta(type):
    """Meta class to enforce nondynamic attributes (no new attributes) for a class"""
    __setattr__ = _swig_setattr_nondynamic_class_variable(type.__setattr__)


import weakref

SWIG_MODULE_ALREADY_DONE = _igc_simulated_ray_caster.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _igc_simulated_ray_caster.delete_SwigPyIterator
    value = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_value)
    incr = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_copy)
    next = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_igc_simulated_ray_caster.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _igc_simulated_ray_caster:
_igc_simulated_ray_caster.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _igc_simulated_ray_caster.SHARED_PTR_DISOWN

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
    return geocal_swig.serialize_function.serialize_read_binary(data)

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_function.serialize_read_binary(data)
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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class IgcSimulatedRayCaster(geocal_swig.calc_raster.CalcRaster):
    r"""

    This is a RasterImage that is a simulated based on an underlying map
    projected image.

    For each point in the image of an ImageGroundConnection, we calculate
    where that point appears on the ground and then assign a value to it
    from the map projected image. This is done using a IgcRayCaster.

    This is essentially the inverse of IgcMapProjected, if you replace the
    image of an ImageGroundConnection with this simulated image and then
    run IgcMapProjected you'll get the original map projected image back
    (with changes from being resampled twice). This is similar to the old
    MisrSim program.

    This class calculates the data on the fly. Sometimes this is what you
    want, but if you are going to be using the resulting data a few times,
    you may want to use a MemoryRasterImage to generate a copy once and
    keep it in memory.

    C++ includes: igc_simulated_ray_caster.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Igc, Map_projected_image, Number_integration_step=1, Raycast_resolution=-1, Max_height=10e3, Number_tile_line=-1, Number_tile_sample=-1, Fill_value=0.0):
        r"""

        IgcSimulatedRayCaster::IgcSimulatedRayCaster(const boost::shared_ptr< ImageGroundConnection > &Igc, const
        boost::shared_ptr< RasterImage > &Map_projected_image, int
        Number_integration_step=1, double Raycast_resolution=-1, double
        Max_height=10e3, int Number_tile_line=-1, int Number_tile_sample=-1,
        double Fill_value=0.0)
        Constructor.

        If the Raycast_resolution is passed as a negative number, then we use
        the resolution of the Map_projected_image. 
        """
        _igc_simulated_ray_caster.IgcSimulatedRayCaster_swiginit(self, _igc_simulated_ray_caster.new_IgcSimulatedRayCaster(Igc, Map_projected_image, Number_integration_step, Raycast_resolution, Max_height, Number_tile_line, Number_tile_sample, Fill_value))
    _v_number_line = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_number_line)
    _v_number_sample = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_number_sample)
    _v_number_tile_line = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_number_tile_line)
    _v_number_tile_sample = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_number_tile_sample)
    _v_has_map_info = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_has_map_info)
    _v_map_info = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_map_info)
    _v_grid_center_line_resolution = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_grid_center_line_resolution)
    _v_grid_center_sample_resolution = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_grid_center_sample_resolution)
    _v_has_rpc = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_has_rpc)
    _v_rpc = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_rpc)
    _v_has_rsm = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_has_rsm)
    _v_rsm = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_rsm)
    _v_igc = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_igc)

    @property
    def igc(self):
        return self._v_igc()

    _v_number_integration_step = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_number_integration_step)

    @property
    def number_integration_step(self):
        return self._v_number_integration_step()

    _v_fill_value = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_fill_value)

    @property
    def fill_value(self):
        return self._v_fill_value()

    _v_max_height = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_max_height)

    @property
    def max_height(self):
        return self._v_max_height()

    _v_raycast_resolution = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_raycast_resolution)

    @property
    def raycast_resolution(self):
        return self._v_raycast_resolution()

    _v_map_projected_image = _swig_new_instance_method(_igc_simulated_ray_caster.IgcSimulatedRayCaster__v_map_projected_image)

    @property
    def map_projected_image(self):
        return self._v_map_projected_image()


    def __reduce__(self):
    #Special handling for when we are doing boost serialization, we set
    #"this" to None
      if(self.this is None):
        return super().__reduce__()
      return _new_from_serialization, (geocal_swig.serialize_function.serialize_write_binary(self),)

    __swig_destroy__ = _igc_simulated_ray_caster.delete_IgcSimulatedRayCaster

# Register IgcSimulatedRayCaster in _igc_simulated_ray_caster:
_igc_simulated_ray_caster.IgcSimulatedRayCaster_swigregister(IgcSimulatedRayCaster)

__all__ = ["IgcSimulatedRayCaster"]


