// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "calc_raster.h"
%}

%geocal_shared_ptr(CalcRaster);

namespace GeoCal {
class CalcRaster : public RasterImageVariable {
public:
  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const;
protected:
  mutable blitz::Array<double, 2> data;
  virtual void calc(int Lstart, int Sstart) const = 0; 
  CalcRaster(int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp,int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp, int Ntileln, int Ntilesmp,
	     int Number_tile = 4);
  CalcRaster(const MapInfo& Mi,int Number_tile = 4);
  CalcRaster(const RasterImage& Img,int Number_tile = 4);
};

}
