#ifndef APPLY_MASK_H
#define APPLY_MASK_H
#include "calc_raster.h"

namespace GeoCal {
/****************************************************************//**
  This applies a mask to a RasterImage. The data either passes through
  the mask, or it set to a value of 0 (the normal gore value we use)
  if a second RasterImage indicates that this is masked.
*******************************************************************/

class ApplyMask : public CalcRaster {
public:
//-----------------------------------------------------------------------
/// Constructor.
/// \param Data Underlying data
/// \param M  Mask image
/// \param Good_value Pixels with this value in the the image M allow
//    Data pixel to pass through. Otherwise pixel is set to 0.
//-----------------------------------------------------------------------

  ApplyMask(const boost::shared_ptr<RasterImage>& Data,
	    const boost::shared_ptr<RasterImage>& M,
	    int Good_value = 0)
    : CalcRaster(*Data), raw_data_(Data), mask_(M), good_value_(Good_value) 
  { 
    if(M->number_line() != number_line() ||
       M->number_sample() != number_sample())
      throw Exception("Mask and underlying data need to be the same size");
  }
  virtual ~ApplyMask() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "ApplyMask\n"
       << "  good value: " << good_value_ << "\n"
       << "  underlying raster: \n" << *raw_data_ << "\n"
       << "  underlying mask:\n" << *mask_ << "\n";
  }
  int good_value() const {return good_value_;}
  boost::shared_ptr<RasterImage> raw_data() const {return raw_data_;}
  boost::shared_ptr<RasterImage> mask() const {return mask_;}
protected:
  virtual void calc(int Lstart, int Sstart) const
  {
    data = raw_data_->read_double(Lstart, Sstart, data.rows(), data.cols());
    data = blitz::where(mask_->read(Lstart, Sstart, data.rows(), data.cols()) 
			== good_value_, data, 0.0);
  }
private:
  boost::shared_ptr<RasterImage> raw_data_, mask_;
  int good_value_;
};  
}
#endif
