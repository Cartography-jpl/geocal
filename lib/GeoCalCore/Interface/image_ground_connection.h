#ifndef IMAGE_GROUND_CONNECTION_H
#define IMAGE_GROUND_CONNECTION_H
#include "ground_coordinate.h"
#include "dem.h"
#include "image_coordinate.h"
#include "printable.h"
#include "raster_image.h"
#include "raster_image_multi_band.h"
#include "sub_raster_image.h"
#include "sub_raster_image_multi_band.h"
#include "raster_image_multi_band_variable.h"
#include "ground_mask.h"
#include "image_mask.h"
#include "with_parameter.h"
#include <boost/make_shared.hpp>
#include <blitz/array.h>

namespace GeoCal {
  class RasterImage;

/****************************************************************//**
   Exception thrown if ImageGroundConnection fails to calculate a
   image coordinate.
*******************************************************************/

class ImageGroundConnectionFailed : public Exception {
public:

//-----------------------------------------------------------------------
/// Default constructor. Can give an optional string describing
/// the error.
//-----------------------------------------------------------------------

  ImageGroundConnectionFailed(const std::string& W = "ImageGroundConnectionFailed") : Exception(W) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageGroundConnectionFailed() throw() {}
};

/****************************************************************//**
  Depending on the the data we are using, we may connect a location in
  an image to a ground location in one of several ways. For example, we
  might have a rigorous camera model and orbit data allowing us to use
  an Ipi. Or we might have a RPC (Rational Polynomial Coefficients)
  that connect an image to the ground. Or we might be working with
  data that has already been map projected, so a MapInfo gives us a
  direct connection.

  For many purposes, we don't care exactly how the connection is done,
  we just use the fact that there is a connection.

  This class gives a generic interface that can be used for any kind
  of a connection between the ground and an image.

  An important implementation issue, because of the way the templates
  work in the boost serialization library if you derive from this
  class and want to use boost serialize on it, make sure to derive
  virtual, e.g 

  class Foo : public virtual ImageGroundConnection {
    blah blah
  };

  This doesn't hurt anything, for other code and because of how boost
  deals with multiple inheritance is required.
*******************************************************************/

class ImageGroundConnection : public Printable<ImageGroundConnection>,
			      public virtual WithParameter {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageGroundConnection() {}
  
  MapInfo cover(const MapInfo& Mi, int boundary = 0) const;

//-----------------------------------------------------------------------
/// Return an array of look vector information. This is really
/// intended for use with ray casting or with python, where calling
/// cf_look_vector repeatedly is costly.
/// This is nline x nsamp x x nsub_line x nsub_sample x
/// nintegration_step x 2 x 3 in size, where we give the position first
/// followed by the look vector. 
///
/// In general, the number of integration steps doesn't have any
/// meaning and we just repeat the data if the number of integration
/// steps is something other than 1. But for certain
/// ImageGroundConnection, this may have meaning (e.g., anything where 
/// we have a camera and orbit data).
///
/// The default implementation just calls cf_look_vector repeatedly,
/// but a derived class can make any kind of optimization that is
/// appropriate.
///
/// Note a subtle difference between this and ground_coordinate. A
/// camera may have a footprint that overlaps from one line to the
/// next. For example, MISR has a pixel spacing of 275 meter, but the
/// footprint may be as much as 700 meter on the ground (for D camera
/// in line direction). The cf_look_vector_arr with the subpixels
/// refer to the actual camera, i.e., the 750 meter footprint. This
/// means that in general the results of calling ground_coordinate
/// (which corresponds to the 275 meter pixel spacing) won't match the
/// location you get from cf_look_vector_arr. This is intended, not a
/// bug, and simple reflects that we are talking about 2 different
/// things here.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 7> 
  cf_look_vector_arr(int ln_start, int smp_start, int nline, int nsamp,
		     int nsubpixel_line = 1, 
		     int nsubpixel_sample = 1,
		     int nintegration_step = 1) const;

//-----------------------------------------------------------------------
/// Return look vector for given coordinate, along with a position
/// that lies along the direction of the look vector (so position of
/// satellite, or a position on the surface.
//-----------------------------------------------------------------------
  
  virtual void cf_look_vector(const ImageCoordinate& Ic, 
			      CartesianFixedLookVector& Lv,
			      boost::shared_ptr<CartesianFixed>& P) const
  {
    Lv = *cf_look_vector_lv(Ic);
    P = cf_look_vector_pos(Ic);
  }

//-----------------------------------------------------------------------
/// SWIG/python doesn't like returning 2 items through a director, so
/// we implement cf_look_vector in 2 parts. In general, C++ to
/// override cf_look_vector rather than these 2 functions (although it
/// could do these 2 if useful for some reason.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixedLookVector>
  cf_look_vector_lv(const ImageCoordinate& Ic) const
  { 
    CartesianFixedLookVector lv;
    boost::shared_ptr<CartesianFixed> p;
    cf_look_vector(Ic, lv, p);
    return boost::shared_ptr<CartesianFixedLookVector>
      (new CartesianFixedLookVector(lv));
  }
  virtual boost::shared_ptr<CartesianFixed>
  cf_look_vector_pos(const ImageCoordinate& Ic) const
  { 
    CartesianFixedLookVector lv;
    boost::shared_ptr<CartesianFixed> p;
    cf_look_vector(Ic, lv, p);
    return p;
  }
  virtual blitz::Array<double, 1> 
  collinearity_residual(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  virtual blitz::Array<double, 2> 
  collinearity_residual_jacobian(const GroundCoordinate& Gc,
			const ImageCoordinate& Ic_actual) const;
  void compare_igc(const ImageGroundConnection& Igc_true,
		   int Number_line_spacing,
		   int Number_sample_spacing, double Height,
		   blitz::Array<double, 2>& True_line,
		   blitz::Array<double, 2>& True_sample,
		   blitz::Array<double, 2>& Calc_line,
		   blitz::Array<double, 2>& Calc_sample)
    const;
  
    
//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image
/// coordinate. 
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate(const ImageCoordinate& Ic) const 
  { return ground_coordinate_dem(Ic, dem()); }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image
/// coordinate. This version supplies a Dem to use.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, 
			const Dem& D) const = 0;

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, 
				  double H) const;

//-----------------------------------------------------------------------
/// Naif code for ImageGroundConnection (e.g., what planet is this for)
//-----------------------------------------------------------------------
  virtual int naif_code() const
  { return ground_coordinate_approx_height(ImageCoordinate(0,0),0)->naif_code(); }

//-----------------------------------------------------------------------
/// CartesianFixed for ImageGroundConnection (e.g., depends on what
/// planet this is this for).
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> cartesian_fixed() const
  { return ground_coordinate_approx_height(ImageCoordinate(0,0),0)->convert_to_cf(); }
  
//-----------------------------------------------------------------------
/// Underlying image (if present)
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<RasterImage> image() const {return image_;}
  virtual void image(const boost::shared_ptr<RasterImage>& Img) 
  {image_ = Img;}

//-----------------------------------------------------------------------
/// Underlying RasterImageMultiBand (if present)
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<RasterImageMultiBand> 
  image_multi_band() const {return image_mb_;}
  virtual void image_multi_band
  (const boost::shared_ptr<RasterImageMultiBand>& Img_mb) 
  {image_mb_ = Img_mb;}

//-----------------------------------------------------------------------
/// Mask to apply to image.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<ImageMask> image_mask() const {return image_mask_;}
  virtual void image_mask(const boost::shared_ptr<ImageMask>& Image_mask) 
  { image_mask_ = Image_mask;}

//-----------------------------------------------------------------------
/// Mask to apply to ground.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundMask> ground_mask() const 
  {return ground_mask_;}
  virtual void ground_mask(const boost::shared_ptr<GroundMask>& Ground_mask) 
  { ground_mask_ = Ground_mask; }

//-----------------------------------------------------------------------
/// Not all ImageGroundConnection has a time associated with each
/// pixel (for example, one based on an underlying existing map). This
/// indicates if we have the functionality.
//-----------------------------------------------------------------------
  
