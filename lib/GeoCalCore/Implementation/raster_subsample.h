#ifndef RASTER_SUBSAMPLE_H
#define RASTER_SUBSAMPLE_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This subsamples a higher resolution RasterImage into a lower
  resolution one.

  Typically the pixel subsampling factor doesn't exactly divide the
  number of lines and samples of the full resolution data. We just
  trim the data to the largest low resolution pixel that completely
  fits in the higher resolution data.

*******************************************************************/

class RasterSubSample : public RasterImageVariable {
public:
  RasterSubSample(const boost::shared_ptr<RasterImage>& Data,
		 int Number_line_per_pixel, 
		 int Number_sample_per_pixel);
  virtual ~RasterSubSample() {}

//-----------------------------------------------------------------------
/// High resolution image that this object is based on.
//-----------------------------------------------------------------------

  const RasterImage& high_resolution_image() const {return *data_; }

//-----------------------------------------------------------------------
/// Pointer to high resolution image that this object is based on.
//-----------------------------------------------------------------------

  const boost::shared_ptr<RasterImage>& high_resolution_image_ptr() const 
  {return data_; }

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
/// Return pixel value at given line and sample.
//-----------------------------------------------------------------------

  virtual int unchecked_read(int Line, int Sample) const
  {
    return data_->unchecked_read(Line * number_line_per_pixel(),
				 Sample * number_sample_per_pixel());
  }
  virtual double unchecked_read_double(int Line, int Sample) const
  {
    return data_->unchecked_read_double(Line * number_line_per_pixel(),
				 Sample * number_sample_per_pixel());
  }
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void unchecked_write(int Line, int Sample, int Val);
  virtual void unchecked_write(int Line, int Sample, double Val);
  virtual bool copy_needs_double() const {return data_->copy_needs_double();}
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RasterImage> data_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
};

}
#endif

