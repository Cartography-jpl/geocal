# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _igc_map_projected.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_map_projected', [dirname(__file__)])
        except ImportError:
            import _igc_map_projected
            return _igc_map_projected
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_map_projected', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_map_projected = swig_import_helper()
    del swig_import_helper
else:
    import _igc_map_projected
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


SHARED_PTR_DISOWN = _igc_map_projected.SHARED_PTR_DISOWN
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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.calc_raster_multi_band
import geocal_swig.raster_image_multi_band_variable
import geocal_swig.raster_image_multi_band
import geocal_swig.geocal_exception
class IgcMapProjected(geocal_swig.calc_raster.CalcRaster):
    """
    This is a RasterImage that has been map projected using an
    ImageGroundConnection.

    To do this we do two steps:

    We calculate roughly what the difference in resolution is between the
    original data and final MapInfo. We do this by looking at the center
    pixel of the original data and the pixel +1 in line and sample. We
    then use RasterAveraged to average the original data to roughly the
    resolution of the final MapInfo. If the final MapInfo is near the same
    resolution as the original, or if it has a higher resolution, then we
    don't do any averaging. Alternatively, you can pass in the averaging
    factor (include a value of 1 which turns this behavior off).

    We then interpolate the possibly averaged data to the final
    projection.

    It is ok if the final MapInfo contains areas outside of the original
    data. For any pixel outside of the original data, we just return a
    value of 0.

    This class calculates the data on the fly. Sometimes this is what you
    want, but if you are going to be using the resulting data a few times,
    you may want to use a MemoryRasterImage to generate a copy once and
    keep it in memory.

    C++ includes: igc_map_projected.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        IgcMapProjected::IgcMapProjected(const MapInfo &Mi, const boost::shared_ptr< ImageGroundConnection >
        &Igc, int Grid_spacing=1, int Avg_fact=-1, bool Read_into_memory=true,
        int Number_tile_line=-1, int Number_tile_sample=-1, double
        Fill_value=0.0)
        Constructor.

        We average the data either by the factor given as Avg_fact, or by
        ratio of the Mapinfo resolution and the Igc resolution.

        You can optionally pass a grid spacing to use. We calculate image
        coordinates in the input exactly at the grid spacing, and interpolate
        in betweeen. This is much faster than calculating every point, and if
        the grid spacing is small compared to the Dem and any nonlinearities
        then it gives results very close to the full calculation. 
        """
        _igc_map_projected.IgcMapProjected_swiginit(self,_igc_map_projected.new_IgcMapProjected(*args))
    def _v_igc_original(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcMapProjectedBase::igc_original() const

        """
        return _igc_map_projected.IgcMapProjected__v_igc_original(self)

    @property
    def igc_original(self):
        return self._v_igc_original()

    def _v_avg_factor(self):
        """
        int GeoCal::IgcMapProjectedBase::avg_factor() const

        """
        return _igc_map_projected.IgcMapProjected__v_avg_factor(self)

    @property
    def avg_factor(self):
        return self._v_avg_factor()

    def _v_grid_spacing(self):
        """
        int GeoCal::IgcMapProjectedBase::grid_spacing() const

        """
        return _igc_map_projected.IgcMapProjected__v_grid_spacing(self)

    @property
    def grid_spacing(self):
        return self._v_grid_spacing()

    def _v_fill_value(self):
        """
        double GeoCal::IgcMapProjectedBase::fill_value() const

        """
        return _igc_map_projected.IgcMapProjected__v_fill_value(self)

    @property
    def fill_value(self):
        return self._v_fill_value()

    def _v_read_into_memory(self):
        """
        bool GeoCal::IgcMapProjectedBase::read_into_memory() const

        """
        return _igc_map_projected.IgcMapProjected__v_read_into_memory(self)

    @property
    def read_into_memory(self):
        return self._v_read_into_memory()

    @classmethod
    def pickle_format_version(cls):
      return 2

    def __reduce__(self):
      return _new_from_init, (self.__class__, 2, self.map_info,self.igc_original,self.grid_spacing,self.avg_factor,self.read_into_memory,self.number_tile_line,self.number_tile_sample,self.fill_value)

    __swig_destroy__ = _igc_map_projected.delete_IgcMapProjected
IgcMapProjected._v_igc_original = new_instancemethod(_igc_map_projected.IgcMapProjected__v_igc_original,None,IgcMapProjected)
IgcMapProjected._v_avg_factor = new_instancemethod(_igc_map_projected.IgcMapProjected__v_avg_factor,None,IgcMapProjected)
IgcMapProjected._v_grid_spacing = new_instancemethod(_igc_map_projected.IgcMapProjected__v_grid_spacing,None,IgcMapProjected)
IgcMapProjected._v_fill_value = new_instancemethod(_igc_map_projected.IgcMapProjected__v_fill_value,None,IgcMapProjected)
IgcMapProjected._v_read_into_memory = new_instancemethod(_igc_map_projected.IgcMapProjected__v_read_into_memory,None,IgcMapProjected)
IgcMapProjected_swigregister = _igc_map_projected.IgcMapProjected_swigregister
IgcMapProjected_swigregister(IgcMapProjected)

class IgcMapProjectedMultiBand(geocal_swig.calc_raster_multi_band.CalcRasterMultiBand):
    """
    This is a RasterImageMultiBand that has been map projected using an
    ImageGroundConnection.

    To do this we do two steps:

    We calculate roughly what the difference in resolution is between the
    original data and final MapInfo. We do this by looking at the center
    pixel of the original data and the pixel +1 in line and sample. We
    then use RasterAveraged to average the original data to roughly the
    resolution of the final MapInfo. If the final MapInfo is near the same
    resolution as the original, or if it has a higher resolution, then we
    don't do any averaging. Alternatively, you can pass in the averaging
    factor (include a value of 1 which turns this behavior off).

    We then interpolate the possibly averaged data to the final
    projection.

    It is ok if the final MapInfo contains areas outside of the original
    data. For any pixel outside of the original data, we just return a
    value of 0.

    This class calculates the data on the fly. Sometimes this is what you
    want, but if you are going to be using the resulting data a few times,
    you may want to use a MemoryRasterImage to generate a copy once and
    keep it in memory.

    C++ includes: igc_map_projected.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        IgcMapProjectedMultiBand::IgcMapProjectedMultiBand(const MapInfo &Mi, const boost::shared_ptr< ImageGroundConnection >
        &Igc, int Grid_spacing=1, int Avg_fact=-1, bool Read_into_memory=true,
        int Number_tile_line=-1, int Number_tile_sample=-1, double
        Fill_value=0.0)
        Constructor.

        We average the data either by the factor given as Avg_fact, or by
        ratio of the Mapinfo resolution and the Igc resolution.

        You can optionally pass a grid spacing to use. We calculate image
        coordinates in the input exactly at the grid spacing, and interpolate
        in betweeen. This is much faster than calculating every point, and if
        the grid spacing is small compared to the Dem and any nonlinearities
        then it gives results very close to the full calculation. 
        """
        _igc_map_projected.IgcMapProjectedMultiBand_swiginit(self,_igc_map_projected.new_IgcMapProjectedMultiBand(*args))
    def _v_igc_original(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcMapProjectedBase::igc_original() const

        """
        return _igc_map_projected.IgcMapProjectedMultiBand__v_igc_original(self)

    @property
    def igc_original(self):
        return self._v_igc_original()

    def _v_avg_factor(self):
        """
        int GeoCal::IgcMapProjectedBase::avg_factor() const

        """
        return _igc_map_projected.IgcMapProjectedMultiBand__v_avg_factor(self)

    @property
    def avg_factor(self):
        return self._v_avg_factor()

    def _v_grid_spacing(self):
        """
        int GeoCal::IgcMapProjectedBase::grid_spacing() const

        """
        return _igc_map_projected.IgcMapProjectedMultiBand__v_grid_spacing(self)

    @property
    def grid_spacing(self):
        return self._v_grid_spacing()

    def _v_fill_value(self):
        """
        double GeoCal::IgcMapProjectedBase::fill_value() const

        """
        return _igc_map_projected.IgcMapProjectedMultiBand__v_fill_value(self)

    @property
    def fill_value(self):
        return self._v_fill_value()

    def _v_read_into_memory(self):
        """
        bool GeoCal::IgcMapProjectedBase::read_into_memory() const

        """
        return _igc_map_projected.IgcMapProjectedMultiBand__v_read_into_memory(self)

    @property
    def read_into_memory(self):
        return self._v_read_into_memory()

    @classmethod
    def pickle_format_version(cls):
      return 2

    def __reduce__(self):
      return _new_from_init, (self.__class__, 2, self.raster_image(0).map_info,self.igc_original,self.grid_spacing,self.avg_factor,self.read_into_memory,self.raster_image(0).number_tile_line,self.raster_image(0).number_tile_sample,self.fill_value)

    __swig_destroy__ = _igc_map_projected.delete_IgcMapProjectedMultiBand
IgcMapProjectedMultiBand._v_igc_original = new_instancemethod(_igc_map_projected.IgcMapProjectedMultiBand__v_igc_original,None,IgcMapProjectedMultiBand)
IgcMapProjectedMultiBand._v_avg_factor = new_instancemethod(_igc_map_projected.IgcMapProjectedMultiBand__v_avg_factor,None,IgcMapProjectedMultiBand)
IgcMapProjectedMultiBand._v_grid_spacing = new_instancemethod(_igc_map_projected.IgcMapProjectedMultiBand__v_grid_spacing,None,IgcMapProjectedMultiBand)
IgcMapProjectedMultiBand._v_fill_value = new_instancemethod(_igc_map_projected.IgcMapProjectedMultiBand__v_fill_value,None,IgcMapProjectedMultiBand)
IgcMapProjectedMultiBand._v_read_into_memory = new_instancemethod(_igc_map_projected.IgcMapProjectedMultiBand__v_read_into_memory,None,IgcMapProjectedMultiBand)
IgcMapProjectedMultiBand_swigregister = _igc_map_projected.IgcMapProjectedMultiBand_swigregister
IgcMapProjectedMultiBand_swigregister(IgcMapProjectedMultiBand)



