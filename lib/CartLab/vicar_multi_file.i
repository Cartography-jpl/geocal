// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "vicar_multi_file.h"
%}
%base_import(raster_multifile)
%geocal_shared_ptr(GeoCal::VicarMultiFile);
namespace GeoCal {
class VicarMultiFile : public RasterMultifile {
public:
  VicarMultiFile(const std::string& Db_name, const std::string& Dirbase,
		 const std::string& Extension = "",
		 int Number_tile_line = 10000, 
		 int Number_tile_sample = 10000, 
		 int Number_line_per_tile = 100000, 
		 int Number_tile_each_file = 4,
		 int Number_tile = 4, bool Favor_memory_mapped = true,
		 bool No_coverage_is_error = true, 
		 int No_coverage_fill_value = 0,
		 bool Force_area_pixel = false);
  virtual void write(int Line, int Sample, int Val);
  %python_attribute(db_name, std::string)
  %python_attribute(dirbase, std::string)
  %python_attribute(extension, std::string)
  %python_attribute(number_tile_line, int)
  %python_attribute(number_tile_sample, int)
  %python_attribute(number_line_per_tile, int)
  %python_attribute(number_tile_each_file, int)
  %python_attribute(favor_memory_mapped, bool)
  %python_attribute(force_area_pixel, bool)
  %python_attribute(no_coverage_is_error, bool)
  %python_attribute(no_coverage_fill_value, int)
  %python_attribute(number_tile, int)
  %pickle_init(1, self.db_name, self.dirbase, self.extension, 
	       self.number_tile_line, self.number_tile_sample,
	       self.number_line_per_tile, self.number_tile_each_file,
	       self.number_tile, self.favor_memory_mapped,
	       self.no_coverage_is_error, self.no_coverage_fill_value,
	       self.force_area_pixel)
};
}
