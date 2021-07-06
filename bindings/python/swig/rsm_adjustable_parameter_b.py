# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_adjustable_parameter_b.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_rsm_adjustable_parameter_b')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_rsm_adjustable_parameter_b')
    _rsm_adjustable_parameter_b = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_adjustable_parameter_b', [dirname(__file__)])
        except ImportError:
            import _rsm_adjustable_parameter_b
            return _rsm_adjustable_parameter_b
        try:
            _mod = imp.load_module('_rsm_adjustable_parameter_b', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _rsm_adjustable_parameter_b = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_adjustable_parameter_b
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
    __swig_destroy__ = _rsm_adjustable_parameter_b.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_adjustable_parameter_b.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_adjustable_parameter_b.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _rsm_adjustable_parameter_b.SHARED_PTR_DISOWN

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

import geocal_swig.rsm_adjustable_parameter
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.dem
import geocal_swig.geocal_exception
class RsmAdjustableParameterB(geocal_swig.rsm_adjustable_parameter.RsmAdjustableParameter):
    """

    This is used to handle the RSM adjustable parameters.

    This supports RSMAPB.

    C++ includes: rsm_adjustable_parameter_b.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, *args):
        """

        RsmAdjustableParameterB::RsmAdjustableParameterB(const ImageGroundConnection &Igc, const RsmId &Rsm_id, const
        std::string &Triangulation_id="")
        Create a RsmAdjustableParameterB.

        We create a LocalRcConverter that matches the given Igc, set the
        Image_identifier and Rsm_support_data_edition to match that of the
        given Rsm_id, and can optionally activate a set of the parameters for
        the correction. 
        """
        _rsm_adjustable_parameter_b.RsmAdjustableParameterB_swiginit(self, _rsm_adjustable_parameter_b.new_RsmAdjustableParameterB(*args))

    def tre_string(self):
        """

        std::string RsmAdjustableParameterB::tre_string() const

        """
        return _rsm_adjustable_parameter_b.RsmAdjustableParameterB_tre_string(self)


    def read_tre_string(Tre_in):
        """

        boost::shared_ptr< RsmAdjustableParameterB > RsmAdjustableParameterB::read_tre_string(const std::string &Tre_in)
        Read a TRE string.

        Note that the TRE does not contain all the fields we have in a
        RsmRationalPolynomial. However the fields that aren't contained are
        ones used for fitting the RSM, so in practice this doesn't matter. We
        just set the various fields to the default values found in the
        constructor.

        This should have all the TRE except for the front CETAG and CEL. It is
        convenient to treat these fields as special. (We can revisit this in
        the future if we need to). 
        """
        return _rsm_adjustable_parameter_b.RsmAdjustableParameterB_read_tre_string(Tre_in)

    read_tre_string = staticmethod(read_tre_string)

    @property
    def parameter_name(self):
        return self._v_parameter_name()


    def _v_coordinate_converter(self, *args):
        """

        void GeoCal::RsmAdjustableParameterB::coordinate_converter(const boost::shared_ptr< LocalRcConverter > &V)

        """
        return _rsm_adjustable_parameter_b.RsmAdjustableParameterB__v_coordinate_converter(self, *args)


    @property
    def coordinate_converter(self):
        return self._v_coordinate_converter()

    @coordinate_converter.setter
    def coordinate_converter(self, value):
      self._v_coordinate_converter(value)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_adjustable_parameter_b.delete_RsmAdjustableParameterB
RsmAdjustableParameterB.tre_string = new_instancemethod(_rsm_adjustable_parameter_b.RsmAdjustableParameterB_tre_string, None, RsmAdjustableParameterB)
RsmAdjustableParameterB._v_coordinate_converter = new_instancemethod(_rsm_adjustable_parameter_b.RsmAdjustableParameterB__v_coordinate_converter, None, RsmAdjustableParameterB)
RsmAdjustableParameterB_swigregister = _rsm_adjustable_parameter_b.RsmAdjustableParameterB_swigregister
RsmAdjustableParameterB_swigregister(RsmAdjustableParameterB)

def RsmAdjustableParameterB_read_tre_string(Tre_in):
    """

    boost::shared_ptr< RsmAdjustableParameterB > RsmAdjustableParameterB::read_tre_string(const std::string &Tre_in)
    Read a TRE string.

    Note that the TRE does not contain all the fields we have in a
    RsmRationalPolynomial. However the fields that aren't contained are
    ones used for fitting the RSM, so in practice this doesn't matter. We
    just set the various fields to the default values found in the
    constructor.

    This should have all the TRE except for the front CETAG and CEL. It is
    convenient to treat these fields as special. (We can revisit this in
    the future if we need to). 
    """
    return _rsm_adjustable_parameter_b.RsmAdjustableParameterB_read_tre_string(Tre_in)


__all__ = ["RsmAdjustableParameterB"]



