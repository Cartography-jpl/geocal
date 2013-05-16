# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _spice_helper.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_spice_helper', [dirname(__file__)])
        except ImportError:
            import _spice_helper
            return _spice_helper
        if fp is not None:
            try:
                _mod = imp.load_module('_spice_helper', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _spice_helper = swig_import_helper()
    del swig_import_helper
else:
    import _spice_helper
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


SHARED_PTR_DISOWN = _spice_helper.SHARED_PTR_DISOWN
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

class SpiceHelper(object):
    """
    This class centralizes all of the CSPICE calls from the CSPICE library
    (available from NAIF athttp://naif.jpl.nasa.gov/naif/pds.html).

    This class handles initializing CSPICE, and provides one place to
    modify if we ever need to replace the dependency on this library.

    SPICE depends on various kernels. We load the file found at
    $SPICEDATA/geocal.ker. This file in turn loads a number of other
    kernels, you can edit this file to change which kernels are used.

    C++ includes: spice_helper.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def spice_available():
        """
        bool SpiceHelper::spice_available()
        Return true if we have SPICE functionality available, false otherwise.

        """
        return _spice_helper.SpiceHelper_spice_available()

    spice_available = staticmethod(spice_available)
    def spice_setup(Kernel="geocal.ker"):
        """
        void SpiceHelper::spice_setup(const std::string &Kernel="geocal.ker")
        Set SPICE errors to just return, rather than aborting. 
        """
        return _spice_helper.SpiceHelper_spice_setup(Kernel)

    spice_setup = staticmethod(spice_setup)
    def __init__(self): 
        _spice_helper.SpiceHelper_swiginit(self,_spice_helper.new_SpiceHelper())
    __swig_destroy__ = _spice_helper.delete_SpiceHelper
SpiceHelper_swigregister = _spice_helper.SpiceHelper_swigregister
SpiceHelper_swigregister(SpiceHelper)

def SpiceHelper_spice_available():
  """
    bool SpiceHelper::spice_available()
    Return true if we have SPICE functionality available, false otherwise.

    """
  return _spice_helper.SpiceHelper_spice_available()

def SpiceHelper_spice_setup(Kernel="geocal.ker"):
  """
    void SpiceHelper::spice_setup(const std::string &Kernel="geocal.ker")
    Set SPICE errors to just return, rather than aborting. 
    """
  return _spice_helper.SpiceHelper_spice_setup(Kernel)



