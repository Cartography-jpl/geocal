// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "memory_dem.h"
%}
%base_import(dem_map_info)
%geocal_shared_ptr(GeoCal::MemoryDem);
namespace GeoCal {
class MemoryDem : public DemMapInfo {
public:
  MemoryDem(const boost::shared_ptr<Datum>& D, const MapInfo& M, 
	    bool Outside_dem_is_error = false);
  virtual double elevation(int Y_index, int X_index) const;
  boost::multi_array<double, 2> elevation_data;
};
}
