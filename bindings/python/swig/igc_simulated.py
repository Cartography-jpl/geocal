# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _igc_simulated.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_igc_simulated', [dirname(__file__)])
        except ImportError:
            import _igc_simulated
            return _igc_simulated
        if fp is not None:
            try:
                _mod = imp.load_module('_igc_simulated', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _igc_simulated = swig_import_helper()
    del swig_import_helper
else:
    import _igc_simulated
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


SHARED_PTR_DISOWN = _igc_simulated.SHARED_PTR_DISOWN
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

import geocal_swig.calc_raster
import geocal_swig.raster_image_variable
import geocal_swig.raster_image
import geocal_swig.generic_object
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class IgcSimulated(geocal_swig.calc_raster.CalcRaster):
    """
    This is a RasterImage that is a simulated based on an underlying map
    projected image.

    For each point in the image of an ImageGroundConnection, we calculate
    where that point appears on the ground and then assign a value to it
    from the map projected image.

    This is essentially the inverse of IgcMapProjected, if you replace the
    image of an ImageGroundConnection with this simulated image and then
    run IgcMapProjected you'll get the original map projected image back
    (with changes from being resampled twice). This is similar to the old
    MisrSim program.

    We do this in two steps:

    We calculate roughly what the difference in resolution is between the
    original data and the image we are generating. We do this by looking
    at the center pixel of the original data and the pixel +1 in line and
    sample. We then use RasterAveraged to average the original data to
    roughly the resolution of the final image. If the final image is near
    the same resolution as the original, or if it has a higher resolution,
    then we don't do any averaging. Alternatively, you can pass in the
    averaging factor (include a value of 1 which turns this behavior off).

    We then interpolate the possibly averaged data to the final image.

    This class calculates the data on the fly. Sometimes this is what you
    want, but if you are going to be using the resulting data a few times,
    you may want to use a MemoryRasterImage to generate a copy once and
    keep it in memory.

    C++ includes: igc_simulated.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        IgcSimulated::IgcSimulated(const boost::shared_ptr< ImageGroundConnection > &Igc, const
        boost::shared_ptr< RasterImage > &Map_projected_image, int
        Avg_fact=-1, bool Read_into_memory=true, int Number_tile_line=-1, int
        Number_tile_sample=-1, double Fill_value=0.0)
        Constructor. 
        """
        _igc_simulated.IgcSimulated_swiginit(self,_igc_simulated.new_IgcSimulated(*args))
    def _v_igc(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::IgcSimulated::igc() const

        """
        return _igc_simulated.IgcSimulated__v_igc(self)

    @property
    def igc(self):
        return self._v_igc()

    def _v_avg_factor(self):
        """
        int GeoCal::IgcSimulated::avg_factor() const

        """
        return _igc_simulated.IgcSimulated__v_avg_factor(self)

    @property
    def avg_factor(self):
        return self._v_avg_factor()

    def _v_fill_value(self):
        """
        double GeoCal::IgcSimulated::fill_value() const

        """
        return _igc_simulated.IgcSimulated__v_fill_value(self)

    @property
    def fill_value(self):
        return self._v_fill_value()

    def _v_read_into_memory(self):
        """
        bool GeoCal::IgcSimulated::read_into_memory() const

        """
        return _igc_simulated.IgcSimulated__v_read_into_memory(self)

    @property
    def read_into_memory(self):
        return self._v_read_into_memory()

    def _v_map_projected_image(self):
        """
        const boost::shared_ptr<RasterImage>& GeoCal::IgcSimulated::map_projected_image() const

        """
        return _igc_simulated.IgcSimulated__v_map_projected_image(self)

    @property
    def map_projected_image(self):
        return self._v_map_projected_image()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.igc,self.map_projected_image,self.avg_factor,self.read_into_memory,self.number_tile_line,self.number_tile_sample,self.fill_value)

    __swig_destroy__ = _igc_simulated.delete_IgcSimulated
IgcSimulated._v_igc = new_instancemethod(_igc_simulated.IgcSimulated__v_igc,None,IgcSimulated)
IgcSimulated._v_avg_factor = new_instancemethod(_igc_simulated.IgcSimulated__v_avg_factor,None,IgcSimulated)
IgcSimulated._v_fill_value = new_instancemethod(_igc_simulated.IgcSimulated__v_fill_value,None,IgcSimulated)
IgcSimulated._v_read_into_memory = new_instancemethod(_igc_simulated.IgcSimulated__v_read_into_memory,None,IgcSimulated)
IgcSimulated._v_map_projected_image = new_instancemethod(_igc_simulated.IgcSimulated__v_map_projected_image,None,IgcSimulated)
IgcSimulated_swigregister = _igc_simulated.IgcSimulated_swigregister
IgcSimulated_swigregister(IgcSimulated)



