# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _vicar_lite_file.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_vicar_lite_file', [dirname(__file__)])
        except ImportError:
            import _vicar_lite_file
            return _vicar_lite_file
        if fp is not None:
            try:
                _mod = imp.load_module('_vicar_lite_file', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _vicar_lite_file = swig_import_helper()
    del swig_import_helper
else:
    import _vicar_lite_file
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


SHARED_PTR_DISOWN = _vicar_lite_file.SHARED_PTR_DISOWN
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
import geocal_swig.raster_image
import geocal_swig.look_vector
import geocal_swig.dem_map_info
import geocal_swig.dem
import geocal_swig.geocal_exception
class VicarLiteFile(geocal_swig.generic_object.GenericObject):
    """
    This handles opening and closing a Vicar file.

    This class doesn't actually read and write data, but is used by other
    classes which do.

    This class is a simple implementation of Vicar. It doesn't use the
    Vicar RTL library, which is the primary attraction of it. We can use
    this to read Vicar files in a context where we don't want to use the
    RTL library, or don't want to be constrained to running in the Vicar
    environment.

    This class does not handle new VICAR features such as compression.

    C++ includes: vicar_lite_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    VICAR_BYTE = _vicar_lite_file.VicarLiteFile_VICAR_BYTE
    VICAR_HALF = _vicar_lite_file.VicarLiteFile_VICAR_HALF
    VICAR_FULL = _vicar_lite_file.VicarLiteFile_VICAR_FULL
    VICAR_FLOAT = _vicar_lite_file.VicarLiteFile_VICAR_FLOAT
    VICAR_DOUBLE = _vicar_lite_file.VicarLiteFile_VICAR_DOUBLE
    READ = _vicar_lite_file.VicarLiteFile_READ
    WRITE = _vicar_lite_file.VicarLiteFile_WRITE
    UPDATE = _vicar_lite_file.VicarLiteFile_UPDATE
    def __init__(self, *args): 
        """
        VicarLiteFile::VicarLiteFile(const std::string &Fname, int Number_line, int Number_sample, const
        std::string &Type="BYTE")
        Create a new VICAR file with the given size. 
        """
        _vicar_lite_file.VicarLiteFile_swiginit(self,_vicar_lite_file.new_VicarLiteFile(*args))
    __swig_destroy__ = _vicar_lite_file.delete_VicarLiteFile
    def _v_access(self):
        """
        access_type GeoCal::VicarLiteFile::access() const
        Access type of file. 
        """
        return _vicar_lite_file.VicarLiteFile__v_access(self)

    @property
    def access(self):
        return self._v_access()

    def _v_force_area_pixel(self):
        """
        bool GeoCal::VicarLiteFile::force_area_pixel() const
        If true, then force the file to be treated as "pixel is area".

        This is really just meant as a work around for the SRTM data, which
        incorrectly labels the data as "point" rather than "area". Since
        this is a 15 meter difference, it matters for many applications. Most
        users should just ignore this value. 
        """
        return _vicar_lite_file.VicarLiteFile__v_force_area_pixel(self)

    @property
    def force_area_pixel(self):
        return self._v_force_area_pixel()

    def _v_data_offset(self):
        """
        int GeoCal::VicarLiteFile::data_offset() const
        Offset to where the data starts. 
        """
        return _vicar_lite_file.VicarLiteFile__v_data_offset(self)

    @property
    def data_offset(self):
        return self._v_data_offset()

    def _v_file_name(self):
        """
        const std::string& GeoCal::VicarLiteFile::file_name() const
        File name. 
        """
        return _vicar_lite_file.VicarLiteFile__v_file_name(self)

    @property
    def file_name(self):
        return self._v_file_name()

    def is_vicar_file(*args):
        """
        bool VicarLiteFile::is_vicar_file(const std::string &Fname)
        Determine if a given file is a VICAR file or not.

        This looks for the first few characters of the file being "LBLSIZE="

        """
        return _vicar_lite_file.VicarLiteFile_is_vicar_file(*args)

    is_vicar_file = staticmethod(is_vicar_file)
    def _v_number_line(self):
        """
        int GeoCal::VicarLiteFile::number_line() const
        Number of lines in file. 
        """
        return _vicar_lite_file.VicarLiteFile__v_number_line(self)

    @property
    def number_line(self):
        return self._v_number_line()

    def _v_number_sample(self):
        """
        int GeoCal::VicarLiteFile::number_sample() const
        Number of samples in file. 
        """
        return _vicar_lite_file.VicarLiteFile__v_number_sample(self)

    @property
    def number_sample(self):
        return self._v_number_sample()

    def _v_number_band(self):
        """
        int GeoCal::VicarLiteFile::number_band() const
        Number of bands in file. 
        """
        return _vicar_lite_file.VicarLiteFile__v_number_band(self)

    @property
    def number_band(self):
        return self._v_number_band()

    def _v_type(self):
        """
        data_type GeoCal::VicarLiteFile::type() const
        Type of data in file. 
        """
        return _vicar_lite_file.VicarLiteFile__v_type(self)

    @property
    def type(self):
        return self._v_type()

    def _v_map_info(self, *args):
        """
        void VicarLiteFile::map_info(const MapInfo &M)
        Set metadata for MapInfo. 
        """
        return _vicar_lite_file.VicarLiteFile__v_map_info(self, *args)

    @property
    def map_info(self):
        return self._v_map_info()

    @map_info.setter
    def map_info(self, value):
      self._v_map_info(value)

    def _v_rpc(self, *args):
        """
        void GeoCal::VicarLiteFile::rpc(const Rpc &R)

        """
        return _vicar_lite_file.VicarLiteFile__v_rpc(self, *args)

    @property
    def rpc(self):
        return self._v_rpc()

    @rpc.setter
    def rpc(self, value):
      self._v_rpc(value)

    def _v_is_compressed(self):
        """
        bool GeoCal::VicarLiteFile::is_compressed() const
        Indicate if the file is compressed.

        If it is, we can't actually read the data but we can still read all of
        the labels. 
        """
        return _vicar_lite_file.VicarLiteFile__v_is_compressed(self)

    @property
    def is_compressed(self):
        return self._v_is_compressed()

    def label_string(self, *args):
        """
        std::string GeoCal::VicarLiteFile::label_string(const std::string &F, const std::string &Property="") const
        Non template form of label, useful in some contexts. 
        """
        return _vicar_lite_file.VicarLiteFile_label_string(self, *args)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_name,self.access,self.force_area_pixel)

VicarLiteFile._v_access = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_access,None,VicarLiteFile)
VicarLiteFile._v_force_area_pixel = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_force_area_pixel,None,VicarLiteFile)
VicarLiteFile._v_data_offset = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_data_offset,None,VicarLiteFile)
VicarLiteFile._v_file_name = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_file_name,None,VicarLiteFile)
VicarLiteFile._v_number_line = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_number_line,None,VicarLiteFile)
VicarLiteFile._v_number_sample = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_number_sample,None,VicarLiteFile)
VicarLiteFile._v_number_band = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_number_band,None,VicarLiteFile)
VicarLiteFile._v_type = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_type,None,VicarLiteFile)
VicarLiteFile._v_map_info = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_map_info,None,VicarLiteFile)
VicarLiteFile._v_rpc = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_rpc,None,VicarLiteFile)
VicarLiteFile.__str__ = new_instancemethod(_vicar_lite_file.VicarLiteFile___str__,None,VicarLiteFile)
VicarLiteFile._v_is_compressed = new_instancemethod(_vicar_lite_file.VicarLiteFile__v_is_compressed,None,VicarLiteFile)
VicarLiteFile.label_string = new_instancemethod(_vicar_lite_file.VicarLiteFile_label_string,None,VicarLiteFile)
VicarLiteFile.label_list = new_instancemethod(_vicar_lite_file.VicarLiteFile_label_list,None,VicarLiteFile)
VicarLiteFile_swigregister = _vicar_lite_file.VicarLiteFile_swigregister
VicarLiteFile_swigregister(VicarLiteFile)

def VicarLiteFile_is_vicar_file(*args):
  """
    bool VicarLiteFile::is_vicar_file(const std::string &Fname)
    Determine if a given file is a VICAR file or not.

    This looks for the first few characters of the file being "LBLSIZE="

    """
  return _vicar_lite_file.VicarLiteFile_is_vicar_file(*args)

class VicarLiteRasterImage(geocal_swig.raster_image.RasterImage):
    """
    This uses VicarLiteFile to implement a RasterImage.

    We use a VicarLiteFile to implement a RasterImage.

    C++ includes: vicar_lite_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::VicarLiteRasterImage::VicarLiteRasterImage(const std::string &Fname, int Band_id=1, access_type
        Access=VicarLiteFile::READ, int Number_tile_line=-1, int
        Number_tile_sample=-1, bool Force_area_pixel=false)
        Constructor.

        The Force_area_pixel forces the file to be treated as "pixel as
        area" rather than "pixel as point". This is really just meant as a
        work around for the SRTM data, which incorrectly labels the data as
        "point" rather than "area". Since this is a 15 meter difference,
        it matters for many applications. Most users should just ignore this
        value. 
        """
        _vicar_lite_file.VicarLiteRasterImage_swiginit(self,_vicar_lite_file.new_VicarLiteRasterImage(*args))
    def _v_file(self):
        """
        boost::shared_ptr<VicarLiteFile> GeoCal::VicarLiteRasterImage::file_ptr() const

        """
        return _vicar_lite_file.VicarLiteRasterImage__v_file(self)

    @property
    def file(self):
        return self._v_file()

    def _v_is_compressed(self):
        """
        bool GeoCal::VicarLiteRasterImage::is_compressed() const
        Indicate if the file is compressed.

        If it is, we can't actually read the data but we can still read all of
        the labels. 
        """
        return _vicar_lite_file.VicarLiteRasterImage__v_is_compressed(self)

    @property
    def is_compressed(self):
        return self._v_is_compressed()

    def _v_band_id(self):
        """
        int GeoCal::VicarLiteRasterImage::band_id() const
        Return band number. This is 1 based (like GDAL). 
        """
        return _vicar_lite_file.VicarLiteRasterImage__v_band_id(self)

    @property
    def band_id(self):
        return self._v_band_id()

    @classmethod
    def pickle_format_version(cls):
      return 2

    def __reduce__(self):
      return _new_from_init, (self.__class__, 2, self.file.file_name,self.band_id,self.file.access,self.number_tile_line,self.number_tile_sample,self.file.force_area_pixel)

    __swig_destroy__ = _vicar_lite_file.delete_VicarLiteRasterImage
VicarLiteRasterImage._v_file = new_instancemethod(_vicar_lite_file.VicarLiteRasterImage__v_file,None,VicarLiteRasterImage)
VicarLiteRasterImage._v_is_compressed = new_instancemethod(_vicar_lite_file.VicarLiteRasterImage__v_is_compressed,None,VicarLiteRasterImage)
VicarLiteRasterImage._v_band_id = new_instancemethod(_vicar_lite_file.VicarLiteRasterImage__v_band_id,None,VicarLiteRasterImage)
VicarLiteRasterImage_swigregister = _vicar_lite_file.VicarLiteRasterImage_swigregister
VicarLiteRasterImage_swigregister(VicarLiteRasterImage)

class VicarLiteDem(geocal_swig.dem_map_info.DemMapInfo):
    """
    This uses VicarLiteFile to implement a Dem.

    We use VicarLiteFile to implement a Dem

    C++ includes: vicar_lite_file.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::VicarLiteDem::VicarLiteDem(const std::string &Fname, bool Outside_dem_is_error=false, const
        boost::shared_ptr< Datum > &D=boost::shared_ptr< Datum >(new
        NoDatum()), int Band_id=1)
        Constructor.

        If Outside_dem_is_error is true, then calls for Dem data outside of
        the range of the Dem cause an exception to be thrown, otherwise we
        just return a height of 0. 
        """
        _vicar_lite_file.VicarLiteDem_swiginit(self,_vicar_lite_file.new_VicarLiteDem(*args))
    def _v_file(self):
        """
        boost::shared_ptr<VicarLiteFile> GeoCal::VicarLiteDem::file_ptr() const

        """
        return _vicar_lite_file.VicarLiteDem__v_file(self)

    @property
    def file(self):
        return self._v_file()

    def elevation(self, *args):
        """
        virtual double GeoCal::VicarLiteDem::elevation(int Y_index, int X_index) const
        Return height in meters relative to datum(). 
        """
        return _vicar_lite_file.VicarLiteDem_elevation(self, *args)

    def _v_band(self):
        """
        int GeoCal::VicarLiteDem::band() const
        Return band number. 
        """
        return _vicar_lite_file.VicarLiteDem__v_band(self)

    @property
    def band(self):
        return self._v_band()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file.file_name,self.outside_dem_is_error,self.datum,self.band)

    __swig_destroy__ = _vicar_lite_file.delete_VicarLiteDem
VicarLiteDem._v_file = new_instancemethod(_vicar_lite_file.VicarLiteDem__v_file,None,VicarLiteDem)
VicarLiteDem.elevation = new_instancemethod(_vicar_lite_file.VicarLiteDem_elevation,None,VicarLiteDem)
VicarLiteDem._v_band = new_instancemethod(_vicar_lite_file.VicarLiteDem__v_band,None,VicarLiteDem)
VicarLiteDem_swigregister = _vicar_lite_file.VicarLiteDem_swigregister
VicarLiteDem_swigregister(VicarLiteDem)



