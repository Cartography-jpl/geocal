// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "calc_raster_multi_band.h"
%}

%geocal_shared_ptr(CalcRasterMultiBand);

namespace GeoCal {
class CalcRasterMultiBand: public RasterImageMultiBandVariable {
public:
  virtual blitz::Array<double, 3> read_double(int Lstart, int Sstart, 
					      int Nline, int Nsamp) const;
protected:
  mutable blitz::Array<double, 3> data;
  virtual void calc(int Lstart, int Sstart) const = 0; 
  CalcRasterMultiBand();
  CalcRasterMultiBand(const RasterImage& Img, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4);
  void initialize(const RasterImage& Img, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
};

}
