# This file was automatically generated by SWIG (http://www.swig.org).
# Version 2.0.9
#
# Do not make changes to this file unless you know what you are doing--modify
# the SWIG interface file instead.



from sys import version_info
if version_info >= (3,0,0):
    new_instancemethod = lambda func, inst, cls: _image_ground_connection.SWIG_PyInstanceMethod_New(func)
else:
    from new import instancemethod as new_instancemethod
if version_info >= (2,6,0):
    def swig_import_helper():
        from os.path import dirname
        import imp
        fp = None
        try:
            fp, pathname, description = imp.find_module('_image_ground_connection', [dirname(__file__)])
        except ImportError:
            import _image_ground_connection
            return _image_ground_connection
        if fp is not None:
            try:
                _mod = imp.load_module('_image_ground_connection', fp, pathname, description)
            finally:
                fp.close()
            return _mod
    _image_ground_connection = swig_import_helper()
    del swig_import_helper
else:
    import _image_ground_connection
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


SHARED_PTR_DISOWN = _image_ground_connection.SHARED_PTR_DISOWN
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
import geocal_swig.with_parameter
import geocal_swig.geocal_exception
class ImageGroundConnectionFailed(geocal_swig.geocal_exception.Exception):
    """
    Exception thrown if ImageGroundConnection fails to calculate a image
    coordinate.

    C++ includes: image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ImageGroundConnectionFailed::ImageGroundConnectionFailed(const std::string &W="ImageGroundConnectionFailed")
        Default constructor.

        Can give an optional string describing the error. 
        """
        _image_ground_connection.ImageGroundConnectionFailed_swiginit(self,_image_ground_connection.new_ImageGroundConnectionFailed(*args))
    __swig_destroy__ = _image_ground_connection.delete_ImageGroundConnectionFailed
ImageGroundConnectionFailed_swigregister = _image_ground_connection.ImageGroundConnectionFailed_swigregister
ImageGroundConnectionFailed_swigregister(ImageGroundConnectionFailed)

