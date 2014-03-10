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
  // Don't bother with access routines. We probably aren't ever going
  // to need them. If we do, we can add them as needed for the
  // particular types (e.g., "value_double");
  std::string print_to_string() const;
  %pickle_init(1, self.file_name);
};
}
