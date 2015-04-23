// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_l1b1.h"
%}
%base_import(raster_image_tiled_file)
%geocal_shared_ptr(GeoCal::AirMspiL1b1);

namespace GeoCal {
  // This is actually a TiledFile<float, 2>, but we don't expose
  // TiledFile to python
class AirMspiL1b1: public GenericObject {
public:
  AirMspiL1b1(const std::string& Fname);
};
}
