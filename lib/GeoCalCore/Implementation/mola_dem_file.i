// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "mola_dem_file.h"
%}
%base_import(gdal_dem)
%geocal_shared_ptr(GeoCal::MolaDemFile);

namespace GeoCal {
class MolaDemFile : public GdalDem {
public:
  MolaDemFile(const std::string& Fname, bool Outside_dem_is_error = false,
	      int Number_tile = 4, int Tile_number_line = -1,
	      int Tile_number_sample = -1);
  virtual double elevation(int Y_index, int X_index) const;
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("MolaDemFile")
