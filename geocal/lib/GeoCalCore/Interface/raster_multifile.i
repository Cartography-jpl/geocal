// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "raster_multifile.h"
%}

%geocal_shared_ptr(RasterMultifile);

namespace GeoCal {
%nodefaultctor RasterMultifile;
class RasterMultifile : public RasterImageVariable {
public:
};
}
