# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _statistic.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_statistic', [dirname(__file__)])
        except ImportError:
            import _statistic
            return _statistic
        if fp is not None:
            try:
                _mod = imp.load_module('_statistic', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _statistic = swig_import_helper()
    del swig_import_helper
else:
    import _statistic
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


SHARED_PTR_DISOWN = _statistic.SHARED_PTR_DISOWN
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
class Statistic(geocal_swig.generic_object.GenericObject):
    """
    This calculates basic statistics.

    C++ includes: statistic.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self): 
        """
        GeoCal::Statistic::Statistic()
        Constructor. 
        """
        _statistic.Statistic_swiginit(self,_statistic.new_Statistic())
    def add(self, *args):
        """
        void Statistic::add(double x)
        Add a new data point. 
        """
        return _statistic.Statistic_add(self, *args)

    def _v_min(self):
        """
        double GeoCal::Statistic::min() const
        Minimum value encountered. 
        """
        return _statistic.Statistic__v_min(self)

    @property
    def min(self):
        return self._v_min()

    def _v_max(self):
        """
        double GeoCal::Statistic::max() const
        Maximum value encountered. 
        """
        return _statistic.Statistic__v_max(self)

    @property
    def max(self):
        return self._v_max()

    def _v_mean(self):
        """
        double GeoCal::Statistic::mean() const
        Mean value. 
        """
        return _statistic.Statistic__v_mean(self)

    @property
    def mean(self):
        return self._v_mean()

    def _v_sigma(self):
        """
        double GeoCal::Statistic::sigma() const
        Standard deviation. 
        """
        return _statistic.Statistic__v_sigma(self)

    @property
    def sigma(self):
        return self._v_sigma()

    def _v_mean_abs(self):
        """
        double GeoCal::Statistic::mean_abs() const
        Mean of the absolute value. 
        """
        return _statistic.Statistic__v_mean_abs(self)

    @property
    def mean_abs(self):
        return self._v_mean_abs()

    def _v_min_index(self):
        """
        int GeoCal::Statistic::min_index() const
        Index number of minimum point, where the first point added is index 0,
        second is 1 and so on. 
        """
        return _statistic.Statistic__v_min_index(self)

    @property
    def min_index(self):
        return self._v_min_index()

    def _v_max_index(self):
        """
        int GeoCal::Statistic::max_index() const
        Index number of maximum point, where the first point added is index 0,
        second is 1 and so on. 
        """
        return _statistic.Statistic__v_max_index(self)

    @property
    def max_index(self):
        return self._v_max_index()

    def _v_count(self):
        """
        int GeoCal::Statistic::count() const
        Number of points added. 
        """
        return _statistic.Statistic__v_count(self)

    @property
    def count(self):
        return self._v_count()

    __swig_destroy__ = _statistic.delete_Statistic
Statistic.add = new_instancemethod(_statistic.Statistic_add,None,Statistic)
Statistic.__iadd__ = new_instancemethod(_statistic.Statistic___iadd__,None,Statistic)
Statistic._v_min = new_instancemethod(_statistic.Statistic__v_min,None,Statistic)
Statistic._v_max = new_instancemethod(_statistic.Statistic__v_max,None,Statistic)
Statistic._v_mean = new_instancemethod(_statistic.Statistic__v_mean,None,Statistic)
Statistic._v_sigma = new_instancemethod(_statistic.Statistic__v_sigma,None,Statistic)
Statistic._v_mean_abs = new_instancemethod(_statistic.Statistic__v_mean_abs,None,Statistic)
Statistic._v_min_index = new_instancemethod(_statistic.Statistic__v_min_index,None,Statistic)
Statistic._v_max_index = new_instancemethod(_statistic.Statistic__v_max_index,None,Statistic)
Statistic._v_count = new_instancemethod(_statistic.Statistic__v_count,None,Statistic)
Statistic.__str__ = new_instancemethod(_statistic.Statistic___str__,None,Statistic)
Statistic_swigregister = _statistic.Statistic_swigregister
Statistic_swigregister(Statistic)



