#ifndef RASTER_AVERAGED_H
#define RASTER_AVERAGED_H
#include "calc_raster.h"
#include "calc_raster_multi_band.h"
#include "image_ground_connection.h"

namespace GeoCal {
/****************************************************************//**
  This averages a higher resolution RasterImage into a lower
  resolution one.

  The data is a straight average of a given number of pixels in the
  line and sample direction of the full resolution data.

  Typically the pixel averaging factor doesn't exactly divide the
  number of lines and samples of the full resolution data. We just
  trim the data to the largest low resolution pixel that completely
  fits in the higher resolution data.

  This class calculates the average on the fly. Sometimes this is what
  you want, but if you are going to be using the resulting data a few
  times, you may want to use a MemoryRasterImage to generate a copy
  once and keep it in memory.
*******************************************************************/

class RasterAveraged : public CalcRaster {
public:
  RasterAveraged(const boost::shared_ptr<RasterImage>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel,
		 bool Ignore_zero = false);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RasterAveraged() {}

//-----------------------------------------------------------------------
/// High resolution image that this object is based on.
//-----------------------------------------------------------------------

  const RasterImage& high_resolution_image() const {return *raw_data_; }

//-----------------------------------------------------------------------
/// Pointer to high resolution image that this object is based on.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>& high_resolution_image_ptr() const 
  {return raw_data_; }

//-----------------------------------------------------------------------
/// Number of lines of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_line_per_pixel() const {return number_line_per_pixel_;}

//-----------------------------------------------------------------------
/// Number of samples of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_sample_per_pixel() const {return number_sample_per_pixel_;}

//-----------------------------------------------------------------------
/// If true, we ignore zeros when calculating the average.
//-----------------------------------------------------------------------
  
  bool ignore_zero() const {return ignore_zero_;}
  virtual void print(std::ostream& Os) const;
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> raw_data_;
  bool ignore_zero_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
};

/****************************************************************//**
  This averages a higher resolution RasterImageMultiBand into a lower
  resolution one.

  The data is a straight average of a given number of pixels in the
  line and sample direction of the full resolution data.

  Typically the pixel averaging factor doesn't exactly divide the
  number of lines and samples of the full resolution data. We just
  trim the data to the largest low resolution pixel that completely
  fits in the higher resolution data.

  This class calculates the average on the fly. Sometimes this is what
  you want, but if you are going to be using the resulting data a few
  times, you may want to use a MemoryRasterImage to generate a copy
  once and keep it in memory.
*******************************************************************/

class RasterAveragedMultiBand : public CalcRasterMultiBand {
public:
  RasterAveragedMultiBand(const boost::shared_ptr<RasterImageMultiBand>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel,
		 bool Ignore_zero = false);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~RasterAveragedMultiBand() {}

//-----------------------------------------------------------------------
/// High resolution image that this object is based on.
//-----------------------------------------------------------------------

  const RasterImageMultiBand& high_resolution_image() 
    const {return *raw_data_; }

//-----------------------------------------------------------------------
/// Pointer to high resolution image that this object is based on.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImageMultiBand>& high_resolution_image_ptr() 
    const {return raw_data_; }

//-----------------------------------------------------------------------
/// Number of lines of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_line_per_pixel() const {return number_line_per_pixel_;}

//-----------------------------------------------------------------------
/// Number of samples of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_sample_per_pixel() const {return number_sample_per_pixel_;}

//-----------------------------------------------------------------------
/// If true, we ignore zeros when calculating the average.
//-----------------------------------------------------------------------
  
  bool ignore_zero() const {return ignore_zero_;}
  virtual void print(std::ostream& Os) const;
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImageMultiBand> raw_data_;
  bool ignore_zero_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
};

/****************************************************************//**
  Take an existing image mask, and averages it like RasterAveraged.
  We mask a point if any of the points that make it up are masked.
  This does the calculation on fly.
*******************************************************************/

class ImageMaskAveraged : public ImageMask {
public:
  ImageMaskAveraged(const boost::shared_ptr<ImageMask>& Data,
		    int Number_line_per_pixel,
		    int Number_sample_per_pixel)
    : data_(Data), number_line_per_pixel_(Number_line_per_pixel),
      number_sample_per_pixel_(Number_sample_per_pixel) { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ImageMaskAveraged() {}

//-----------------------------------------------------------------------
/// High resolution image mask that this object is based on.
//-----------------------------------------------------------------------

  const ImageMask& high_resolution_image_mask() const {return *data_; }

//-----------------------------------------------------------------------
/// Pointer to high resolution image mask that this object is based on.
//-----------------------------------------------------------------------

  const boost::shared_ptr<ImageMask>& high_resolution_image_mask_ptr() const 
  {return data_; }

//-----------------------------------------------------------------------
/// Number of lines of high resolution data per pixel of this lower
/// resolution ImageMatch
//-----------------------------------------------------------------------

  int number_line_per_pixel() const {return number_line_per_pixel_;}

//-----------------------------------------------------------------------
/// Number of samples of high resolution data per pixel of this lower
/// resolution ImageMask
//-----------------------------------------------------------------------

  int number_sample_per_pixel() const {return number_sample_per_pixel_;}


  virtual bool mask(int Line, int Sample) const;
  virtual bool area_any_masked(int Line, int Sample, int Number_line,
			       int Number_sample) const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<ImageMask> data_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
};

/****************************************************************//**
  This takes an existing ImageGroundConnection and it averages the
  raster image, accounting for this in the ground/image
  calculation. You can either have the image averaged on the fly, or
  have it done once and kept in memory.
*******************************************************************/

class AveragedImageGroundConnection : public ImageGroundConnection {
public:
  AveragedImageGroundConnection
  (const boost::shared_ptr<ImageGroundConnection> Igc,
   int Number_line_per_pixel, 
   int Number_sample_per_pixel,
   bool In_memory = false,
   bool Ignore_zero = false);

