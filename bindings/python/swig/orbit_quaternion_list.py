# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _orbit_quaternion_list.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_quaternion_list', [dirname(__file__)])
        except ImportError:
            import _orbit_quaternion_list
            return _orbit_quaternion_list
        if fp is not None:
            try:
                _mod = imp.load_module('_orbit_quaternion_list', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _orbit_quaternion_list = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_quaternion_list
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
    __swig_destroy__ = _orbit_quaternion_list.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_quaternion_list.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_quaternion_list.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _orbit_quaternion_list.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_orbit_quaternion_list.SHARED_PTR_DISOWN_swigconstant(_orbit_quaternion_list)
SHARED_PTR_DISOWN = _orbit_quaternion_list.SHARED_PTR_DISOWN

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

import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class OrbitQuaternionList(geocal_swig.orbit.Orbit):
    """

    This is an implementation of an Orbit that is a list of
    QuaternionOrbitData values.

    For times that fall between these values, we interpolate to get the
    OrbitData.

    Note that for some classes the calculation of the full list of
    QuaternionOrbitData might not be needed, for example an Orbit file
    covering a full day of which we are only using a subset of the data.
    To support this, we allow a lazy evaluation of the
    QuaternionOrbitData. It can initially be supplied as a null
    boost::shared_ptr, and when we encounter a null we call the function
    orbit_data_create. This function should be overriden by a derived
    class to supply the calculation of a QuaternionOrbitData for a
    particular time on demand.

    C++ includes: orbit_quaternion_list.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Data):
        """

        GeoCal::OrbitQuaternionList::OrbitQuaternionList(iterator ibeg, iterator iend)
        Constructor that takes a list of QuaternionOrbitData values.

        Note that the data doesn't need to be sorted, we handle sorting as we
        ingest the data.

        QuaternionOrbitData pointer can be null if we want to do a lazy
        evaluation of the data (see description of class for details). 
        """
        _orbit_quaternion_list.OrbitQuaternionList_swiginit(self, _orbit_quaternion_list.new_OrbitQuaternionList(Data))

    def orbit_data(self, *args):
        """

        boost::shared_ptr< OrbitData > OrbitQuaternionList::orbit_data(const TimeWithDerivative &T) const

        """
        return _orbit_quaternion_list.OrbitQuaternionList_orbit_data(self, *args)


    def _v_quaternion_orbit_data(self):
        """

        std::vector< boost::shared_ptr< QuaternionOrbitData > > OrbitQuaternionList::quaternion_orbit_data() const
        Return the list of QuaternionOrbitData. 
        """
        return _orbit_quaternion_list.OrbitQuaternionList__v_quaternion_orbit_data(self)


    @property
    def quaternion_orbit_data(self):
        return self._v_quaternion_orbit_data()


    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.quaternion_orbit_data)

    __swig_destroy__ = _orbit_quaternion_list.delete_OrbitQuaternionList
OrbitQuaternionList.orbit_data = new_instancemethod(_orbit_quaternion_list.OrbitQuaternionList_orbit_data, None, OrbitQuaternionList)
OrbitQuaternionList._v_quaternion_orbit_data = new_instancemethod(_orbit_quaternion_list.OrbitQuaternionList__v_quaternion_orbit_data, None, OrbitQuaternionList)
OrbitQuaternionList_swigregister = _orbit_quaternion_list.OrbitQuaternionList_swigregister
OrbitQuaternionList_swigregister(OrbitQuaternionList)