class ImageGroundConnection(geocal_swig.with_parameter.WithParameter):
    """
    Depending on the the data we are using, we may connect a location in
    an image to a ground location in one of several ways.

    For example, we might have a rigorous camera model and orbit data
    allowing us to use an Ipi. Or we might have a RPC (Rational Polynomial
    Coefficients) that connect an image to the ground. Or we might be
    working with data that has already been map projected, so a MapInfo
    gives us a direct connection.

    For many purposes, we don't care exactly how the connection is done,
    we just use the fact that there is a connection.

    This class gives a generic interface that can be used for any kind of
    a connection between the ground and an image.

    C++ includes: image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        if self.__class__ == ImageGroundConnection:
            _self = None
        else:
            _self = self
        _image_ground_connection.ImageGroundConnection_swiginit(self,_image_ground_connection.new_ImageGroundConnection(_self, *args))
    __swig_destroy__ = _image_ground_connection.delete_ImageGroundConnection
    def cf_look_vector_lv(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixedLookVector> GeoCal::ImageGroundConnection::cf_look_vector_lv(const ImageCoordinate &Ic) const
        SWIG/python doesn't like returning 2 items through a director, so we
        implement cf_look_vector in 2 parts.

        In general, C++ to override cf_look_vector rather than these 2
        functions (although it could do these 2 if useful for some reason. 
        """
        return _image_ground_connection.ImageGroundConnection_cf_look_vector_lv(self, *args)

    def cf_look_vector_pos(self, *args):
        """
        virtual boost::shared_ptr<CartesianFixed> GeoCal::ImageGroundConnection::cf_look_vector_pos(const ImageCoordinate &Ic) const

        """
        return _image_ground_connection.ImageGroundConnection_cf_look_vector_pos(self, *args)

    def __ground_coordinate(self, *args):
        """
        virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate(const ImageCoordinate &Ic) const
        Return ground coordinate that goes with a particular image coordinate.

        """
        return _image_ground_connection.ImageGroundConnection___ground_coordinate(self, *args)

    def ground_coordinate_dem(self, *args):
        """
        virtual boost::shared_ptr<GroundCoordinate> GeoCal::ImageGroundConnection::ground_coordinate_dem(const ImageCoordinate &Ic, const Dem &D) const =0
        Return ground coordinate that goes with a particular image coordinate.

        This version supplies a Dem to use. 
        """
        return _image_ground_connection.ImageGroundConnection_ground_coordinate_dem(self, *args)

    def ground_coordinate_approx_height(self, *args):
        """
        boost::shared_ptr< GroundCoordinate > ImageGroundConnection::ground_coordinate_approx_height(const ImageCoordinate &Ic, double H) const
        Return ground coordinate that is nearly the given height above the
        reference surface.

        This is exact in the sense that the returned point matches the image
        coordinate (to igc.image_coordinate(res) = Ic up to roundoff errors),
        but it is approximate in the sense that the height might not be
        exactly the supplied height. This is similar to
        CartesianFixed::reference_surface_intersect_approximate. A particular
        implementation can be much faster than ground_coordinate_dem, since it
        doesn't need to do ray tracing. 
        """
        return _image_ground_connection.ImageGroundConnection_ground_coordinate_approx_height(self, *args)

    def image_coordinate(self, *args):
        """
        virtual ImageCoordinate GeoCal::ImageGroundConnection::image_coordinate(const GroundCoordinate &Gc) const =0
        Return image coordinate that goes with a particular GroundCoordinate.

        For some types of ImageGroundConnection, we might not be able to
        calculate image_coordinate for all values (e.g., Ipi might fail). In
        those cases, we will throw a ImageGroundConnectionFailed exception.
        This means that nothing is wrong, other than that we can't calculate
        the image_coordinate. Callers can catch this exception if they have
        some way of handling no image coordinate data. 
        """
        return _image_ground_connection.ImageGroundConnection_image_coordinate(self, *args)

    def image_coordinate_jac_cf(self, *args):
        """
        blitz::Array< double, 2 > ImageGroundConnection::image_coordinate_jac_cf(const CartesianFixed &Gc) const
        Return the Jacobian of the image coordinates with respect to the X, Y,
        and Z components of the CartesianFixed ground location. 
        """
        return _image_ground_connection.ImageGroundConnection_image_coordinate_jac_cf(self, *args)

    def image_coordinate_jac_parm(self, *args):
        """
        virtual blitz::Array<double, 2> GeoCal::ImageGroundConnection::image_coordinate_jac_parm(const GroundCoordinate &Gc) const
        Return the Jacobian of the image coordinates with respect to the
        parameters. 
        """
        return _image_ground_connection.ImageGroundConnection_image_coordinate_jac_parm(self, *args)

    def cover(self, *args):
        """
        MapInfo ImageGroundConnection::cover(const MapInfo &Mi, int boundary=0) const
        Find a MapInfo that covers the ground coordinate of this
        ImageGroundConnection.

        We calculate the ground coordinate of the four corners, then find the
        MapInfo that covers those corners, optionally adding a boundary in map
        pixels. Depending on the actual warping involved, this may or may not
        fully cover the edges of the image. 
        """
        return _image_ground_connection.ImageGroundConnection_cover(self, *args)

    def _v_image(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::image(const boost::shared_ptr< RasterImage > &Img)

        """
        return _image_ground_connection.ImageGroundConnection__v_image(self, *args)

    @property
    def image(self):
        return self._v_image()

    @image.setter
    def image(self, value):
      self._v_image(value)

    def _v_image_multi_band(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::image_multi_band(const boost::shared_ptr< RasterImageMultiBand > &Img_mb)

        """
        return _image_ground_connection.ImageGroundConnection__v_image_multi_band(self, *args)

    @property
    def image_multi_band(self):
        return self._v_image_multi_band()

    @image_multi_band.setter
    def image_multi_band(self, value):
      self._v_image_multi_band(value)

    def _v_image_mask(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::image_mask(const boost::shared_ptr< ImageMask > &Image_mask)

        """
        return _image_ground_connection.ImageGroundConnection__v_image_mask(self, *args)

    @property
    def image_mask(self):
        return self._v_image_mask()

    @image_mask.setter
    def image_mask(self, value):
      self._v_image_mask(value)

    def _v_ground_mask(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::ground_mask(const boost::shared_ptr< GroundMask > &Ground_mask)

        """
        return _image_ground_connection.ImageGroundConnection__v_ground_mask(self, *args)

    @property
    def ground_mask(self):
        return self._v_ground_mask()

    @ground_mask.setter
    def ground_mask(self, value):
      self._v_ground_mask(value)

    def _v_number_line(self):
        """
        virtual int GeoCal::ImageGroundConnection::number_line() const
        Return number of lines in underlying image. 
        """
        return _image_ground_connection.ImageGroundConnection__v_number_line(self)

    @property
    def number_line(self):
        return self._v_number_line()

    def _v_number_sample(self):
        """
        virtual int GeoCal::ImageGroundConnection::number_sample() const
        Return number of samples in underlying image. 
        """
        return _image_ground_connection.ImageGroundConnection__v_number_sample(self)

    @property
    def number_sample(self):
        return self._v_number_sample()

    def _v_number_band(self):
        """
        virtual int GeoCal::ImageGroundConnection::number_band() const
        Return number of bands. 
        """
        return _image_ground_connection.ImageGroundConnection__v_number_band(self)

    @property
    def number_band(self):
        return self._v_number_band()

    def _v_title(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::title(const std::string &Title)
        Set image title. 
        """
        return _image_ground_connection.ImageGroundConnection__v_title(self, *args)

    @property
    def title(self):
        return self._v_title()

    @title.setter
    def title(self, value):
      self._v_title(value)

    def _v_has_time(self):
        """
        virtual bool GeoCal::ImageGroundConnection::has_time() const
        Not all ImageGroundConnection has a time associated with each pixel
        (for example, one based on an underlying existing map).

        This indicates if we have the functionality. 
        """
        return _image_ground_connection.ImageGroundConnection__v_has_time(self)

    @property
    def has_time(self):
        return self._v_has_time()

    def resolution_meter(self, *args):
        """
        double ImageGroundConnection::resolution_meter() const
        Variation of resolution_meter that find the resolution of the center
        pixel. 
        """
        return _image_ground_connection.ImageGroundConnection_resolution_meter(self, *args)

    def _v_parameter(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::parameter(const blitz::Array< double, 1 > &P)

        """
        return _image_ground_connection.ImageGroundConnection__v_parameter(self, *args)

    @property
    def parameter(self):
        return self._v_parameter()

    @parameter.setter
    def parameter(self, value):
      self._v_parameter(value)

    def _v_parameter_with_derivative(self, *args):
        """
        virtual void GeoCal::ImageGroundConnection::parameter_with_derivative(const ArrayAd< double, 1 > &P)

        """
        return _image_ground_connection.ImageGroundConnection__v_parameter_with_derivative(self, *args)

    @property
    def parameter_with_derivative(self):
        return self._v_parameter_with_derivative()

    @parameter_with_derivative.setter
    def parameter_with_derivative(self, value):
      self._v_parameter_with_derivative(value)

    @property
    def parameter_name(self):
        return self._v_parameter_name()

    @property
    def parameter_subset(self):
        return self._v_parameter_subset()

    @parameter_subset.setter
    def parameter_subset(self, value):
      self._v_parameter_subset(value)

    @property
    def parameter_with_derivative_subset(self):
        return self._v_parameter_with_derivative_subset()

    @parameter_with_derivative_subset.setter
    def parameter_with_derivative_subset(self, value):
      self._v_parameter_with_derivative_subset(value)

    @property
    def parameter_name_subset(self):
        return self._v_parameter_name_subset()

    @property
    def parameter_mask(self):
        return self._v_parameter_mask()

    def cf_look_vector_arr(self, *args):
        """
        blitz::Array< double, 7 > ImageGroundConnection::cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp, int
        nsubpixel_line=1, int nsubpixel_sample=1, int nintegration_step=1)
        const
        Return an array of look vector information.

        This is really intended for use with ray casting or with python, where
        calling cf_look_vector repeatedly is costly. This is nline x nsamp x x
        nsub_line x nsub_sample x nintegration_step x 2 x 3 in size, where we
        give the position first followed by the look vector.

        In general, the number of integration steps doesn't have any meaning
        and we just repeat the data if the number of integration steps is
        something other than 1. But for certain ImageGroundConnection, this
        may have meaning (e.g., anything where we have a camera and orbit
        data).

        The default implementation just calls cf_look_vector repeatedly, but a
        derived class can make any kind of optimization that is appropriate.

        Note a subtle difference between this and ground_coordinate. A camera
        may have a footprint that overlaps from one line to the next. For
        example, MISR has a pixel spacing of 275 meter, but the footprint may
        be as much as 700 meter on the ground (for D camera in line
        direction). The cf_look_vector_arr with the subpixels refer to the
        actual camera, i.e., the 750 meter footprint. This means that in
        general the results of calling ground_coordinate (which corresponds to
        the 275 meter pixel spacing) won't match the location you get from
        cf_look_vector_arr. This is intended, not a bug, and simple reflects
        that we are talking about 2 different things here.

        This is really intended for use with python. This is nline x nsamp x 2
        x 3 in size, where we give the position first followed by the look
        vector. 
        """
        return _image_ground_connection.ImageGroundConnection_cf_look_vector_arr(self, *args)

    def _v_dem(self):
        """
        const boost::shared_ptr<Dem>& GeoCal::ImageGroundConnection::dem_ptr() const
        DEM used by ground_coordinate. 
        """
        return _image_ground_connection.ImageGroundConnection__v_dem(self)

    @property
    def dem(self):
        return self._v_dem()

    def __dem(self, *args):
        """
        const Dem& GeoCal::ImageGroundConnection::dem() const
        Dem used by ground_coordinate. 
        """
        return _image_ground_connection.ImageGroundConnection___dem(self, *args)

    @dem.setter
    def dem(self, value):  
      self.__dem(value)

    def cf_look_vector(self, ic):
      return (self.cf_look_vector_lv(ic), self.cf_look_vector_pos(ic))

    def ground_coordinate(self, ic, dem = None):
      '''Return ground coordinate for the given image coordinate. You can specify
       a dem to use, or we use the dem associated with the class.'''
      if(dem):
        return self.ground_coordinate_dem(ic, dem)
      else:
        return self.__ground_coordinate(ic)

    def __disown__(self):
        self.this.disown()
        _image_ground_connection.disown_ImageGroundConnection(self)
        return weakref_proxy(self)
ImageGroundConnection.initialize = new_instancemethod(_image_ground_connection.ImageGroundConnection_initialize,None,ImageGroundConnection)
ImageGroundConnection.cf_look_vector_lv = new_instancemethod(_image_ground_connection.ImageGroundConnection_cf_look_vector_lv,None,ImageGroundConnection)
ImageGroundConnection.cf_look_vector_pos = new_instancemethod(_image_ground_connection.ImageGroundConnection_cf_look_vector_pos,None,ImageGroundConnection)
ImageGroundConnection.__ground_coordinate = new_instancemethod(_image_ground_connection.ImageGroundConnection___ground_coordinate,None,ImageGroundConnection)
ImageGroundConnection.ground_coordinate_dem = new_instancemethod(_image_ground_connection.ImageGroundConnection_ground_coordinate_dem,None,ImageGroundConnection)
ImageGroundConnection.ground_coordinate_approx_height = new_instancemethod(_image_ground_connection.ImageGroundConnection_ground_coordinate_approx_height,None,ImageGroundConnection)
ImageGroundConnection.image_coordinate = new_instancemethod(_image_ground_connection.ImageGroundConnection_image_coordinate,None,ImageGroundConnection)
ImageGroundConnection.image_coordinate_jac_cf = new_instancemethod(_image_ground_connection.ImageGroundConnection_image_coordinate_jac_cf,None,ImageGroundConnection)
ImageGroundConnection.image_coordinate_jac_parm = new_instancemethod(_image_ground_connection.ImageGroundConnection_image_coordinate_jac_parm,None,ImageGroundConnection)
ImageGroundConnection.cover = new_instancemethod(_image_ground_connection.ImageGroundConnection_cover,None,ImageGroundConnection)
ImageGroundConnection._v_image = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_image,None,ImageGroundConnection)
ImageGroundConnection._v_image_multi_band = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_image_multi_band,None,ImageGroundConnection)
ImageGroundConnection._v_image_mask = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_image_mask,None,ImageGroundConnection)
ImageGroundConnection._v_ground_mask = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_ground_mask,None,ImageGroundConnection)
ImageGroundConnection._v_number_line = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_number_line,None,ImageGroundConnection)
ImageGroundConnection._v_number_sample = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_number_sample,None,ImageGroundConnection)
ImageGroundConnection._v_number_band = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_number_band,None,ImageGroundConnection)
ImageGroundConnection._v_title = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_title,None,ImageGroundConnection)
ImageGroundConnection._v_has_time = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_has_time,None,ImageGroundConnection)
ImageGroundConnection.__str__ = new_instancemethod(_image_ground_connection.ImageGroundConnection___str__,None,ImageGroundConnection)
ImageGroundConnection.resolution_meter = new_instancemethod(_image_ground_connection.ImageGroundConnection_resolution_meter,None,ImageGroundConnection)
ImageGroundConnection._v_parameter = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_with_derivative = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_with_derivative,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_name = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_name,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_subset = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_subset,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_with_derivative_subset = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_with_derivative_subset,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_name_subset = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_name_subset,None,ImageGroundConnection)
ImageGroundConnection._v_parameter_mask = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_parameter_mask,None,ImageGroundConnection)
ImageGroundConnection.cf_look_vector_arr = new_instancemethod(_image_ground_connection.ImageGroundConnection_cf_look_vector_arr,None,ImageGroundConnection)
ImageGroundConnection._v_dem = new_instancemethod(_image_ground_connection.ImageGroundConnection__v_dem,None,ImageGroundConnection)
ImageGroundConnection.__dem = new_instancemethod(_image_ground_connection.ImageGroundConnection___dem,None,ImageGroundConnection)
ImageGroundConnection_swigregister = _image_ground_connection.ImageGroundConnection_swigregister
ImageGroundConnection_swigregister(ImageGroundConnection)

