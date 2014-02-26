// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "dem_tiled_file.h"
%}
%base_import(dem_map_info)
%geocal_shared_ptr(GeoCal::DemTiledFile);
namespace GeoCal {
class DemTiledFile : public DemMapInfo {
public:
  %python_attribute(number_tile, unsigned int)
  %python_attribute(number_line_per_tile, int)
  %python_attribute(number_sample_per_tile, int)
  %python_attribute(number_swap, int)
  void reset_number_swap();
};

}
