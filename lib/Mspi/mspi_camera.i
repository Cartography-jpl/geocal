// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_camera.h"
%}

%base_import(quaternion_camera)

%geocal_shared_ptr(GeoCal::MspiCamera);
namespace GeoCal {
class MspiCamera : public QuaternionCamera {
public:
  MspiCamera(const std::string& Fname);
  MspiCamera(const std::string& Fname, const blitz::Array<double, 1>& Param);
  void read_config_file(const std::string& File_name);
  %python_attribute(file_name, std::string);
  %python_attribute(granule_id, std::string);
  %python_attribute(epsilon, double);
  %python_attribute(psi, double);
  %python_attribute(theta, double);
  %python_attribute(yaw, double);
  %python_attribute(pitch, double);
  %python_attribute(roll, double);
  %python_attribute(boresight_angle, double);
  %python_attribute(inversion, bool);
  int row_number(int Band) const;
  int band_number(int Row_number) const;
  std::string print_to_string() const;
  %pickle_init(1, self.file_name, self.parameter);
};
}
