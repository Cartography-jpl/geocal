// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_gimbal.h"
%}

%base_import(with_parameter)
%base_import(observer)
%import "auto_derivative.i"
%geocal_shared_ptr(GeoCal::MspiGimbal);

%geocal_shared_ptr(GeoCal::Observable<GeoCal::MspiGimbal>);
%geocal_shared_ptr(GeoCal::Observer<GeoCal::MspiGimbal>);

namespace GeoCal {

%template(ObservableMspiGimbal) GeoCal::Observable<GeoCal::MspiGimbal>;
%template(ObserverMspiGimbal) GeoCal::Observer<GeoCal::MspiGimbal>;

class MspiGimbal : public WithParameter, public Observable<MspiGimbal> {
public:
  MspiGimbal(double Epsilon, double Psi, double Theta);
  MspiGimbal(const std::string& File_name, 
	     const std::string& Extra_config_file = "");

  virtual void add_observer(Observer<MspiGimbal>& Obs);
  virtual void remove_observer(Observer<MspiGimbal>& Obs);

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
  station_to_sc_with_derivative(const AutoDerivative<double>& Gimbal_pos) const;
  boost::math::quaternion<double>  
  station_to_sc(double Gimbal_pos) const;
  std::string print_to_string() const;
  %pickle_serialization();
protected:
  void notify_update_do(const MspiGimbal& Self);
};
}
