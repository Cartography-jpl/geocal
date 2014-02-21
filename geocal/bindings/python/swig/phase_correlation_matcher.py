# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _phase_correlation_matcher.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_phase_correlation_matcher', [dirname(__file__)])
        except ImportError:
            import _phase_correlation_matcher
            return _phase_correlation_matcher
        if fp is not None:
            try:
                _mod = imp.load_module('_phase_correlation_matcher', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _phase_correlation_matcher = swig_import_helper()
    del swig_import_helper
else:
    import _phase_correlation_matcher
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


SHARED_PTR_DISOWN = _phase_correlation_matcher.SHARED_PTR_DISOWN
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
class PhaseCorrelationMatcher(geocal.image_matcher.ImageMatcher):
    """
    This class performs image matching.

    It uses phase correlation. This is an adaption of Al Zobrist's
    picmtch4 AFIDS procedure.

    That program tries to match a number of points as one executable call.
    This includes both the matching, and the update of the predictor for
    the matching. We have extracted out just the image matching, we have
    other functionality for handling the predictor.

    The original code was based on the paper: C. D. Kuglin and D. C.
    Hines, "The phase correlation     image alignment method," Proc.
    IEEE 1975 International Conference on Cybernetics and Society,
    September, 1975, pp. 163-165.

    C++ includes: phase_correlation_matcher.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, Template_size=32, Search_size=32): 
        """
        PhaseCorrelationMatcher::PhaseCorrelationMatcher(int Template_size=32, int Search_size=32)
        Constructor.

        Parameters:
        -----------

        Template_size:  Size of the area that we do use in the first image.
        This was called FFTSIZE in Al Zobrist's picmtch4 program. Note that
        this must be even.

        Search_size:  Size to search in the second image. This should be >
        Template_size, and even. 
        """
        _phase_correlation_matcher.PhaseCorrelationMatcher_swiginit(self,_phase_correlation_matcher.new_PhaseCorrelationMatcher(Template_size, Search_size))
    def _v_template_size(self):
        """
        int GeoCal::PhaseCorrelationMatcher::template_size() const
        TODO Go through and change the variable names used here.

        But for now, match Al's code Size of the area that we do use in the
        first image. This was called FFTSIZE in Al Zobrist's picmtch4 program.
        Note that this must be a power of 2. There is a note in picmtch4 that
        this should be at least 32 and a power of 2, but this isn't actually
        required by the code, just that it is even. 
        """
        return _phase_correlation_matcher.PhaseCorrelationMatcher__v_template_size(self)

    @property
    def template_size(self):
        return self._v_template_size()

    def _v_search_size(self):
        """
        int GeoCal::PhaseCorrelationMatcher::search_size() const
        The search size. This should be >= template_size(). 
        """
        return _phase_correlation_matcher.PhaseCorrelationMatcher__v_search_size(self)

    @property
    def search_size(self):
        return self._v_search_size()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.template_size,self.search_size)

    __swig_destroy__ = _phase_correlation_matcher.delete_PhaseCorrelationMatcher
PhaseCorrelationMatcher._v_template_size = new_instancemethod(_phase_correlation_matcher.PhaseCorrelationMatcher__v_template_size,None,PhaseCorrelationMatcher)
PhaseCorrelationMatcher._v_search_size = new_instancemethod(_phase_correlation_matcher.PhaseCorrelationMatcher__v_search_size,None,PhaseCorrelationMatcher)
PhaseCorrelationMatcher_swigregister = _phase_correlation_matcher.PhaseCorrelationMatcher_swigregister
PhaseCorrelationMatcher_swigregister(PhaseCorrelationMatcher)


