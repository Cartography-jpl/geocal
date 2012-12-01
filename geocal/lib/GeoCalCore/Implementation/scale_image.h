#ifndef SCALE_IMAGE_H
#define SCALE_IMAGE_H
#include "calc_raster.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple adapter that multiples a underlying image by a
  scale factor.
*******************************************************************/

class ScaleImage : public CalcRaster {
public:
  ScaleImage(const boost::shared_ptr<RasterImage>& Data,
	     double Scale_factor)
    : CalcRaster(*Data), raw_data_(Data), scale_(Scale_factor) {}
  virtual ~ScaleImage() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "ScaleImage\n"
       << "  scale factor: " << scale_ << "\n"
       << "  underlying raster: \n" << *raw_data_ << "\n";
  }
  const boost::shared_ptr<RasterImage>& raw_data() const {return raw_data_;}
  double scale_factor() const {return scale_;}
protected:
  virtual void calc(int Lstart, int Sstart) const
  {
    data = scale_ * raw_data_->read_double(Lstart, Sstart, data.rows(), 
					 data.cols());
  }
private:
  boost::shared_ptr<RasterImage> raw_data_;
  double scale_;
};
}
#endif
