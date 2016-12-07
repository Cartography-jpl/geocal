// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "mspi_paraxial_transform.h"
%}

%base_import(generic_object)
%import "auto_derivative.i"

%geocal_shared_ptr(GeoCal::MspiParaxialTransform);
namespace GeoCal {
class MspiParaxialTransform : public GenericObject {
public:
  MspiParaxialTransform(const std::string& Fname);
  %python_attribute_with_set(file_name, std::string);
  void paraxial_to_real(int Row_number, double Paraxial_x,
			double Paraxial_y, double& OUTPUT,
			double& OUTPUT);
  void paraxial_to_real(int Row_number, const AutoDerivative<double>& 
			Paraxial_x,
			const AutoDerivative<double>& Paraxial_y, 
			AutoDerivative<double>& OUTPUT, 
			AutoDerivative<double>& OUTPUT) const;
  void real_to_paraxial(int Row_number, double Real_x,
			double Real_y, double& OUTPUT,
			double& OUTPUT);
  void real_to_paraxial(int Row_number,
			const AutoDerivative<double>& Real_x,
			const AutoDerivative<double>& Real_y,
			AutoDerivative<double>& OUTPUT,
			AutoDerivative<double>& OUTPUT) const;
  bool has_row(int Row_number) const;
  std::string print_to_string() const;
  %python_attribute(a, blitz::Array<double, 2>);
  %python_attribute(b, blitz::Array<double, 2>);
  %python_attribute(c, blitz::Array<double, 2>);
  %python_attribute(d, blitz::Array<double, 2>);
  %pickle_serialization();
};
}
