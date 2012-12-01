// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "doughnut_average.h"
%}

%geocal_shared_ptr(DoughnutAverage);

namespace GeoCal {
class DoughnutAverage : public CalcRasterMultiBand {
public:
  DoughnutAverage(const boost::shared_ptr<RasterImageMultiBand>& Rimg, 
		  int Window_size = 11, int Doughnut_size = 5, 
		  bool Allow_gore = true,
		  int Tile_number_line = -1, int Tile_number_sample = -1);
  blitz::Array<double, 2> cvdnorm(int band, int Lstart, int Sstart,
				  int Number_line, int Number_sample) const;
  blitz::Array<double, 2> pandif(int Lstart, int Sstart,
				 int Number_line, int Number_sample) const;
  boost::shared_ptr<RasterImage> cvdnorm_raster_image(int band) const;
  %python_attribute(pandif_raster_image, boost::shared_ptr<RasterImage>)
  %python_attribute(window_size, int)
  %python_attribute(doughnut_size, int)
  %python_attribute(allow_gore, bool)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};
}
