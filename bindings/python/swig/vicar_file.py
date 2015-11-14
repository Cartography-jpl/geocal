# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _vicar_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_file', [dirname(__file__)])
        except ImportError:
            import _vicar_file
            return _vicar_file
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_file', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_file = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_file
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
    __swig_destroy__ = _vicar_file.delete_SwigPyIterator
    def __iter__(self): return self
SwigPyIterator.value = new_instancemethod(_vicar_file.SwigPyIterator_value,None,SwigPyIterator)
SwigPyIterator.incr = new_instancemethod(_vicar_file.SwigPyIterator_incr,None,SwigPyIterator)
SwigPyIterator.decr = new_instancemethod(_vicar_file.SwigPyIterator_decr,None,SwigPyIterator)
SwigPyIterator.distance = new_instancemethod(_vicar_file.SwigPyIterator_distance,None,SwigPyIterator)
SwigPyIterator.equal = new_instancemethod(_vicar_file.SwigPyIterator_equal,None,SwigPyIterator)
SwigPyIterator.copy = new_instancemethod(_vicar_file.SwigPyIterator_copy,None,SwigPyIterator)
SwigPyIterator.next = new_instancemethod(_vicar_file.SwigPyIterator_next,None,SwigPyIterator)
SwigPyIterator.__next__ = new_instancemethod(_vicar_file.SwigPyIterator___next__,None,SwigPyIterator)
SwigPyIterator.previous = new_instancemethod(_vicar_file.SwigPyIterator_previous,None,SwigPyIterator)
SwigPyIterator.advance = new_instancemethod(_vicar_file.SwigPyIterator_advance,None,SwigPyIterator)
SwigPyIterator.__eq__ = new_instancemethod(_vicar_file.SwigPyIterator___eq__,None,SwigPyIterator)
SwigPyIterator.__ne__ = new_instancemethod(_vicar_file.SwigPyIterator___ne__,None,SwigPyIterator)
SwigPyIterator.__iadd__ = new_instancemethod(_vicar_file.SwigPyIterator___iadd__,None,SwigPyIterator)
SwigPyIterator.__isub__ = new_instancemethod(_vicar_file.SwigPyIterator___isub__,None,SwigPyIterator)
SwigPyIterator.__add__ = new_instancemethod(_vicar_file.SwigPyIterator___add__,None,SwigPyIterator)
SwigPyIterator.__sub__ = new_instancemethod(_vicar_file.SwigPyIterator___sub__,None,SwigPyIterator)
SwigPyIterator_swigregister = _vicar_file.SwigPyIterator_swigregister
SwigPyIterator_swigregister(SwigPyIterator)

