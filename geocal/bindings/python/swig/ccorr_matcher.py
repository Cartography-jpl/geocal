# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _ccorr_matcher.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_ccorr_matcher', [dirname(__file__)])
        except ImportError:
            import _ccorr_matcher
            return _ccorr_matcher
        if fp is not None:
            try:
                _mod = imp.load_module('_ccorr_matcher', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _ccorr_matcher = swig_import_helper()
    del swig_import_helper
else:
    import _ccorr_matcher
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


SHARED_PTR_DISOWN = _ccorr_matcher.SHARED_PTR_DISOWN
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

import geocal.image_matcher
import geocal.generic_object
class CcorrMatcher(geocal.image_matcher.ImageMatcher):
    """
    This class performs image matching.

    It does a normalized cross correlation match.

    C++ includes: ccorr_matcher.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, Target_nline=37, Target_nsamp=37, Template_nline=9, Template_nsamp=9, Min_ccorr=0.3, 
    Min_variance=0): 
        """
        CcorrMatcher::CcorrMatcher(int Target_nline=37, int Target_nsamp=37, int Template_nline=9, int
        Template_nsamp=9, double Min_ccorr=0.3, double Min_variance=0)

        """
        _ccorr_matcher.CcorrMatcher_swiginit(self,_ccorr_matcher.new_CcorrMatcher(Target_nline, Target_nsamp, Template_nline, Template_nsamp, Min_ccorr, Min_variance))
    def _v_min_correlation(self):
        """
        double GeoCal::CcorrMatcher::min_correlation() const
        Minimum correlation threshold. 
        """
        return _ccorr_matcher.CcorrMatcher__v_min_correlation(self)

    @property
    def min_correlation(self):
        return self._v_min_correlation()

    def _v_min_variance(self):
        """
        double GeoCal::CcorrMatcher::min_variance() const
        Minimum variance threshold. 
        """
        return _ccorr_matcher.CcorrMatcher__v_min_variance(self)

    @property
    def min_variance(self):
        return self._v_min_variance()

    def _v_target_number_line(self):
        """
        int GeoCal::CcorrMatcher::target_number_line() const
        Number of lines in target window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_target_number_line(self)

    @property
    def target_number_line(self):
        return self._v_target_number_line()

    def _v_target_number_sample(self):
        """
        int GeoCal::CcorrMatcher::target_number_sample() const
        Number of samples in target window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_target_number_sample(self)

    @property
    def target_number_sample(self):
        return self._v_target_number_sample()

    def _v_template_number_line(self):
        """
        int GeoCal::CcorrMatcher::template_number_line() const
        Number of lines in template window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_template_number_line(self)

    @property
    def template_number_line(self):
        return self._v_template_number_line()

    def _v_template_number_sample(self):
        """
        int GeoCal::CcorrMatcher::template_number_sample() const
        Number of samples in template window. 
        """
        return _ccorr_matcher.CcorrMatcher__v_template_number_sample(self)

    @property
    def template_number_sample(self):
        return self._v_template_number_sample()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.target_number_line,self.target_number_sample,self.template_number_line,self.template_number_sample,self.min_correlation,self.min_variance)

    __swig_destroy__ = _ccorr_matcher.delete_CcorrMatcher
CcorrMatcher._v_min_correlation = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_min_correlation,None,CcorrMatcher)
CcorrMatcher._v_min_variance = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_min_variance,None,CcorrMatcher)
CcorrMatcher._v_target_number_line = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_target_number_line,None,CcorrMatcher)
CcorrMatcher._v_target_number_sample = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_target_number_sample,None,CcorrMatcher)
CcorrMatcher._v_template_number_line = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_template_number_line,None,CcorrMatcher)
CcorrMatcher._v_template_number_sample = new_instancemethod(_ccorr_matcher.CcorrMatcher__v_template_number_sample,None,CcorrMatcher)
CcorrMatcher_swigregister = _ccorr_matcher.CcorrMatcher_swigregister
CcorrMatcher_swigregister(CcorrMatcher)



