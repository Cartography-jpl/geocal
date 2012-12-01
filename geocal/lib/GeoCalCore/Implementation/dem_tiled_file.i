// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "dem_tiled_file.h"
%}

%geocal_shared_ptr(DemTiledFile);
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
