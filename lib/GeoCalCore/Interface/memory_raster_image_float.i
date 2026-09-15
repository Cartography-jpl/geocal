%include "geocal_common.i"

%{
#include "memory_raster_image_float.h"
#include "image_ground_connection.h"
%}

%base_import(raster_image_variable)

%geocal_shared_ptr(GeoCal::MemoryRasterImageFloat);
namespace GeoCal {
class MemoryRasterImageFloat : public RasterImageVariable {
public:
  MemoryRasterImageFloat(int Number_line = 0, int Number_sample = 0);
  MemoryRasterImageFloat(int Number_line, int Number_sample, double Fill_value);
  MemoryRasterImageFloat(const MapInfo& Mi);
  virtual void write(int Lstart, int Sstart, const blitz::Array<int, 2>& A);
  virtual void write(int Lstart, int Sstart, const blitz::Array<double, 2>& A);
  %python_attribute(data, blitz::Array<double, 2>);
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("MemoryRasterImageFloat")
