// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  void add(const std::string& Keyword, const std::string& Value);
  bool have_key(const std::string& Key) const;
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double value_double(const std::string& Key) const
    { return $self->value<double>(Key); }
    std::vector<double> value_vector_double(const std::string& Key) const
    { return $self->value<std::vector<double> >(Key); }
    int value_int(const std::string& Key) const
    { return $self->value<int>(Key); }
    std::vector<int> value_vector_int(const std::string& Key) const
    { return $self->value<std::vector<int> >(Key); }
    std::string value_string(const std::string& Key) const
    { return $self->value<std::string>(Key); }
    std::vector<std::string> value_vector_string(const std::string& Key) const
    { return $self->value<std::vector<std::string> >(Key); }
    bool value_bool(const std::string& Key) const
    { return $self->value<bool>(Key); }
    std::vector<bool> value_vector_bool(const std::string& Key) const
    { return $self->value<std::vector<bool> >(Key); }
  }
  std::string print_to_string() const;
  %pickle_serialization();
};

class MspiConfigTable : public GenericObject {
public:
  MspiConfigTable(const MspiConfigFile& Config, const std::string& Table_name);
  %python_attribute(number_row, int);
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double value_double(int Index, const std::string& Column) const
    { return $self->value<double>(Index, Column); }
    int value_int(int Index, const std::string& Column) const
    { return $self->value<int>(Index, Column); }
    std::string value_string(int Index, const std::string& Column) const
    { return $self->value<std::string>(Index, Column); }
  }
  std::string print_to_string() const;
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("MspiConfigFile", "MspiConfigTable")
