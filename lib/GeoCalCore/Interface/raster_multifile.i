// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "raster_multifile.h"
%}
%base_import(raster_image_variable)
%geocal_shared_ptr(GeoCal::RasterMultifile);

namespace GeoCal {
%nodefaultctor RasterMultifile;
class RasterMultifile : public RasterImageVariable {
  %python_attribute(number_file_read, int);
  void reset_number_file_read();
public:
};
}
// List of things "import *" will include
%python_export("RasterMultifile")
