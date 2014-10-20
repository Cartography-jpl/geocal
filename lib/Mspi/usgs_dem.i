// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "usgs_dem.h"
%}

%base_import(dem_map_info)
%base_import(raster_multifile)

%geocal_shared_ptr(GeoCal::UsgsDemData);
%geocal_shared_ptr(GeoCal::UsgsDem);
namespace GeoCal {
class UsgsDemData: public RasterMultifile {
public:
  enum {FILL_VALUE=0};
  UsgsDemData(const std::string& Dir = "",
	      bool No_coverage_is_error = true,
	      int Number_line_per_tile = -1,
	      int Number_sample_per_tile = -1, 
	      int Number_tile_each_file = 4, int Number_file = 4);
protected:
  virtual RasterMultifileTile get_file(int Line, int Sample) const;
};

class UsgsDem : public DemMapInfo {
public:
  UsgsDem(const std::string& Dir = "",
	  bool Outside_dem_is_error = true);
  UsgsDem(const std::string& Dir,
	  bool Outside_dem_is_error,
	  const boost::shared_ptr<Datum>& D);

  %python_attribute(directory_base, std::string);
  %pickle_init(1, self.directory_base, self.outside_dem_is_error, self.datum);

protected:
  virtual double elevation(int Y_index, int X_index) const;
};
}
