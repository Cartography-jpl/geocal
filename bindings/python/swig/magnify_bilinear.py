# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _magnify_bilinear.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_magnify_bilinear')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_magnify_bilinear')
    _magnify_bilinear = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_magnify_bilinear', [dirname(__file__)])
        except ImportError:
            import _magnify_bilinear
            return _magnify_bilinear
        try:
            _mod = imp.load_module('_magnify_bilinear', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _magnify_bilinear = swig_import_helper()
    del swig_import_helper
else:
    import _magnify_bilinear
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
    __swig_destroy__ = _magnify_bilinear.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_magnify_bilinear.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_magnify_bilinear.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_magnify_bilinear.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_magnify_bilinear.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_magnify_bilinear.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_magnify_bilinear.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_magnify_bilinear.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_magnify_bilinear.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_magnify_bilinear.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_magnify_bilinear.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _magnify_bilinear.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _magnify_bilinear.SHARED_PTR_DISOWN

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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.image_ground_connection
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class MagnifyBilinear(geocal_swig.calc_raster.CalcRaster):
    """

    This creates a magnified image of a RasterImage.

    We do a bilinear interpolation to get the values

    C++ includes: magnify_bilinear.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Data, Magfactor, Number_tile=4):
        """

        MagnifyBilinear::MagnifyBilinear(const boost::shared_ptr< RasterImage > &Data, int Magfactor, int
        Number_tile=4)
        Constructor. 
        """
        _magnify_bilinear.MagnifyBilinear_swiginit(self, _magnify_bilinear.new_MagnifyBilinear(Data, Magfactor, Number_tile))

    def _v_underlying_data(self):
        """

        const boost::shared_ptr<RasterImage>& GeoCal::MagnifyBilinear::underlying_data() const

        """
        return _magnify_bilinear.MagnifyBilinear__v_underlying_data(self)


    @property
    def underlying_data(self):
        return self._v_underlying_data()


    def _v_magnification_factor(self):
        """

        int GeoCal::MagnifyBilinear::magnification_factor() const

        """
        return _magnify_bilinear.MagnifyBilinear__v_magnification_factor(self)


    @property
    def magnification_factor(self):
        return self._v_magnification_factor()


    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.underlying_data,self.magnification_factor)

    __swig_destroy__ = _magnify_bilinear.delete_MagnifyBilinear
MagnifyBilinear._v_underlying_data = new_instancemethod(_magnify_bilinear.MagnifyBilinear__v_underlying_data, None, MagnifyBilinear)
MagnifyBilinear._v_magnification_factor = new_instancemethod(_magnify_bilinear.MagnifyBilinear__v_magnification_factor, None, MagnifyBilinear)
MagnifyBilinear_swigregister = _magnify_bilinear.MagnifyBilinear_swigregister
MagnifyBilinear_swigregister(MagnifyBilinear)

class MagnifyBilinearImageGroundConnection(geocal_swig.image_ground_connection.ImageGroundConnection):
    """

    This create an ImageGroundConnection that is magnified the same way
    MagnifyBilinear does.

    C++ includes: magnify_bilinear.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Ig_original, Magfactor):
        """

        GeoCal::MagnifyBilinearImageGroundConnection::MagnifyBilinearImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Ig_original, int
        Magfactor)
        Constructor. 
        """
        _magnify_bilinear.MagnifyBilinearImageGroundConnection_swiginit(self, _magnify_bilinear.new_MagnifyBilinearImageGroundConnection(Ig_original, Magfactor))

    def cf_look_vector(self, Ic):
        """

        virtual void GeoCal::MagnifyBilinearImageGroundConnection::cf_look_vector(const ImageCoordinate &Ic, CartesianFixedLookVector &Lv,
        boost::shared_ptr< CartesianFixed > &P) const

        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection_cf_look_vector(self, Ic)


    def _v_original_image_ground_connection(self):
        """

        boost::shared_ptr<ImageGroundConnection> GeoCal::MagnifyBilinearImageGroundConnection::original_image_ground_connection() const
        Underlying ImageGroundConnection. 
        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection__v_original_image_ground_connection(self)


    @property
    def original_image_ground_connection(self):
        return self._v_original_image_ground_connection()


    def _v_magnification_factor(self):
        """

        int GeoCal::MagnifyBilinearImageGroundConnection::magnification_factor() const
        Return magnification factor. 
        """
        return _magnify_bilinear.MagnifyBilinearImageGroundConnection__v_magnification_factor(self)


    @property
    def magnification_factor(self):
        return self._v_magnification_factor()


    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.original_image_ground_connection,self.magnification_factor)

    __swig_destroy__ = _magnify_bilinear.delete_MagnifyBilinearImageGroundConnection
MagnifyBilinearImageGroundConnection.cf_look_vector = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection_cf_look_vector, None, MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection._v_original_image_ground_connection = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection__v_original_image_ground_connection, None, MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection._v_magnification_factor = new_instancemethod(_magnify_bilinear.MagnifyBilinearImageGroundConnection__v_magnification_factor, None, MagnifyBilinearImageGroundConnection)
MagnifyBilinearImageGroundConnection_swigregister = _magnify_bilinear.MagnifyBilinearImageGroundConnection_swigregister
MagnifyBilinearImageGroundConnection_swigregister(MagnifyBilinearImageGroundConnection)


__all__ = ["MagnifyBilinear","MagnifyBilinearImageGroundConnection"]



