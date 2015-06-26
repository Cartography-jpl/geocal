// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_gimbal.h"
%}

%base_import(with_parameter)
%import "auto_derivative.i"
%geocal_shared_ptr(GeoCal::MspiGimbal);

namespace GeoCal {
class MspiGimbal : public WithParameter {
public:
  MspiGimbal(double Epsilon, double Psi, double Theta);
  MspiGimbal(const std::string& File_name, 
	     const std::string& Extra_config_file = "");
  void read_config_file(const std::string& File_name,
			const std::string& Extra_config_file = "");
  %python_attribute(epsilon, double);
  %python_attribute(epsilon_with_derivative, AutoDerivative<double>);
  %python_attribute(psi, double);
  %python_attribute(psi_with_derivative, AutoDerivative<double>);
  %python_attribute(theta, double);
  %python_attribute(theta_with_derivative, AutoDerivative<double>);
  %python_attribute_with_set(parameter_mask, blitz::Array<bool, 1>);
  boost::math::quaternion<AutoDerivative<double> >  
  station_to_sc(const AutoDerivative<double>& Gimbal_pos) const;
  %pickle_serialization();
};
}
