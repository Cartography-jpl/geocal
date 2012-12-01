#ifndef RASTER_AVERAGED_H
#define RASTER_AVERAGED_H
#include "raster_image_variable.h"

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

class RasterAveraged : public RasterImageVariable {
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
/// If true, we ignore zeros when calculating the average.
//-----------------------------------------------------------------------
  
  bool ignore_zero() const {return ignore_zero_;}
  virtual int unchecked_read(int Line, int Sample) const;
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual void unchecked_write(int Line, int Sample, int Val);
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RasterImage> data_;
  bool ignore_zero_;
  int number_line_per_pixel_;
  int number_sample_per_pixel_;
};

}
#endif