  virtual bool has_time() const { return false; }

//-----------------------------------------------------------------------
/// Time associated with the given pixel. This will throw an exception
/// if has_time is false.
//-----------------------------------------------------------------------

  virtual Time pixel_time(const ImageCoordinate& Ic) const
  { throw Exception("pixel_time is not implemented"); }

//-----------------------------------------------------------------------
/// Title that we can use to describe the image. This can be any
/// string that is useful as a label.
//-----------------------------------------------------------------------

  virtual std::string title() const { return title_;}

//-----------------------------------------------------------------------
/// Set image title
//-----------------------------------------------------------------------

  virtual void title(const std::string& Title) { title_ = Title; }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular
/// GroundCoordinate.
///
/// For some types of ImageGroundConnection, we might not be able to
/// calculate image_coordinate for all values (e.g., Ipi might fail).
/// In those cases, we will throw a ImageGroundConnectionFailed
/// exception. This means that nothing is wrong, other than that we
/// can't calculate the image_coordinate. Callers can catch this
/// exception if they have some way of handling no image coordinate
/// data. 
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const = 0;

//-----------------------------------------------------------------------
/// Variation of image_coordinate that returns a status instead of 
/// throwing an exception. If there are many points calls that might
/// throw an exception (e.g., looking at area near the edge of the
/// image footprint on the ground) the cost of setting up and catching
/// the exceptions can be expensive.
///
/// The default implementation just catches any
/// ImageGroundConnectionFailed exceptions and set the status
/// accordingly. But derived classes can give a more efficient
/// implementation. 
//-----------------------------------------------------------------------

  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const 
  {
    try {
      Res = image_coordinate(Gc);
      Success = true;
    } catch(const ImageGroundConnectionFailed& E) {
      Success = false;
    }
  }
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) 
    const;

  virtual blitz::Array<double, 2> image_coordinate_jac_cf_fd(const CartesianFixed& Gc, double Step_size) const;

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { // Default is no parameters
    return blitz::Array<double, 2>(2, 0);
  }

  virtual blitz::Array<double, 2>
  image_coordinate_jac_parm_fd(const GroundCoordinate& Gc,
			       const blitz::Array<double, 1>& Eps) const;
  
