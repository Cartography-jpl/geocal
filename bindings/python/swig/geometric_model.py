# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _geometric_model.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geometric_model', [dirname(__file__)])
        except ImportError:
            import _geometric_model
            return _geometric_model
        if fp is not None:
            try:
                _mod = imp.load_module('_geometric_model', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geometric_model = swig_import_helper()
    del swig_import_helper
else:
    import _geometric_model
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
    __swig_destroy__ = _geometric_model.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_geometric_model.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_geometric_model.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_geometric_model.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_geometric_model.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_geometric_model.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_geometric_model.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_geometric_model.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_geometric_model.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_geometric_model.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_geometric_model.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_geometric_model.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_geometric_model.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_geometric_model.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_geometric_model.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_geometric_model.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_geometric_model.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _geometric_model.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_geometric_model.SHARED_PTR_DISOWN_swigconstant(_geometric_model)
SHARED_PTR_DISOWN = _geometric_model.SHARED_PTR_DISOWN

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
class GeometricModel(geocal_swig.generic_object.GenericObject):
    """

    This supplies a geometric model that can be used to deform an image,
    e.g., resample an image to match to geometry of another reference
    image.

    C++ includes: geometric_model.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined")
    __repr__ = _swig_repr

    def original_image_coordinate(self, Resampled_ic):
        """

        virtual ImageCoordinate GeoCal::GeometricModel::original_image_coordinate(const ImageCoordinate &Resampled_ic) const =0
        Map image coordinates to an underlying set of image coordinates.

        This takes the image coordinates of the resampled image (magnified,
        rotated, whatever) and returns the corresponding coordinate in the
        original image. 
        """
        return _geometric_model.GeometricModel_original_image_coordinate(self, Resampled_ic)


    def resampled_image_coordinate(self, Resampled_ic):
        """

        virtual ImageCoordinate GeoCal::GeometricModel::resampled_image_coordinate(const ImageCoordinate &Original_ic) const =0
        This is the inversion of original_image_coordinate, taking the
        original image coordinates and returning the resampled image
        coordinates. 
        """
        return _geometric_model.GeometricModel_resampled_image_coordinate(self, Resampled_ic)

    __swig_destroy__ = _geometric_model.delete_GeometricModel
GeometricModel.original_image_coordinate = new_instancemethod(_geometric_model.GeometricModel_original_image_coordinate, None, GeometricModel)
GeometricModel.resampled_image_coordinate = new_instancemethod(_geometric_model.GeometricModel_resampled_image_coordinate, None, GeometricModel)
GeometricModel.__str__ = new_instancemethod(_geometric_model.GeometricModel___str__, None, GeometricModel)
GeometricModel_swigregister = _geometric_model.GeometricModel_swigregister
GeometricModel_swigregister(GeometricModel)

class GeometricTiePoints(geocal_swig.generic_object.GenericObject):
    """

    Often GeometricModels are created by fitting a set of points tieing
    the image together.

    This is just complicated enough to need a class to maintain it. This
    is little more than a structure.

    C++ includes: geometric_model.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self):
        """

        GeoCal::GeometricTiePoints::GeometricTiePoints()

        """
        _geometric_model.GeometricTiePoints_swiginit(self, _geometric_model.new_GeometricTiePoints())

    def add_point(self, Resampled_ic, Original_ic):
        """

        void GeoCal::GeometricTiePoints::add_point(const ImageCoordinate &Resampled_ic, const ImageCoordinate
        &Original_ic)
        Add a point. 
        """
        return _geometric_model.GeometricTiePoints_add_point(self, Resampled_ic, Original_ic)


    def _v_x(self):
        """

        blitz::Array<double, 2> GeoCal::GeometricTiePoints::x() const
        Return the resampled_ic as 2 columns, first is line second is sample;.

        """
        return _geometric_model.GeometricTiePoints__v_x(self)


    @property
    def x(self):
        return self._v_x()


    def _v_y(self):
        """

        blitz::Array<double, 2> GeoCal::GeometricTiePoints::y() const
        Return the resampled_ic as 2 columns, first is line second is sample;.

        """
        return _geometric_model.GeometricTiePoints__v_y(self)


    @property
    def y(self):
        return self._v_y()

    __swig_destroy__ = _geometric_model.delete_GeometricTiePoints
GeometricTiePoints.add_point = new_instancemethod(_geometric_model.GeometricTiePoints_add_point, None, GeometricTiePoints)
GeometricTiePoints._v_x = new_instancemethod(_geometric_model.GeometricTiePoints__v_x, None, GeometricTiePoints)
GeometricTiePoints._v_y = new_instancemethod(_geometric_model.GeometricTiePoints__v_y, None, GeometricTiePoints)
GeometricTiePoints.__str__ = new_instancemethod(_geometric_model.GeometricTiePoints___str__, None, GeometricTiePoints)
GeometricTiePoints_swigregister = _geometric_model.GeometricTiePoints_swigregister
GeometricTiePoints_swigregister(GeometricTiePoints)



