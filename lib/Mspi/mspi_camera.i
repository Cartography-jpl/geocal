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
  MspiParaxialTransform(const std::string& Fname);
  void paraxial_to_real(int Row_number, double Paraxial_x,
			double Paraxial_y, double& OUTPUT,
			double& OUTPUT);
  void real_to_paraxial(int Row_number, double Real_x,
			double Real_y, double& OUTPUT,
			double& OUTPUT);
  bool has_row(int Row_number) const;
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
    int Band) const;
  %python_attribute(file_name, std::string);
  std::string print_to_string() const;
  %pickle_init(1, self.file_name);
};
}
