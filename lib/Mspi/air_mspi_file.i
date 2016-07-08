// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "common.i"

%{
#include "air_mspi_file.h"
%}

namespace GeoCal {
  std::string air_mspi_permanent_file_name(const std::string& Fname);
  std::string air_mspi_local_file_name(const std::string& Fname);
}