//-----------------------------------------------------------------------
/// Return number of lines in underlying image.
//-----------------------------------------------------------------------

  virtual int number_line() const 
  {
    if(image())
      return image()->number_line();
    if(image_multi_band())
      return image_multi_band()->raster_image(0).number_line();
    return 0;
  }

//-----------------------------------------------------------------------
/// Return number of samples in underlying image.
//-----------------------------------------------------------------------

  virtual int number_sample() const
  {
    if(image())
      return image()->number_sample();
    if(image_multi_band())
      return image_multi_band()->raster_image(0).number_sample();
    return 0;
  }

//-----------------------------------------------------------------------
/// Return number of bands.
//-----------------------------------------------------------------------

  virtual int number_band() const 
  {
    if(image())
      return 1;
    if(image_multi_band())
      return image_multi_band()->number_band();
    return 0;
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "ImageGroundConnection"; }

  virtual double resolution_meter(const ImageCoordinate& Ic) const;
  virtual double resolution_meter() const;

  virtual void footprint_resolution(int Line, int Sample, 
				    double &Line_resolution_meter, 
				    double &Sample_resolution_meter) const;

//-----------------------------------------------------------------------
/// SWIG/python doesn't like returning 2 items through a director, so
/// we implement cf_look_vector in 2 parts. In general, C++ should
/// override footprint_resolution rather than these 2 functions (although it
/// could do these 2 if useful for some reason.
//-----------------------------------------------------------------------

  virtual double footprint_resolution_line(int Line, int Sample) const
  { 
    double lres, sres;
    footprint_resolution(Line, Sample, lres, sres);
    return lres;
  }

  virtual double footprint_resolution_sample(int Line, int Sample) const
  { 
    double lres, sres;
    footprint_resolution(Line, Sample, lres, sres);
    return sres;
  }

//-----------------------------------------------------------------------
/// DEM used by ground_coordinate.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Dem>& dem_ptr() const {return dem_;}

//-----------------------------------------------------------------------
/// Set the DEM to use in ground_coordinate.
//-----------------------------------------------------------------------

  void dem(const boost::shared_ptr<Dem>& D) { dem_ = D; }

//-----------------------------------------------------------------------
/// Dem used by ground_coordinate
//-----------------------------------------------------------------------

  const Dem& dem() const { return *dem_;}

//-----------------------------------------------------------------------
/// Default is that ImageGroundConnection ignores AutoDerivative
/// information in the parameters. Derived classes can override this
/// if they support this. This should mostly be an internal matter,
/// the derived class can use the AutoDerivative in its implementation
/// of image_coordinate_jac_parm, but it can also do this calculation
/// in another manner if desired.
//-----------------------------------------------------------------------

  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return ArrayAd<double, 1>(parameter()); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& P) 
  { parameter(P.value()); }

//-----------------------------------------------------------------------
/// Default is no parameters.
//-----------------------------------------------------------------------
  virtual blitz::Array<double, 1> parameter() const
  { return blitz::Array<double, 1>(); }
  virtual void parameter(const blitz::Array<double, 1>& P)
  { 
    if(P.rows() != 0)
      throw Exception("No parameters supported");
  }