SHARED_PTR_DISOWN = _vicar_file.SHARED_PTR_DISOWN
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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class VicarFile(geocal_swig.generic_object.GenericObject):
    """
    This handles opening and closing a Vicar file.

    This class doesn't actually read and write data, but is used by other
    classes which do.

    This uses the VICAR RTL, which is documented athttp://www-
    mipl.jpl.nasa.gov/RTL

    If the VICAR RTL library wasn't available when this geocal was built,
    then this class will just throw an exception when we try to do
    anything with a VICAR file.

    As an implementation detail, the class VicarFile and IbisFile need to
    coordinate in translating file names to unit numbers since the same
    underlying Vicar routine zvunit is used by both. We arbitrarily have
    VicarFile handle this, and IbisFile accesses VicarFile when it needs
    to do this.

    Also a note for developers. The Vicar RTL library uses various
    variable argument functions. You need to end the list of arguments
    with "NULL". Note that a cast to a point type is mandatory, you
    can't just say "0". If say "0" then you will get fairly difficult
    to track down errors. Without the cast, this gets passed as an int,
    which the RTL library code will then try to interpret as a char *.
    va_args has undefined behavior when called with the wrong type,
    ranging from seeming to work to core dumping.

    Because of the complication involved, we have separated out the
    functionality of reading and writing MapInfo metadata. This is done by
    the class VicarOgr. This is handled behind the scenes, so for a user
    of this class this separation makes no difference. But if you are
    looking for the code that does this, you'll need to look in VicarOgr.

    The current implementation of the MapInfo metadata requires the GDAL
    library to be available. If this isn't you can still build the GeoCal
    system but any attempt to read or write MapInfo metadata will trigger
    an exception.

    C++ includes: vicar_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    VICAR_INT = _vicar_file.VicarFile_VICAR_INT
    VICAR_REAL = _vicar_file.VicarFile_VICAR_REAL
    VICAR_STRING = _vicar_file.VicarFile_VICAR_STRING
    VICAR_BYTE = _vicar_file.VicarFile_VICAR_BYTE
    VICAR_HALF = _vicar_file.VicarFile_VICAR_HALF
    VICAR_FULL = _vicar_file.VicarFile_VICAR_FULL
    VICAR_FLOAT = _vicar_file.VicarFile_VICAR_FLOAT
    VICAR_DOUBLE = _vicar_file.VicarFile_VICAR_DOUBLE
    READ = _vicar_file.VicarFile_READ
    WRITE = _vicar_file.VicarFile_WRITE
    UPDATE = _vicar_file.VicarFile_UPDATE
    NONE = _vicar_file.VicarFile_NONE
    BASIC = _vicar_file.VicarFile_BASIC
    BASIC2 = _vicar_file.VicarFile_BASIC2
    def __init__(self, *args): 
        """
        VicarFile::VicarFile(int Instance, int Number_line, int Number_sample, int Number_band=1,
        const std::string &Type="BYTE", const std::string &Name="OUT",
        const std::string &Org="BSQ", compression C=NONE)
        Create a new VICAR file with the given size.

        Use the VICAR Name and Instance input (so for example, "INP" and 2
        is the second INP file passed to a VICAR program. 
        """
        _vicar_file.VicarFile_swiginit(self,_vicar_file.new_VicarFile(*args))
    __swig_destroy__ = _vicar_file.delete_VicarFile
    def _v_access(self):
        """
        access_type GeoCal::VicarFile::access() const
        Access type of file. 
        """
        return _vicar_file.VicarFile__v_access(self)

    @property
    def access(self):
        return self._v_access()

    def _v_force_area_pixel(self):
        """
        bool GeoCal::VicarFile::force_area_pixel() const
        If true, then force the file to be treated as "pixel is area".

        This is really just meant as a work around for the SRTM data, which
        incorrectly labels the data as "point" rather than "area". Since
        this is a 15 meter difference, it matters for many applications. Most
        users should just ignore this value. 
        """
        return _vicar_file.VicarFile__v_force_area_pixel(self)

    @property
    def force_area_pixel(self):
        return self._v_force_area_pixel()

    def _v_file_name(self):
        """
        const std::string& GeoCal::VicarFile::file_name() const
        File name. 
        """
        return _vicar_file.VicarFile__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def is_vicar_file(*args):
        """
        bool VicarFile::is_vicar_file(const std::string &Fname)
        Determine if a given file is a VICAR file or not.

        This looks for the first few characters of the file being "LBLSIZE="

        """
        return _vicar_file.VicarFile_is_vicar_file(*args)

    is_vicar_file = staticmethod(is_vicar_file)
    def file_name_to_unit(*args):
        """
        int VicarFile::file_name_to_unit(const std::string &Fname)
        Return a VICAR unit number that is tied to the given file name. 
        """
        return _vicar_file.VicarFile_file_name_to_unit(*args)

    file_name_to_unit = staticmethod(file_name_to_unit)
    def _v_number_band(self):
        """
        int GeoCal::VicarFile::number_band() const
        Number of bands in file. 
        """
        return _vicar_file.VicarFile__v_number_band(self)

    @property
    def number_band(self):
        return self._v_number_band()

    def _v_number_line(self):
        """
        int GeoCal::VicarFile::number_line() const
        Number of lines in file. 
        """
        return _vicar_file.VicarFile__v_number_line(self)

    @property
    def number_line(self):
        return self._v_number_line()

    def _v_number_sample(self):
        """
        int GeoCal::VicarFile::number_sample() const
        Number of samples in file. 
        """
        return _vicar_file.VicarFile__v_number_sample(self)

    @property
    def number_sample(self):
        return self._v_number_sample()

    def _v_type(self):
        """
        data_type GeoCal::VicarFile::type() const
        Type of data in file. 
        """
        return _vicar_file.VicarFile__v_type(self)

    @property
    def type(self):
        return self._v_type()

    def _v_unit(self):
        """
        int GeoCal::VicarFile::unit() const
        Unit number for VicarFile. 
        """
        return _vicar_file.VicarFile__v_unit(self)

    @property
    def unit(self):
        return self._v_unit()

    def _v_map_info(self, *args):
        """
        void VicarFile::map_info(const MapInfo &M)
        Set metadata for MapInfo. 
        """
        return _vicar_file.VicarFile__v_map_info(self, *args)

    @property
    def map_info(self):
        return self._v_map_info()

    @map_info.setter
    def map_info(self, value):
      self._v_map_info(value)

    def _v_rpc(self, *args):
        """
        void VicarFile::rpc(const Rpc &R)
        Set metadata for Rpc. 
        """
        return _vicar_file.VicarFile__v_rpc(self, *args)

    @property
    def rpc(self):
        return self._v_rpc()

    @rpc.setter
    def rpc(self, value):
      self._v_rpc(value)

    def close(self):
        """
        void VicarFile::close()
        Close file. 
        """
        return _vicar_file.VicarFile_close(self)

    def flush(self):
        """
        void GeoCal::VicarFile::flush() const
        Flush data to disk. 
        """
        return _vicar_file.VicarFile_flush(self)

    def label_delete(self, *args):
        """
        void VicarFile::label_delete(const std::string &F, const std::string &Property="")
        Delete a label from a file. 
        """
        return _vicar_file.VicarFile_label_delete(self, *args)

    def label_set(self, *args):
        """
        void VicarFile::label_set(const std::string &F, const std::string &Val, const std::string
        &Property="")
        Set the value of a label.

        If the label is already in the file, is is deleted and replaced with
        this new value. Otherwise, it is simply added. Optionally the label
        can be part of a Property. 
        """
        return _vicar_file.VicarFile_label_set(self, *args)

    def label_string(self, *args):
        """
        std::string GeoCal::VicarFile::label_string(const std::string &F, const std::string &Property="") const
        Non template form of label, useful in some contexts. 
        """
        return _vicar_file.VicarFile_label_string(self, *args)

    def __getitem__(self, key):
        if(isinstance(key, list) or isinstance(key, tuple)):
          prop, ky = key
          tp = self.ltype(prop + " " + ky)
          if(tp == VicarFile.VICAR_INT):
             return self.label_int(ky, prop)
          elif(tp == VicarFile.VICAR_REAL):
             return self.label_float(ky, prop)
          else:
             return self.label_string(ky, prop)
        else:
          tp = self.ltype(key)
          if(tp == VicarFile.VICAR_INT):
             return self.label_int(key)
          elif(tp == VicarFile.VICAR_REAL):
             return self.label_float(key)
          else:
             return self.label_string(key)

    def __setitem__(self, key, v):
        if(isinstance(key, list) or isinstance(key, tuple)):
          prop, ky = key
          self.label_set(ky, v, prop)
        else:
          self.label_set(key, v)
      
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.access,self.force_area_pixel)

VicarFile._v_access = new_instancemethod(_vicar_file.VicarFile__v_access,None,VicarFile)
VicarFile._v_force_area_pixel = new_instancemethod(_vicar_file.VicarFile__v_force_area_pixel,None,VicarFile)
VicarFile._v_file_name = new_instancemethod(_vicar_file.VicarFile__v_file_name,None,VicarFile)
VicarFile._v_number_band = new_instancemethod(_vicar_file.VicarFile__v_number_band,None,VicarFile)
VicarFile._v_number_line = new_instancemethod(_vicar_file.VicarFile__v_number_line,None,VicarFile)
VicarFile._v_number_sample = new_instancemethod(_vicar_file.VicarFile__v_number_sample,None,VicarFile)
VicarFile._v_type = new_instancemethod(_vicar_file.VicarFile__v_type,None,VicarFile)
VicarFile._v_unit = new_instancemethod(_vicar_file.VicarFile__v_unit,None,VicarFile)
VicarFile._v_map_info = new_instancemethod(_vicar_file.VicarFile__v_map_info,None,VicarFile)
VicarFile._v_rpc = new_instancemethod(_vicar_file.VicarFile__v_rpc,None,VicarFile)
VicarFile.close = new_instancemethod(_vicar_file.VicarFile_close,None,VicarFile)
VicarFile.flush = new_instancemethod(_vicar_file.VicarFile_flush,None,VicarFile)
VicarFile.__str__ = new_instancemethod(_vicar_file.VicarFile___str__,None,VicarFile)
VicarFile.label_delete = new_instancemethod(_vicar_file.VicarFile_label_delete,None,VicarFile)
VicarFile.label_set = new_instancemethod(_vicar_file.VicarFile_label_set,None,VicarFile)
VicarFile.ltype = new_instancemethod(_vicar_file.VicarFile_ltype,None,VicarFile)
VicarFile.label_int = new_instancemethod(_vicar_file.VicarFile_label_int,None,VicarFile)
VicarFile.label_float = new_instancemethod(_vicar_file.VicarFile_label_float,None,VicarFile)
VicarFile.label_string = new_instancemethod(_vicar_file.VicarFile_label_string,None,VicarFile)
VicarFile.label_list = new_instancemethod(_vicar_file.VicarFile_label_list,None,VicarFile)
VicarFile.property_list = new_instancemethod(_vicar_file.VicarFile_property_list,None,VicarFile)
VicarFile_swigregister = _vicar_file.VicarFile_swigregister
VicarFile_swigregister(VicarFile)

def VicarFile_is_vicar_file(*args):
  """
    bool VicarFile::is_vicar_file(const std::string &Fname)
    Determine if a given file is a VICAR file or not.

    This looks for the first few characters of the file being "LBLSIZE="

    """
  return _vicar_file.VicarFile_is_vicar_file(*args)

def VicarFile_file_name_to_unit(*args):
  """
    int VicarFile::file_name_to_unit(const std::string &Fname)
    Return a VICAR unit number that is tied to the given file name. 
    """
  return _vicar_file.VicarFile_file_name_to_unit(*args)



