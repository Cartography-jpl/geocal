# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_adjustable_parameter.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_rsm_adjustable_parameter')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_rsm_adjustable_parameter')
    _rsm_adjustable_parameter = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_adjustable_parameter', [dirname(__file__)])
        except ImportError:
            import _rsm_adjustable_parameter
            return _rsm_adjustable_parameter
        try:
            _mod = imp.load_module('_rsm_adjustable_parameter', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _rsm_adjustable_parameter = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_adjustable_parameter
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
    __swig_destroy__ = _rsm_adjustable_parameter.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_adjustable_parameter.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_adjustable_parameter.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _rsm_adjustable_parameter.SHARED_PTR_DISOWN

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
class RsmAdjustableParameter(geocal_swig.with_parameter.WithParameter):
    """

    This is used to handle the RSM adjustable parameters.

    There are currently two version of this, RSMAPA and RSMAPB. We handle
    the specific versions by subclasses, this base class just gives the
    overall interface.

    C++ includes: rsm_adjustable_parameter.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr

    def adjustment(self, Gc, Gc_adjusted, Lndelta, Smpdelta):
        """

        virtual void GeoCal::RsmAdjustableParameter::adjustment(const GroundCoordinate &Gc, boost::shared_ptr< GroundCoordinate >
        &Gc_adjusted, double &Lndelta, double &Smpdelta) const =0
        Adjustment for given ground location. 
        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter_adjustment(self, Gc, Gc_adjusted, Lndelta, Smpdelta)


    def adjustment_with_derivative(self, Gc, Cf_adjusted, Lndelta, Smpdelta):
        """

        virtual void GeoCal::RsmAdjustableParameter::adjustment_with_derivative(const GroundCoordinate &Gc, ArrayAd< double, 1 > &Cf_adjusted,
        AutoDerivative< double > &Lndelta, AutoDerivative< double > &Smpdelta)
        const =0
        Adjustment including sample derivatives. 
        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter_adjustment_with_derivative(self, Gc, Cf_adjusted, Lndelta, Smpdelta)


    def base_tre_string(self):
        """

        std::string RsmAdjustableParameter::base_tre_string() const
        Write the part of the TRE string for the image identification, RSM
        support data edition, and triangulation_id. 
        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter_base_tre_string(self)


    def base_read_tre_string(self, In):
        """

        void RsmAdjustableParameter::base_read_tre_string(std::istream &In)

        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter_base_read_tre_string(self, In)


    def _v_image_identifier(self, *args):
        """

        void GeoCal::RsmAdjustableParameter::image_identifier(const std::string &V)

        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter__v_image_identifier(self, *args)


    @property
    def image_identifier(self):
        return self._v_image_identifier()

    @image_identifier.setter
    def image_identifier(self, value):
      self._v_image_identifier(value)


    def _v_rsm_support_data_edition(self, *args):
        """

        void GeoCal::RsmAdjustableParameter::rsm_support_data_edition(const std::string &V)

        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter__v_rsm_support_data_edition(self, *args)


    @property
    def rsm_support_data_edition(self):
        return self._v_rsm_support_data_edition()

    @rsm_support_data_edition.setter
    def rsm_support_data_edition(self, value):
      self._v_rsm_support_data_edition(value)


    def _v_triangulation_id(self, *args):
        """

        void GeoCal::RsmAdjustableParameter::triangulation_id(const std::string &V)

        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter__v_triangulation_id(self, *args)


    @property
    def triangulation_id(self):
        return self._v_triangulation_id()

    @triangulation_id.setter
    def triangulation_id(self, value):
      self._v_triangulation_id(value)


    def _v_naif_code(self, *args):
        """

        virtual void GeoCal::RsmAdjustableParameter::naif_code(int Naif_code)=0
        Set the NAIF code for the planet/body we are working with.

        Note that the NITF TRE structure does not have a place to store the
        NAIF code, it implicitly assumes earth. So when we read a TRE, even
        for something like Mars, we have the NAIF code set to earth. We need
        to update this with other metadata (e.g. TARGET_NAME in PDS label).

        This is not a problem for boost serialization (which keeps the NAIF
        code), just for NITF TRE. 
        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter__v_naif_code(self, *args)


    @property
    def naif_code(self):
        return self._v_naif_code()

    @naif_code.setter
    def naif_code(self, value):
      self._v_naif_code(value)


    def _v_has_ground_coordinate_parameter(self):
        """

        virtual bool GeoCal::RsmAdjustableParameter::has_ground_coordinate_parameter() const =0
        The derivative handling requires different handling for ground
        coordinate adjusting parameters vs.

        just image coordinate adjusting parameters. This function lets the Rsm
        class know if it needs to worry about the ground coordinate adjusting
        parameters. 
        """
        return _rsm_adjustable_parameter.RsmAdjustableParameter__v_has_ground_coordinate_parameter(self)


    @property
    def has_ground_coordinate_parameter(self):
        return self._v_has_ground_coordinate_parameter()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_adjustable_parameter.delete_RsmAdjustableParameter
RsmAdjustableParameter.adjustment = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter_adjustment, None, RsmAdjustableParameter)
RsmAdjustableParameter.adjustment_with_derivative = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter_adjustment_with_derivative, None, RsmAdjustableParameter)
RsmAdjustableParameter.base_tre_string = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter_base_tre_string, None, RsmAdjustableParameter)
RsmAdjustableParameter.base_read_tre_string = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter_base_read_tre_string, None, RsmAdjustableParameter)
RsmAdjustableParameter._v_image_identifier = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter__v_image_identifier, None, RsmAdjustableParameter)
RsmAdjustableParameter._v_rsm_support_data_edition = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter__v_rsm_support_data_edition, None, RsmAdjustableParameter)
RsmAdjustableParameter._v_triangulation_id = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter__v_triangulation_id, None, RsmAdjustableParameter)
RsmAdjustableParameter._v_naif_code = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter__v_naif_code, None, RsmAdjustableParameter)
RsmAdjustableParameter._v_has_ground_coordinate_parameter = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter__v_has_ground_coordinate_parameter, None, RsmAdjustableParameter)
RsmAdjustableParameter.__str__ = new_instancemethod(_rsm_adjustable_parameter.RsmAdjustableParameter___str__, None, RsmAdjustableParameter)
RsmAdjustableParameter_swigregister = _rsm_adjustable_parameter.RsmAdjustableParameter_swigregister
RsmAdjustableParameter_swigregister(RsmAdjustableParameter)


__all__ = ["RsmAdjustableParameter"]



