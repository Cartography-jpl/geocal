// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "doughnut_average.h"
%}
%base_import(calc_raster_multi_band)
%base_import(calc_raster)
%geocal_shared_ptr(GeoCal::DoughnutAverage);
%geocal_shared_ptr(GeoCal::RasterImageWrapPandif);
%geocal_shared_ptr(GeoCal::RasterImageWrapCvdNorm);

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
  %python_attribute(window_size, int)
  %python_attribute(doughnut_size, int)
  %python_attribute(allow_gore, bool)
  %python_attribute(underlying_image, 
		    boost::shared_ptr<RasterImageMultiBand>)
  %pickle_init(1, self.underlying_image, self.window_size, self.doughnut_size,
	       self.allow_gore, self.raster_image(0).number_tile_line, 
	       self.raster_image(0).number_tile_sample)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

class RasterImageWrapCvdNorm: public CalcRaster {
public:
  RasterImageWrapCvdNorm(const boost::shared_ptr<DoughnutAverage>& Davg, 
			 int Band);
  %python_attribute(band, int)
  %python_attribute(davg, boost::shared_ptr<DoughnutAverage>)
  %pickle_init(1, self.davg, self.band)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

class RasterImageWrapPandif: public CalcRaster {
public:
  RasterImageWrapPandif(const boost::shared_ptr<DoughnutAverage>& Davg);
  %python_attribute(davg, boost::shared_ptr<DoughnutAverage>)
  %pickle_init(1, self.davg)
protected:
  virtual void calc(int Lstart, int Sstart) const; 
};

}
