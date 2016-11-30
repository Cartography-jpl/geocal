// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "dem_map_info_offset.h"
%}
%base_import(dem_map_info)
%geocal_shared_ptr(GeoCal::DemMapInfoOffset);

namespace GeoCal {
class DemMapInfoOffset : public DemMapInfo {
public:
  DemMapInfoOffset(const boost::shared_ptr<DemMapInfo>& Dem_underlying,
		   double Height_offset);
  %python_attribute(dem_underlying, boost::shared_ptr<DemMapInfo>)
  %python_attribute(height_offset, double)
  %pickle_serialization();
protected:
  virtual double elevation(int Y_index, int X_index) const;
};
}
