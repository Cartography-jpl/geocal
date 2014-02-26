#ifndef SMOOTH_IMAGE_H
#define SMOOTH_IMAGE_H
#include "calc_raster.h"
namespace GeoCal {
/****************************************************************//**
  This creates a smoothed version of a RasterImage. We do this by
  averaging a given region of the image, average 
  (2 * nhs + 1) x (2 * nhs + 1) pixels centered on the resulting
  pixel. 

  Rather than doing something complicated at the edges, we simply trim
  the underlying image by a border of nhs.
*******************************************************************/
class SmoothImage : public CalcRaster {
public:
  SmoothImage(const boost::shared_ptr<RasterImage>& Data,
	      int Nhs = 3, int Number_tile = 4);
  virtual ~SmoothImage() {}
  virtual void print(std::ostream& Os) const
  {
    Os << "Smooth Image\n"
       << "  Number half size: " << nhs << "\n"
       << "  underlying raster: \n" << *raw_data << "\n";
  }
  const boost::shared_ptr<RasterImage>& raw_data_ptr() const {return raw_data;}
  int number_half_size() const {return nhs;}
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> raw_data;
  int nhs;
};
}
#endif
