# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _image_mask.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_image_mask', [dirname(__file__)])
        except ImportError:
            import _image_mask
            return _image_mask
        if fp is not None:
            try:
                _mod = imp.load_module('_image_mask', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _image_mask = swig_import_helper()
    del swig_import_helper
else:
    import _image_mask
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
    __swig_destroy__ = _image_mask.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_image_mask.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_image_mask.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_image_mask.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_image_mask.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_image_mask.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_image_mask.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_image_mask.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_image_mask.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_image_mask.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_image_mask.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_image_mask.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_image_mask.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_image_mask.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_image_mask.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_image_mask.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_image_mask.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _image_mask.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _image_mask.SHARED_PTR_DISOWN
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
class ImageMask(geocal.generic_object.GenericObject):
    """
    This provides a ImageMask, which can be used to prevent doing some
    kind of processing.

    An example is a using a Cloud mask to restrict image matching to cloud
    free areas.

    The mask can look at specific ImageCoordinate points indicating if the
    that point is masked or not. You can also look at a Image area to see
    if any of the area is masked (e.g., avoid image matching on this area
    because it is masked).

    This mask works in ImageCoordinates, cf. GroundMask.

    C++ includes: image_mask.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def mask(self, *args):
        """
        virtual bool GeoCal::ImageMask::mask(int Line, int Sample) const =0
        Indicate if a particular point is masked.

        If true, the point is masked and should not be used in processing
        (e.g., don't do image matching) 
        """
        return _image_mask.ImageMask_mask(self, *args)

    def area_any_masked(self, *args):
        """
        bool ImageMask::area_any_masked(int Line, int Sample, int Number_line, int Number_sample) const
        Indicated if a area has any masking or not.

        See the discussion in the comments of Mask for detailed discussion of
        the check. 
        """
        return _image_mask.ImageMask_area_any_masked(self, *args)

    __swig_destroy__ = _image_mask.delete_ImageMask
ImageMask.mask = new_instancemethod(_image_mask.ImageMask_mask,None,ImageMask)
ImageMask.area_any_masked = new_instancemethod(_image_mask.ImageMask_area_any_masked,None,ImageMask)
ImageMask.__str__ = new_instancemethod(_image_mask.ImageMask___str__,None,ImageMask)
ImageMask_swigregister = _image_mask.ImageMask_swigregister
ImageMask_swigregister(ImageMask)

class CombinedImageMask(ImageMask):
    """
    This is combines a set of masks into a single one.

    The combination just check each mask in turn, and if any of them
    indicates a point or a area is masked, then we return true.

    Note that this class handles the degenerate case of no masked. This is
    particular useful in handling situations where you might or might not
    have mask, e.g. a general routine that can take a mask but where you
    don't happen to have any masks in a particular case.

    When there are no masks, we return true.

    C++ includes: image_mask.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::CombinedImageMask::CombinedImageMask(It First, It Last)
        Create a CombinedImageMask using the given set of masks, where we get
        this from a general InputIterator. 
        """
        _image_mask.CombinedImageMask_swiginit(self,_image_mask.new_CombinedImageMask(*args))
    mask_list = _swig_property(_image_mask.CombinedImageMask_mask_list_get, _image_mask.CombinedImageMask_mask_list_set)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.mask_list)

    __swig_destroy__ = _image_mask.delete_CombinedImageMask
CombinedImageMask_swigregister = _image_mask.CombinedImageMask_swigregister
CombinedImageMask_swigregister(CombinedImageMask)

class Vector_ImageMask(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __iter__(self): return self.iterator()
    def __init__(self, *args): 
        _image_mask.Vector_ImageMask_swiginit(self,_image_mask.new_Vector_ImageMask(*args))
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, list(self))

    __swig_destroy__ = _image_mask.delete_Vector_ImageMask
Vector_ImageMask.iterator = new_instancemethod(_image_mask.Vector_ImageMask_iterator,None,Vector_ImageMask)
Vector_ImageMask.__nonzero__ = new_instancemethod(_image_mask.Vector_ImageMask___nonzero__,None,Vector_ImageMask)
Vector_ImageMask.__bool__ = new_instancemethod(_image_mask.Vector_ImageMask___bool__,None,Vector_ImageMask)
Vector_ImageMask.__len__ = new_instancemethod(_image_mask.Vector_ImageMask___len__,None,Vector_ImageMask)
Vector_ImageMask.pop = new_instancemethod(_image_mask.Vector_ImageMask_pop,None,Vector_ImageMask)
Vector_ImageMask.__getslice__ = new_instancemethod(_image_mask.Vector_ImageMask___getslice__,None,Vector_ImageMask)
Vector_ImageMask.__setslice__ = new_instancemethod(_image_mask.Vector_ImageMask___setslice__,None,Vector_ImageMask)
Vector_ImageMask.__delslice__ = new_instancemethod(_image_mask.Vector_ImageMask___delslice__,None,Vector_ImageMask)
Vector_ImageMask.__delitem__ = new_instancemethod(_image_mask.Vector_ImageMask___delitem__,None,Vector_ImageMask)
Vector_ImageMask.__getitem__ = new_instancemethod(_image_mask.Vector_ImageMask___getitem__,None,Vector_ImageMask)
Vector_ImageMask.__setitem__ = new_instancemethod(_image_mask.Vector_ImageMask___setitem__,None,Vector_ImageMask)
Vector_ImageMask.append = new_instancemethod(_image_mask.Vector_ImageMask_append,None,Vector_ImageMask)
Vector_ImageMask.empty = new_instancemethod(_image_mask.Vector_ImageMask_empty,None,Vector_ImageMask)
Vector_ImageMask.size = new_instancemethod(_image_mask.Vector_ImageMask_size,None,Vector_ImageMask)
Vector_ImageMask.clear = new_instancemethod(_image_mask.Vector_ImageMask_clear,None,Vector_ImageMask)
Vector_ImageMask.swap = new_instancemethod(_image_mask.Vector_ImageMask_swap,None,Vector_ImageMask)
Vector_ImageMask.get_allocator = new_instancemethod(_image_mask.Vector_ImageMask_get_allocator,None,Vector_ImageMask)
Vector_ImageMask.begin = new_instancemethod(_image_mask.Vector_ImageMask_begin,None,Vector_ImageMask)
Vector_ImageMask.end = new_instancemethod(_image_mask.Vector_ImageMask_end,None,Vector_ImageMask)
Vector_ImageMask.rbegin = new_instancemethod(_image_mask.Vector_ImageMask_rbegin,None,Vector_ImageMask)
Vector_ImageMask.rend = new_instancemethod(_image_mask.Vector_ImageMask_rend,None,Vector_ImageMask)
Vector_ImageMask.pop_back = new_instancemethod(_image_mask.Vector_ImageMask_pop_back,None,Vector_ImageMask)
Vector_ImageMask.erase = new_instancemethod(_image_mask.Vector_ImageMask_erase,None,Vector_ImageMask)
Vector_ImageMask.push_back = new_instancemethod(_image_mask.Vector_ImageMask_push_back,None,Vector_ImageMask)
Vector_ImageMask.front = new_instancemethod(_image_mask.Vector_ImageMask_front,None,Vector_ImageMask)
Vector_ImageMask.back = new_instancemethod(_image_mask.Vector_ImageMask_back,None,Vector_ImageMask)
Vector_ImageMask.assign = new_instancemethod(_image_mask.Vector_ImageMask_assign,None,Vector_ImageMask)
Vector_ImageMask.resize = new_instancemethod(_image_mask.Vector_ImageMask_resize,None,Vector_ImageMask)
Vector_ImageMask.insert = new_instancemethod(_image_mask.Vector_ImageMask_insert,None,Vector_ImageMask)
Vector_ImageMask.reserve = new_instancemethod(_image_mask.Vector_ImageMask_reserve,None,Vector_ImageMask)
Vector_ImageMask.capacity = new_instancemethod(_image_mask.Vector_ImageMask_capacity,None,Vector_ImageMask)
Vector_ImageMask_swigregister = _image_mask.Vector_ImageMask_swigregister
Vector_ImageMask_swigregister(Vector_ImageMask)



