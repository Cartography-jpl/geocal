// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "igc_msp.h"
%}
%base_import(image_ground_connection)
%import "raster_image.i"
%geocal_shared_ptr(GeoCal::IgcMsp);
namespace GeoCal {
class IgcMsp : public ImageGroundConnectionCopy {
public:
  IgcMsp(const std::string& Fname);
  %pickle_serialization;
};

}

// List of things "import *" will include
%python_export("IgcMsp")
