// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "air_mspi_file.h"
%}

namespace GeoCal {
  std::string air_mspi_permanent_file_name(const std::string& Fname);
  std::string air_mspi_local_file_name(const std::string& Fname);
}

// List of things "import *" will include
%python_export("air_mspi_local_file_name", "air_mspi_permanent_file_name")
