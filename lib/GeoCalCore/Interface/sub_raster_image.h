#ifndef SUB_RASTER_IMAGE_H
#define SUB_RASTER_IMAGE_H
#include "raster_image_variable.h"

namespace GeoCal {
/****************************************************************//**
  This allows you to take a subset of a larger RasterImage and treat
  it like a RasterImage in its own right.
*******************************************************************/

class SubRasterImage : public RasterImageVariable {
public:
  SubRasterImage(const boost::shared_ptr<RasterImage>& Raster_image,
		 int Start_line, int Start_sample,
		 int Number_line, int Number_sample);
  SubRasterImage(const boost::shared_ptr<RasterImage>& Raster_image,
     const std::vector<boost::shared_ptr<GroundCoordinate> >& Pt,
     int boundary = 0);
  virtual ~SubRasterImage() {}

//-----------------------------------------------------------------------
/// Start line of underlying data.
//-----------------------------------------------------------------------
  
  int start_line() const { return start_line_; }

//-----------------------------------------------------------------------
/// Start sample of underlying data.
//-----------------------------------------------------------------------
  
  int start_sample() const { return start_sample_; }

//-----------------------------------------------------------------------
/// Underlying full raster image.
//-----------------------------------------------------------------------
  
  const RasterImage& full_raster_image() const {return *data_; }

//-----------------------------------------------------------------------
/// Underlying full raster image.
//-----------------------------------------------------------------------
  
  const boost::shared_ptr<RasterImage>& full_raster_image_ptr() const 
  {return data_; }

//-----------------------------------------------------------------------
/// Underlying full raster image.
//-----------------------------------------------------------------------
  
  RasterImage& full_raster_image() {return *data_; }
  virtual int unchecked_read(int Line, int Sample) const
  {
    return data_->unchecked_read(Line + start_line_, Sample + start_sample_);
  }
  virtual double unchecked_read_double(int Line, int Sample) const
  {
    return data_->unchecked_read_double(Line + start_line_, 
					Sample + start_sample_);
  }
  virtual void read_ptr(int Lstart, int Sstart, int Number_line, 
			int Number_sample, int* Res) const;
  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const
  { 
    return data_->read_double(Lstart + start_line_, Sstart + start_sample_,
			      Number_line, Number_sample);
  }
  virtual void unchecked_write(int Line, int Sample, int Val);

  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<RasterImage> data_; ///<Underlying data.
  int start_line_;			    ///<Starting line of
					    ///underlying data
  int start_sample_;			    ///<Starting sample of
					    ///underlying data
};
}
#endif