protected:
//-----------------------------------------------------------------------
/// Constructor. As a convenience, if Img_mask or Ground_mask are null
/// pointer, we replace them with an empty CombinedImageMask or
/// CombinedGroundMask.
//-----------------------------------------------------------------------

  ImageGroundConnection(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask = 
			boost::shared_ptr<ImageMask>(),
			const boost::shared_ptr<GroundMask>& Ground_mask =
			boost::shared_ptr<GroundMask>())
  { initialize(d, Img, Img_mb, Title, Img_mask, Ground_mask); }
  ImageGroundConnection() {}
  void initialize(const boost::shared_ptr<Dem>& d, 
		  const boost::shared_ptr<RasterImage>& Img, 
		  const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
		  const std::string& Title,
		  const boost::shared_ptr<ImageMask>& Img_mask = 
		  boost::shared_ptr<ImageMask>(),
		  const boost::shared_ptr<GroundMask>& Ground_mask =
		  boost::shared_ptr<GroundMask>());
  /// DEM to use, should be set by constructor.
  boost::shared_ptr<Dem> dem_;
  /// Raster image to use, should be set by constructor. This might be
  /// null, indicating we have no image.
  boost::shared_ptr<RasterImage> image_;
  /// Raster image multiband to use, should be set by constructor. 
  /// This might be null, indicating we have no image.
  boost::shared_ptr<RasterImageMultiBand> image_mb_;
  /// Title of image to use, should be set by constructor.
  std::string title_;
  /// Image mask to use, should be set by constructor (can set to
  /// empty CombinedImageMask if there is no mask).
  boost::shared_ptr<ImageMask> image_mask_;
  /// Ground mask to use, should be set by constructor (can set to
  /// empty CombinedGroundMask if there is no mask).
  boost::shared_ptr<GroundMask> ground_mask_;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This class creates an ImageGroundConnection that that is an offset
  of a given one. This would be used for example when a image is
  subsetted to create a ImageGroundConnection to go with that
  subsetted image.
*******************************************************************/

class OffsetImageGroundConnection : public virtual ImageGroundConnection {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  OffsetImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection>& Ig_original, 
   double Line_offset, double Sample_offset)
    :  ImageGroundConnection(Ig_original->dem_ptr(),
			     Ig_original->image(),
			     Ig_original->image_multi_band(),
			     Ig_original->title(),
			     Ig_original->image_mask(),
			     Ig_original->ground_mask()),
       ig_(Ig_original), line_offset_(Line_offset), 
       sample_offset_(Sample_offset)
  {
    if(image_mask_)
      image_mask_ = boost::make_shared<OffsetImageMask>
	(image_mask_, line_offset_.value(), sample_offset_.value());
  }

//-----------------------------------------------------------------------
/// Constructor that subsets an existing Igc.
//-----------------------------------------------------------------------

  OffsetImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection>& Ig_original, 
   int Lstart, int Sstart, int Number_line, int Number_sample)
    :  ImageGroundConnection(Ig_original->dem_ptr(),
			     Ig_original->image(),
			     Ig_original->image_multi_band(),
			     Ig_original->title(),
			     Ig_original->image_mask(),
			     Ig_original->ground_mask()),
       ig_(Ig_original), line_offset_(-Lstart), 
       sample_offset_(-Sstart)
  {
    if(image_mask_)
      image_mask_ = boost::make_shared<OffsetImageMask>
	(image_mask_, line_offset_.value(), sample_offset_.value());
    if(image_)
      image_ = boost::make_shared<SubRasterImage>
	(image_, Lstart, Sstart, Number_line, Number_sample);
    if(image_mb_)
      image_mb_ = boost::make_shared<SubRasterImageMultiBand>
	(image_mb_, Lstart, Sstart, Number_line, Number_sample);
  }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OffsetImageGroundConnection() {}

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    ImageCoordinate ic2(Ic.line - line_offset_.value(), 
			Ic.sample - sample_offset_.value());
    ig_->cf_look_vector(ic2, Lv, P);
  }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image coordinate.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  { ImageCoordinate ic2(Ic.line - line_offset_.value(), 
			Ic.sample - sample_offset_.value());
    return ig_->ground_coordinate_dem(ic2, D); 
  }

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { ImageCoordinate ic2(Ic.line - line_offset_.value(), 
			Ic.sample - sample_offset_.value());
    return ig_->ground_coordinate_approx_height(ic2, H); 
  }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular GroundCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  {
    ImageCoordinate ic = ig_->image_coordinate(Gc);
    ic.line += line_offset_.value();
    ic.sample += sample_offset_.value();
    return ic;
  }
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const
  {
    ig_->image_coordinate_with_status(Gc, Res, Success);
    if(Success) {
      Res.line += line_offset_.value();
      Res.sample += sample_offset_.value();
    }
  }
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const
  { return ig_->image_coordinate_jac_cf(Gc); }

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual blitz::Array<double, 1> parameter() const;
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { 
    Os << "OffsetImageGroundConnection\n"
       << "  Line offset:   " << line_offset_.value() << "\n"
       << "  Sample offset: " << sample_offset_.value() << "\n"
       << "  Original ImageGroundConnection:\n"
       << *ig_ << "\n";
  }

