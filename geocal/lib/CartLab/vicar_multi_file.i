// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "vicar_multi_file.h"
%}

%geocal_shared_ptr(VicarMultiFile);
namespace GeoCal {
class VicarMultiFile : public RasterMultifile {
public:
  VicarMultiFile(const std::string& Db_name, const std::string& Dirbase,
		 const std::string& Extension = "",
		 int Number_tile_line = 10000, 
		 int Number_tile_sample = 10000, 
		 int Number_line_per_tile = 100000, 
		 int Number_tile_each_file = 4,
		 int Number_file = 4, bool Favor_memory_mapped = true,
		 bool No_coverage_is_error = true, 
		 int No_coverage_fill_value = 0,
		 bool Force_area_pixel = false);
  virtual void write(int Line, int Sample, int Val);
};
}
