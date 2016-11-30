// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "mspi_camera.h"
%}

%base_import(quaternion_camera)

%geocal_shared_ptr(GeoCal::MspiCamera);
namespace GeoCal {
class MspiCamera : public QuaternionCamera {
public:
  MspiCamera(const std::string& Fname, 
	     const std::string& Extra_config_file = "");
  MspiCamera(const std::string& Fname, const blitz::Array<double, 1>& Param);
  void read_config_file(const std::string& File_name);
  %python_attribute_with_set(file_name, std::string);
  %python_attribute_with_set(granule_id, std::string);
  %python_attribute(epsilon, double);
  %python_attribute(psi, double);
  %python_attribute(theta, double);
  %python_attribute(yaw, double);
  %python_attribute(pitch, double);
  %python_attribute(roll, double);
  %python_attribute(yaw_with_derivative, AutoDerivative<double>);
  %python_attribute(pitch_with_derivative, AutoDerivative<double>);
  %python_attribute(roll_with_derivative, AutoDerivative<double>);
  %python_attribute(boresight_angle, double);
  %python_attribute(inversion, bool);
  %python_attribute_with_set(parameter_mask, blitz::Array<bool, 1>);
  double angular_separation(int Reference_band, int Target_band) const;
  void paraxial_offset(int Band,
		       const FrameCoordinate& F,
		       double& OUTPUT,
		       double& OUTPUT) const;
  int row_number(int Band) const;
  int band_number(int Row_number) const;
  std::string print_to_string() const;
  %pickle_serialization();
};
}
