#ifndef PAINT_CLASS_H
#define PAINT_CLASS_H
# include "calc_raster_multi_band.h"

namespace GeoCal {
/****************************************************************//**
  This takes two images, a image with the class ID and a pan band.
  It then produces a three color image that is either the given pan
  band or a color given by the class ID if the class ID is > 0.
  
  This duplicates what was done with f2multi. This is a pretty
  specific routine, and is likely to be replaced with something else
  at some point.
*******************************************************************/

class PaintClass : public CalcRasterMultiBand {
public:
  PaintClass(const boost::shared_ptr<RasterImage>& Class_id_img,
	     const boost::shared_ptr<RasterImage>& Pan_img,
	     const blitz::Array<int, 2>& Color,
	     int Class_id_first_color = 8881);
  virtual ~PaintClass() {}
protected:
  virtual void calc(int Lstart, int Sstart) const;
private:
  boost::shared_ptr<RasterImage> class_id_img, pan_img;
  blitz::Array<int, 2> color;
  int class_id_first_color;
};
}
#endif
