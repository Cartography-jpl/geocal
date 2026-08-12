// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "calc_raster.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::CalcRaster);

namespace GeoCal {
// Allow this class to be derived from in Python.
%feature("director") CalcRaster;

class CalcRaster : public RasterImageVariable {
protected:
  %python_attribute2_nonconst(data, data_ptr, blitz::Array<double, 2>&)
  virtual std::string desc() const;
  virtual void calc(int Lstart, int Sstart) const = 0; 
  CalcRaster(int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp,int Number_tile = 4);
  CalcRaster(int Nline, int Nsamp, int Ntileln, int Ntilesmp,
	     int Number_tile = 4);
  CalcRaster(const MapInfo& Mi,int Number_tile = 4);
  CalcRaster(const RasterImage& Img,int Number_tile = 4);
  %pickle_serialization();
};

}

// Extra code for handling boost serialization/python pickle of
// director classes
%geocal_director_serialization(calc_raster, CalcRaster)

// List of things "import *" will include
%python_export("CalcRaster")
