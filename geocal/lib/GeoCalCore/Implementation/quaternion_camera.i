// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "quaternion_camera.h"
%}
%base_import(camera)
%geocal_shared_ptr(GeoCal::QuaternionCamera);
namespace GeoCal {

class QuaternionCamera : public Camera {
public:
  QuaternionCamera(boost::math::quaternion<double> frame_to_sc_q, 
		   double Number_line, double Number_sample,
		   double Line_pitch, double Sample_pitch,
		   double Focal_length, 
		   const FrameCoordinate& Principal_point,
		   double Line_scale = 1.0,
		   double Sample_scale = 1.0);
  virtual int number_line(int Band) const;
  virtual int number_sample(int Band) const;
  %python_attribute_with_set(focal_length, double)
  %python_attribute_with_set(principal_point, FrameCoordinate)
  %python_attribute_with_set(line_pitch, double)
  %python_attribute_with_set(sample_pitch, double)
  %python_attribute_with_set(line_scale, double)
  %python_attribute_with_set(sample_scale, double)
  virtual FrameCoordinate frame_coordinate(const ScLookVector& Sl, 
					   int Band) const;
  virtual ScLookVector sc_look_vector(const FrameCoordinate& F, 
				      int Band) const;
  %python_attribute(frame_to_sc, boost::math::quaternion<double>)
  %pickle_init(1, self.frame_to_sc, self.number_line(0), self.number_sample(0), 
	       self.line_pitch, self.sample_pitch, self.focal_length, 
	       self.principal_point, self.line_scale, self.sample_scale)
};
}

