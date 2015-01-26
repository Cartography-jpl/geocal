# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _vicar_multi_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_multi_file', [dirname(__file__)])
        except ImportError:
            import _vicar_multi_file
            return _vicar_multi_file
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_multi_file', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_multi_file = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_multi_file
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


SHARED_PTR_DISOWN = _vicar_multi_file.SHARED_PTR_DISOWN
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

import geocal_swig.raster_multifile
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
class VicarMultiFile(geocal_swig.raster_multifile.RasterMultifile):
    """
    This gives read access to some of the larger AFIDS datasets, such as
    CIB and SRTM.

    These larger datasets split into a number of VICAR files. Each one of
    these is a some extent in latitude/longitude, for example 1 x 1 degree
    tiles for the SRTM. There is an index IBIS file that gives a listing
    of all the input datasets and where they are located.

    This class gives an interface to this set of VICAR files that looks
    like one really big RasterImage.

    Note that the files might not cover the full extent of area, for
    example one of the 1x1 degree SRTM tiles might be missing. If we
    attempt to access the area covered by a missing file then an error is
    thrown. We may want to come up with a more robust interface, but for
    now this interface seems to be sufficient.

    C++ includes: vicar_multi_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        VicarMultiFile::VicarMultiFile(const std::string &Db_name, const std::string &Dirbase, const
        std::string &Extension="", int Number_tile_line=10000, int
        Number_tile_sample=10000, int Number_line_per_tile=100000, int
        Number_tile_each_file=4, int Number_tile=4, bool
        Favor_memory_mapped=true, bool No_coverage_is_error=true, int
        No_coverage_fill_value=0, bool Force_area_pixel=false)
        Constructor.

        This takes the database file (an IBIS file) and the directory that the
        files pointed to by the database file are located. The directory is
        needed because although the files are often in the same directory as
        the database file they aren't always. The database file doesn't
        contain directory information, just the file name, so we need to pass
        the directory in.

        For some odd reason, the file names also don't always have the
        extension on them. For some databases, you need to add something like
        ".img" to get the actual file name. You can pass any extension that
        is needed.

        There are two kinds of tiling going on. At the top level, we have a
        number of files open at one time, given by Number_tile. For each file,
        we read that it tiles with the given Number_line_per_tile and
        Number_tile_each_file tiles.

        There are two strategies that can be used to reading a Vicar file. One
        reads a tile of a given number of lines and store it in memory. The
        other uses memory mapped file io to read data on demand. Often but not
        always the memory mapped file is faster, particularly for small files
        or for files with a large number of samples where only a portion is
        used. You can control which is used by setting Favor_memory_mapped. If
        the underlying data uses VICAR compression, then we always use the
        VICAR IO routines since memory mapping won't work. This class handles
        this case - it checks if the file is compressed before trying to do
        memory mapping.

        The Force_area_pixel forces the file to be treated as "pixel as
        area" rather than "pixel as point". This is really just meant as a
        work around for the SRTM data, which incorrectly labels the data as
        "point" rather than "area". Since this is a 15 meter difference,
        it matters for many applications. Most users should just ignore this
        value. 
        """
        _vicar_multi_file.VicarMultiFile_swiginit(self,_vicar_multi_file.new_VicarMultiFile(*args))
    def _v_db_name(self):
        """
        const std::string& GeoCal::VicarMultiFile::db_name() const
        IBIS index file name. 
        """
        return _vicar_multi_file.VicarMultiFile__v_db_name(self)

    @property
    def db_name(self):
        return self._v_db_name()

    def _v_dirbase(self):
        """
        const std::string& GeoCal::VicarMultiFile::dirbase() const
        Directory base, where the underlying files are. 
        """
        return _vicar_multi_file.VicarMultiFile__v_dirbase(self)

    @property
    def dirbase(self):
        return self._v_dirbase()

    def _v_extension(self):
        """
        const std::string& GeoCal::VicarMultiFile::extension() const
        Extension to add to names in IBIS index file to get file names (e.g.,
        ".img" if it is left out in IBIS index file name) 
        """
        return _vicar_multi_file.VicarMultiFile__v_extension(self)

    @property
    def extension(self):
        return self._v_extension()

    @property
    def number_tile_line(self):
        return self._v_number_tile_line()

    @property
    def number_tile_sample(self):
        return self._v_number_tile_sample()

    def _v_number_line_per_tile(self):
        """
        int GeoCal::VicarMultiFile::number_line_per_tile() const
        Number of lines to include in each tile in the underlying VICAR files
        we read.

        This doesn't apply if we are doing memory mapping. 
        """
        return _vicar_multi_file.VicarMultiFile__v_number_line_per_tile(self)

    @property
    def number_line_per_tile(self):
        return self._v_number_line_per_tile()

    def _v_number_tile_each_file(self):
        """
        int GeoCal::VicarMultiFile::number_tile_each_file() const
        Number of tiles to use for each underlying VICAR file.

        This doesn't apply if we are doing memory mapping. 
        """
        return _vicar_multi_file.VicarMultiFile__v_number_tile_each_file(self)

    @property
    def number_tile_each_file(self):
        return self._v_number_tile_each_file()

    def _v_favor_memory_mapped(self):
        """
        bool GeoCal::VicarMultiFile::favor_memory_mapped() const
        Whether we use memory mapping or not when reading an uncompressed
        files. 
        """
        return _vicar_multi_file.VicarMultiFile__v_favor_memory_mapped(self)

    @property
    def favor_memory_mapped(self):
        return self._v_favor_memory_mapped()

    def _v_force_area_pixel(self):
        """
        bool GeoCal::VicarMultiFile::force_area_pixel() const
        If true, force the file to be treated as "pixel as area" rather than
        "pixel as point".

        This is really just meant as a work around for the SRTM data, which
        incorrectly labels the data as "point" rather than "area". Since
        this is a 15 meter difference, it matters for many applications. Most
        users should just ignore this value. 
        """
        return _vicar_multi_file.VicarMultiFile__v_force_area_pixel(self)

    @property
    def force_area_pixel(self):
        return self._v_force_area_pixel()

    @property
    def no_coverage_is_error(self):
        return self._v_no_coverage_is_error()

    @property
    def no_coverage_fill_value(self):
        return self._v_no_coverage_fill_value()

    @property
    def number_tile(self):
        return self._v_number_tile()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.db_name,self.dirbase,self.extension,self.number_tile_line,self.number_tile_sample,self.number_line_per_tile,self.number_tile_each_file,self.number_tile,self.favor_memory_mapped,self.no_coverage_is_error,self.no_coverage_fill_value,self.force_area_pixel)

    __swig_destroy__ = _vicar_multi_file.delete_VicarMultiFile
