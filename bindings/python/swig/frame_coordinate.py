# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _frame_coordinate.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_frame_coordinate', [dirname(__file__)])
        except ImportError:
            import _frame_coordinate
            return _frame_coordinate
        if fp is not None:
            try:
                _mod = imp.load_module('_frame_coordinate', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _frame_coordinate = swig_import_helper()
    del swig_import_helper
else:
    import _frame_coordinate
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


SHARED_PTR_DISOWN = _frame_coordinate.SHARED_PTR_DISOWN
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
class FrameCoordinate(geocal_swig.generic_object.GenericObject):
    """
    This is the coordinates of a frame camera.

    C++ includes: frame_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::FrameCoordinate::FrameCoordinate(double L, double S)
        Create a FrameCoordinate with the given coordinates. 
        """
        _frame_coordinate.FrameCoordinate_swiginit(self,_frame_coordinate.new_FrameCoordinate(*args))
    line = _swig_property(_frame_coordinate.FrameCoordinate_line_get, _frame_coordinate.FrameCoordinate_line_set)
    sample = _swig_property(_frame_coordinate.FrameCoordinate_sample_get, _frame_coordinate.FrameCoordinate_sample_set)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.line,self.sample)

    __swig_destroy__ = _frame_coordinate.delete_FrameCoordinate
FrameCoordinate.__str__ = new_instancemethod(_frame_coordinate.FrameCoordinate___str__,None,FrameCoordinate)
FrameCoordinate_swigregister = _frame_coordinate.FrameCoordinate_swigregister
FrameCoordinate_swigregister(FrameCoordinate)

class FrameCoordinateWithDerivative(geocal_swig.generic_object.GenericObject):
    """
    This is the coordinates of a frame camera, including derivatives.

    C++ includes: frame_coordinate.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::FrameCoordinateWithDerivative::FrameCoordinateWithDerivative(const AutoDerivative< double > &L, const AutoDerivative< double > &S)
        Create a FrameCoordinateWithDerivative with the given coordinates. 
        """
        _frame_coordinate.FrameCoordinateWithDerivative_swiginit(self,_frame_coordinate.new_FrameCoordinateWithDerivative(*args))
    line = _swig_property(_frame_coordinate.FrameCoordinateWithDerivative_line_get, _frame_coordinate.FrameCoordinateWithDerivative_line_set)
    sample = _swig_property(_frame_coordinate.FrameCoordinateWithDerivative_sample_get, _frame_coordinate.FrameCoordinateWithDerivative_sample_set)
    def _v_value(self):
        """
        FrameCoordinate GeoCal::FrameCoordinateWithDerivative::value() const
        Strip off derivative information and return FrameCoordinate. 
        """
        return _frame_coordinate.FrameCoordinateWithDerivative__v_value(self)

    @property
    def value(self):
        return self._v_value()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.line,self.sample)

    __swig_destroy__ = _frame_coordinate.delete_FrameCoordinateWithDerivative
FrameCoordinateWithDerivative.__str__ = new_instancemethod(_frame_coordinate.FrameCoordinateWithDerivative___str__,None,FrameCoordinateWithDerivative)
FrameCoordinateWithDerivative._v_value = new_instancemethod(_frame_coordinate.FrameCoordinateWithDerivative__v_value,None,FrameCoordinateWithDerivative)
FrameCoordinateWithDerivative_swigregister = _frame_coordinate.FrameCoordinateWithDerivative_swigregister
FrameCoordinateWithDerivative_swigregister(FrameCoordinateWithDerivative)



