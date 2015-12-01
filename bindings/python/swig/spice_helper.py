# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _spice_helper.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_spice_helper', [dirname(__file__)])
        except ImportError:
            import _spice_helper
            return _spice_helper
        if fp is not None:
            try:
                _mod = imp.load_module('_spice_helper', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _spice_helper = swig_import_helper()
    del swig_import_helper
else:
    import _spice_helper
del version_info
try:
    _swig_property = property
except NameError:
    pass # Python < 2.2 doesn't have 'property'.
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError(name)

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
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
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _spice_helper.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_spice_helper.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_spice_helper.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_spice_helper.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_spice_helper.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_spice_helper.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_spice_helper.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_spice_helper.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_spice_helper.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_spice_helper.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_spice_helper.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_spice_helper.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_spice_helper.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_spice_helper.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_spice_helper.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_spice_helper.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_spice_helper.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _spice_helper.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _spice_helper.SHARED_PTR_DISOWN
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

import geocal_swig.generic_object
class Vector_Time(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _spice_helper.Vector_Time_swiginit(self,_spice_helper.new_Vector_Time(*args))
    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _spice_helper.delete_Vector_Time
Vector_Time.iterator = new_instancemethod(_spice_helper.Vector_Time_iterator,None,Vector_Time)
Vector_Time.__nonzero__ = new_instancemethod(_spice_helper.Vector_Time___nonzero__,None,Vector_Time)
Vector_Time.__bool__ = new_instancemethod(_spice_helper.Vector_Time___bool__,None,Vector_Time)
Vector_Time.__len__ = new_instancemethod(_spice_helper.Vector_Time___len__,None,Vector_Time)
Vector_Time.pop = new_instancemethod(_spice_helper.Vector_Time_pop,None,Vector_Time)
Vector_Time.__getslice__ = new_instancemethod(_spice_helper.Vector_Time___getslice__,None,Vector_Time)
Vector_Time.__setslice__ = new_instancemethod(_spice_helper.Vector_Time___setslice__,None,Vector_Time)
Vector_Time.__delslice__ = new_instancemethod(_spice_helper.Vector_Time___delslice__,None,Vector_Time)
Vector_Time.__delitem__ = new_instancemethod(_spice_helper.Vector_Time___delitem__,None,Vector_Time)
Vector_Time.__getitem__ = new_instancemethod(_spice_helper.Vector_Time___getitem__,None,Vector_Time)
Vector_Time.__setitem__ = new_instancemethod(_spice_helper.Vector_Time___setitem__,None,Vector_Time)
Vector_Time.append = new_instancemethod(_spice_helper.Vector_Time_append,None,Vector_Time)
Vector_Time.empty = new_instancemethod(_spice_helper.Vector_Time_empty,None,Vector_Time)
Vector_Time.size = new_instancemethod(_spice_helper.Vector_Time_size,None,Vector_Time)
Vector_Time.clear = new_instancemethod(_spice_helper.Vector_Time_clear,None,Vector_Time)
Vector_Time.swap = new_instancemethod(_spice_helper.Vector_Time_swap,None,Vector_Time)
Vector_Time.get_allocator = new_instancemethod(_spice_helper.Vector_Time_get_allocator,None,Vector_Time)
Vector_Time.begin = new_instancemethod(_spice_helper.Vector_Time_begin,None,Vector_Time)
Vector_Time.end = new_instancemethod(_spice_helper.Vector_Time_end,None,Vector_Time)
Vector_Time.rbegin = new_instancemethod(_spice_helper.Vector_Time_rbegin,None,Vector_Time)
Vector_Time.rend = new_instancemethod(_spice_helper.Vector_Time_rend,None,Vector_Time)
Vector_Time.pop_back = new_instancemethod(_spice_helper.Vector_Time_pop_back,None,Vector_Time)
Vector_Time.erase = new_instancemethod(_spice_helper.Vector_Time_erase,None,Vector_Time)
Vector_Time.push_back = new_instancemethod(_spice_helper.Vector_Time_push_back,None,Vector_Time)
Vector_Time.front = new_instancemethod(_spice_helper.Vector_Time_front,None,Vector_Time)
Vector_Time.back = new_instancemethod(_spice_helper.Vector_Time_back,None,Vector_Time)
Vector_Time.assign = new_instancemethod(_spice_helper.Vector_Time_assign,None,Vector_Time)
Vector_Time.resize = new_instancemethod(_spice_helper.Vector_Time_resize,None,Vector_Time)
Vector_Time.insert = new_instancemethod(_spice_helper.Vector_Time_insert,None,Vector_Time)
Vector_Time.reserve = new_instancemethod(_spice_helper.Vector_Time_reserve,None,Vector_Time)
Vector_Time.capacity = new_instancemethod(_spice_helper.Vector_Time_capacity,None,Vector_Time)
Vector_Time_swigregister = _spice_helper.Vector_Time_swigregister
Vector_Time_swigregister(Vector_Time)

class Vector_Time2(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _spice_helper.Vector_Time2_swiginit(self,_spice_helper.new_Vector_Time2(*args))
    __swig_destroy__ = _spice_helper.delete_Vector_Time2
Vector_Time2.iterator = new_instancemethod(_spice_helper.Vector_Time2_iterator,None,Vector_Time2)
Vector_Time2.__nonzero__ = new_instancemethod(_spice_helper.Vector_Time2___nonzero__,None,Vector_Time2)
Vector_Time2.__bool__ = new_instancemethod(_spice_helper.Vector_Time2___bool__,None,Vector_Time2)
Vector_Time2.__len__ = new_instancemethod(_spice_helper.Vector_Time2___len__,None,Vector_Time2)
Vector_Time2.pop = new_instancemethod(_spice_helper.Vector_Time2_pop,None,Vector_Time2)
Vector_Time2.__getslice__ = new_instancemethod(_spice_helper.Vector_Time2___getslice__,None,Vector_Time2)
Vector_Time2.__setslice__ = new_instancemethod(_spice_helper.Vector_Time2___setslice__,None,Vector_Time2)
Vector_Time2.__delslice__ = new_instancemethod(_spice_helper.Vector_Time2___delslice__,None,Vector_Time2)
Vector_Time2.__delitem__ = new_instancemethod(_spice_helper.Vector_Time2___delitem__,None,Vector_Time2)
Vector_Time2.__getitem__ = new_instancemethod(_spice_helper.Vector_Time2___getitem__,None,Vector_Time2)
Vector_Time2.__setitem__ = new_instancemethod(_spice_helper.Vector_Time2___setitem__,None,Vector_Time2)
Vector_Time2.append = new_instancemethod(_spice_helper.Vector_Time2_append,None,Vector_Time2)
Vector_Time2.empty = new_instancemethod(_spice_helper.Vector_Time2_empty,None,Vector_Time2)
Vector_Time2.size = new_instancemethod(_spice_helper.Vector_Time2_size,None,Vector_Time2)
Vector_Time2.clear = new_instancemethod(_spice_helper.Vector_Time2_clear,None,Vector_Time2)
Vector_Time2.swap = new_instancemethod(_spice_helper.Vector_Time2_swap,None,Vector_Time2)
Vector_Time2.get_allocator = new_instancemethod(_spice_helper.Vector_Time2_get_allocator,None,Vector_Time2)
Vector_Time2.begin = new_instancemethod(_spice_helper.Vector_Time2_begin,None,Vector_Time2)
Vector_Time2.end = new_instancemethod(_spice_helper.Vector_Time2_end,None,Vector_Time2)
Vector_Time2.rbegin = new_instancemethod(_spice_helper.Vector_Time2_rbegin,None,Vector_Time2)
Vector_Time2.rend = new_instancemethod(_spice_helper.Vector_Time2_rend,None,Vector_Time2)
Vector_Time2.pop_back = new_instancemethod(_spice_helper.Vector_Time2_pop_back,None,Vector_Time2)
Vector_Time2.erase = new_instancemethod(_spice_helper.Vector_Time2_erase,None,Vector_Time2)
Vector_Time2.push_back = new_instancemethod(_spice_helper.Vector_Time2_push_back,None,Vector_Time2)
Vector_Time2.front = new_instancemethod(_spice_helper.Vector_Time2_front,None,Vector_Time2)
Vector_Time2.back = new_instancemethod(_spice_helper.Vector_Time2_back,None,Vector_Time2)
Vector_Time2.assign = new_instancemethod(_spice_helper.Vector_Time2_assign,None,Vector_Time2)
Vector_Time2.resize = new_instancemethod(_spice_helper.Vector_Time2_resize,None,Vector_Time2)
Vector_Time2.insert = new_instancemethod(_spice_helper.Vector_Time2_insert,None,Vector_Time2)
Vector_Time2.reserve = new_instancemethod(_spice_helper.Vector_Time2_reserve,None,Vector_Time2)
Vector_Time2.capacity = new_instancemethod(_spice_helper.Vector_Time2_capacity,None,Vector_Time2)
Vector_Time2_swigregister = _spice_helper.Vector_Time2_swigregister
Vector_Time2_swigregister(Vector_Time2)

class SpiceHelper(object):
    """
    This class centralizes all of the CSPICE calls from the CSPICE library
    (available from NAIF athttp://naif.jpl.nasa.gov/naif/pds.html).

    This class handles initializing CSPICE, and provides one place to
    modify if we ever need to replace the dependency on this library.

    SPICE depends on various kernels. We load the file found at
    $SPICEDATA/geocal.ker. This file in turn loads a number of other
    kernels, you can edit this file to change which kernels are used.

    C++ includes: spice_helper.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def body_name(*args):
        """
        std::string SpiceHelper::body_name(int Body_id)
        Return the body name for the given id. 
        """
        return _spice_helper.SpiceHelper_body_name(*args)

    body_name = staticmethod(body_name)
    def fixed_frame_name(*args):
        """
        std::string SpiceHelper::fixed_frame_name(int Body_id)
        Return the fixed frame name for the given body.

        We pull this out so we don't have lots of switch statements elsewhere.

        """
        return _spice_helper.SpiceHelper_fixed_frame_name(*args)

    fixed_frame_name = staticmethod(fixed_frame_name)
    def spice_available():
        """
        bool SpiceHelper::spice_available()
        Return true if we have SPICE functionality available, false otherwise.

        """
        return _spice_helper.SpiceHelper_spice_available()

    spice_available = staticmethod(spice_available)
    def spice_setup(Kernel="geocal.ker"):
        """
        void SpiceHelper::spice_setup(const std::string &Kernel="geocal.ker")
        Set SPICE errors to just return, rather than aborting. 
        """
        return _spice_helper.SpiceHelper_spice_setup(Kernel)

    spice_setup = staticmethod(spice_setup)
    def add_kernel(*args):
        """
        void SpiceHelper::add_kernel(const std::string &Kernel_dir, const std::string &Kernel)
        Add an additional kernel, after the one we automatically get (i.e.,
        $SPICEDATA/geocal.ker).

        We change to the given Kernel_dir, so you can use relative paths if
        desired for the kernel names. 
        """
        return _spice_helper.SpiceHelper_add_kernel(*args)

    add_kernel = staticmethod(add_kernel)
    def conversion_quaternion(*args):
        """
        boost::math::quaternion< double > SpiceHelper::conversion_quaternion(const std::string &From, const std::string &To, const Time &T)
        Return quaternion that converts between the two named coordinate
        systems. 
        """
        return _spice_helper.SpiceHelper_conversion_quaternion(*args)

    conversion_quaternion = staticmethod(conversion_quaternion)
    def __init__(self): 
        _spice_helper.SpiceHelper_swiginit(self,_spice_helper.new_SpiceHelper())
    __swig_destroy__ = _spice_helper.delete_SpiceHelper
SpiceHelper_swigregister = _spice_helper.SpiceHelper_swigregister
SpiceHelper_swigregister(SpiceHelper)

def SpiceHelper_body_name(*args):
  """
    std::string SpiceHelper::body_name(int Body_id)
    Return the body name for the given id. 
    """
  return _spice_helper.SpiceHelper_body_name(*args)

def SpiceHelper_fixed_frame_name(*args):
  """
    std::string SpiceHelper::fixed_frame_name(int Body_id)
    Return the fixed frame name for the given body.

    We pull this out so we don't have lots of switch statements elsewhere.

    """
  return _spice_helper.SpiceHelper_fixed_frame_name(*args)

def SpiceHelper_spice_available():
  """
    bool SpiceHelper::spice_available()
    Return true if we have SPICE functionality available, false otherwise.

    """
  return _spice_helper.SpiceHelper_spice_available()

def SpiceHelper_spice_setup(Kernel="geocal.ker"):
  """
    void SpiceHelper::spice_setup(const std::string &Kernel="geocal.ker")
    Set SPICE errors to just return, rather than aborting. 
    """
  return _spice_helper.SpiceHelper_spice_setup(Kernel)

def SpiceHelper_add_kernel(*args):
  """
    void SpiceHelper::add_kernel(const std::string &Kernel_dir, const std::string &Kernel)
    Add an additional kernel, after the one we automatically get (i.e.,
    $SPICEDATA/geocal.ker).

    We change to the given Kernel_dir, so you can use relative paths if
    desired for the kernel names. 
    """
  return _spice_helper.SpiceHelper_add_kernel(*args)

def SpiceHelper_conversion_quaternion(*args):
  """
    boost::math::quaternion< double > SpiceHelper::conversion_quaternion(const std::string &From, const std::string &To, const Time &T)
    Return quaternion that converts between the two named coordinate
    systems. 
    """
  return _spice_helper.SpiceHelper_conversion_quaternion(*args)