VicarMultiFile.write = new_instancemethod(_vicar_multi_file.VicarMultiFile_write,None,VicarMultiFile)
VicarMultiFile._v_db_name = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_db_name,None,VicarMultiFile)
VicarMultiFile._v_dirbase = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_dirbase,None,VicarMultiFile)
VicarMultiFile._v_extension = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_extension,None,VicarMultiFile)
VicarMultiFile._v_number_tile_line = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_number_tile_line,None,VicarMultiFile)
VicarMultiFile._v_number_tile_sample = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_number_tile_sample,None,VicarMultiFile)
VicarMultiFile._v_number_line_per_tile = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_number_line_per_tile,None,VicarMultiFile)
VicarMultiFile._v_number_tile_each_file = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_number_tile_each_file,None,VicarMultiFile)
VicarMultiFile._v_favor_memory_mapped = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_favor_memory_mapped,None,VicarMultiFile)
VicarMultiFile._v_force_area_pixel = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_force_area_pixel,None,VicarMultiFile)
VicarMultiFile._v_no_coverage_is_error = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_no_coverage_is_error,None,VicarMultiFile)
VicarMultiFile._v_no_coverage_fill_value = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_no_coverage_fill_value,None,VicarMultiFile)
VicarMultiFile._v_number_tile = new_instancemethod(_vicar_multi_file.VicarMultiFile__v_number_tile,None,VicarMultiFile)
VicarMultiFile_swigregister = _vicar_multi_file.VicarMultiFile_swigregister
VicarMultiFile_swigregister(VicarMultiFile)



