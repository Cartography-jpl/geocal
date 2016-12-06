// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "constant_raster_image.h"
%}
%base_import(raster_image_variable)

%geocal_shared_ptr(GeoCal::ConstantRasterImage);

namespace GeoCal {
class ConstantRasterImage : public RasterImageVariable {
public:
  ConstantRasterImage(int Number_line, int Number_sample, int Value);
  ConstantRasterImage(const MapInfo& Mi, int Value);
};

}
