# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _gdal_raster_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_gdal_raster_image', [dirname(__file__)])
        except ImportError:
            import _gdal_raster_image
            return _gdal_raster_image
        if fp is not None:
            try:
                _mod = imp.load_module('_gdal_raster_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _gdal_raster_image = swig_import_helper()
    del swig_import_helper
else:
    import _gdal_raster_image
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


SHARED_PTR_DISOWN = _gdal_raster_image.SHARED_PTR_DISOWN
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

import geocal_swig.raster_image_tiled_file
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.look_vector
import geocal_swig.geocal_exception
class GdalRasterImage(geocal_swig.raster_image_tiled_file.RasterImageTiledFile):
    """
    This wraps a Gdal<T> to make a RasterImage.

    The type of T should be one we can convert to and from a int, since
    this is what a RasterImage works in.

    C++ includes: gdal_raster_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    Byte = _gdal_raster_image.GdalRasterImage_Byte
    UInt16 = _gdal_raster_image.GdalRasterImage_UInt16
    Int16 = _gdal_raster_image.GdalRasterImage_Int16
    UInt32 = _gdal_raster_image.GdalRasterImage_UInt32
    Int32 = _gdal_raster_image.GdalRasterImage_Int32
    Float32 = _gdal_raster_image.GdalRasterImage_Float32
    Float64 = _gdal_raster_image.GdalRasterImage_Float64
    def __init__(self, *args): 
        """
        GeoCal::GdalRasterImage::GdalRasterImage(const boost::shared_ptr< Gdal< T > > &Data)
        Constructor. 
        """
        _gdal_raster_image.GdalRasterImage_swiginit(self,_gdal_raster_image.new_GdalRasterImage(*args))
    def read_all(*args):
        """
        std::vector< boost::shared_ptr< RasterImage > > GdalRasterImage::read_all(const std::string &Fname)
        For some formats, reading all of the data for all the bands at one
        time is much quicker than reading each band separately (assuming you
        actually want all the data).

        For example, for jpeg reading three separate bands requires
        decompressing the data three times while reading all at once only
        decompresses once. Since almost the entire time reading jpeg is used
        in decompressing the data this means it takes about three times as
        long to read all the bands as to read them all at once.

        This function reads all of the data into memory and returns
        RasterImages that can be used to read the data. 
        """
        return _gdal_raster_image.GdalRasterImage_read_all(*args)

    read_all = staticmethod(read_all)
    def _v_gdal_data_base(self):
        """
        boost::shared_ptr<GdalBase> GeoCal::GdalRasterImage::gdal_data_base_ptr() const

        """
        return _gdal_raster_image.GdalRasterImage__v_gdal_data_base(self)

    @property
    def gdal_data_base(self):
        return self._v_gdal_data_base()

    def flush(self):
        """
        virtual void GeoCal::GdalRasterImage::flush() const
        Flush data to disk. 
        """
        return _gdal_raster_image.GdalRasterImage_flush(self)

    def close(self):
        """
        void GeoCal::GdalRasterImage::close()

        """
        return _gdal_raster_image.GdalRasterImage_close(self)

    def _v_is_closed(self):
        """
        bool GeoCal::GdalRasterImage::is_closed() const

        """
        return _gdal_raster_image.GdalRasterImage__v_is_closed(self)

    @property
    def is_closed(self):
        return self._v_is_closed()

    def set_map_info(self, *args):
        """
        void GdalRasterImage::set_map_info(const MapInfo &Mi)
        Set the map info. This overwrites whatever was previously set. 
        """
        return _gdal_raster_image.GdalRasterImage_set_map_info(self, *args)

    def set_rpc(self, *args):
        """
        void GdalRasterImage::set_rpc(const Rpc &R)
        Set the RPC info. This overwrites whatever was previously set. 
        """
        return _gdal_raster_image.GdalRasterImage_set_rpc(self, *args)

    def has_metadata(self, *args):
        """
        bool GeoCal::GdalRasterImage::has_metadata(const std::string &Metadata_item, const std::string &Domain="")
        const
        Indicates if a metadata item is found in the file. 
        """
        return _gdal_raster_image.GdalRasterImage_has_metadata(self, *args)

    def set_metadata(self, *args):
        """
        void GeoCal::GdalRasterImage::set_metadata(const std::string &M, const std::string &Val, const std::string
        &Domain="")
        Write the requested metadata. 
        """
        return _gdal_raster_image.GdalRasterImage_set_metadata(self, *args)

    def _v_file_names(self):
        """
        std::vector<std::string> GeoCal::GdalRasterImage::file_names() const

        """
        return _gdal_raster_image.GdalRasterImage__v_file_names(self)

    @property
    def file_names(self):
        return self._v_file_names()

    def map_info_from_nitf_corner(self, Approx_ok=False):
        """
        boost::shared_ptr< MapInfo > GdalRasterImage::map_info_from_nitf_corner(bool Approx_ok=false) const
        In AFIDS, we calculate what is called the "nitf corners".

        This gives approximate corner coordinates for an image. Depending on
        the application, it can be useful to treat this as an approximate
        MapInfo.

        This return a null pointer if we don't have the NITF corner metadata.

        """
        return _gdal_raster_image.GdalRasterImage_map_info_from_nitf_corner(self, Approx_ok)

    def _v_band_id(self):
        """
        int GeoCal::GdalRasterImage::band_id() const

        """
        return _gdal_raster_image.GdalRasterImage__v_band_id(self)

    @property
    def band_id(self):
        return self._v_band_id()

    def _v_update(self):
        """
        bool GeoCal::GdalRasterImage::update() const

        """
        return _gdal_raster_image.GdalRasterImage__v_update(self)

    @property
    def update(self):
        return self._v_update()

    def _v_rpc(self):
        """
        Rpc GeoCal::GdalRasterImage::rpc() const
        Return Rpc for image. 
        """
        return _gdal_raster_image.GdalRasterImage__v_rpc(self)

    @property
    def rpc(self):
        return self._v_rpc()

    @property
    def map_info(self):
        return self._v_map_info()

    @rpc.setter
    def rpc(self, val):
       self.set_rpc(val)

    @map_info.setter
    def map_info(self, val):
       self.set_map_info(val)

    def __getitem__(self, key):
        if(isinstance(key, list) or isinstance(key, tuple)):
          domain, ky = key
          return self.metadata(ky, domain)
        else:
          return self.metadata(key)

    def __setitem__(self, key, v):
        if(isinstance(key, list) or isinstance(key, tuple)):
          domain, ky = key
          self.set_metadata(ky, v, domain)
        else:
          self.set_metadata(key, v)

    def __contains__(self, key):
        if(isinstance(key, list) or isinstance(key, tuple)):
          domain, ky = key
          return self.has_metadata(ky, domain)
        else:
          return self.has_metadata(key)
        
      
    def metadata(self, *args):
        """
        S GeoCal::GdalRasterImage::metadata(const std::string &Metadata_item, const std::string &Domain="")
        const
        Read a metadata item and cast it to the given type. 
        """
        return _gdal_raster_image.GdalRasterImage_metadata(self, *args)

    save_to_file = staticmethod(_gdal_raster_image.GdalRasterImage_save_to_file)
    def save_to_erdas(*args):
        """
        void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImageMultiBand &d, int
        Dt=GDT_Byte, int Fill_value=0)
        Copy the given RasterImages into a new Erdas file.

        This calculates the statistics and image pyramid needed by Erdas also.
        The given Fill_value is excluded from the statistics calculation. 
        """
        return _gdal_raster_image.GdalRasterImage_save_to_erdas(*args)

    save_to_erdas = staticmethod(save_to_erdas)
    def save(*args):
        """
        void GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
        std::vector< const RasterImage * > &d, int Dt=GDT_Byte, const
        std::string &Option="", bool Require_copy=false, int
        Fill_value=-1000)
        Copy the given RasterImages into a new file, using the given driver
        and options.

        Some drivers only allow "CreateCopy" rather than "Create",
        requiring us to use an intermediate memory version of Gdal. In that
        case, set "require_copy" to true. 
        """
        return _gdal_raster_image.GdalRasterImage_save(*args)

    save = staticmethod(save)
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.file_names[0],self.band_id,self.number_tile,self.update,self.number_tile_line,self.number_tile_sample)

    __swig_destroy__ = _gdal_raster_image.delete_GdalRasterImage
GdalRasterImage._v_gdal_data_base = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_gdal_data_base,None,GdalRasterImage)
GdalRasterImage.flush = new_instancemethod(_gdal_raster_image.GdalRasterImage_flush,None,GdalRasterImage)
GdalRasterImage.close = new_instancemethod(_gdal_raster_image.GdalRasterImage_close,None,GdalRasterImage)
GdalRasterImage._v_is_closed = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_is_closed,None,GdalRasterImage)
GdalRasterImage.set_map_info = new_instancemethod(_gdal_raster_image.GdalRasterImage_set_map_info,None,GdalRasterImage)
GdalRasterImage.set_rpc = new_instancemethod(_gdal_raster_image.GdalRasterImage_set_rpc,None,GdalRasterImage)
GdalRasterImage.has_metadata = new_instancemethod(_gdal_raster_image.GdalRasterImage_has_metadata,None,GdalRasterImage)
GdalRasterImage.set_metadata = new_instancemethod(_gdal_raster_image.GdalRasterImage_set_metadata,None,GdalRasterImage)
GdalRasterImage._v_file_names = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_file_names,None,GdalRasterImage)
GdalRasterImage.map_info_from_nitf_corner = new_instancemethod(_gdal_raster_image.GdalRasterImage_map_info_from_nitf_corner,None,GdalRasterImage)
GdalRasterImage._v_band_id = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_band_id,None,GdalRasterImage)
GdalRasterImage._v_update = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_update,None,GdalRasterImage)
GdalRasterImage._v_rpc = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_rpc,None,GdalRasterImage)
GdalRasterImage._v_map_info = new_instancemethod(_gdal_raster_image.GdalRasterImage__v_map_info,None,GdalRasterImage)
GdalRasterImage.metadata = new_instancemethod(_gdal_raster_image.GdalRasterImage_metadata,None,GdalRasterImage)
GdalRasterImage_swigregister = _gdal_raster_image.GdalRasterImage_swigregister
GdalRasterImage_swigregister(GdalRasterImage)

def GdalRasterImage_read_all(*args):
  """
    std::vector< boost::shared_ptr< RasterImage > > GdalRasterImage::read_all(const std::string &Fname)
    For some formats, reading all of the data for all the bands at one
    time is much quicker than reading each band separately (assuming you
    actually want all the data).

    For example, for jpeg reading three separate bands requires
    decompressing the data three times while reading all at once only
    decompresses once. Since almost the entire time reading jpeg is used
    in decompressing the data this means it takes about three times as
    long to read all the bands as to read them all at once.

    This function reads all of the data into memory and returns
    RasterImages that can be used to read the data. 
    """
  return _gdal_raster_image.GdalRasterImage_read_all(*args)

def GdalRasterImage_save_to_file(*args):
  return _gdal_raster_image.GdalRasterImage_save_to_file(*args)
GdalRasterImage_save_to_file = _gdal_raster_image.GdalRasterImage_save_to_file

def GdalRasterImage_save_to_erdas(*args):
  """
    void GdalRasterImage::save_to_erdas(const std::string &Oname, const RasterImageMultiBand &d, int
    Dt=GDT_Byte, int Fill_value=0)
    Copy the given RasterImages into a new Erdas file.

    This calculates the statistics and image pyramid needed by Erdas also.
    The given Fill_value is excluded from the statistics calculation. 
    """
  return _gdal_raster_image.GdalRasterImage_save_to_erdas(*args)

def GdalRasterImage_save(*args):
  """
    void GdalRasterImage::save(const std::string &Oname, const std::string &Driver, const
    std::vector< const RasterImage * > &d, int Dt=GDT_Byte, const
    std::string &Option="", bool Require_copy=false, int
    Fill_value=-1000)
    Copy the given RasterImages into a new file, using the given driver
    and options.

    Some drivers only allow "CreateCopy" rather than "Create",
    requiring us to use an intermediate memory version of Gdal. In that
    case, set "require_copy" to true. 
    """
  return _gdal_raster_image.GdalRasterImage_save(*args)



