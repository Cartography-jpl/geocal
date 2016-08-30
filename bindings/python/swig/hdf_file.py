# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.7
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.





from sys import version_info
if version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _hdf_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_hdf_file', [dirname(__file__)])
        except ImportError:
            import _hdf_file
            return _hdf_file
        if fp is not None:
            try:
                _mod = imp.load_module('_hdf_file', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _hdf_file = swig_import_helper()
    del swig_import_helper
else:
    import _hdf_file
del version_info
try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.


def _swig_setattr_nondynamic(self, class_type, name, value, static=1):
    if (name == "thisown"):
        return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'SwigPyObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name, None)
    if method:
        return method(self, value)
    if (not static):
        object.__setattr__(self, name, value)
    else:
        raise AttributeError("You cannot add attributes to %s" % self)


def _swig_setattr(self, class_type, name, value):
    return _swig_setattr_nondynamic(self, class_type, name, value, 0)


def _swig_getattr_nondynamic(self, class_type, name, static=1):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    if (not static):
        return object.__getattr__(self, name)
    else:
        raise AttributeError(name)

def _swig_getattr(self, class_type, name):
    return _swig_getattr_nondynamic(self, class_type, name, 0)


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

try:
    _object = object
    _newclass = 1
except AttributeError:
    class _object:
        pass
    _newclass = 0



def _swig_setattr_nondynamic_method(set):
    def set_attr(self, name, value):
        if (name == "thisown"):
            return self.this.own(value)
        if hasattr(self, name) or (name == "this"):
            set(self, name, value)
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

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _hdf_file.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_hdf_file.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_hdf_file.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_hdf_file.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_hdf_file.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_hdf_file.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_hdf_file.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_hdf_file.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_hdf_file.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_hdf_file.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_hdf_file.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_hdf_file.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_hdf_file.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_hdf_file.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_hdf_file.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_hdf_file.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_hdf_file.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _hdf_file.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)


_hdf_file.SHARED_PTR_DISOWN_swigconstant(_hdf_file)
SHARED_PTR_DISOWN = _hdf_file.SHARED_PTR_DISOWN

import os

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

def _new_from_serialization_dir(dir, data):
    curdir = os.getcwd()
    try:
      os.chdir(dir)
      return geocal_swig.serialize_read_binary(data)
    finally:
      os.chdir(curdir)


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
class HdfFile(geocal_swig.generic_object.GenericObject):
    """

    This class reads a HDF5 file.

    Note that this is just a thin layer on top of the HDF 5 libraries to
    make the file operations we need to do in GeoCal easier. There are
    many other things that can be done with a HDF 5 than what this class
    exposes.

    Right now, we don't support writing. This is just because I don't
    happen to need this. We can add this if we ever want to write files.

    C++ includes: hdf_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    READ = _hdf_file.HdfFile_READ
    CREATE = _hdf_file.HdfFile_CREATE
    READ_WRITE = _hdf_file.HdfFile_READ_WRITE

    def __init__(self, *args):
        """

        HdfFile::HdfFile(const std::string &Fname, Mode M=READ)
        Open the given file with the given mode. 
        """
        _hdf_file.HdfFile_swiginit(self, _hdf_file.new_HdfFile(*args))

    def close(self):
        """

        void GeoCal::HdfFile::close()
        Close the underlying file.

        This is automatically done by the destructor, so you only need to call
        this if you want to force a close (e.g., for a unit test) 
        """
        return _hdf_file.HdfFile_close(self)


    def _v_file_name(self):
        """

        const std::string& GeoCal::HdfFile::file_name() const
        File name. 
        """
        return _hdf_file.HdfFile__v_file_name(self)


    @property
    def file_name(self):
        return self._v_file_name()


    def is_hdf(Fname):
        """

        static bool GeoCal::HdfFile::is_hdf(const std::string &Fname)
        Return true if the given file is an HDF file. 
        """
        return _hdf_file.HdfFile_is_hdf(Fname)

    is_hdf = staticmethod(is_hdf)
    __swig_destroy__ = _hdf_file.delete_HdfFile
HdfFile.__str__ = new_instancemethod(_hdf_file.HdfFile___str__, None, HdfFile)
HdfFile.close = new_instancemethod(_hdf_file.HdfFile_close, None, HdfFile)
HdfFile._v_file_name = new_instancemethod(_hdf_file.HdfFile__v_file_name, None, HdfFile)
HdfFile.read_double_1d = new_instancemethod(_hdf_file.HdfFile_read_double_1d, None, HdfFile)
HdfFile.read_double_2d = new_instancemethod(_hdf_file.HdfFile_read_double_2d, None, HdfFile)
HdfFile.read_double_3d = new_instancemethod(_hdf_file.HdfFile_read_double_3d, None, HdfFile)
HdfFile.read_double_4d = new_instancemethod(_hdf_file.HdfFile_read_double_4d, None, HdfFile)
HdfFile.read_int_1d = new_instancemethod(_hdf_file.HdfFile_read_int_1d, None, HdfFile)
HdfFile.read_int_2d = new_instancemethod(_hdf_file.HdfFile_read_int_2d, None, HdfFile)
HdfFile.read_int_3d = new_instancemethod(_hdf_file.HdfFile_read_int_3d, None, HdfFile)
HdfFile.read_int_4d = new_instancemethod(_hdf_file.HdfFile_read_int_4d, None, HdfFile)
HdfFile_swigregister = _hdf_file.HdfFile_swigregister
HdfFile_swigregister(HdfFile)

def HdfFile_is_hdf(Fname):
    """

    static bool GeoCal::HdfFile::is_hdf(const std::string &Fname)
    Return true if the given file is an HDF file. 
    """
    return _hdf_file.HdfFile_is_hdf(Fname)



