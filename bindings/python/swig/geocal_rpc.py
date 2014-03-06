# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _geocal_rpc.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_geocal_rpc', [dirname(__file__)])
        except ImportError:
            import _geocal_rpc
            return _geocal_rpc
        if fp is not None:
            try:
                _mod = imp.load_module('_geocal_rpc', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _geocal_rpc = swig_import_helper()
    del swig_import_helper
else:
    import _geocal_rpc
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


SHARED_PTR_DISOWN = _geocal_rpc.SHARED_PTR_DISOWN
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
import geocal_swig.geocal_exception
def _new_rpc(cls, version, is_rpc_a, error_bias, error_random, height_offset, 
             height_scale, latitude_offset,
             latitude_scale, longitude_offset, longitude_scale,
             line_offset, line_scale, sample_offset, sample_scale,
             line_denominator, line_numerator, sample_denominator,
             sample_numerator, fit_line_numerator, fit_sample_numerator):
    if(cls.pickle_format_version() != version):
      raise RuntimeException("Class is expecting a pickled object with version number %d, but we found %d" % (cls.pickle_format_version(), version))
    rpc = Rpc()
    rpc.rpc_type = Rpc.RPC_A if(is_rpc_a) else Rpc.RPC_B
    rpc.error_bias = error_bias
    rpc.error_random = error_random				
    rpc.height_offset = height_offset
    rpc.height_scale = height_scale
    rpc.latitude_offset = latitude_offset
    rpc.latitude_scale = latitude_scale
    rpc.longitude_offset = longitude_offset
    rpc.longitude_scale = longitude_scale
    rpc.line_offset = line_offset
    rpc.line_scale = line_scale
    rpc.sample_offset = sample_offset
    rpc.sample_scale = sample_scale
    rpc.line_denominator = line_denominator
    rpc.line_numerator = line_numerator
    rpc.sample_denominator = sample_denominator
    rpc.sample_numerator = sample_numerator
    rpc.fit_line_numerator = fit_line_numerator
    rpc.fit_sample_numerator = fit_sample_numerator
    return rpc

class Rpc(geocal_swig.generic_object.GenericObject):
    """
    This is used to handle RPC (Rational Polynomial Coefficients).

    RPC is an alternative to using a rigorous camera/orbit model to map
    project satellite data.

    This is a common technique, and there are numerous references. One
    reference is Fraser, CS, Dial, G, Grodecki, J "Sensor orientation via
    RPCs" ISPRS J PHOTOGRAMM 60 (3): 182-194 MAY 2006.

    Note that there are two versions of the RPCs, type "A" and type
    "B". The difference is the order of the terms. The military tends to
    use "A" and the rest of the world tends to use "B". This class
    supports both types, based on the setting of the rpc_type variable.

    By definition of RPC, the leading term of the denominator coefficient
    should be 1.0 - so there are 20 degrees of freedom for the numerator
    but only 19 for the denominator. After some debate, I decided to do
    nothing in this class to enforce this restriction, you can assign a
    value other than 1 to the first coefficient of the denominator.
    However, this class will complain when you actually try to use this.

    The definition of line, sample and height offset and scale allow these
    numbers to be floats. However file formats such as geotiff and NITF
    treat these as integers. If you store a RPC into a file with one of
    these formats, the data will be truncated. If you plan on saving to
    one of these formats, you should be aware of this and keep these an
    whole numbers. This limitation is not present in the VICAR file
    format.

    C++ includes: geocal_rpc.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_rpc, (self.__class__, 1, self.rpc_type == Rpc.RPC_A,
    		    self.error_bias,
                        self.error_random,
    		    self.height_offset,
    		    self.height_scale,
    		    self.latitude_offset,
    		    self.latitude_scale,
    		    self.longitude_offset,
    		    self.longitude_scale,
    		    self.line_offset,
    		    self.line_scale,
    		    self.sample_offset,
    		    self.sample_scale,
    		    list(self.line_denominator),
    		    list(self.line_numerator),
    		    list(self.sample_denominator),
    		    list(self.sample_numerator),
    		    list(self.fit_line_numerator),
    		    list(self.fit_sample_numerator))

    RPC_A = _geocal_rpc.Rpc_RPC_A
    RPC_B = _geocal_rpc.Rpc_RPC_B
    rpc_type = _swig_property(_geocal_rpc.Rpc_rpc_type_get, _geocal_rpc.Rpc_rpc_type_set)
    error_bias = _swig_property(_geocal_rpc.Rpc_error_bias_get, _geocal_rpc.Rpc_error_bias_set)
    error_random = _swig_property(_geocal_rpc.Rpc_error_random_get, _geocal_rpc.Rpc_error_random_set)
    height_offset = _swig_property(_geocal_rpc.Rpc_height_offset_get, _geocal_rpc.Rpc_height_offset_set)
    height_scale = _swig_property(_geocal_rpc.Rpc_height_scale_get, _geocal_rpc.Rpc_height_scale_set)
    latitude_offset = _swig_property(_geocal_rpc.Rpc_latitude_offset_get, _geocal_rpc.Rpc_latitude_offset_set)
    latitude_scale = _swig_property(_geocal_rpc.Rpc_latitude_scale_get, _geocal_rpc.Rpc_latitude_scale_set)
    longitude_offset = _swig_property(_geocal_rpc.Rpc_longitude_offset_get, _geocal_rpc.Rpc_longitude_offset_set)
    longitude_scale = _swig_property(_geocal_rpc.Rpc_longitude_scale_get, _geocal_rpc.Rpc_longitude_scale_set)
    line_offset = _swig_property(_geocal_rpc.Rpc_line_offset_get, _geocal_rpc.Rpc_line_offset_set)
    line_scale = _swig_property(_geocal_rpc.Rpc_line_scale_get, _geocal_rpc.Rpc_line_scale_set)
    sample_offset = _swig_property(_geocal_rpc.Rpc_sample_offset_get, _geocal_rpc.Rpc_sample_offset_set)
    sample_scale = _swig_property(_geocal_rpc.Rpc_sample_scale_get, _geocal_rpc.Rpc_sample_scale_set)
    @property
    def line_denominator(self):
        return self._line_denominator()

    @line_denominator.setter
    def line_denominator(self, value):
      self._line_denominator(value)

    @property
    def line_numerator(self):
        return self._line_numerator()

    @line_numerator.setter
    def line_numerator(self, value):
      self._line_numerator(value)

    @property
    def sample_denominator(self):
        return self._sample_denominator()

    @sample_denominator.setter
    def sample_denominator(self, value):
      self._sample_denominator(value)

    @property
    def sample_numerator(self):
        return self._sample_numerator()

    @sample_numerator.setter
    def sample_numerator(self, value):
      self._sample_numerator(value)

    @property
    def fit_line_numerator(self):
        return self._fit_line_numerator()

    @fit_line_numerator.setter
    def fit_line_numerator(self, value):
      self._fit_line_numerator(value)

    @property
    def fit_sample_numerator(self):
        return self._fit_sample_numerator()

    @fit_sample_numerator.setter
    def fit_sample_numerator(self, value):
      self._fit_sample_numerator(value)

    def resolution_meter(self, *args):
        """
        double Rpc::resolution_meter(const Dem &D) const
        Calculate the approximate resolution on the ground.

        This finds the intersection for the center pixel with the surface + 1
        in the line and sample direction. We find the difference in meters
        between these points, and select the maximum value. We the height from
        the Dem. 
        """
        return _geocal_rpc.Rpc_resolution_meter(self, *args)

    def fit(self, *args):
        """
        void Rpc::fit(const std::vector< boost::shared_ptr< GroundCoordinate > > &Gc, const
        std::vector< ImageCoordinate > &Ic, const std::vector< double >
        &Line_sigma, const std::vector< double > &Sample_sigma, std::set< int
        > &Blunder, double Blunder_threshold=3, double Chisq_threshold=0.8,
        bool Blunder_detect=true)
        This updates the RPC parameters to better match a given set of data.

        This class takes a list of GroundCoordinates, and the ImageCoordinate
        that should be returned by the RPC. Each of the ImageCoordinate has an
        associated line and sample sigma used to weight it (i.e., the line and
        sigma returned by ImageMatcher).

        We update the set of parameters given by fit_line_numerator and
        fit_sample_numerator. Right now, we only do a linear fit and don't try
        to fit the nonlinear denominator pieces. We can add that in the future
        if it is needed.

        The blunder detection used is iterative data snooping, where the point
        with the largest standardized residual is removed. If the Chisq of the
        match improves by the Chisq_threshold, we toss the point out,
        otherwise we keep it. We then proceed to the next largest residual,
        and so.

        However, if we expect large numbers of blunders in otherwise well
        matched data (e.g., image matching between two images with lots of
        differences), it might be preferable to toss out all large outliers.
        So, if Chisq_threshold is >= 1 we just toss out all points outside of
        the Blunder_threshold. 
        """
        return _geocal_rpc.Rpc_fit(self, *args)

    def fit_all(self, *args):
        """
        void Rpc::fit_all(const std::vector< double > &Line, const std::vector< double >
        &Sample, const std::vector< double > &Latitude, const std::vector<
        double > &Longitude, const std::vector< double > &Height)
        You may not want to use this function directly, but rather use
        Rpc::generate_rpc which sets up the entire RPC.

        This is a low level function used to create an RPC that matches a set
        of data. The RPC should have already had the various offset and scales
        filled in. This function then fills in line_denominator,
        line_numerator, sample_denominator, and sample_numerator.

        This fits all of the rpc parameters (the full 78 parameters) doing a
        linear least squares fit. This doesn't to any blunder detection, or
        allow a subset to be used.

        The idea is that through some other method you can determine latitude,
        longitude, height vs line, sample (e.g., using a Orbit and Camera).
        You then generate a regular set of points, set up the offset and
        scale, and finally call this function to fit the RPC. 
        """
        return _geocal_rpc.Rpc_fit_all(self, *args)

    def generate_rpc(*args):
        """
        Rpc Rpc::generate_rpc(const ImageGroundConnection &Igc, double Min_height, double
        Max_height, int Nlat=20, int Nlon=20, int Nheight=20, bool
        Skip_masked_point=false, bool Ignore_error=false)
        Generate a RPC that approximates the calculation done by a
        ImageGroundConnection.

        We determine that latitude and longitude range to use automatically to
        cover the range given by the ImageGroundConnection.

        This routine always ignores ImageGroundConnectionFailed exceptions,
        and just skips to the next point. But if we are using python code for
        the ImageGroundConnection we can't translate errors to
        ImageGroundConnectionFailed (this is a limitation of SWIG). So you can
        optionally specify Ignore_error as true, in which case we ignore all
        exceptions and just skip to the next point.

        We normally look at all image points when generating the RPC. You can
        optionally specify Skip_masked_point to skip all image points that are
        masked. 
        """
        return _geocal_rpc.Rpc_generate_rpc(*args)

    generate_rpc = staticmethod(generate_rpc)
    def ground_coordinate(self, *args):
        """
        Geodetic Rpc::ground_coordinate(const ImageCoordinate &Ic, double Height) const
        Invert the RPC to find the ground coordinate that maps to a give
        ImageCoordinate.

        This routine may fail to find a solution, in which case a
        ConvergenceFailure exception will be thrown. 
        """
        return _geocal_rpc.Rpc_ground_coordinate(self, *args)

    def image_coordinate(self, *args):
        """
        blitz::Array<double, N + 1> GeoCal::Rpc::image_coordinate(const blitz::Array< double, N > &Latitude, const blitz::Array<
        double, N > &Longitude, const blitz::Array< double, N >
        &Height_ellipsoid) const
        Convert from object space to image space (i.e., calculate RPC).

        The longitude and latitude are given in degrees (this is the geodetic
        latitude). The height is in meters, relative to the reference
        ellipsoid (e.g., WGS-84).

        This converts a whole block of latitude, longitude and height points
        at once. The results returned have a first dimension of 2, the first
        value is line and the second sample. 
        """
        return _geocal_rpc.Rpc_image_coordinate(self, *args)

    def image_coordinate_jac_parm(self, *args):
        """
        blitz::Array<double, 2> GeoCal::Rpc::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
        Jacobian of image_coordinate with respect to the parameters that are
        marked as ones we are fitting.

        This has two rows, the first is for line and the second sample. The
        columns are first the true values in fit_line_numerator, and then
        fit_sample_numerator. 
        """
        return _geocal_rpc.Rpc_image_coordinate_jac_parm(self, *args)

    def image_coordinate_jac(self, *args):
        """
        blitz::Array<double, 2> GeoCal::Rpc::image_coordinate_jac(double Latitude, double Longitude, double Height_ellipsoid) const
        Jacobian of image_coordinate with respect to the Latitude, Longitude,
        and Height_ellipsoid.

        This is a 2 x 3 matrix, with the first row being line and the second
        sample. The columns are in Latitude, Longitude and Height_ellipsoid
        order. 
        """
        return _geocal_rpc.Rpc_image_coordinate_jac(self, *args)

    def rpc_project(self, *args):
        """
        void Rpc::rpc_project(RasterImage &Res, const RasterImage &Img, const Dem &D, double
        Line_scale=1.0, double Sample_scale=1.0) const
        This uses the RPC to project a RasterImage to a MapInfo.

        On entry, the RasterImage Res should already have the size and MapInfo
        that we want to project to.

        In some cases, Img might be a lower resolution image than is described
        by the RPC (e.g., we are imaging matching 60 cm Quickbird data to 5 m
        CIB data, so we down sample by a factor of 9). In those cases, a scale
        to apply to the line and sample returned by the RPC before looking
        into Img can be given. The default value is to not scale. 
        """
        return _geocal_rpc.Rpc_rpc_project(self, *args)

    def rpc_type_a(self):
        """
        Rpc Rpc::rpc_type_a() const
        This return a Rpc that has type RPC_A.

        If this rpc is already type RPC_A, then this is just a copy. Otherwise
        we rearrange the coefficients to give a type RPC_A. 
        """
        return _geocal_rpc.Rpc_rpc_type_a(self)

    def rpc_type_b(self):
        """
        Rpc Rpc::rpc_type_b() const
        This return a Rpc that has type RPC_B.

        If this rpc is already type RPC_B, then this is just a copy. Otherwise
        we rearrange the coefficients to give a type RPC_B. 
        """
        return _geocal_rpc.Rpc_rpc_type_b(self)

    def __init__(self): 
        """
        Rpc::Rpc()
        Default constructor.

        Initializes fit_line_numerator and fit_sample_numerator to all false.

        """
        _geocal_rpc.Rpc_swiginit(self,_geocal_rpc.new_Rpc())
    __swig_destroy__ = _geocal_rpc.delete_Rpc
Rpc.__str__ = new_instancemethod(_geocal_rpc.Rpc___str__,None,Rpc)
Rpc._line_denominator = new_instancemethod(_geocal_rpc.Rpc__line_denominator,None,Rpc)
Rpc._line_numerator = new_instancemethod(_geocal_rpc.Rpc__line_numerator,None,Rpc)
Rpc._sample_denominator = new_instancemethod(_geocal_rpc.Rpc__sample_denominator,None,Rpc)
Rpc._sample_numerator = new_instancemethod(_geocal_rpc.Rpc__sample_numerator,None,Rpc)
Rpc._fit_line_numerator = new_instancemethod(_geocal_rpc.Rpc__fit_line_numerator,None,Rpc)
Rpc._fit_sample_numerator = new_instancemethod(_geocal_rpc.Rpc__fit_sample_numerator,None,Rpc)
Rpc.resolution_meter = new_instancemethod(_geocal_rpc.Rpc_resolution_meter,None,Rpc)
Rpc.fit = new_instancemethod(_geocal_rpc.Rpc_fit,None,Rpc)
Rpc.fit_all = new_instancemethod(_geocal_rpc.Rpc_fit_all,None,Rpc)
Rpc.ground_coordinate = new_instancemethod(_geocal_rpc.Rpc_ground_coordinate,None,Rpc)
Rpc.image_coordinate = new_instancemethod(_geocal_rpc.Rpc_image_coordinate,None,Rpc)
Rpc.image_coordinate_jac_parm = new_instancemethod(_geocal_rpc.Rpc_image_coordinate_jac_parm,None,Rpc)
Rpc.image_coordinate_jac = new_instancemethod(_geocal_rpc.Rpc_image_coordinate_jac,None,Rpc)
Rpc.rpc_project = new_instancemethod(_geocal_rpc.Rpc_rpc_project,None,Rpc)
Rpc.rpc_type_a = new_instancemethod(_geocal_rpc.Rpc_rpc_type_a,None,Rpc)
Rpc.rpc_type_b = new_instancemethod(_geocal_rpc.Rpc_rpc_type_b,None,Rpc)
Rpc_swigregister = _geocal_rpc.Rpc_swigregister
Rpc_swigregister(Rpc)

def Rpc_generate_rpc(*args):
  """
    Rpc Rpc::generate_rpc(const ImageGroundConnection &Igc, double Min_height, double
    Max_height, int Nlat=20, int Nlon=20, int Nheight=20, bool
    Skip_masked_point=false, bool Ignore_error=false)
    Generate a RPC that approximates the calculation done by a
    ImageGroundConnection.

    We determine that latitude and longitude range to use automatically to
    cover the range given by the ImageGroundConnection.

    This routine always ignores ImageGroundConnectionFailed exceptions,
    and just skips to the next point. But if we are using python code for
    the ImageGroundConnection we can't translate errors to
    ImageGroundConnectionFailed (this is a limitation of SWIG). So you can
    optionally specify Ignore_error as true, in which case we ignore all
    exceptions and just skip to the next point.

    We normally look at all image points when generating the RPC. You can
    optionally specify Skip_masked_point to skip all image points that are
    masked. 
    """
  return _geocal_rpc.Rpc_generate_rpc(*args)



