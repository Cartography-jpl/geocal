// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "scale_image.h"
%}

%geocal_shared_ptr(ScaleImage);

namespace GeoCal {
class ScaleImage : public CalcRaster {
public:
  ScaleImage(const boost::shared_ptr<RasterImage>& Data,
	     double Scale_factor);
  %python_attribute(raw_data, boost::shared_ptr<RasterImage>)
  %python_attribute(scale_factor, double)
  %pickle_init(self.raw_data, self.scale_factor)
protected:
  virtual void calc(int Lstart, int Sstart) const;
};

}
