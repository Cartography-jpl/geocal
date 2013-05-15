// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%{
#include "camera.h"
#include "argus_camera.h"
#include "quaternion_camera.h"
#include "quickbird_camera.h"
#include "spot_camera.h"
%}

%geocal_shared_ptr(GeoCal::Camera);
%geocal_shared_ptr(GeoCal::PushBroomCamera);
%geocal_shared_ptr(GeoCal::SimplePushBroomCamera);

%shared_ptr_dynamic_list(GeoCal::Camera,
			 GeoCal::ArgusCamera,
			 GeoCal::QuaternionCamera,
			 GeoCal::SimplePushBroomCamera,
			 GeoCal::QuickBirdCamera,
			 GeoCal::SpotCamera,
			 GeoCal::PushBroomCamera);

%shared_ptr_dynamic_list(GeoCal::PushBroomCamera,
			 GeoCal::SimplePushBroomCamera,
			 GeoCal::QuickBirdCamera,
			 GeoCal::SpotCamera)

namespace GeoCal {
class Camera {
public:
  enum Direction {FORWARD, AFTWARD};
  Camera();
  virtual double integration_time(int Band) const;
  %python_attribute(direction, virtual Direction)
  %python_attribute(number_band, virtual int)
  virtual int number_line(int Band) const = 0;
  virtual int number_sample(int Band) const = 0;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const = 0;
  virtual double frame_line_coordinate(const ScLookVector& Sl, int Band) 
    const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
    int Band) const = 0;
  std::string print_to_string() const;
};

class PushBroomCamera : public Camera {
public:
  virtual int number_line(int Band) const;
};

class SimplePushBroomCamera : public PushBroomCamera {
public:
  SimplePushBroomCamera(double Beta=58*Constant::deg_to_rad, double
     Delta=-2.7*Constant::deg_to_rad, double Epsilon=0, 
     double Focal=123.8e-3, double
     Pitch_line=18e-6, double Pitch_sample=21e-6, int Number_sample=1504);
  virtual int number_sample(int Band) const;
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  %python_attribute(beta, double)
  %python_attribute(delta, double)
  %python_attribute(epsilon, double)
  %python_attribute(focal_length, double)
  %python_attribute(line_pitch, double)
  %python_attribute(sample_pitch, double)
  %pickle_init(1, self.beta, self.delta, self.epsilon, self.focal_length,
	       self.line_pitch, self.sample_pitch, self.number_sample(0))
};
}
%template(Vector_Camera) std::vector<boost::shared_ptr<GeoCal::Camera> >;
