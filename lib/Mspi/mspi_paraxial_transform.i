// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_paraxial_transform.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::MspiParaxialTransform);
namespace GeoCal {
class MspiParaxialTransform : public GenericObject {
public:
  MspiParaxialTransform(const std::string& Fname);
  %python_attribute(file_name, std::string);
  void paraxial_to_real(int Row_number, double Paraxial_x,
			double Paraxial_y, double& OUTPUT,
			double& OUTPUT);
  void real_to_paraxial(int Row_number, double Real_x,
			double Real_y, double& OUTPUT,
			double& OUTPUT);
  bool has_row(int Row_number) const;
  std::string print_to_string() const;
  %pickle_serialization();
};
}
