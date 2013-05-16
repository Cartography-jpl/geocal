// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "gdal_multi_file.h"
%}

%geocal_shared_ptr(GeoCal::GdalMultiFile);
namespace GeoCal {
class GdalMultiFile : public MapProjectedMultifile {
public:
  GdalMultiFile(const std::vector<std::string>& File_list, int band = 1,
		bool No_coverage_is_error = true,
		int Size_multiple_of = 1, int Number_line_per_tile = -1,
		int Number_sample_per_tile = -1, 
		int Number_tile_each_file = 4, int Number_file = 4,
		int No_coverage_fill_value = 0);
  virtual void write(int Line, int Sample, int Val);
};
}
