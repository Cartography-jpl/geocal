// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "raster_multifile.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::RasterMultifile);

namespace GeoCal {
%nodefaultctor RasterMultifile;
class RasterMultifile : public RasterImageVariable {
public:
};
}