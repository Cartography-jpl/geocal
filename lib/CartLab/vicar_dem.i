// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "vicar_dem.h"
#include "ecr.h"
#include "image_ground_connection.h"
%}
%base_import(dem_tiled_file)
%import "vicar_file.i"
%geocal_shared_ptr(GeoCal::VicarDem);
namespace GeoCal {
class VicarDem : public DemTiledFile {
public:
// SWIG gets confused by too complicated a default argument. We split this
// into calls with and without the Datum, although the same C++ function is
// called in both cases. The default Datum is NoDatum.
  VicarDem(const std::string& Fname, 
	   bool Outside_dem_is_error = false, 
	   int Number_line_per_tile = 100, int Number_tile = 4);
  VicarDem(const std::string& Fname, 
	   bool Outside_dem_is_error, 
	   int Number_line_per_tile, int Number_tile,
	   const boost::shared_ptr<Datum>& D);
  VicarDem(int Instance, 
	   bool Outside_dem_is_error = false, 
	   int Number_line_per_tile = 100, int Number_tile = 4,
	   const std::string& Name = "INP");
  VicarDem(int Instance, 
	   bool Outside_dem_is_error, 
	   int Number_line_per_tile, int Number_tile,
	   const std::string& Name,
	   const boost::shared_ptr<Datum>& D);
  %python_attribute2(vicar_file, vicar_file_ptr, boost::shared_ptr<VicarFile>)
  virtual double elevation(int Y_index, int X_index) const;
  %pickle_serialization();
};
}
