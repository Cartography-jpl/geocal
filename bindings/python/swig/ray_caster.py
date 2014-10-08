# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ray_caster.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ray_caster', [dirname(__file__)])
        except ImportError:
            import _ray_caster
            return _ray_caster
        if fp is not None:
            try:
                _mod = imp.load_module('_ray_caster', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ray_caster = swig_import_helper()
    del swig_import_helper
else:
    import _ray_caster
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
    __swig_destroy__ = _ray_caster.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_ray_caster.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ray_caster.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ray_caster.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ray_caster.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ray_caster.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ray_caster.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ray_caster.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ray_caster.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ray_caster.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ray_caster.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ray_caster.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ray_caster.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ray_caster.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ray_caster.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ray_caster.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ray_caster.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _ray_caster.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ray_caster.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
    return inst

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
class RayCaster(geocal_swig.generic_object.GenericObject):
    """
    This class is used to do ray casting, determining what in instrument
    is looking at on the ground (e.g., a camera at a particular pose).

    This includes the calculation of obscuration.

    This class chooses what order it is going to go through the various
    camera positions. You can step to the next position, getting the
    ground locations for that position, by calling "next_position". All
    positions have been stepped through from start_position() to
    start_position() + number_position() - 1 (in whatever order the class
    wants) when last_position() returns true.The current position can be
    queried by "current_position". The mapping of position to something
    like image line depends on the particular derived class used, so for
    example a push broom camera might have the position corresponding to
    line number.

    C++ includes: ray_caster.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def _v_start_position(self):
        """
        virtual int GeoCal::RayCaster::start_position() const =0
        The smallest position covered by this class. 
        """
        return _ray_caster.RayCaster__v_start_position(self)

    @property
    def start_position(self):
        return self._v_start_position()

    def _v_number_position(self):
        """
        virtual int GeoCal::RayCaster::number_position() const =0
        The number of position covered by this class. 
        """
        return _ray_caster.RayCaster__v_number_position(self)

    @property
    def number_position(self):
        return self._v_number_position()

    def _v_last_position(self):
        """
        virtual bool GeoCal::RayCaster::last_position() const =0
        Returns true when we have stepped through all positions from
        start_position() to start_position() + number_position() - 1. 
        """
        return _ray_caster.RayCaster__v_last_position(self)

    @property
    def last_position(self):
        return self._v_last_position()

    def _v_current_position(self):
        """
        virtual int GeoCal::RayCaster::current_position() const =0
        The position that was last returned by next_position. 
        """
        return _ray_caster.RayCaster__v_current_position(self)

    @property
    def current_position(self):
        return self._v_current_position()

    def next_position(self):
        """
        virtual blitz::Array<double, 6> GeoCal::RayCaster::next_position()=0
        Return the ground locations seen at the next position.

        This is This is nline x nsamp x x nsub_line x nsub_sample x
        nintegration_step x 3 in size, where we give the ground location as a
        CartesianFixed coordinate (e.g., Ecr for the Earth) 
        """
        return _ray_caster.RayCaster_next_position(self)

    __swig_destroy__ = _ray_caster.delete_RayCaster
RayCaster._v_start_position = new_instancemethod(_ray_caster.RayCaster__v_start_position,None,RayCaster)
RayCaster._v_number_position = new_instancemethod(_ray_caster.RayCaster__v_number_position,None,RayCaster)
RayCaster._v_last_position = new_instancemethod(_ray_caster.RayCaster__v_last_position,None,RayCaster)
RayCaster._v_current_position = new_instancemethod(_ray_caster.RayCaster__v_current_position,None,RayCaster)
RayCaster.next_position = new_instancemethod(_ray_caster.RayCaster_next_position,None,RayCaster)
RayCaster.__str__ = new_instancemethod(_ray_caster.RayCaster___str__,None,RayCaster)
RayCaster_swigregister = _ray_caster.RayCaster_swigregister
RayCaster_swigregister(RayCaster)


