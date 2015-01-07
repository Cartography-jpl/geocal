// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_config_file.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::MspiConfigFile);
%geocal_shared_ptr(GeoCal::MspiConfigTable);
namespace GeoCal {
class MspiConfigFile : public GenericObject {
public:
  MspiConfigFile(const std::string& Fname);
  void add_file(const std::string& Fname);
  bool have_key(const std::string& Key) const;
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double value_double(const std::string& Key) const
    { return $self->value<double>(Key); }
    double value_int(const std::string& Key) const
    { return $self->value<int>(Key); }
    std::string value_string(const std::string& Key) const
    { return $self->value<std::string>(Key); }
  }
  std::string print_to_string() const;
  // Doesn't work, we'll want to extend this to use boost
  // serialization at some point.
  //  %pickle_init(1, self.file_name);
};

class MspiConfigTable : public GenericObject {
public:
  MspiConfigTable(const MspiConfigFile& Config, const std::string& Table_name);
  %python_attribute(number_row);
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double value_double(int Index, const std::string& Column) const
    { return $self->value<double>(Index, Column); }
    double value_int(int Index, const std::string& Column) const
    { return $self->value<int>(Index, Column); }
    std::string value_string(int Index, const std::string& Column) const
    { return $self->value<std::string>(Index, Column); }
  }
  std::string print_to_string() const;
  // Doesn't work, we'll want to extend this to use boost
  // serialization at some point.
  //  %pickle_init(1, self.file_name);
};
}
