# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _local_rectangular_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_local_rectangular_coordinate', [dirname(__file__)])
        except ImportError:
            import _local_rectangular_coordinate
            return _local_rectangular_coordinate
        if fp is not None:
            try:
                _mod = imp.load_module('_local_rectangular_coordinate', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _local_rectangular_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _local_rectangular_coordinate
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
    __swig_destroy__ = _local_rectangular_coordinate.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_local_rectangular_coordinate.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _local_rectangular_coordinate.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_local_rectangular_coordinate.SHARED_PTR_DISOWN_swigconstant(_local_rectangular_coordinate)
SHARED_PTR_DISOWN = _local_rectangular_coordinate.SHARED_PTR_DISOWN

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
import geocal_swig.ground_coordinate
import geocal_swig.look_vector
import geocal_swig.coordinate_converter
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class LocalRcParameter(geocal_swig.generic_object.GenericObject):
    """

    This is a support class for LocalRectangularCoordinate.

    C++ includes: local_rectangular_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        LocalRcParameter::LocalRcParameter(const ImageGroundConnection &Igc, double Height=0)
        Constructor.

        This sets up coordinates so the origin is at the given height on the
        surface for the center pixel of the Igc. 
        """
        _local_rectangular_coordinate.LocalRcParameter_swiginit(self, _local_rectangular_coordinate.new_LocalRcParameter(*args))

    def check_rc_assumption(Igc, Height=0):
        """

        void LocalRcParameter::check_rc_assumption(const ImageGroundConnection &Igc, double Height=0)
        The assumption with the LocalRcParameter is that we create a
        coordinate system with z along the look direction, x mostly in the
        sample direction, and y mostly in the line direction.

        This function actually checks this by calculating the dot product
        between a unit vector in the look direction, sample direction, and
        line direction. These dot products should be close to zero, the
        farther away from 0 the less true our assumption is.

        Note that you can still completely defined LocalRcParameter (as long
        as none of the directions are exactly parallel), the problem is just
        that the LocalRcParameter isn't very useful when this assumption
        doesn't hold 
        """
        return _local_rectangular_coordinate.LocalRcParameter_check_rc_assumption(Igc, Height)

    check_rc_assumption = staticmethod(check_rc_assumption)
    cf_to_rc = _swig_property(_local_rectangular_coordinate.LocalRcParameter_cf_to_rc_get, _local_rectangular_coordinate.LocalRcParameter_cf_to_rc_set)
    cf_offset = _swig_property(_local_rectangular_coordinate.LocalRcParameter_cf_offset_get, _local_rectangular_coordinate.LocalRcParameter_cf_offset_set)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _local_rectangular_coordinate.delete_LocalRcParameter
LocalRcParameter.__str__ = new_instancemethod(_local_rectangular_coordinate.LocalRcParameter___str__, None, LocalRcParameter)
LocalRcParameter_swigregister = _local_rectangular_coordinate.LocalRcParameter_swigregister
LocalRcParameter_swigregister(LocalRcParameter)

def LocalRcParameter_check_rc_assumption(Igc, Height=0):
    """

    void LocalRcParameter::check_rc_assumption(const ImageGroundConnection &Igc, double Height=0)
    The assumption with the LocalRcParameter is that we create a
    coordinate system with z along the look direction, x mostly in the
    sample direction, and y mostly in the line direction.

    This function actually checks this by calculating the dot product
    between a unit vector in the look direction, sample direction, and
    line direction. These dot products should be close to zero, the
    farther away from 0 the less true our assumption is.

    Note that you can still completely defined LocalRcParameter (as long
    as none of the directions are exactly parallel), the problem is just
    that the LocalRcParameter isn't very useful when this assumption
    doesn't hold 
    """
    return _local_rectangular_coordinate.LocalRcParameter_check_rc_assumption(Igc, Height)

class LocalRectangularCoordinate(geocal_swig.ground_coordinate.GroundCoordinate):
    """

    This is a local rectangular coordinate system.

    This is used by Rsm as one of its coordinates choices, this is a
    rotation and offset of a standard CartesianFixed coordinate system
    such that the origin is the at the center of a ImageGroundConnection
    projection to the surface, oriented to Z in in the look direction, X
    is mostly in the scan direction (orthogonal to Z), and Y completes the
    right handed coordinate system.

    C++ includes: local_rectangular_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        LocalRectangularCoordinate::LocalRectangularCoordinate(const boost::shared_ptr< LocalRcParameter > &P, const
        GroundCoordinate &Gc)

        """
        _local_rectangular_coordinate.LocalRectangularCoordinate_swiginit(self, _local_rectangular_coordinate.new_LocalRectangularCoordinate(*args))
    parameter = _swig_property(_local_rectangular_coordinate.LocalRectangularCoordinate_parameter_get, _local_rectangular_coordinate.LocalRectangularCoordinate_parameter_set)
    position = _swig_property(_local_rectangular_coordinate.LocalRectangularCoordinate_position_get, _local_rectangular_coordinate.LocalRectangularCoordinate_position_set)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _local_rectangular_coordinate.delete_LocalRectangularCoordinate
LocalRectangularCoordinate_swigregister = _local_rectangular_coordinate.LocalRectangularCoordinate_swigregister
LocalRectangularCoordinate_swigregister(LocalRectangularCoordinate)

class LocalRcConverter(geocal_swig.coordinate_converter.CoordinateConverter):
    """

    C++ includes: local_rectangular_coordinate.h

    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, P):
        """

        GeoCal::LocalRcConverter::LocalRcConverter(const boost::shared_ptr< LocalRcParameter > &P)

        """
        _local_rectangular_coordinate.LocalRcConverter_swiginit(self, _local_rectangular_coordinate.new_LocalRcConverter(P))

    def convert_from_coordinate(self, X, Y, Z=0):
        """

        virtual boost::shared_ptr<GroundCoordinate> GeoCal::LocalRcConverter::convert_from_coordinate(double X, double Y, double Z=0) const

        """
        return _local_rectangular_coordinate.LocalRcConverter_convert_from_coordinate(self, X, Y, Z)


    def _v_parameter(self):
        """

        const boost::shared_ptr<LocalRcParameter>& GeoCal::LocalRcConverter::parameter() const

        """
        return _local_rectangular_coordinate.LocalRcConverter__v_parameter(self)


    @property
    def parameter(self):
        return self._v_parameter()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _local_rectangular_coordinate.delete_LocalRcConverter
LocalRcConverter.convert_from_coordinate = new_instancemethod(_local_rectangular_coordinate.LocalRcConverter_convert_from_coordinate, None, LocalRcConverter)
LocalRcConverter._v_parameter = new_instancemethod(_local_rectangular_coordinate.LocalRcConverter__v_parameter, None, LocalRcConverter)
LocalRcConverter_swigregister = _local_rectangular_coordinate.LocalRcConverter_swigregister
LocalRcConverter_swigregister(LocalRcConverter)


