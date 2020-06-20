# This file was automatically generated by SWIG (http://www.swig.org).
# Version 3.0.12
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.

from sys import version_info as _swig_python_version_info
if _swig_python_version_info >= (3, 0, 0):
    new_instancemethod = lambda func, inst, cls: _mspi_config_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if _swig_python_version_info >= (2, 7, 0):
    def swig_import_helper():
        import importlib
        pkg = __name__.rpartition('.')[0]
        mname = '.'.join((pkg, '_mspi_config_file')).lstrip('.')
        try:
            return importlib.import_module(mname)
        except ImportError:
            return importlib.import_module('_mspi_config_file')
    _mspi_config_file = swig_import_helper()
    del swig_import_helper
elif _swig_python_version_info >= (2, 6, 0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_mspi_config_file', [dirname(__file__)])
        except ImportError:
            import _mspi_config_file
            return _mspi_config_file
        try:
            _mod = imp.load_module('_mspi_config_file', fp, pathname, description)
        finally:
            if fp is not None:
                fp.close()
        return _mod
    _mspi_config_file = swig_import_helper()
    del swig_import_helper
else:
    import _mspi_config_file
del _swig_python_version_info

try:
    _swig_property = property
except NameError:
    pass  # Python < 2.2 doesn't have 'property'.

try:
    import builtins as __builtin__
except ImportError:
    import __builtin__

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


def _swig_getattr(self, class_type, name):
    if (name == "thisown"):
        return self.this.own()
    method = class_type.__swig_getmethods__.get(name, None)
    if method:
        return method(self)
    raise AttributeError("'%s' object has no attribute '%s'" % (class_type.__name__, name))


def _swig_repr(self):
    try:
        strthis = "proxy of " + self.this.__repr__()
    except __builtin__.Exception:
        strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)


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
except __builtin__.Exception:
    weakref_proxy = lambda x: x


class SwigPyIterator(object):
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')

    def __init__(self, *args, **kwargs):
        raise AttributeError("No constructor defined - class is abstract")
    __repr__ = _swig_repr
    __swig_destroy__ = _mspi_config_file.delete_SwigPyIterator
    def __iter__(self):
        return self
