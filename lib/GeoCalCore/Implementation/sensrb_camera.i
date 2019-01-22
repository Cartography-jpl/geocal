// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "sensrb_camera.h"
%}

%base_import(quaternion_camera)

%geocal_shared_ptr(GeoCal::SensrbCamera);
namespace GeoCal {
class SensrbCamera : public QuaternionCamera {
public:
  SensrbCamera(boost::math::quaternion<double> Frame_to_sc_q,
	       double K1, double K2, double K3, double P1, double P2,
	       double B1, double B2,
	       int Number_line, int Number_sample,
	       double Line_pitch, double Sample_pitch,
	       double Focal_length, 
	       const FrameCoordinate& Principal_point,
	       QuaternionCamera::FrameConvention Frame_convention =
	       QuaternionCamera::LINE_IS_X,
	       QuaternionCamera::FrameDirection Line_direction =
	       QuaternionCamera::INCREASE_IS_POSITIVE,
	       QuaternionCamera::FrameDirection Sample_direction =
	       QuaternionCamera::INCREASE_IS_POSITIVE);
  %python_attribute(p_distort, const blitz::Array<double, 1>&);
  %python_attribute_with_set(k1, double);
  %python_attribute_with_set(k2, double);
  %python_attribute_with_set(k3, double);
  %python_attribute_with_set(p1, double);
  %python_attribute_with_set(p2, double);
  %python_attribute_with_set(b1, double);
  %python_attribute_with_set(b2, double);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("SensrbCamera")
