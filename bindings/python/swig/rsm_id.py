# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _rsm_id.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rsm_id', [dirname(__file__)])
        except ImportError:
            import _rsm_id
            return _rsm_id
        if fp is not None:
            try:
                _mod = imp.load_module('_rsm_id', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _rsm_id = swig_import_helper()
    del swig_import_helper
else:
    import _rsm_id
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
    __swig_destroy__ = _rsm_id.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_rsm_id.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_rsm_id.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_rsm_id.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_rsm_id.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_rsm_id.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_rsm_id.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_rsm_id.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_rsm_id.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_rsm_id.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_rsm_id.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_rsm_id.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_rsm_id.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_rsm_id.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_rsm_id.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_rsm_id.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_rsm_id.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _rsm_id.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_rsm_id.SHARED_PTR_DISOWN_swigconstant(_rsm_id)
SHARED_PTR_DISOWN = _rsm_id.SHARED_PTR_DISOWN

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
class RsmId(geocal_swig.generic_object.GenericObject):
    """

    This provides additional information about the Rsm, it is what is used
    to populate.

    C++ includes: rsm_id.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Base, Cconv):
        """

        GeoCal::RsmId::RsmId(const boost::shared_ptr< RsmBase > &Base, const boost::shared_ptr<
        CoordinateConverter > &Cconv)

        """
        _rsm_id.RsmId_swiginit(self, _rsm_id.new_RsmId(Base, Cconv))

    def tre_string(self):
        """

        std::string RsmId::tre_string() const
        Write to TRE string.

        Note that this is all the fields except the CETAG and CEL (the front
        two). It is convenient to treat those special. (We can revisit this in
        the future if we need to). 
        """
        return _rsm_id.RsmId_tre_string(self)


    def read_tre_string(Tre_in):
        """

        boost::shared_ptr< RsmId > RsmId::read_tre_string(const std::string &Tre_in)
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
        return _rsm_id.RsmId_read_tre_string(Tre_in)

    read_tre_string = staticmethod(read_tre_string)

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _rsm_id.delete_RsmId
RsmId.tre_string = new_instancemethod(_rsm_id.RsmId_tre_string, None, RsmId)
RsmId_swigregister = _rsm_id.RsmId_swigregister
RsmId_swigregister(RsmId)

def RsmId_read_tre_string(Tre_in):
    """

    boost::shared_ptr< RsmId > RsmId::read_tre_string(const std::string &Tre_in)
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
    return _rsm_id.RsmId_read_tre_string(Tre_in)



