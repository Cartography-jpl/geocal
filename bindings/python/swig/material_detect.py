# This file was automatically generated by SWIG (https://www.swig.org).
# Version 4.1.1
#
# Do not make changes to this file unless you know what you are doing - modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
from ._swig_wrap import _material_detect

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

_swig_new_instance_method = _material_detect.SWIG_PyInstanceMethod_New
_swig_new_static_method = _material_detect.SWIG_PyStaticMethod_New

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

SWIG_MODULE_ALREADY_DONE = _material_detect.SWIG_MODULE_ALREADY_DONE
class SwigPyIterator(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _material_detect.delete_SwigPyIterator
    value = _swig_new_instance_method(_material_detect.SwigPyIterator_value)
    incr = _swig_new_instance_method(_material_detect.SwigPyIterator_incr)
    decr = _swig_new_instance_method(_material_detect.SwigPyIterator_decr)
    distance = _swig_new_instance_method(_material_detect.SwigPyIterator_distance)
    equal = _swig_new_instance_method(_material_detect.SwigPyIterator_equal)
    copy = _swig_new_instance_method(_material_detect.SwigPyIterator_copy)
    next = _swig_new_instance_method(_material_detect.SwigPyIterator_next)
    __next__ = _swig_new_instance_method(_material_detect.SwigPyIterator___next__)
    previous = _swig_new_instance_method(_material_detect.SwigPyIterator_previous)
    advance = _swig_new_instance_method(_material_detect.SwigPyIterator_advance)
    __eq__ = _swig_new_instance_method(_material_detect.SwigPyIterator___eq__)
    __ne__ = _swig_new_instance_method(_material_detect.SwigPyIterator___ne__)
    __iadd__ = _swig_new_instance_method(_material_detect.SwigPyIterator___iadd__)
    __isub__ = _swig_new_instance_method(_material_detect.SwigPyIterator___isub__)
    __add__ = _swig_new_instance_method(_material_detect.SwigPyIterator___add__)
    __sub__ = _swig_new_instance_method(_material_detect.SwigPyIterator___sub__)
    def __iter__(self):
        return self

# Register SwigPyIterator in _material_detect:
_material_detect.SwigPyIterator_swigregister(SwigPyIterator)
SHARED_PTR_DISOWN = _material_detect.SHARED_PTR_DISOWN

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
class MaterialDetect(geocal_swig.calc_raster.CalcRaster):
    r"""

    This class is used to do change detection and identify the material
    for pixels that have changed.

    This duplicates what was done in the VICAR proc "detwvpan", and in
    particular the "f2ratio" process.

    We read a IBIS file that describes the thresholds for a number of
    material classes. For each material class, we do the following:

    First, the difference in the pan bands is compared to a threshold. The
    difference is normally calculated by DoughnutAverage, using the
    pandif_raster_image. But something else could be used, all this class
    cares about is having a difference it can compare against a threshold.

    Compare pan data to a shadow threshold. This masks at very dark pixels
    that we assume are in shadow.

    For each point, we calculate the required ratios between the
    multispectral bands, take the difference with the supplied class mean
    and divide by the class sigma.

    We sum the abs value of for each of the band ratios divided by the
    number of band ratios (i.e, we use a L1 norm). This is compared
    against a second threshold.

    We may have more than one class that passes the second threshold. In
    that case, we sort the classes first by a class priority (with the
    lower number being selected first). For ties, we then pick the class
    that has the smallest difference norm.

    C++ includes: material_detect.h 
    """

    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc="The membership flag")
    __repr__ = _swig_repr

    def __init__(self, Pan_data, Pan_diff, Mulspect, Pan_diff_threshold, Spectral_diff_threshold, Class_priority, Ibis_fname, Pan_shadow_threshold):
        r"""

        MaterialDetect::MaterialDetect(const boost::shared_ptr< RasterImage > &Pan_data, const
        boost::shared_ptr< RasterImage > &Pan_diff, const boost::shared_ptr<
        RasterImageMultiBand > &Mulspect, const blitz::Array< double, 1 >
        &Pan_diff_threshold, const blitz::Array< double, 1 >
        &Spectral_diff_threshold, const blitz::Array< int, 1 >
        &Class_priority, const std::string &Ibis_fname, double
        Pan_shadow_threshold)
        Constructor.

        This takes the pan difference band (normally pandif_raster_image from
        DoughnutAverage), the multispectral bands, the threshold for each
        material for the pan difference and spectral difference, the class
        priority for eahc class, and the name of the IBIS file to get the
        material information.

        The IBIS file should have at least 5 columns (which is all that we
        read). The first two columns should be full word values, and are the
        band indices (1 based, rather than the 0 based we use elsewhere). The
        third should be double (for no good reason, it just is), which is the
        class id. The fourth and fifth are also double, and are the mean and
        sigma for that band ratio.

        In generate, the class ID doesn't start from 0. We find the minimum
        value in the table, and use that as the index into Pan_diff_threshold
        etc. (so if first id is 8881 then the threshold for 8881 is
        Pan_diff_threshold[0] and 8885 is Pan_diff_threshold[4]). Obviously we
        could have just used a map instead, but this convention fits better
        with the current way we supply this values in the Shiva scripts. 
        """
        _material_detect.MaterialDetect_swiginit(self, _material_detect.new_MaterialDetect(Pan_data, Pan_diff, Mulspect, Pan_diff_threshold, Spectral_diff_threshold, Class_priority, Ibis_fname, Pan_shadow_threshold))
    _v_closest_material_raster_image = _swig_new_instance_method(_material_detect.MaterialDetect__v_closest_material_raster_image)

    @property
    def closest_material_raster_image(self):
        return self._v_closest_material_raster_image()


    @property
    def material_raster_image(self):
        return self._v_material_raster_image()

    _v_material_raster_image = _swig_new_instance_method(_material_detect.MaterialDetect__v_material_raster_image)
    closest_material_dif = _swig_new_instance_method(_material_detect.MaterialDetect_closest_material_dif)
    material_dif = _swig_new_instance_method(_material_detect.MaterialDetect_material_dif)
    __swig_destroy__ = _material_detect.delete_MaterialDetect

# Register MaterialDetect in _material_detect:
_material_detect.MaterialDetect_swigregister(MaterialDetect)

__all__ = ["MaterialDetect"]


