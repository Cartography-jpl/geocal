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
  %python_attribute(file_name, std::string);
  bool have_key(const std::string& Key) const;
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double value_double(const std::string& Key) const
    { return $self->value<double>(Key); }
    std::string value_string(const std::string& Key) const
    { return $self->value<std::string>(Key); }
  }
  std::string print_to_string() const;
  %pickle_init(1, self.file_name);
};
}
