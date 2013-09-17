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
public:
  virtual boost::shared_ptr<RasterImage> overview(int Min_number_sample) const;
  %python_attribute(number_tile_line, virtual int)
  %python_attribute(number_tile_sample, virtual int)
  virtual int read(int Line, int Sample) const;
  virtual void write(int Line, int Sample, int Val);
  virtual void write(int Lstart, int Sstart, const blitz::Array<int, 2>& A);
  virtual blitz::Array<double, 2> 
  read_double(int Lstart, int Sstart, int Number_line, 
	      int Number_sample) const;
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
