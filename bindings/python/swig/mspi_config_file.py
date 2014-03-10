# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _mspi_config_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_mspi_config_file', [dirname(__file__)])
        except ImportError:
            import _mspi_config_file
            return _mspi_config_file
        if fp is not None:
            try:
                _mod = imp.load_module('_mspi_config_file', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _mspi_config_file = swig_import_helper()
    del swig_import_helper
else:
    import _mspi_config_file
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


SHARED_PTR_DISOWN = _mspi_config_file.SHARED_PTR_DISOWN
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
class MspiConfigFile(geocal_swig.generic_object.GenericObject):
    """
    MSPI uses ASCII configuration files.

    The files can contain comments, which start with a "#" and go to the
    end of the line.

    Everything else should be in a form like: value_1 value_2 ... :
    keyword_name

    It is perfectly ok for there to be multiple lines before the variable
    name, so something like:

    value_1 value_2 value_3 value_4 : keyword_name

    Anything at the end of the file is ignored.

    C++ includes: mspi_config_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        MspiConfigFile::MspiConfigFile(const std::string &Fname)
        Read the given configuration file.

        See the class comments for the details of the format of this string.

        """
        _mspi_config_file.MspiConfigFile_swiginit(self,_mspi_config_file.new_MspiConfigFile(*args))
    def _v_file_name(self):
        """
        const std::string& GeoCal::MspiConfigFile::file_name() const
        File name for MspiConfigFile. 
        """
        return _mspi_config_file.MspiConfigFile__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def have_key(self, *args):
        """
        bool GeoCal::MspiConfigFile::have_key(const std::string &K) const
        True if we have a value for the given keyword. 
        """
        return _mspi_config_file.MspiConfigFile_have_key(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name)

    __swig_destroy__ = _mspi_config_file.delete_MspiConfigFile
MspiConfigFile._v_file_name = new_instancemethod(_mspi_config_file.MspiConfigFile__v_file_name,None,MspiConfigFile)
MspiConfigFile.have_key = new_instancemethod(_mspi_config_file.MspiConfigFile_have_key,None,MspiConfigFile)
MspiConfigFile.__str__ = new_instancemethod(_mspi_config_file.MspiConfigFile___str__,None,MspiConfigFile)
MspiConfigFile_swigregister = _mspi_config_file.MspiConfigFile_swigregister
MspiConfigFile_swigregister(MspiConfigFile)



