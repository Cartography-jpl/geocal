// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "argus_camera.h"
%}
%base_import(generic_object)
%base_import(observer)
%base_import(quaternion_camera)
%geocal_shared_ptr(GeoCal::ArgusCamera);
namespace GeoCal {
class ArgusCamera : public QuaternionCamera {
public:
  ArgusCamera(double Yaw = 0, double Pitch = 0, double Roll = 0, 
    double Focal_length = 400);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  %python_attribute(roll, double)
  %python_attribute(pitch, double)
  %python_attribute(yaw, double)
  %pickle_serialization();
};
}
