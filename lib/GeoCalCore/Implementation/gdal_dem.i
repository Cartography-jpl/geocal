// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "gdal_dem.h"
%}
%base_import(dem_tiled_file)
%import "geocal_datum.i"
%geocal_shared_ptr(GeoCal::GdalDem);

namespace GeoCal {
class GdalDem : public DemTiledFile {
public:
  GdalDem(const std::string& Fname);
  GdalDem(const std::string& Fname, 
       const boost::shared_ptr<Datum>& D,int Band_id = 1, 
	  bool Outside_dem_is_error = false,
	  int Number_tile = 4);
  virtual double elevation(int Y_index, int X_index) const;
  %python_attribute(file_name, std::string)
  %python_attribute(band_id, int)
  %pickle_init(1, self.file_name, self.datum, self.band_id, 
	       self.outside_dem_is_error, self.number_tile)
};
}
