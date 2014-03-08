// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_config_file.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::MspiConfigFile);
namespace GeoCal {
class MspiConfigFile : public GenericObject {
public:
  MspiConfigFile(const std::string& Fname);
  std::string print_to_string() const;
};
}
