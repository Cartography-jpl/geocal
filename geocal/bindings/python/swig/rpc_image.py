# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _rpc_image.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_rpc_image', [dirname(__file__)])
        except ImportError:
            import _rpc_image
            return _rpc_image
        if fp is not None:
            try:
                _mod = imp.load_module('_rpc_image', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _rpc_image = swig_import_helper()
    del swig_import_helper
else:
    import _rpc_image
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


SHARED_PTR_DISOWN = _rpc_image.SHARED_PTR_DISOWN
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

import geocal.raster_image_variable
import geocal.raster_image
import geocal.generic_object
class RpcImage(geocal.raster_image_variable.RasterImageVariable):
    """
    This handles both an Rpc, and the RasterImage data the Rpc map
    projects.

    Some of the data we work with is high resolution (e.g., Quickbird at
    60 cm). For some purposes, we may want to work with a lower resolution
    version (e.g., image match with CIB data at 5 m). This class allows
    the resolution of the map projection to be given, in which case we
    will average the underlying RasterImage using RasterAveraged, before
    doing the map projection.

    We calculate imagery on demand. This means the imagery is always up to
    date, so changes to the Rpc get reflected in the imagery. However, if
    you are going to be accessing the same data more than once w/o
    updating the Rpc, you'll want to generate a static copy so you don't
    repeatedly calculate the same data over and over e.g., use a
    MemoryRasterImage to copy the data into memory.

    As you update the Rpc parameters, the area covered on the surface
    changes. This class does not update the MapInfo describing the area
    covered. However, you can call map_info_bounding_update() to update
    this information if desired.

    C++ includes: rpc_image.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        RpcImage::RpcImage(const boost::shared_ptr< RasterImage > &Raw_img, const Rpc &R, const
        boost::shared_ptr< Dem > &D, const MapInfo &Mi, int Avg_fact=-1)
        Constructor.

        We either average the raw data by the given factor, or we take the
        ratio of the Mi resolution and the Rpc resolution. 
        """
        _rpc_image.RpcImage_swiginit(self,_rpc_image.new_RpcImage(*args))
    def _v_dem(self):
        """
        boost::shared_ptr<Dem> GeoCal::RpcImage::dem_ptr() const

        """
        return _rpc_image.RpcImage__v_dem(self)

    @property
    def dem(self):
        return self._v_dem()

    def map_info_bounding(self):
        """
        MapInfo RpcImage::map_info_bounding() const
        Create a MapInfo that completely covers projection of the raw_image to
        the surface using the current Rpc values.

        This used the same CoordinateConverter, pixel_x_resolution, and
        pixel_y_resolution as the current map_info() - we just change the
        corners of the MapInfo to contain the data. Note that this just
        calculates the MapInfo, it doesn't actually set the map_info() to this
        new value. For that, use map_info_bounding_update. 
        """
        return _rpc_image.RpcImage_map_info_bounding(self)

    def map_info_bounding_update(self):
        """
        void RpcImage::map_info_bounding_update()
        This calls map_info_bounding(), and then sets the map_info() for this
        object to the results of that call. 
        """
        return _rpc_image.RpcImage_map_info_bounding_update(self)

    def _v_raw_image(self):
        """
        boost::shared_ptr<RasterImage> GeoCal::RpcImage::raw_image_ptr() const

        """
        return _rpc_image.RpcImage__v_raw_image(self)

    @property
    def raw_image(self):
        return self._v_raw_image()

    def read_ptr(self, *args):
        """
        void RpcImage::read_ptr(int Lstart, int Sstart, int Number_line, int Number_sample, int *Res)
        const
        Return a subset of the image. 
        """
        return _rpc_image.RpcImage_read_ptr(self, *args)

    def fit(self, *args):
        """
        void RpcImage::fit(const RasterImage &Ref_img, double Max_diff=1000, const Mask
        &M=CombinedMask(), const FeatureDetector
        &Fd=ForstnerFeatureDetector(), const ImageMatcher
        &Coarse_im=CcorrMatcher(), const ImageMatcher
        &Fine_im=CcorrLsmMatcher(true), double Match_search=15, int
        Feature_grid_space=100, double Blunder_threshold=3, double
        Chisq_threshold=1.0, bool Blunder_detect=true)
        This does image matching with a Ref_img, and adjusts the Rpc
        coefficients to give a better match between this map projected imagery
        and the reference imagery.

        This is done in two step. First we do a coarse fit to get the map
        projected roughly right, and then we do a finer adjustment to give a
        better fit.

        The first step is the coarse fit. Since an Rpc is usually based on
        navigation data only, without and ground control points, it can give
        relatively large errors. We do a coarse shift to the Rpc to reduce the
        errors enough that we can do a finer fit against the Ref_img.

        We first averages the image and the Ref_img. We average so that we are
        coarse enough that the given image matching search window size (given
        by Match_search) will contain errors up to Max_diff meters. For
        example, with a 1000 m Max_diff, a search window of Match_search, and
        5 m Ref_img, we would average by a factor of 14 (5 m x 14 factor x 15
        search window = 1050 m error, allowing us to find 1000 m differences).

        We use the FeatureDetector Fd to determine where to match, the areas
        we look for a feature is in each grid square of Feature_grid_space x
        Feature_grid_space. We then match at these features between the
        reference and map projected images. We use the supplied Mask to mask
        out areas that we should not do matching on (e.g., water, clouds). The
        default Mask doesn't mask any areas.

        We update the line_offset and sample_offset only of rpc() to reduce
        the difference between the map projected image and the Ref_img. This
        functions does not fit the other parameters given by
        rpc().fit_line_numerator and rpc().fit_sample_numerator.

        We repeat the coarse fit until the remaining differences between the
        map projected data and the Ref_img fall within Match_search at full
        resolution.

        We follow this with a fine fit. We again use the FeatureDetector Fd to
        determine where to match, the areas we look for a feature is in each
        grid square of Feature_grid_space x Feature_grid_space. We then match
        at these features between the reference and map projected images.

        We fit for the Rpc parameters given by rpc().fit_line_numerator and
        rpc().fit_sample_numerator.

        This fine matching step is repeated until the change in average
        difference from one step to the next is less than a tolerance (5%
        change). 
        """
        return _rpc_image.RpcImage_fit(self, *args)

    def reference_image_ground_point(self):
        """
        const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::reference_image_ground_point() const
        Return the location in the reference image used in the final iteration
        of fit, for diagnostic purposes.

        Note that you should also look at blunder, as some of these points
        might be marked as blunders. 
        """
        return _rpc_image.RpcImage_reference_image_ground_point(self)

    def new_image_ground_point(self):
        """
        const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::new_image_ground_point() const
        Return the location in the new image used in the final iteration of
        fit, for diagnostic purposes.

        Note that you should also look at blunder, as some of these points
        might be marked as blunders. 
        """
        return _rpc_image.RpcImage_new_image_ground_point(self)

    def distance_new_and_reference(self):
        """
        const std::vector<double>& GeoCal::RpcImage::distance_new_and_reference() const
        Return distance in meters between reference_image_ground_point and
        new_image_ground_point, for diagnostic purposes.

        This is a measure of how much the images disagree after the fitting
        done by fit(). Note that you should also look at blunder, as some of
        these points might be marked as blunders. 
        """
        return _rpc_image.RpcImage_distance_new_and_reference(self)

    def blunder(self):
        """
        const std::set<int> GeoCal::RpcImage::blunder() const
        Points in the last iteration of fit() that were marked as blunders. 
        """
        return _rpc_image.RpcImage_blunder(self)

    def failed_ground_point(self):
        """
        const std::vector<boost::shared_ptr<GroundCoordinate> >& GeoCal::RpcImage::failed_ground_point() const
        Return the location in the reference image used in the final iteration
        of fit, for diagnostic purposes.

        This point had image matching fail on it, see image_match_diagnostic
        for code as to why it failed. 
        """
        return _rpc_image.RpcImage_failed_ground_point(self)

    def image_match_diagnostic(self):
        """
        const std::vector<int>& GeoCal::RpcImage::image_match_diagnostic() const
        Return the code indicating why an image matching failed. 
        """
        return _rpc_image.RpcImage_image_match_diagnostic(self)

    __swig_destroy__ = _rpc_image.delete_RpcImage
RpcImage._v_dem = new_instancemethod(_rpc_image.RpcImage__v_dem,None,RpcImage)
RpcImage.map_info_bounding = new_instancemethod(_rpc_image.RpcImage_map_info_bounding,None,RpcImage)
RpcImage.map_info_bounding_update = new_instancemethod(_rpc_image.RpcImage_map_info_bounding_update,None,RpcImage)
RpcImage._v_raw_image = new_instancemethod(_rpc_image.RpcImage__v_raw_image,None,RpcImage)
RpcImage.__call__ = new_instancemethod(_rpc_image.RpcImage___call__,None,RpcImage)
RpcImage.read_ptr = new_instancemethod(_rpc_image.RpcImage_read_ptr,None,RpcImage)
RpcImage.fit = new_instancemethod(_rpc_image.RpcImage_fit,None,RpcImage)
RpcImage.reference_image_ground_point = new_instancemethod(_rpc_image.RpcImage_reference_image_ground_point,None,RpcImage)
RpcImage.new_image_ground_point = new_instancemethod(_rpc_image.RpcImage_new_image_ground_point,None,RpcImage)
RpcImage.distance_new_and_reference = new_instancemethod(_rpc_image.RpcImage_distance_new_and_reference,None,RpcImage)
RpcImage.blunder = new_instancemethod(_rpc_image.RpcImage_blunder,None,RpcImage)
RpcImage.failed_ground_point = new_instancemethod(_rpc_image.RpcImage_failed_ground_point,None,RpcImage)
RpcImage.image_match_diagnostic = new_instancemethod(_rpc_image.RpcImage_image_match_diagnostic,None,RpcImage)
RpcImage_swigregister = _rpc_image.RpcImage_swigregister
RpcImage_swigregister(RpcImage)



