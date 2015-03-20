# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _geocal_datum.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_datum', [dirname(__file__)])
        except ImportError:
            import _geocal_datum
            return _geocal_datum
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_datum', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_datum = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_datum
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


SHARED_PTR_DISOWN = _geocal_datum.SHARED_PTR_DISOWN
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
class Datum(geocal_swig.generic_object.GenericObject):
    """
    This is a Datum, which returns the distance from mean sea level to a
    reference ellipsoid.

    C++ includes: geocal_datum.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self, *args, **kwargs): raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    def undulation(self, *args):
        """
        virtual double GeoCal::Datum::undulation(const Geodetic &Gc) const =0
        Specialization for Gc being Geodetic.

        Since many of our Datums are in geodetic coordinates, this is an
        important specialization for performance. 
        """
        return _geocal_datum.Datum_undulation(self, *args)

    __swig_destroy__ = _geocal_datum.delete_Datum
Datum.undulation = new_instancemethod(_geocal_datum.Datum_undulation,None,Datum)
Datum.__str__ = new_instancemethod(_geocal_datum.Datum___str__,None,Datum)
Datum_swigregister = _geocal_datum.Datum_swigregister
Datum_swigregister(Datum)

class SimpleDatum(Datum):
    """
    This is a Datum which returns a constant undulation everywhere.

    This is useful for testing, and also in the case you are using a Dem
    with height already relative to the reference surface, so no Datum
    correction is needed.

    C++ includes: geocal_datum.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, U=0.0): 
        """
        GeoCal::SimpleDatum::SimpleDatum(double U=0.0)
        Constructor. 
        """
        _geocal_datum.SimpleDatum_swiginit(self,_geocal_datum.new_SimpleDatum(U))
    def _v_u(self):
        """
        double GeoCal::SimpleDatum::u() const
        Fixed value that is returned by this class. 
        """
        return _geocal_datum.SimpleDatum__v_u(self)

    @property
    def u(self):
        return self._v_u()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.u)

    __swig_destroy__ = _geocal_datum.delete_SimpleDatum
SimpleDatum._v_u = new_instancemethod(_geocal_datum.SimpleDatum__v_u,None,SimpleDatum)
SimpleDatum_swigregister = _geocal_datum.SimpleDatum_swigregister
SimpleDatum_swigregister(SimpleDatum)



