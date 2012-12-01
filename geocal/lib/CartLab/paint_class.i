// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "paint_class.h"
%}

%geocal_shared_ptr(PaintClass);

namespace GeoCal {
class PaintClass : public CalcRasterMultiBand {
public:
  PaintClass(const boost::shared_ptr<RasterImage>& Class_id_img,
	     const boost::shared_ptr<RasterImage>& Pan_img,
	     const blitz::Array<int, 2>& Color,
	     int Class_id_first_color);
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}
