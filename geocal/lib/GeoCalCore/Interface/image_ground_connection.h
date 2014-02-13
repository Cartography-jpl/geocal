#ifndef IMAGE_GROUND_CONNECTION_H
#define IMAGE_GROUND_CONNECTION_H
#include "ground_coordinate.h"
#include "dem.h"
#include "image_coordinate.h"
#include "printable.h"
#include "ecr.h"
#include "raster_image.h"
#include "raster_image_multi_band.h"
#include "raster_image_multi_band_variable.h"
#include "ground_mask.h"
#include "image_mask.h"
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

  ImageGroundConnectionFailed(const std::string& W = "") : Exception(W) {}

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

*******************************************************************/

class ImageGroundConnection : public Printable<ImageGroundConnection> {
public:
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageGroundConnection() {}
  
  MapInfo cover(const MapInfo& Mi, int boundary = 0) const;

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
/// In those cases, we will through a ImageGroundConnectionFailed
/// exception. This means that nothing is wrong, other than that we
/// can't calculate the image_coordinate. Callers can catch this
/// exception if they have some way of handling no image coordinate
/// data. 
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) 
    const = 0;
  virtual blitz::Array<double, 2> image_coordinate_jac_ecr(const Ecr& Gc) 
    const;

//-----------------------------------------------------------------------
/// Return the Jacobian of the image coordinates with respect to the
/// parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { // Default is no parameters
    return blitz::Array<double, 2>(2, 0);
  }

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
/// A image to ground connection may depend on a set of parameters,
/// which can by modified (e.g., during a simultaneous bundle
/// adjustment). This returns those parameters.
//-----------------------------------------------------------------------

  virtual blitz::Array<double, 1> parameter() const
  { // Default is no parameters.
    return blitz::Array<double, 1>(0); 
  }

//-----------------------------------------------------------------------
/// Set the value of the parameters.
//-----------------------------------------------------------------------

  virtual void parameter(const blitz::Array<double, 1>& Parm)
  {
    // Default is do nothing
  }

//-----------------------------------------------------------------------
/// Descriptive name of each parameter.
//-----------------------------------------------------------------------

  virtual std::vector<std::string> parameter_name() const
  {
    std::vector<std::string> res;
    return res;
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "ImageGroundConnection"; }

  virtual double resolution_meter(const ImageCoordinate& Ic) const;
  virtual double resolution_meter() const;

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

protected:
  ImageGroundConnection(const boost::shared_ptr<Dem>& d, 
			const boost::shared_ptr<RasterImage>& Img, 
			const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
			const std::string& Title,
			const boost::shared_ptr<ImageMask>& Img_mask = 
			boost::shared_ptr<ImageMask>(),
			const boost::shared_ptr<GroundMask>& Ground_mask =
			boost::shared_ptr<GroundMask>());
  ImageGroundConnection() {}
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
};

/****************************************************************//**
  This class creates an ImageGroundConnection that that is an offset
  of a given one. This would be used for example when a image is
  subsetted to create a ImageGroundConnection to go with that
  subsetted image.
*******************************************************************/

class OffsetImageGroundConnection : public ImageGroundConnection {
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
  }
  
//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~OffsetImageGroundConnection() {}

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    ImageCoordinate ic2(Ic.line - line_offset_, Ic.sample - sample_offset_);
    ig_->cf_look_vector(ic2, Lv, P);
  }

//-----------------------------------------------------------------------
/// Return ground coordinate that goes with a particular image coordinate.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  { ImageCoordinate ic2(Ic.line - line_offset_, Ic.sample - sample_offset_);
    return ig_->ground_coordinate_dem(ic2, D); 
  }

  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { ImageCoordinate ic2(Ic.line - line_offset_, Ic.sample - sample_offset_);
    return ig_->ground_coordinate_approx_height(ic2, H); 
  }

//-----------------------------------------------------------------------
/// Return image coordinate that goes with a particular GroundCoordinate.
//-----------------------------------------------------------------------

  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  {
    ImageCoordinate ic = ig_->image_coordinate(Gc);
    ic.line += line_offset_;
    ic.sample += sample_offset_;
    return ic;
  }
  virtual blitz::Array<double, 2> image_coordinate_jac_ecr(const Ecr& Gc) const
  { return ig_->image_coordinate_jac_ecr(Gc); }

  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const;
  virtual blitz::Array<double, 1> parameter() const;
  virtual void parameter(const blitz::Array<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { 
    Os << "OffsetImageGroundConnection\n"
       << "  Line offset:   " << line_offset_ << "\n"
       << "  Sample offset: " << sample_offset_ << "\n"
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

  double line_offset() const { return line_offset_;}

//-----------------------------------------------------------------------
/// Return sample offset.
//-----------------------------------------------------------------------

  double sample_offset() const { return sample_offset_;}
private:
  boost::shared_ptr<ImageGroundConnection> ig_;
  double line_offset_;
  double sample_offset_;
};

/****************************************************************//**
  Simple ImageGroundConnection where we use the mapping from an 
  underlying ImageGroundConnection, but allow the raster image, 
  dem, title, image mask, or ground mask to be different. You can
  create a copy, and then modify the image etc. without changing the
  underlying ImageGroundConnection.
*******************************************************************/

class ImageGroundConnectionCopy: public ImageGroundConnection
{
public:
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc)
    : ImageGroundConnection(Igc->dem_ptr(), Igc->image(),
			    Igc->image_multi_band(),
			    Igc->title(), Igc->image_mask(), 
			    Igc->ground_mask()),
      igc(Igc)
  {
  }
  ImageGroundConnectionCopy
  (const boost::shared_ptr<ImageGroundConnection>& Igc,
   const boost::shared_ptr<Dem>& d, 
   const boost::shared_ptr<RasterImage>& Img, 
   const boost::shared_ptr<RasterImageMultiBand>& Img_mb, 
   const std::string& Title,
   const boost::shared_ptr<ImageMask>& Img_mask,
   const boost::shared_ptr<GroundMask>& Ground_mask)
    : ImageGroundConnection(d, Img, Img_mb, Title, Img_mask, Ground_mask),
      igc(Igc)
  {
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
  virtual blitz::Array<double, 2> image_coordinate_jac_ecr(const Ecr& Gc) 
    const { return igc->image_coordinate_jac_ecr(Gc); }
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { return igc->image_coordinate_jac_parm(Gc); }
  virtual blitz::Array<double, 1> parameter() const
  { return igc->parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { igc->parameter(Parm); }
  virtual std::vector<std::string> parameter_name() const
  { return igc->parameter_name(); }
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<ImageGroundConnection> igc;
};

}
#endif
