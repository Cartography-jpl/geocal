// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "hdf_file.h"
%}

%base_import(generic_object)
%geocal_shared_ptr(GeoCal::HdfFile);

namespace GeoCal {
class HdfFile : public GenericObject {
public:
  std::string print_to_string();
  enum Mode {READ, CREATE, READ_WRITE};
  HdfFile(const std::string& Fname, Mode M = READ);
  void close();
  %python_attribute(file_name, std::string)
  static bool is_hdf(const std::string& Fname);
  %extend {
    blitz::Array<double, 1> read_double_1d(const std::string& fname)
    { return $self->read_field<double, 1>(fname); }
    blitz::Array<double, 2> read_double_2d(const std::string& fname)
    { return $self->read_field<double, 2>(fname); }
    blitz::Array<double, 3> read_double_3d(const std::string& fname)
    { return $self->read_field<double, 3>(fname); }
    blitz::Array<double, 4> read_double_4d(const std::string& fname)
    { return $self->read_field<double, 4>(fname); }
    blitz::Array<int, 1> read_int_1d(const std::string& fname)
    { return $self->read_field<int, 1>(fname); }
    blitz::Array<int, 2> read_int_2d(const std::string& fname)
    { return $self->read_field<int, 2>(fname); }
    blitz::Array<int, 3> read_int_3d(const std::string& fname)
    { return $self->read_field<int, 3>(fname); }
    blitz::Array<int, 4> read_int_4d(const std::string& fname)
    { return $self->read_field<int, 4>(fname); }
  }
};

}
