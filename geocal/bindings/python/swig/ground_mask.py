# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ground_mask.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ground_mask', [dirname(__file__)])
        except ImportError:
            import _ground_mask
            return _ground_mask
        if fp is not None:
            try:
                _mod = imp.load_module('_ground_mask', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ground_mask = swig_import_helper()
    del swig_import_helper
else:
    import _ground_mask
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
    __swig_destroy__ = _ground_mask.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_ground_mask.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_ground_mask.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_ground_mask.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_ground_mask.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_ground_mask.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_ground_mask.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_ground_mask.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_ground_mask.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_ground_mask.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_ground_mask.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_ground_mask.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_ground_mask.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_ground_mask.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_ground_mask.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_ground_mask.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_ground_mask.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _ground_mask.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _ground_mask.SHARED_PTR_DISOWN
def _new_from_init(cls, version, *args):
    '''For use with pickle, covers common case where we just store the
    arguments needed to create an object. See for example HdfFile'''
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    inst = cls.__new__(cls)
    inst.__init__(*args)
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

import geocal.generic_object
class GroundMask(geocal.generic_object.GenericObject):
    """
    This provides a GroundMask, which can be used to prevent doing some
    kind of processing.

    An example is a using a Land/Water mask to restrict image matching to
    land, or a cloud mask to restrict image matching to cloud free areas.

    The mask can look at specific GroundCoordinate points indicating if
    the that point is masked or not. You can also look at a region to see
    if the entire area is masked (e.g., avoid processing on this area
    because it is masked).

    The region search is a bit loose, you give the corners of the area to
    search. The region selected depends on the underlying map projection
    used by the mask data - it might be a region in latitude/longitude, a
    region in UTM, or something else. The region search is useful only for
    small areas where these difference aren't very important. If you need
    detailed, accurate information about a region in a specific map
    projection then you should simply look at each individual point. The
    region is useful for the "don't bother looking here for tie points,
    look somewhere else" sorts of uses.

    If the region search returns true, then all of the region is masked.
    If this is false, then some of the region might be unmasked. Note that
    this is conservative in the true direction - a value of false doesn't
    promise that there is an unmasked pixel, just that we are pretty sure
    that there is one. Edge cases where a false value clips the corner of
    a region might have region search return false, while when you look at
    it pixel by pixel every point ends up being masked.

    Typically a Mask has an underlying RasterImage or something similar. A
    particular GroundCoordinate probably doesn't fall exactly on a pixel
    of the Mask. How we interpolate is defined by the particular derived
    class uses - a common approach is to mask if any of the 4 neighboring
    pixels are masked (but check the derived class for details).

    This mask works in GroundCoordinates, cf. ImageMask.

    C++ includes: ground_mask.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def mask(self, *args):
        """
        virtual bool GeoCal::GroundMask::mask(const GroundCoordinate &Gc) const =0
        Indicate if a particular point is masked.

        If true, the point is masked and should not be used in processing
        (e.g., don't do image matching) 
        """
        return _ground_mask.GroundMask_mask(self, *args)

    def region_masked(self, *args):
        """
        virtual bool GeoCal::GroundMask::region_masked(const GroundCoordinate &Ulc, const GroundCoordinate &Lrc) const =0
        Indicated if a region is all masked or not.

        See the discussion in the comments of Mask for detailed discussion of
        the check. 
        """
        return _ground_mask.GroundMask_region_masked(self, *args)

    __swig_destroy__ = _ground_mask.delete_GroundMask
GroundMask.mask = new_instancemethod(_ground_mask.GroundMask_mask,None,GroundMask)
GroundMask.region_masked = new_instancemethod(_ground_mask.GroundMask_region_masked,None,GroundMask)
GroundMask.__str__ = new_instancemethod(_ground_mask.GroundMask___str__,None,GroundMask)
GroundMask_swigregister = _ground_mask.GroundMask_swigregister
GroundMask_swigregister(GroundMask)

class CombinedGroundMask(GroundMask):
    """
    This is combines a set of masks into a single one.

    The combination just check each mask in turn, and if any of them
    indicates a point or a region is masked, then we return true.

    Note that this class handles the degenerate case of no masked. This is
    particular useful in handling situations where you might or might not
    have mask, e.g. a general routine that can take a mask but where you
    don't happen to have any masks in a particular case.

    When there are no masks, we return true.

    C++ includes: ground_mask.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::CombinedGroundMask::CombinedGroundMask(It First, It Last)
        Create a CombinedMask using the given set of masks, where we get this
        from a general InputIterator. 
        """
        _ground_mask.CombinedGroundMask_swiginit(self,_ground_mask.new_CombinedGroundMask(*args))
    mask_list = _swig_property(_ground_mask.CombinedGroundMask_mask_list_get, _ground_mask.CombinedGroundMask_mask_list_set)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.mask_list)

    __swig_destroy__ = _ground_mask.delete_CombinedGroundMask
CombinedGroundMask_swigregister = _ground_mask.CombinedGroundMask_swigregister
CombinedGroundMask_swigregister(CombinedGroundMask)

class Vector_GroundMask(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _ground_mask.Vector_GroundMask_swiginit(self,_ground_mask.new_Vector_GroundMask(*args))
    __swig_destroy__ = _ground_mask.delete_Vector_GroundMask
Vector_GroundMask.iterator = new_instancemethod(_ground_mask.Vector_GroundMask_iterator,None,Vector_GroundMask)
Vector_GroundMask.__nonzero__ = new_instancemethod(_ground_mask.Vector_GroundMask___nonzero__,None,Vector_GroundMask)
Vector_GroundMask.__bool__ = new_instancemethod(_ground_mask.Vector_GroundMask___bool__,None,Vector_GroundMask)
Vector_GroundMask.__len__ = new_instancemethod(_ground_mask.Vector_GroundMask___len__,None,Vector_GroundMask)
Vector_GroundMask.pop = new_instancemethod(_ground_mask.Vector_GroundMask_pop,None,Vector_GroundMask)
Vector_GroundMask.__getslice__ = new_instancemethod(_ground_mask.Vector_GroundMask___getslice__,None,Vector_GroundMask)
Vector_GroundMask.__setslice__ = new_instancemethod(_ground_mask.Vector_GroundMask___setslice__,None,Vector_GroundMask)
Vector_GroundMask.__delslice__ = new_instancemethod(_ground_mask.Vector_GroundMask___delslice__,None,Vector_GroundMask)
Vector_GroundMask.__delitem__ = new_instancemethod(_ground_mask.Vector_GroundMask___delitem__,None,Vector_GroundMask)
Vector_GroundMask.__getitem__ = new_instancemethod(_ground_mask.Vector_GroundMask___getitem__,None,Vector_GroundMask)
Vector_GroundMask.__setitem__ = new_instancemethod(_ground_mask.Vector_GroundMask___setitem__,None,Vector_GroundMask)
Vector_GroundMask.append = new_instancemethod(_ground_mask.Vector_GroundMask_append,None,Vector_GroundMask)
Vector_GroundMask.empty = new_instancemethod(_ground_mask.Vector_GroundMask_empty,None,Vector_GroundMask)
Vector_GroundMask.size = new_instancemethod(_ground_mask.Vector_GroundMask_size,None,Vector_GroundMask)
Vector_GroundMask.clear = new_instancemethod(_ground_mask.Vector_GroundMask_clear,None,Vector_GroundMask)
Vector_GroundMask.swap = new_instancemethod(_ground_mask.Vector_GroundMask_swap,None,Vector_GroundMask)
Vector_GroundMask.get_allocator = new_instancemethod(_ground_mask.Vector_GroundMask_get_allocator,None,Vector_GroundMask)
Vector_GroundMask.begin = new_instancemethod(_ground_mask.Vector_GroundMask_begin,None,Vector_GroundMask)
Vector_GroundMask.end = new_instancemethod(_ground_mask.Vector_GroundMask_end,None,Vector_GroundMask)
Vector_GroundMask.rbegin = new_instancemethod(_ground_mask.Vector_GroundMask_rbegin,None,Vector_GroundMask)
Vector_GroundMask.rend = new_instancemethod(_ground_mask.Vector_GroundMask_rend,None,Vector_GroundMask)
Vector_GroundMask.pop_back = new_instancemethod(_ground_mask.Vector_GroundMask_pop_back,None,Vector_GroundMask)
Vector_GroundMask.erase = new_instancemethod(_ground_mask.Vector_GroundMask_erase,None,Vector_GroundMask)
Vector_GroundMask.push_back = new_instancemethod(_ground_mask.Vector_GroundMask_push_back,None,Vector_GroundMask)
Vector_GroundMask.front = new_instancemethod(_ground_mask.Vector_GroundMask_front,None,Vector_GroundMask)
Vector_GroundMask.back = new_instancemethod(_ground_mask.Vector_GroundMask_back,None,Vector_GroundMask)
Vector_GroundMask.assign = new_instancemethod(_ground_mask.Vector_GroundMask_assign,None,Vector_GroundMask)
Vector_GroundMask.resize = new_instancemethod(_ground_mask.Vector_GroundMask_resize,None,Vector_GroundMask)
Vector_GroundMask.insert = new_instancemethod(_ground_mask.Vector_GroundMask_insert,None,Vector_GroundMask)
Vector_GroundMask.reserve = new_instancemethod(_ground_mask.Vector_GroundMask_reserve,None,Vector_GroundMask)
Vector_GroundMask.capacity = new_instancemethod(_ground_mask.Vector_GroundMask_capacity,None,Vector_GroundMask)
Vector_GroundMask_swigregister = _ground_mask.Vector_GroundMask_swigregister
Vector_GroundMask_swigregister(Vector_GroundMask)