//-----------------------------------------------------------------------
/// Underlying ImageGroundConnection.
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageGroundConnection> original_image_ground_connection()
    const {return ig_;}

//-----------------------------------------------------------------------
/// Return line offset.
//-----------------------------------------------------------------------

  double line_offset() const { return line_offset_.value();}

//-----------------------------------------------------------------------
/// Return sample offset.
//-----------------------------------------------------------------------

  double sample_offset() const { return sample_offset_.value();}
protected:
  OffsetImageGroundConnection() {}
private:
  boost::shared_ptr<ImageGroundConnection> ig_;
  AutoDerivative<double> line_offset_;
  AutoDerivative<double> sample_offset_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  Simple ImageGroundConnection where we use the mapping from an 
  underlying ImageGroundConnection, but allow the raster image, 
  dem, title, image mask, or ground mask to be different. You can
  create a copy, and then modify the image etc. without changing the
  underlying ImageGroundConnection.
*******************************************************************/

class ImageGroundConnectionCopy: public virtual ImageGroundConnection
{
public:
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc)
    : igc(Igc)
  {
    initialize(Igc->dem_ptr(), Igc->image(),
	       Igc->image_multi_band(),
	       Igc->title(), Igc->image_mask(), 
	       Igc->ground_mask());
  }
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc,
   const boost::shared_ptr<Dem>& d, 
   const boost::shared_ptr<RasterImage>& Img, 
   const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
   const std::string& Title,
   const boost::shared_ptr<ImageMask>& Img_mask,
   const boost::shared_ptr<GroundMask>& Ground_mask)
    : igc(Igc)
  {
    initialize(d, Img, Img_mb, Title, Img_mask, Ground_mask);
  }
  virtual ~ImageGroundConnectionCopy() {}
  virtual void cf_look_vector(const ImageCoordinate& Ic, 
			      CartesianFixedLookVector& Lv,
			      boost::shared_ptr<CartesianFixed>& P) const
  { igc->cf_look_vector(Ic, Lv, P); }
  virtual boost::shared_ptr<CartesianFixedLookVector>
  cf_look_vector_lv(const ImageCoordinate& Ic) const
  { return igc->cf_look_vector_lv(Ic); }
  virtual boost::shared_ptr<CartesianFixed>
  cf_look_vector_pos(const ImageCoordinate& Ic) const
  {  return igc->cf_look_vector_pos(Ic); }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, 
			const Dem& D) const
  { return igc->ground_coordinate_dem(Ic, D); }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, 
				  double H) const
  { return igc->ground_coordinate_approx_height(Ic, H); }
  const boost::shared_ptr<ImageGroundConnection>& igc_original() const
  { return igc; }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const { return igc->image_coordinate(Gc); }
  virtual void image_coordinate_with_status(const GroundCoordinate& Gc,
					    ImageCoordinate& Res,
					    bool& Success) const
  {
    igc->image_coordinate_with_status(Gc, Res, Success);
  }
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) 
    const { return igc->image_coordinate_jac_cf(Gc); }
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { return igc->image_coordinate_jac_parm(Gc); }
  virtual blitz::Array<double, 1> parameter() const
  { return igc->parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { igc->parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return igc->parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { return igc->parameter_with_derivative(Parm); }
  virtual std::vector<std::string> parameter_name() const
  { return igc->parameter_name(); }
  virtual blitz::Array<bool, 1> parameter_mask() const
  { return igc->parameter_mask(); }
  virtual bool has_time() const
  { return igc->has_time(); }
  virtual Time pixel_time(const ImageCoordinate& Ic) const
  { return igc->pixel_time(Ic); }
  virtual void print(std::ostream& Os) const;
protected:
  ImageGroundConnectionCopy() {}
  boost::shared_ptr<ImageGroundConnection> igc;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

}

GEOCAL_EXPORT_KEY(ImageGroundConnection);
GEOCAL_EXPORT_KEY(OffsetImageGroundConnection);
GEOCAL_EXPORT_KEY(ImageGroundConnectionCopy);
#endif