class OffsetImageGroundConnection(ImageGroundConnection):
    """
    This class creates an ImageGroundConnection that that is an offset of
    a given one.

    This would be used for example when a image is subsetted to create a
    ImageGroundConnection to go with that subsetted image.

    C++ includes: image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::OffsetImageGroundConnection::OffsetImageGroundConnection(const boost::shared_ptr< ImageGroundConnection > &Ig_original, double
        Line_offset, double Sample_offset)
        Constructor. 
        """
        _image_ground_connection.OffsetImageGroundConnection_swiginit(self,_image_ground_connection.new_OffsetImageGroundConnection(*args))
    def _v_original_image_ground_connection(self):
        """
        boost::shared_ptr<ImageGroundConnection> GeoCal::OffsetImageGroundConnection::original_image_ground_connection() const
        Underlying ImageGroundConnection. 
        """
        return _image_ground_connection.OffsetImageGroundConnection__v_original_image_ground_connection(self)

    @property
    def original_image_ground_connection(self):
        return self._v_original_image_ground_connection()

    def _v_line_offset(self):
        """
        double GeoCal::OffsetImageGroundConnection::line_offset() const
        Return line offset. 
        """
        return _image_ground_connection.OffsetImageGroundConnection__v_line_offset(self)

    @property
    def line_offset(self):
        return self._v_line_offset()

    def _v_sample_offset(self):
        """
        double GeoCal::OffsetImageGroundConnection::sample_offset() const
        Return sample offset. 
        """
        return _image_ground_connection.OffsetImageGroundConnection__v_sample_offset(self)

    @property
    def sample_offset(self):
        return self._v_sample_offset()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.original_image_ground_connection,self.line_offset,self.sample_offset)

    __swig_destroy__ = _image_ground_connection.delete_OffsetImageGroundConnection
