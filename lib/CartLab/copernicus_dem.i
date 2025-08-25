// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "copernicus_dem.h"
#include "image_ground_connection.h"
%}
%base_import(dem_map_info)
%base_import(cart_lab_multifile)
%geocal_shared_ptr(GeoCal::CopernicusDem);
%geocal_shared_ptr(GeoCal::CopernicusDemData);
%geocal_shared_ptr(GeoCal::CopernicusLwmData);

namespace GeoCal {
class CopernicusDemData: public GdalCartLabMultifile {
public:
  CopernicusDemData(const std::string& Dir,
		    bool No_coverage_is_error = true,
		    int Number_line_per_tile = -1,
		    int Number_sample_per_tile = -1, 
		    int Number_tile_each_file = 4, int Number_file = 4);
  %pickle_serialization()
};

class CopernicusLwmData: public GdalCartLabMultifile {
public:
  enum {LAND=0, OCEAN=1, MIN_LAKE_DROUGHT=2, ADD_LAKE_DROUGHT=3,
	MIN_RIVER_DROUGHT=4, ADD_RIVER_DROUGHT=5, PERM_LAKE=6,
	MOSTLY_PERM_LAKE=7, SEASONAL_FLOOD=8, EPHEMERAL_FLOOD=9};
  CopernicusLwmData(const std::string& Dir,
		    bool No_coverage_is_error = true,
		    int Number_line_per_tile = -1,
		    int Number_sample_per_tile = -1, 
		    int Number_tile_each_file = 4, int Number_file = 4);
  %pickle_serialization()
};
  
class CopernicusDem : public DemMapInfo {
public:
  CopernicusDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(directory_base, std::string);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("CopernicusDem", "CopernicusDemData", "CopernicusLwmData")
