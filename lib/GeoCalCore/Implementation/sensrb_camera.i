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
  SensrbCamera(const boost::math::quaternion<double>& Frame_to_sc_q,
	       double K1, double K2, double K3, double P1, double P2,
	       double B1, double B2, double Radial_distort_limit,
	       int Number_line, int Number_sample,
	       double Line_pitch, double Sample_pitch,
	       double Focal_length, 
	       const FrameCoordinate& Principal_point,
	       const std::string& Detection_type = "VIS",
	       const std::string& Calibration_date = "20000101",
	       QuaternionCamera::FrameConvention Frame_convention =
	       QuaternionCamera::LINE_IS_Y,
	       QuaternionCamera::FrameDirection Line_direction =
	       QuaternionCamera::INCREASE_IS_POSITIVE,
	       QuaternionCamera::FrameDirection Sample_direction =
	       QuaternionCamera::INCREASE_IS_NEGATIVE);
  %python_attribute(p_distort, const blitz::Array<double, 1>&);
  %python_attribute_with_set(k1, double);
  %python_attribute_with_set(k2, double);
  %python_attribute_with_set(k3, double);
  %python_attribute_with_set(p1, double);
  %python_attribute_with_set(p2, double);
  %python_attribute_with_set(b1, double);
  %python_attribute_with_set(b2, double);
  %python_attribute_with_set(radial_distort_limit, double);
  %python_attribute_with_set(calibration_date, std::string);
  %python_attribute_with_set(detection_type, std::string);
  static void quaternion_to_sensor_angle
  (const boost::math::quaternion<double>& Frame_to_sc_or_ned,
   double& OUTPUT, double& OUTPUT, double& OUTPUT);
  static boost::math::quaternion<double> sensor_angle_to_quaternion
  (double Sensor_angle_1, double Sensor_angle_2, double Sensor_angle_3);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("SensrbCamera")
