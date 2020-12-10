// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "trex_dem.h"
#include "image_ground_connection.h"
%}
%base_import(dem_map_info)
%base_import(cart_lab_multifile)
%geocal_shared_ptr(GeoCal::TrexDem);
%geocal_shared_ptr(GeoCal::TrexDemData);
%geocal_shared_ptr(GeoCal::TrexLwmData);

namespace GeoCal {
class TrexDemData: public VicarCartLabMultifile {
public:
  TrexDemData(const std::string& Dir,
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true);
  %pickle_serialization()
};

class TrexLwmData: public VicarCartLabMultifile {
public:
  enum {OCEAN=0, COSTLINE=1, LAND=2, PERMANENT_INLAND_WATER=3,
	TRANSITIONAL_INLAND_WATER=4, SEASONAL_INLAND_WATER=5,
	EPHEMERAL_INLAND_WATER=6};
  TrexLwmData(const std::string& Dir="",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4,
	      bool Favor_memory_mapped = true);
  %pickle_serialization()
};
  
class TrexDem : public DemMapInfo {
public:
  TrexDem(const std::string& Dirbase ="",
	  bool Outside_dem_is_error = true,
	  const boost::shared_ptr<Datum>& D = 
	  boost::shared_ptr<Datum>(new DatumGeoid96()));
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(directory_base, std::string);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("TrexDem", "TrexDemData", "TrexLwmData")
