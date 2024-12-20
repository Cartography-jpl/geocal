// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "calc_raster_multi_band.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_multi_band_variable)
%geocal_shared_ptr(GeoCal::CalcRasterMultiBand);

namespace GeoCal {
// Allow this class to be derived from in Python.
%feature("director") CalcRasterMultiBand;

class CalcRasterMultiBand: public RasterImageMultiBandVariable {
public:
  virtual boost::shared_ptr<RasterImage> raster_image_ptr(int band);
protected:
  %python_attribute2_nonconst(data, data_ptr, blitz::Array<double, 3>&)
  virtual std::string desc() const;
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
  %pickle_serialization();
};

}

// Extra code for handling boost serialization/python pickle of
// director classes
%geocal_director_serialization(calc_raster_multi_band, CalcRasterMultiBand)

// List of things "import *" will include
%python_export("CalcRasterMultiBand")