  virtual ~AveragedImageGroundConnection() {}

//-----------------------------------------------------------------------
/// Number of lines of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_line_per_pixel() const {return number_line_per_pixel_;}

//-----------------------------------------------------------------------
/// Number of samples of high resolution data per pixel of this lower
/// resolution RasterImage.
//-----------------------------------------------------------------------

  int number_sample_per_pixel() const {return number_sample_per_pixel_;}

//-----------------------------------------------------------------------
/// If true, we ignore zeros when calculating the average.
//-----------------------------------------------------------------------
  
  bool ignore_zero() const {return ignore_zero_;}

//-----------------------------------------------------------------------
/// If true, calculation done once and is in memory.
//-----------------------------------------------------------------------
  
  bool in_memory() const {return in_memory_;}

//-----------------------------------------------------------------------
/// Underlying ImageGroundConnection.
//-----------------------------------------------------------------------

  boost::shared_ptr<ImageGroundConnection> original_image_ground_connection()
    const {return ig_;}
  virtual void print(std::ostream& Os) const;

  virtual blitz::Array<double, 1> parameter() const 
  { return ig_->parameter(); }
  virtual void parameter(const blitz::Array<double, 1>& Parm)
  { ig_->parameter(Parm); }
  virtual ArrayAd<double, 1> parameter_with_derivative() const
  { return ig_->parameter_with_derivative(); }
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm)
  { return ig_->parameter_with_derivative(Parm); }
  virtual std::vector<std::string> parameter_name() const
  { return ig_->parameter_name(); }
  virtual blitz::Array<bool, 1> parameter_mask() const
  { return ig_->parameter_mask(); }

  virtual void
  cf_look_vector(const ImageCoordinate& Ic, CartesianFixedLookVector& Lv,
		 boost::shared_ptr<CartesianFixed>& P) const
  {
    ImageCoordinate ic2((Ic.line + 0.5) * number_line_per_pixel_ - 0.5,
			(Ic.sample + 0.5) * number_sample_per_pixel_ - 0.5);
    return ig_->cf_look_vector(ic2, Lv, P); 
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_dem(const ImageCoordinate& Ic, const Dem& D) const
  { 
    ImageCoordinate ic2((Ic.line + 0.5) * number_line_per_pixel_ - 0.5,
			(Ic.sample + 0.5) * number_sample_per_pixel_ - 0.5);
    return ig_->ground_coordinate_dem(ic2, D); 
  }
  virtual boost::shared_ptr<GroundCoordinate> 
  ground_coordinate_approx_height(const ImageCoordinate& Ic, double H) const
  { 
    ImageCoordinate ic2((Ic.line + 0.5) * number_line_per_pixel_ - 0.5,
			(Ic.sample + 0.5) * number_sample_per_pixel_ - 0.5);
    return ig_->ground_coordinate_approx_height(ic2, H); 
  }
  virtual ImageCoordinate image_coordinate(const GroundCoordinate& Gc) const
  {
    ImageCoordinate ic = ig_->image_coordinate(Gc);
    ic.line = (ic.line + 0.5)  /  number_line_per_pixel_ - 0.5;
    ic.sample = (ic.sample + 0.5) / number_sample_per_pixel_ - 0.5;
    return ic;
  }
  virtual blitz::Array<double, 2> image_coordinate_jac_cf(const CartesianFixed& Gc) const
  { 
    blitz::Array<double, 2> res = ig_->image_coordinate_jac_cf(Gc); 
    res(0) /= number_line_per_pixel_;
    res(1) /= number_sample_per_pixel_;
    return res;
  }
  virtual blitz::Array<double, 2> 
  image_coordinate_jac_parm(const GroundCoordinate& Gc) const
  { 
    blitz::Array<double, 2> res = ig_->image_coordinate_jac_parm(Gc); 
    res(0) /= number_line_per_pixel_;
    res(1) /= number_sample_per_pixel_;
    return res;
  }

private:
  boost::shared_ptr<ImageGroundConnection> ig_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
  bool in_memory_;
  bool ignore_zero_;
};
}
#endif

