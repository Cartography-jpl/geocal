// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "memory_raster_image.h"
%}
%base_import(raster_image_variable)

%geocal_shared_ptr(GeoCal::MemoryRasterImage);

namespace GeoCal {
class MemoryRasterImage : public RasterImageVariable {
public:
  MemoryRasterImage(int Number_line, int Number_sample);
  MemoryRasterImage(int Number_line, int Number_sample, int Fill_value);
  MemoryRasterImage(const RasterImage& Img, int Number_line_to_read = -1, 
	int Number_sample_to_read = -1);
  MemoryRasterImage(const MapInfo& Mi);
  %pickle_serialization();
};

}
// List of things "import *" will include
%python_export("MemoryRasterImage")
