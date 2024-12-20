// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "memory_multi_band.h"
#include "image_ground_connection.h"
%}
%base_import(raster_image_multi_band_variable)
%geocal_shared_ptr(GeoCal::MemoryMultiBand);

namespace GeoCal {
class MemoryMultiBand : public RasterImageMultiBandVariable {
public:
  MemoryMultiBand(const RasterImageMultiBand& R);
  %python_attribute(data, blitz::Array<int, 3>)
};
}
// List of things "import *" will include
%python_export("MemoryMultiBand")
