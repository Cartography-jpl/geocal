# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _gdal_multi_band.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_gdal_multi_band', [dirname(__file__)])
        except ImportError:
            import _gdal_multi_band
            return _gdal_multi_band
        if fp is not None:
            try:
                _mod = imp.load_module('_gdal_multi_band', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _gdal_multi_band = swig_import_helper()
    del swig_import_helper
else:
    import _gdal_multi_band
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


SHARED_PTR_DISOWN = _gdal_multi_band.SHARED_PTR_DISOWN
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

import geocal.raster_image_multi_band_variable
import geocal.raster_image_multi_band
import geocal.generic_object
import geocal.raster_image_tiled_file
import geocal.raster_image_variable
import geocal.geocal_exception
class GdalMultiBand(geocal.raster_image_multi_band_variable.RasterImageMultiBandVariable):
    """
    This is a set of Gdal RasterImages, one for each band.

    C++ includes: gdal_multi_band.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GdalMultiBand::GdalMultiBand(const std::string &Fname, const std::string &Driver_name, int
        Number_line, int Number_sample, int Number_band, int Dt, const
        std::string &Options="", int Number_tile=4, int Tile_number_line=-1,
        int Tile_number_sample=-1)
        Create a new file.

        All the arguments here are the same as for GdalRasterImage. 
        """
        _gdal_multi_band.GdalMultiBand_swiginit(self,_gdal_multi_band.new_GdalMultiBand(*args))
    def set_rpc(self, *args):
        """
        void GeoCal::GdalMultiBand::set_rpc(const Rpc &R)

        """
        return _gdal_multi_band.GdalMultiBand_set_rpc(self, *args)

    def set_map_info(self, *args):
        """
        void GeoCal::GdalMultiBand::set_map_info(const MapInfo &M)

        """
        return _gdal_multi_band.GdalMultiBand_set_map_info(self, *args)

    def save(*args):
        """
        void GdalMultiBand::save(const std::string &Oname, const std::string &Driver, const
        RasterImageMultiBand &d, bool Log_progress=false, int Dt=GDT_Byte,
        const std::string &Options="")
        Copy the RasterImageMultiBand into a new file.

        We copy map and RPC information (if found) from the first band in
        RasterImageMultiBand. 
        """
        return _gdal_multi_band.GdalMultiBand_save(*args)

    save = staticmethod(save)
    def gdal_raster_image(self, *args):
        """
        boost::shared_ptr<GdalRasterImage> GeoCal::GdalMultiBand::gdal_raster_image(int band) const
        RasterImage, cast to GdalRasterImage. 
        """
        return _gdal_multi_band.GdalMultiBand_gdal_raster_image(self, *args)

    def data_set(self):
        """
        const boost::shared_ptr<GDALDataset>& GeoCal::GdalMultiBand::data_set() const
        The underlying dataset object. 
        """
        return _gdal_multi_band.GdalMultiBand_data_set(self)

    def flush(self):
        """
        void GdalMultiBand::flush() const
        Flush data to disk for all bands. 
        """
        return _gdal_multi_band.GdalMultiBand_flush(self)

    def close(self):
        """
        void GdalMultiBand::close() const
        Flush data to disk for all bands and close file.

        You don't normally need to call this directly, since the destructor of
        the files automatically does this. But this can be useful in some
        cases. 
        """
        return _gdal_multi_band.GdalMultiBand_close(self)

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.gdal_raster_image(0).file_names[0],self.gdal_raster_image(0).number_tile,self.gdal_raster_image(0).number_tile_line,self.gdal_raster_image(0).number_tile_sample)

    __swig_destroy__ = _gdal_multi_band.delete_GdalMultiBand
GdalMultiBand.set_rpc = new_instancemethod(_gdal_multi_band.GdalMultiBand_set_rpc,None,GdalMultiBand)
GdalMultiBand.set_map_info = new_instancemethod(_gdal_multi_band.GdalMultiBand_set_map_info,None,GdalMultiBand)
GdalMultiBand.gdal_raster_image = new_instancemethod(_gdal_multi_band.GdalMultiBand_gdal_raster_image,None,GdalMultiBand)
GdalMultiBand.data_set = new_instancemethod(_gdal_multi_band.GdalMultiBand_data_set,None,GdalMultiBand)
GdalMultiBand.flush = new_instancemethod(_gdal_multi_band.GdalMultiBand_flush,None,GdalMultiBand)
GdalMultiBand.close = new_instancemethod(_gdal_multi_band.GdalMultiBand_close,None,GdalMultiBand)
GdalMultiBand_swigregister = _gdal_multi_band.GdalMultiBand_swigregister
GdalMultiBand_swigregister(GdalMultiBand)

def GdalMultiBand_save(*args):
  """
    void GdalMultiBand::save(const std::string &Oname, const std::string &Driver, const
    RasterImageMultiBand &d, bool Log_progress=false, int Dt=GDT_Byte,
    const std::string &Options="")
    Copy the RasterImageMultiBand into a new file.

    We copy map and RPC information (if found) from the first band in
    RasterImageMultiBand. 
    """
  return _gdal_multi_band.GdalMultiBand_save(*args)



