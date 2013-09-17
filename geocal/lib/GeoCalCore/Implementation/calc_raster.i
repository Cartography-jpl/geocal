// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "calc_raster.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::CalcRaster);

namespace GeoCal {
// Allow this class to be derived from in Python.
%feature("director") CalcRaster;

class CalcRaster : public RasterImageVariable {
protected:
  %python_attribute2_nonconst(data, data_ptr, blitz::Array<double, 2>&)
  virtual void calc(int Lstart, int Sstart) const = 0; 
  CalcRaster(int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp,int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp, int Ntileln, int Ntilesmp,
	     int Number_tile = 4);
  CalcRaster(const MapInfo& Mi,int Number_tile = 4);
  CalcRaster(const RasterImage& Img,int Number_tile = 4);
};

}
