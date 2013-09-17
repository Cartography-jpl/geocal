// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "calc_raster_multi_band.h"
%}
%base_import(raster_image_multi_band_variable)
%geocal_shared_ptr(GeoCal::CalcRasterMultiBand);

namespace GeoCal {
// Allow this class to be derived from in Python.
%feature("director") CalcRasterMultiBand;

class CalcRasterMultiBand: public RasterImageMultiBandVariable {
public:
  virtual blitz::Array<double, 3> read_double(int Lstart, int Sstart, 
					      int Nline, int Nsamp) const;
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band);
  %python_attribute(number_band, virtual int)
protected:
  %python_attribute2_nonconst(data, data_ptr, blitz::Array<double, 3>&)
  virtual void calc(int Lstart, int Sstart) const = 0; 
  CalcRasterMultiBand();
  CalcRasterMultiBand(const RasterImage& Img, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4);
  CalcRasterMultiBand(int Nline, int Nsamp, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4);
  CalcRasterMultiBand(const MapInfo& Mi, int Nband,
		      int Number_tile_line = -1, int Number_tile_sample = -1, 
		      int Number_tile = 4);
  void initialize(const RasterImage& Img, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
  void initialize(int Nline, int Nsamp, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
  void initialize(const MapInfo&, int Nband,
		  int Number_tile_line = -1, int Number_tile_sample = -1, 
		  int Number_tile = 4);
};

}
