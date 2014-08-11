// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "usgs_dem.h"
%}

%base_import(raster_multifile)

%geocal_shared_ptr(GeoCal::UsgsDemData);
namespace GeoCal {
class UsgsDemData: public RasterMultifile {
public:
  enum {FILL_VALUE=-9999};
  UsgsDemData(const std::string& Dir = "",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
protected:
  virtual RasterMultifileTile get_file(int Line, int Sample) const;
};
}
