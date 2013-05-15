// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "raster_multifile.h"
%}

%geocal_shared_ptr(GeoCal::RasterMultifile);

namespace GeoCal {
%nodefaultctor RasterMultifile;
class RasterMultifile : public RasterImageVariable {
public:
};
}
