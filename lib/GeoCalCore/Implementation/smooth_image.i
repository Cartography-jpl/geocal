// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "smooth_image.h"
%}
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::SmoothImage);

namespace GeoCal {
class SmoothImage : public CalcRaster {
public:
  SmoothImage(const boost::shared_ptr<RasterImage>& Data,
	      int Nhs = 3, int Number_tile = 4);
  %python_attribute2(raw_data, raw_data_ptr, boost::shared_ptr<RasterImage>)
  %python_attribute(number_half_size, int)
  %pickle_init(1, self.raw_data, self.number_half_size, self.number_tile)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
