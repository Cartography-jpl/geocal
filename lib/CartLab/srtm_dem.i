// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "srtm_dem.h"
%}
%base_import(dem_map_info)
%base_import(cart_lab_multifile)
%geocal_shared_ptr(GeoCal::SrtmDem);
%geocal_shared_ptr(GeoCal::SrtmDemData);
%geocal_shared_ptr(GeoCal::SrtmLwmData);

namespace GeoCal {
class SrtmDemData: public VicarCartLabMultifile {
public:
  SrtmDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true, bool Force_area_pixel = true);
  %pickle_serialization()
};

class SrtmLwmData: public VicarCartLabMultifile {
public:
  SrtmLwmData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true, bool Force_area_pixel = true);
  %pickle_serialization()
};
  
class SrtmDem : public DemMapInfo {
public:
  SrtmDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(directory_base, std::string);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("SrtmDem", "SrtmDemData", "SrtmLwmData")
