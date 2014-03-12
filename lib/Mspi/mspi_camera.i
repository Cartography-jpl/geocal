// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "mspi_camera.h"
%}

%base_import(camera)

%geocal_shared_ptr(GeoCal::MspiCamera);
namespace GeoCal {
class MspiCamera : public Camera {
public:
  MspiCamera(const std::string& Fname);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
    int Band) const;
  %python_attribute(file_name, std::string);
  %python_attribute(epsilon, double);
  %python_attribute(psi, double);
  %python_attribute(theta, double);
  %python_attribute(yaw, double);
  %python_attribute(pitch, double);
  %python_attribute(roll, double);
  %python_attribute(boresight_angle, double);
  %python_attribute(focal_length, double);
  %python_attribute(dx, double);
  %python_attribute(dy, double);
  %python_attribute(ypitch, double);
  %python_attribute(s_origin, double);
  %python_attribute(line_direction, int);
  %python_attribute(pixel_order, int);
  std::string print_to_string() const;
  %pickle_init(1, self.file_name);
};
}
