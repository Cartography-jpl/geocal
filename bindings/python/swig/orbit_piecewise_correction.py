# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _orbit_piecewise_correction.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_orbit_piecewise_correction', [dirname(__file__)])
        except ImportError:
            import _orbit_piecewise_correction
            return _orbit_piecewise_correction
        if fp is not None:
            try:
                _mod = imp.load_module('_orbit_piecewise_correction', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _orbit_piecewise_correction = swig_import_helper()
    del swig_import_helper
else:
    import _orbit_piecewise_correction
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
    __swig_destroy__ = _orbit_piecewise_correction.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_orbit_piecewise_correction.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _orbit_piecewise_correction.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _orbit_piecewise_correction.SHARED_PTR_DISOWN
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

import geocal_swig.orbit_correction
import geocal_swig.orbit
import geocal_swig.generic_object
import geocal_swig.observer
import geocal_swig.with_parameter
class OrbitPiecewiseCorrection(geocal_swig.orbit_correction.OrbitCorrection):
    """
    This class gives an orbit that tries to correct errors in another
    underlying orbit.

    The correction is a piecewise correction to the position, in local ENU
    coordinate system.

    The underlying orbit should return a QuaternionOrbitData orbit data,
    since this is currently the only type supported.

    C++ includes: orbit_piecewise_correction.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::OrbitPiecewiseCorrection::OrbitPiecewiseCorrection(const boost::shared_ptr< Orbit > Orb_uncorr, const PiecewiseLinear
        &E_corr, const PiecewiseLinear &N_corr, const PiecewiseLinear &U_corr)

        """
        _orbit_piecewise_correction.OrbitPiecewiseCorrection_swiginit(self,_orbit_piecewise_correction.new_OrbitPiecewiseCorrection(*args))
    def _v_e_corr(self):
        """
        const PiecewiseLinear& GeoCal::OrbitPiecewiseCorrection::e_corr() const
        Correction in local east direction. 
        """
        return _orbit_piecewise_correction.OrbitPiecewiseCorrection__v_e_corr(self)

    @property
    def e_corr(self):
        return self._v_e_corr()

    def _v_n_corr(self):
        """
        const PiecewiseLinear& GeoCal::OrbitPiecewiseCorrection::n_corr() const
        Correction in local north direction. 
        """
        return _orbit_piecewise_correction.OrbitPiecewiseCorrection__v_n_corr(self)

    @property
    def n_corr(self):
        return self._v_n_corr()

    def _v_u_corr(self):
        """
        const PiecewiseLinear& GeoCal::OrbitPiecewiseCorrection::u_corr() const
        Correction in local up direction. 
        """
        return _orbit_piecewise_correction.OrbitPiecewiseCorrection__v_u_corr(self)

    @property
    def u_corr(self):
        return self._v_u_corr()

    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _orbit_piecewise_correction.delete_OrbitPiecewiseCorrection
OrbitPiecewiseCorrection._v_e_corr = new_instancemethod(_orbit_piecewise_correction.OrbitPiecewiseCorrection__v_e_corr,None,OrbitPiecewiseCorrection)
OrbitPiecewiseCorrection._v_n_corr = new_instancemethod(_orbit_piecewise_correction.OrbitPiecewiseCorrection__v_n_corr,None,OrbitPiecewiseCorrection)
OrbitPiecewiseCorrection._v_u_corr = new_instancemethod(_orbit_piecewise_correction.OrbitPiecewiseCorrection__v_u_corr,None,OrbitPiecewiseCorrection)
OrbitPiecewiseCorrection_swigregister = _orbit_piecewise_correction.OrbitPiecewiseCorrection_swigregister
OrbitPiecewiseCorrection_swigregister(OrbitPiecewiseCorrection)



