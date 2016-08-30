# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _sub_raster_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_sub_raster_image', [dirname(__file__)])
        except ImportError:
            import _sub_raster_image
            return _sub_raster_image
        if fp is not None:
            try:
                _mod = imp.load_module('_sub_raster_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _sub_raster_image = swig_import_helper()
    del swig_import_helper
else:
    import _sub_raster_image
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
    __swig_destroy__ = _sub_raster_image.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_sub_raster_image.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_sub_raster_image.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_sub_raster_image.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_sub_raster_image.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_sub_raster_image.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_sub_raster_image.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_sub_raster_image.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_sub_raster_image.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_sub_raster_image.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_sub_raster_image.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_sub_raster_image.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_sub_raster_image.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_sub_raster_image.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_sub_raster_image.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_sub_raster_image.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_sub_raster_image.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _sub_raster_image.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_sub_raster_image.SHARED_PTR_DISOWN_swigconstant(_sub_raster_image)
SHARED_PTR_DISOWN = _sub_raster_image.SHARED_PTR_DISOWN

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

import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class SubRasterImage(geocal_swig.raster_image_variable.RasterImageVariable):
    """

    This allows you to take a subset of a larger RasterImage and treat it
    like a RasterImage in its own right.

    C++ includes: sub_raster_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        SubRasterImage::SubRasterImage(const boost::shared_ptr< RasterImage > &Raster_image, const MapInfo
        &Mi, int boundary=0)
        Create a subset of Raster_image that covers the corner points of the
        given MapInfo plus a optional boundary.

        Note that the area given by Mi may be larger than the area covered by
        Raster_image. In that case, we only give the portion that falls within
        this image. 
        """
        _sub_raster_image.SubRasterImage_swiginit(self, _sub_raster_image.new_SubRasterImage(*args))

    def _v_start_line(self):
        """

        int GeoCal::SubRasterImage::start_line() const
        Start line of underlying data. 
        """
        return _sub_raster_image.SubRasterImage__v_start_line(self)


    @property
    def start_line(self):
        return self._v_start_line()


    def _v_start_sample(self):
        """

        int GeoCal::SubRasterImage::start_sample() const
        Start sample of underlying data. 
        """
        return _sub_raster_image.SubRasterImage__v_start_sample(self)


    @property
    def start_sample(self):
        return self._v_start_sample()


    def _v_full_raster_image(self):
        """

        const boost::shared_ptr<RasterImage>& GeoCal::SubRasterImage::full_raster_image_ptr() const
        Underlying full raster image. 
        """
        return _sub_raster_image.SubRasterImage__v_full_raster_image(self)


    @property
    def full_raster_image(self):
        return self._v_full_raster_image()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _sub_raster_image.delete_SubRasterImage
SubRasterImage._v_start_line = new_instancemethod(_sub_raster_image.SubRasterImage__v_start_line, None, SubRasterImage)
SubRasterImage._v_start_sample = new_instancemethod(_sub_raster_image.SubRasterImage__v_start_sample, None, SubRasterImage)
SubRasterImage._v_full_raster_image = new_instancemethod(_sub_raster_image.SubRasterImage__v_full_raster_image, None, SubRasterImage)
SubRasterImage_swigregister = _sub_raster_image.SubRasterImage_swigregister
SubRasterImage_swigregister(SubRasterImage)



