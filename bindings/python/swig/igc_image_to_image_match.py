# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _igc_image_to_image_match.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_image_to_image_match', [dirname(__file__)])
        except ImportError:
            import _igc_image_to_image_match
            return _igc_image_to_image_match
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_image_to_image_match', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_image_to_image_match = swig_import_helper()
    del swig_import_helper
else:
    import _igc_image_to_image_match
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


SHARED_PTR_DISOWN = _igc_image_to_image_match.SHARED_PTR_DISOWN
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

import geocal_swig.image_to_image_match
import geocal_swig.generic_object
import geocal_swig.geocal_exception
import geocal_swig.look_vector
class IgcImageToImageMatch(geocal_swig.image_to_image_match.ImageToImageMatch):
    """
    ImageToImageMatch where the images and prediction comes from
    ImageGroundConnection objects.

    C++ includes: igc_image_to_image_match.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::IgcImageToImageMatch::IgcImageToImageMatch(const boost::shared_ptr< ImageGroundConnection > &Igc1, const
        boost::shared_ptr< ImageGroundConnection > &Igc2, const
        boost::shared_ptr< ImageMatcher > &Matcher)

        """
        _igc_image_to_image_match.IgcImageToImageMatch_swiginit(self,_igc_image_to_image_match.new_IgcImageToImageMatch(*args))
    def _v_image_ground_connection1(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcImageToImageMatch::image_ground_connection1() const
        ImageGroundConnection for image 1. 
        """
        return _igc_image_to_image_match.IgcImageToImageMatch__v_image_ground_connection1(self)

    @property
    def image_ground_connection1(self):
        return self._v_image_ground_connection1()

    def _v_image_ground_connection2(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcImageToImageMatch::image_ground_connection2() const
        ImageGroundConnection for image 2. 
        """
        return _igc_image_to_image_match.IgcImageToImageMatch__v_image_ground_connection2(self)

    @property
    def image_ground_connection2(self):
        return self._v_image_ground_connection2()

    def _v_matcher(self):
        """
        const boost::shared_ptr<ImageMatcher>& GeoCal::IgcImageToImageMatch::matcher() const
        Matcher we are using. 
        """
        return _igc_image_to_image_match.IgcImageToImageMatch__v_matcher(self)

    @property
    def matcher(self):
        return self._v_matcher()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.image_ground_connection1,self.image_ground_connection2,self.matcher)

    __swig_destroy__ = _igc_image_to_image_match.delete_IgcImageToImageMatch
IgcImageToImageMatch._v_image_ground_connection1 = new_instancemethod(_igc_image_to_image_match.IgcImageToImageMatch__v_image_ground_connection1,None,IgcImageToImageMatch)
IgcImageToImageMatch._v_image_ground_connection2 = new_instancemethod(_igc_image_to_image_match.IgcImageToImageMatch__v_image_ground_connection2,None,IgcImageToImageMatch)
IgcImageToImageMatch._v_matcher = new_instancemethod(_igc_image_to_image_match.IgcImageToImageMatch__v_matcher,None,IgcImageToImageMatch)
IgcImageToImageMatch_swigregister = _igc_image_to_image_match.IgcImageToImageMatch_swigregister
IgcImageToImageMatch_swigregister(IgcImageToImageMatch)



