// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "msp_support.h"
%}
%import "image_coordinate.i"
%import "ecr.i"

namespace GeoCal {
  bool have_msp_supported();
  Ecr msp_terrain_point(const std::string& Fname, const ImageCoordinate& Ic);
}

// List of things "import *" will include
%python_export("have_msp_supported", "msp_terrain_point")