OffsetImageGroundConnection._v_original_image_ground_connection = new_instancemethod(_image_ground_connection.OffsetImageGroundConnection__v_original_image_ground_connection,None,OffsetImageGroundConnection)
OffsetImageGroundConnection._v_line_offset = new_instancemethod(_image_ground_connection.OffsetImageGroundConnection__v_line_offset,None,OffsetImageGroundConnection)
OffsetImageGroundConnection._v_sample_offset = new_instancemethod(_image_ground_connection.OffsetImageGroundConnection__v_sample_offset,None,OffsetImageGroundConnection)
OffsetImageGroundConnection_swigregister = _image_ground_connection.OffsetImageGroundConnection_swigregister
OffsetImageGroundConnection_swigregister(OffsetImageGroundConnection)

class ImageGroundConnectionCopy(ImageGroundConnection):
    """
    Simple ImageGroundConnection where we use the mapping from an
    underlying ImageGroundConnection, but allow the raster image, dem,
    title, image mask, or ground mask to be different.

    You can create a copy, and then modify the image etc. without changing
    the underlying ImageGroundConnection.

    C++ includes: image_ground_connection.h 
    """
    thisown = _swig_property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args): 
        """
        GeoCal::ImageGroundConnectionCopy::ImageGroundConnectionCopy(const boost::shared_ptr< ImageGroundConnection > &Igc, const
        boost::shared_ptr< Dem > &d, const boost::shared_ptr< RasterImage >
        &Img, const boost::shared_ptr< RasterImageMultiBand > &Img_mb, const
        std::string &Title, const boost::shared_ptr< ImageMask > &Img_mask,
        const boost::shared_ptr< GroundMask > &Ground_mask)

        """
        _image_ground_connection.ImageGroundConnectionCopy_swiginit(self,_image_ground_connection.new_ImageGroundConnectionCopy(*args))
    def _v_igc_original(self):
        """
        const boost::shared_ptr<ImageGroundConnection>& GeoCal::ImageGroundConnectionCopy::igc_original() const

        """
        return _image_ground_connection.ImageGroundConnectionCopy__v_igc_original(self)

    @property
    def igc_original(self):
        return self._v_igc_original()

    @classmethod
    def pickle_format_version(cls):
      return 1

    def __reduce__(self):
      return _new_from_init, (self.__class__, 1, self.igc_original,self.dem,self.image,self.image_multi_band,self.title,self.image_mask,self.ground_mask)

    __swig_destroy__ = _image_ground_connection.delete_ImageGroundConnectionCopy
ImageGroundConnectionCopy._v_igc_original = new_instancemethod(_image_ground_connection.ImageGroundConnectionCopy__v_igc_original,None,ImageGroundConnectionCopy)
ImageGroundConnectionCopy_swigregister = _image_ground_connection.ImageGroundConnectionCopy_swigregister
ImageGroundConnectionCopy_swigregister(ImageGroundConnectionCopy)



