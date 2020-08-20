# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _ray_caster_resampler.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_ray_caster_resampler')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_ray_caster_resampler')
    _ray_caster_resampler = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ray_caster_resampler', [dirname(__file__)])
        except ImportError:
            import _ray_caster_resampler
            return _ray_caster_resampler
        try:
            _mod = imp.load_module('_ray_caster_resampler', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _ray_caster_resampler = swig_import_helper()
    del swig_import_helper
else:
    import _ray_caster_resampler
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
    __swig_destroy__ = _ray_caster_resampler.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_ray_caster_resampler.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ray_caster_resampler.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ray_caster_resampler.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ray_caster_resampler.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ray_caster_resampler.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ray_caster_resampler.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ray_caster_resampler.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ray_caster_resampler.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ray_caster_resampler.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ray_caster_resampler.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _ray_caster_resampler.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ray_caster_resampler.SHARED_PTR_DISOWN

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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class RayCasterResampler(geocal_swig.generic_object.GenericObject):
    """

    This is used to project data to the surface using a RayCaster.

    This is similar to IgcMapProjected, except rather than going from the
    ground to the image we go from the image to the ground. Depending on
    the type of ImageGroundConnection we are using, one or the other
    direction can be much faster.

    This class is really designed to be wrapped in python. Although it
    could be used from C++, it would probably be a bit awkward. We can
    always rework this if needed, but I expect the primary use of this
    will be programs like igc_project.

    There are two steps involved:

    The initial ray casting, determining where each pixel goes in the
    final map.

    Resampling the data to create the final radiance data.

    Step 1 can be done in parallel to speed things up. We currently have
    step 2 needing to be done serially, I'm not sure that it could be done
    in parallel.

    For step 1, The subsetting should be done on Ray_caster (so like
    IgcRayCaster where we specify the start and number of lines).
    ray_cast_step() steps through all the positions in the ray caster.

    C++ includes: ray_caster_resampler.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Ray_caster, Map_info):
        """

        RayCasterResampler::RayCasterResampler(const boost::shared_ptr< RayCaster > &Ray_caster, const
        boost::shared_ptr< MapInfo > &Map_info)
        Constructor. 
        """
        _ray_caster_resampler.RayCasterResampler_swiginit(self, _ray_caster_resampler.new_RayCasterResampler(Ray_caster, Map_info))

    def ray_cast_step(self, Ray_cast_res):
        """

        void RayCasterResampler::ray_cast_step(blitz::Array< int, 6 > Ray_cast_res)
        Do ray cast step.

        We fill in Res, which should be number_line x number_sample x
        nsub_line x nsub_sample x nintegration_step. This gets filled with the
        image line/sample in the map_info that each pixel goes. We only fill
        in the portion covered by our ray_caster().

        The Res will often be a mmap array created in python (to handle large
        images).

        This is a clumsy function, but this should be looked at as a low level
        step used by the python wrapper.

        Note the argument Res being passed as an Array rather than a reference
        is actually correct. Normally we pass things in that shouldn't change
        from python. In this case, we really do want to change. But we don't
        want a nonconst version of Array& in SWIG because this is almost
        always an error (just in this case it isn't). 
        """
        return _ray_caster_resampler.RayCasterResampler_ray_cast_step(self, Ray_cast_res)


    def final_rad_step(self, Input_img, Ray_cast_res, Rad_res, Scratch_count):
        """

        void RayCasterResampler::final_rad_step(const RasterImage &Input_img, blitz::Array< int, 6 > Ray_cast_res,
        blitz::Array< int, 2 > Rad_res, blitz::Array< int, 2 > Scratch_count)
        Final step, taking all the ray caster results and resampling data.

        We have the final output in Rad_res, which should be the size from the
        map_info(). We also need a scratch variable for holding the count. We
        don't actually generate a RasterImage, since there might be a type
        change (e.g., int32 for Rad_res and output int16).

        Rad_res and Scratch_count should be initialized to zero, we don't do
        that in this function.

        This is a clumsy function, but this should be looked at as a low level
        step used by the python wrapper. 
        """
        return _ray_caster_resampler.RayCasterResampler_final_rad_step(self, Input_img, Ray_cast_res, Rad_res, Scratch_count)


    def _v_ray_caster(self):
        """

        const boost::shared_ptr<RayCaster>& GeoCal::RayCasterResampler::ray_caster() const

        """
        return _ray_caster_resampler.RayCasterResampler__v_ray_caster(self)


    @property
    def ray_caster(self):
        return self._v_ray_caster()


    def _v_map_info(self):
        """

        const boost::shared_ptr<MapInfo>& GeoCal::RayCasterResampler::map_info() const

        """
        return _ray_caster_resampler.RayCasterResampler__v_map_info(self)


    @property
    def map_info(self):
        return self._v_map_info()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _ray_caster_resampler.delete_RayCasterResampler
RayCasterResampler.ray_cast_step = new_instancemethod(_ray_caster_resampler.RayCasterResampler_ray_cast_step, None, RayCasterResampler)
RayCasterResampler.final_rad_step = new_instancemethod(_ray_caster_resampler.RayCasterResampler_final_rad_step, None, RayCasterResampler)
RayCasterResampler._v_ray_caster = new_instancemethod(_ray_caster_resampler.RayCasterResampler__v_ray_caster, None, RayCasterResampler)
RayCasterResampler._v_map_info = new_instancemethod(_ray_caster_resampler.RayCasterResampler__v_map_info, None, RayCasterResampler)
RayCasterResampler_swigregister = _ray_caster_resampler.RayCasterResampler_swigregister
RayCasterResampler_swigregister(RayCasterResampler)


__all__ = ["RayCasterResampler"]