SwigPyIterator.value = new_instancemethod(_mspi_config_file.SwigPyIterator_value, None, SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_mspi_config_file.SwigPyIterator_incr, None, SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_mspi_config_file.SwigPyIterator_decr, None, SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_mspi_config_file.SwigPyIterator_distance, None, SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_mspi_config_file.SwigPyIterator_equal, None, SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_mspi_config_file.SwigPyIterator_copy, None, SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_mspi_config_file.SwigPyIterator_next, None, SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_mspi_config_file.SwigPyIterator___next__, None, SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_mspi_config_file.SwigPyIterator_previous, None, SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_mspi_config_file.SwigPyIterator_advance, None, SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_mspi_config_file.SwigPyIterator___eq__, None, SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_mspi_config_file.SwigPyIterator___ne__, None, SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_mspi_config_file.SwigPyIterator___iadd__, None, SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_mspi_config_file.SwigPyIterator___isub__, None, SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_mspi_config_file.SwigPyIterator___add__, None, SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_mspi_config_file.SwigPyIterator___sub__, None, SwigPyIterator)
SwigPyIterator_swigregister = _mspi_config_file.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _mspi_config_file.SHARED_PTR_DISOWN

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

    def __init__(self, Fname):
        """

        MspiConfigFile::MspiConfigFile(const std::string &Fname)
        Read the given configuration file.

        See the class comments for the details of the format of this string.

        """
        _mspi_config_file.MspiConfigFile_swiginit(self, _mspi_config_file.new_MspiConfigFile(Fname))

    def add_file(self, Fname):
        """

        void MspiConfigFile::add_file(const std::string &Fname)
        Add a file, replacing any keywords already in this class from earlier
        files. 
        """
        return _mspi_config_file.MspiConfigFile_add_file(self, Fname)


    def add(self, Keyword, Value):
        """

        void GeoCal::MspiConfigFile::add(const std::string &Keyword, const std::string &Value)
        Add or replace a value. 
        """
        return _mspi_config_file.MspiConfigFile_add(self, Keyword, Value)


    def have_key(self, Key):
        """

        bool GeoCal::MspiConfigFile::have_key(const std::string &K) const
        True if we have a value for the given keyword. 
        """
        return _mspi_config_file.MspiConfigFile_have_key(self, Key)


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _mspi_config_file.delete_MspiConfigFile
MspiConfigFile.add_file = new_instancemethod(_mspi_config_file.MspiConfigFile_add_file, None, MspiConfigFile)
MspiConfigFile.add = new_instancemethod(_mspi_config_file.MspiConfigFile_add, None, MspiConfigFile)
MspiConfigFile.have_key = new_instancemethod(_mspi_config_file.MspiConfigFile_have_key, None, MspiConfigFile)
MspiConfigFile.value_double = new_instancemethod(_mspi_config_file.MspiConfigFile_value_double, None, MspiConfigFile)
MspiConfigFile.value_vector_double = new_instancemethod(_mspi_config_file.MspiConfigFile_value_vector_double, None, MspiConfigFile)
MspiConfigFile.value_int = new_instancemethod(_mspi_config_file.MspiConfigFile_value_int, None, MspiConfigFile)
MspiConfigFile.value_vector_int = new_instancemethod(_mspi_config_file.MspiConfigFile_value_vector_int, None, MspiConfigFile)
MspiConfigFile.value_string = new_instancemethod(_mspi_config_file.MspiConfigFile_value_string, None, MspiConfigFile)
MspiConfigFile.value_vector_string = new_instancemethod(_mspi_config_file.MspiConfigFile_value_vector_string, None, MspiConfigFile)
MspiConfigFile.value_bool = new_instancemethod(_mspi_config_file.MspiConfigFile_value_bool, None, MspiConfigFile)
MspiConfigFile.value_vector_bool = new_instancemethod(_mspi_config_file.MspiConfigFile_value_vector_bool, None, MspiConfigFile)
MspiConfigFile.__str__ = new_instancemethod(_mspi_config_file.MspiConfigFile___str__, None, MspiConfigFile)
MspiConfigFile_swigregister = _mspi_config_file.MspiConfigFile_swigregister
MspiConfigFile_swigregister(MspiConfigFile)

class MspiConfigTable(geocal_swig.generic_object.GenericObject):
    """

    Small adapter that lets you refer to a table by column name.

    The convention is that the table "FOO" has a config entry of
    FOO.columns giving the column names, and FOO.table giving the actual
    values. We look of a value by doing "value<Type>(index,
    column_name)".

    C++ includes: mspi_config_file.h 
    """

    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr

    def __init__(self, Config, Table_name):
        """

        MspiConfigTable::MspiConfigTable(const MspiConfigFile &Config, const std::string &Table_name)
        Create a MspiConfigTable for the given table name in the given file.

        """
        _mspi_config_file.MspiConfigTable_swiginit(self, _mspi_config_file.new_MspiConfigTable(Config, Table_name))

    def _v_number_row(self):
        """

        int GeoCal::MspiConfigTable::number_row() const

        """
        return _mspi_config_file.MspiConfigTable__v_number_row(self)


    @property
    def number_row(self):
        return self._v_number_row()


    def __reduce__(self):
      return _new_from_serialization, (geocal_swig.serialize_write_binary(self),)

    __swig_destroy__ = _mspi_config_file.delete_MspiConfigTable
MspiConfigTable._v_number_row = new_instancemethod(_mspi_config_file.MspiConfigTable__v_number_row, None, MspiConfigTable)
MspiConfigTable.value_double = new_instancemethod(_mspi_config_file.MspiConfigTable_value_double, None, MspiConfigTable)
MspiConfigTable.value_int = new_instancemethod(_mspi_config_file.MspiConfigTable_value_int, None, MspiConfigTable)
MspiConfigTable.value_string = new_instancemethod(_mspi_config_file.MspiConfigTable_value_string, None, MspiConfigTable)
MspiConfigTable.__str__ = new_instancemethod(_mspi_config_file.MspiConfigTable___str__, None, MspiConfigTable)
MspiConfigTable_swigregister = _mspi_config_file.MspiConfigTable_swigregister
MspiConfigTable_swigregister(MspiConfigTable)


__all__ = ["MspiConfigFile","MspiConfigTable"]



