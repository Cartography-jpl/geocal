// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "argus_camera.h"
%}

%geocal_shared_ptr(ArgusCamera);
namespace GeoCal {
class ArgusCamera : public Camera {
public:
  ArgusCamera(double Yaw = 0, double Pitch = 0, double Roll = 0, 
    double Focal_length = 400);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  %python_attribute(focal_length, double)
  %python_attribute(line_pitch, double)
  %python_attribute(sample_pitch, double)
  %python_attribute(roll, double)
  %python_attribute(pitch, double)
  %python_attribute(yaw, double)
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  %pickle_init(self.yaw, self.pitch, self.roll, self.focal_length)
};
}
