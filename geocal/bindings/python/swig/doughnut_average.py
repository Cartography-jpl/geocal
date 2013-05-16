# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _doughnut_average.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_doughnut_average', [dirname(__file__)])
        except ImportError:
            import _doughnut_average
            return _doughnut_average
        if fp is not None:
            try:
                _mod = imp.load_module('_doughnut_average', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _doughnut_average = swig_import_helper()
    del swig_import_helper
else:
    import _doughnut_average
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


SHARED_PTR_DISOWN = _doughnut_average.SHARED_PTR_DISOWN
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

import geocal.calc_raster_multi_band
import geocal.raster_image_multi_band_variable
import geocal.raster_image_multi_band
import geocal.generic_object
class DoughnutAverage(geocal.calc_raster_multi_band.CalcRasterMultiBand):
    """
    This class does a "doughnut average" of an underlying RasterImage.

    This produces the average value of all the pixels in a given window
    size, excluding a smaller doughnut. We exclude all points that a value
    of 0 in the average (e.g., the normal gore used in VICAR images), as
    well as all points that are beyond the edge of the image. We also
    preserve gore, so if a point has a value of 0 in any of the bands then
    we set the doughnut average to 0 for that point.

    Depending on the application, you may or may not want to include
    points that have gore within the doughnut window. You can specify this
    in the constructor. If "Allow_gore" is true, then any point other
    than the center can be gore and we just exclude it from the average.
    If "Allow_gore" is false, we set a value to 0 if any value in the
    window is 0. In all cases, we set a value to 0 if the center is 0.

    This is one of the building block used to do the "cvdnorm", you can
    consult that program for details. But basically this is used to
    examine small features (smaller than the doughnut whole) that are
    significantly different than the nearby background.

    C++ includes: doughnut_average.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        DoughnutAverage::DoughnutAverage(const boost::shared_ptr< RasterImageMultiBand > &Rimg, int
        Window_size=11, int Doughnut_size=5, bool Allow_gore=true, int
        Number_tile_line=-1, int Number_tile_sample=-1, int Number_tile=4)
        Constructor.

        Parameters:
        -----------

        Rimg:  The underlying image we are producing the average for.

        Window_size:  The window size we are averaging. Should be odd number.

        Doughnut_size:  The doughnut we are excluding in the average. Should
        be odd, and < Window_size.

        Allow_gore:  If "Allow_gore" is true, then any point other than the
        center can be gore and we just exclude it from the average. If
        "Allow_gore" is false, we set a value to 0 if any value in the
        window is 0. In all cases, we set a value to 0 if the center

        Number_tile_line:  Number of lines in each tile

        Number_tile_sample:  Number of samples in each tile

        Number_tile:  Number of tiles to use 
        """
        _doughnut_average.DoughnutAverage_swiginit(self,_doughnut_average.new_DoughnutAverage(*args))
    def cvdnorm(self, *args):
        """
        Array< double, 2 > DoughnutAverage::cvdnorm(int band, int Lstart, int Sstart, int Number_line, int Number_sample)
        const
        Very closely related in the doughnut average is the cvdnorm for one of
        the bands.

        So we provide the calculation for that. 
        """
        return _doughnut_average.DoughnutAverage_cvdnorm(self, *args)

    def pandif(self, *args):
        """
        blitz::Array< double, 2 > DoughnutAverage::pandif(int Lstart, int Sstart, int Number_line, int Number_sample) const
        This is the difference of the first two cvdnorm bands. 
        """
        return _doughnut_average.DoughnutAverage_pandif(self, *args)

    def cvdnorm_raster_image(self, *args):
        """
        boost::shared_ptr< RasterImage > DoughnutAverage::cvdnorm_raster_image(int band) const
        Present the cvdnorm as a RasterImage. 
        """
        return _doughnut_average.DoughnutAverage_cvdnorm_raster_image(self, *args)

    def _v_pandif_raster_image(self):
        """
        boost::shared_ptr< RasterImage > DoughnutAverage::pandif_raster_image() const
        Present the pandif as a RasterImage. 
        """
        return _doughnut_average.DoughnutAverage__v_pandif_raster_image(self)

    @property
    def pandif_raster_image(self):
        return self._v_pandif_raster_image()

    def _v_window_size(self):
        """
        int GeoCal::DoughnutAverage::window_size() const
        The window size. 
        """
        return _doughnut_average.DoughnutAverage__v_window_size(self)

    @property
    def window_size(self):
        return self._v_window_size()

    def _v_doughnut_size(self):
        """
        int GeoCal::DoughnutAverage::doughnut_size() const
        The doughnut size. 
        """
        return _doughnut_average.DoughnutAverage__v_doughnut_size(self)

    @property
    def doughnut_size(self):
        return self._v_doughnut_size()

    def _v_allow_gore(self):
        """
        bool GeoCal::DoughnutAverage::allow_gore() const
        If true, we allow gore in a window and just exclude it in the average.

        Otherwise we set a point to 0 if any point in the window is 0. 
        """
        return _doughnut_average.DoughnutAverage__v_allow_gore(self)

    @property
    def allow_gore(self):
        return self._v_allow_gore()

    __swig_destroy__ = _doughnut_average.delete_DoughnutAverage
DoughnutAverage.cvdnorm = new_instancemethod(_doughnut_average.DoughnutAverage_cvdnorm,None,DoughnutAverage)
DoughnutAverage.pandif = new_instancemethod(_doughnut_average.DoughnutAverage_pandif,None,DoughnutAverage)
DoughnutAverage.cvdnorm_raster_image = new_instancemethod(_doughnut_average.DoughnutAverage_cvdnorm_raster_image,None,DoughnutAverage)
DoughnutAverage._v_pandif_raster_image = new_instancemethod(_doughnut_average.DoughnutAverage__v_pandif_raster_image,None,DoughnutAverage)
DoughnutAverage._v_window_size = new_instancemethod(_doughnut_average.DoughnutAverage__v_window_size,None,DoughnutAverage)
DoughnutAverage._v_doughnut_size = new_instancemethod(_doughnut_average.DoughnutAverage__v_doughnut_size,None,DoughnutAverage)
DoughnutAverage._v_allow_gore = new_instancemethod(_doughnut_average.DoughnutAverage__v_allow_gore,None,DoughnutAverage)
DoughnutAverage_swigregister = _doughnut_average.DoughnutAverage_swigregister
DoughnutAverage_swigregister(DoughnutAverage)



