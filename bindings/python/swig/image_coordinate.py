# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _image_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_image_coordinate', [dirname(__file__)])
        except ImportError:
            import _image_coordinate
            return _image_coordinate
        if fp is not None:
            try:
                _mod = imp.load_module('_image_coordinate', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _image_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _image_coordinate
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
    __swig_destroy__ = _image_coordinate.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_image_coordinate.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_image_coordinate.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_image_coordinate.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_image_coordinate.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_image_coordinate.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_image_coordinate.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_image_coordinate.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_image_coordinate.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_image_coordinate.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_image_coordinate.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_image_coordinate.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_image_coordinate.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_image_coordinate.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_image_coordinate.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_image_coordinate.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_image_coordinate.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _image_coordinate.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_image_coordinate.SHARED_PTR_DISOWN_swigconstant(_image_coordinate)
SHARED_PTR_DISOWN = _image_coordinate.SHARED_PTR_DISOWN

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
class ImageCoordinate(geocal_swig.generic_object.GenericObject):
    """

    This describes an image coordinate.

    We also define the print operator <<, and equality operator ==. Two
    image coordinates are considers equal if they are within 0.01 in the
    line and sample.

    The convention used throughout GeoCal is that an integer value of the
    ImageCoordinate line or sample refers to the center of a pixel. This
    means the upper left corner of the bounding box of an image is at
    (-0.5, -0.5).

    C++ includes: image_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ImageCoordinate::ImageCoordinate()
        Default constructor, line and sample aren't initialized. 
        """
        _image_coordinate.ImageCoordinate_swiginit(self, _image_coordinate.new_ImageCoordinate(*args))
    line = _swig_property(_image_coordinate.ImageCoordinate_line_get, _image_coordinate.ImageCoordinate_line_set)
    sample = _swig_property(_image_coordinate.ImageCoordinate_sample_get, _image_coordinate.ImageCoordinate_sample_set)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _image_coordinate.delete_ImageCoordinate
ImageCoordinate.__str__ = new_instancemethod(_image_coordinate.ImageCoordinate___str__, None, ImageCoordinate)
ImageCoordinate_swigregister = _image_coordinate.ImageCoordinate_swigregister
ImageCoordinate_swigregister(ImageCoordinate)

class VicarImageCoordinate(geocal_swig.generic_object.GenericObject):
    """

    VICAR programs use a different convention than our ImageCoordinate.

    Instead of the upper left hand corner of the bounding box being
    (-0.5,-0.5), VICAR uses a 1-based convention where the upper left
    corner is (0.5, 0.5). This is just a difference of 1-pixel. To avoid
    any confusion, we define a VicarImageCoordinate class, and allow
    conversions to and from ImageCoordinate.

    C++ includes: image_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::VicarImageCoordinate::VicarImageCoordinate()
        Default constructor, line and sample aren't initialized. 
        """
        _image_coordinate.VicarImageCoordinate_swiginit(self, _image_coordinate.new_VicarImageCoordinate(*args))
    line = _swig_property(_image_coordinate.VicarImageCoordinate_line_get, _image_coordinate.VicarImageCoordinate_line_set)
    sample = _swig_property(_image_coordinate.VicarImageCoordinate_sample_get, _image_coordinate.VicarImageCoordinate_sample_set)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _image_coordinate.delete_VicarImageCoordinate
VicarImageCoordinate.__str__ = new_instancemethod(_image_coordinate.VicarImageCoordinate___str__, None, VicarImageCoordinate)
VicarImageCoordinate_swigregister = _image_coordinate.VicarImageCoordinate_swigregister
VicarImageCoordinate_swigregister(VicarImageCoordinate)

class ImageCoordinateWithDerivative(geocal_swig.generic_object.GenericObject):
    """

    This describes an image coordinate, including the gradient of the line
    and sample.

    C++ includes: image_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        GeoCal::ImageCoordinateWithDerivative::ImageCoordinateWithDerivative()
        Default constructor, line and sample aren't initialized. 
        """
        _image_coordinate.ImageCoordinateWithDerivative_swiginit(self, _image_coordinate.new_ImageCoordinateWithDerivative(*args))
    line = _swig_property(_image_coordinate.ImageCoordinateWithDerivative_line_get, _image_coordinate.ImageCoordinateWithDerivative_line_set)
    sample = _swig_property(_image_coordinate.ImageCoordinateWithDerivative_sample_get, _image_coordinate.ImageCoordinateWithDerivative_sample_set)

    def value(self):
        """

        ImageCoordinate GeoCal::ImageCoordinateWithDerivative::value() const
        Strip off derivative information and return ImageCoordinate. 
        """
        return _image_coordinate.ImageCoordinateWithDerivative_value(self)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _image_coordinate.delete_ImageCoordinateWithDerivative
ImageCoordinateWithDerivative.value = new_instancemethod(_image_coordinate.ImageCoordinateWithDerivative_value, None, ImageCoordinateWithDerivative)
ImageCoordinateWithDerivative.__str__ = new_instancemethod(_image_coordinate.ImageCoordinateWithDerivative___str__, None, ImageCoordinateWithDerivative)
ImageCoordinateWithDerivative_swigregister = _image_coordinate.ImageCoordinateWithDerivative_swigregister
ImageCoordinateWithDerivative_swigregister(ImageCoordinateWithDerivative)

class Vector_ImageCoordinate(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _image_coordinate.Vector_ImageCoordinate_swiginit(self, _image_coordinate.new_Vector_ImageCoordinate(*args))
    __swig_destroy__ = _image_coordinate.delete_Vector_ImageCoordinate
Vector_ImageCoordinate.iterator = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_iterator, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__nonzero__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___nonzero__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__bool__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___bool__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__len__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___len__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.pop = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_pop, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__getslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___getslice__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__setslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___setslice__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__delslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___delslice__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__delitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___delitem__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__getitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___getitem__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.__setitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinate___setitem__, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.append = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_append, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.empty = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_empty, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.size = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_size, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.clear = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_clear, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.swap = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_swap, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.get_allocator = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_get_allocator, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.begin = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_begin, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.end = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_end, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.rbegin = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_rbegin, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.rend = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_rend, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.pop_back = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_pop_back, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.erase = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_erase, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.push_back = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_push_back, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.front = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_front, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.back = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_back, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.assign = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_assign, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.resize = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_resize, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.insert = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_insert, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.reserve = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_reserve, None, Vector_ImageCoordinate)
Vector_ImageCoordinate.capacity = new_instancemethod(_image_coordinate.Vector_ImageCoordinate_capacity, None, Vector_ImageCoordinate)
Vector_ImageCoordinate_swigregister = _image_coordinate.Vector_ImageCoordinate_swigregister
Vector_ImageCoordinate_swigregister(Vector_ImageCoordinate)

class Vector_ImageCoordinatePtr(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self):
        return self.iterator()

    def __init__(self, *args):
        _image_coordinate.Vector_ImageCoordinatePtr_swiginit(self, _image_coordinate.new_Vector_ImageCoordinatePtr(*args))
    __swig_destroy__ = _image_coordinate.delete_Vector_ImageCoordinatePtr
Vector_ImageCoordinatePtr.iterator = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_iterator, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__nonzero__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___nonzero__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__bool__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___bool__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__len__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___len__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.pop = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_pop, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__getslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___getslice__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__setslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___setslice__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__delslice__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___delslice__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__delitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___delitem__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__getitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___getitem__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.__setitem__ = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr___setitem__, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.append = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_append, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.empty = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_empty, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.size = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_size, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.clear = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_clear, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.swap = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_swap, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.get_allocator = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_get_allocator, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.begin = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_begin, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.end = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_end, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.rbegin = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_rbegin, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.rend = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_rend, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.pop_back = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_pop_back, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.erase = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_erase, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.push_back = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_push_back, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.front = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_front, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.back = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_back, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.assign = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_assign, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.resize = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_resize, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.insert = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_insert, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.reserve = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_reserve, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr.capacity = new_instancemethod(_image_coordinate.Vector_ImageCoordinatePtr_capacity, None, Vector_ImageCoordinatePtr)
Vector_ImageCoordinatePtr_swigregister = _image_coordinate.Vector_ImageCoordinatePtr_swigregister
Vector_ImageCoordinatePtr_swigregister(Vector_ImageCoordinatePtr)



