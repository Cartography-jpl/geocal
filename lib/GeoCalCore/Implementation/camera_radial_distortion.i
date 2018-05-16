// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "camera_radial_distortion.h"
%}

%base_import(quaternion_camera)

%geocal_shared_ptr(GeoCal::CameraRadialDistortion);
namespace GeoCal {
class CameraRadialDistortion : public QuaternionCamera {
public:
  CameraRadialDistortion(boost::math::quaternion<double> Frame_to_sc_q,
			  const blitz::Array<double, 1>& K_distort,
			  double Number_line, double Number_sample,
			  double Line_pitch, double Sample_pitch,
			  double Focal_length, 
			  const FrameCoordinate& Principal_point,
			  QuaternionCamera::FrameConvention Frame_convention =
			  QuaternionCamera::LINE_IS_X,
			  QuaternionCamera::FrameDirection Line_direction =
			  QuaternionCamera::INCREASE_IS_POSITIVE,
			  QuaternionCamera::FrameDirection Sample_direction =
			  QuaternionCamera::INCREASE_IS_POSITIVE);
  CameraRadialDistortion(boost::math::quaternion<double> Frame_to_sc_q, 
			  const blitz::Array<double, 1>& K_distort,
			  double Number_line, double Number_sample,
			  double Line_pitch, double Sample_pitch,
			  double Focal_length, 
			  const FrameCoordinate& Principal_point,
			  QuaternionCamera::FrameConvention Frame_convention,
			  QuaternionCamera::FrameDirection Line_direction,
			  QuaternionCamera::FrameDirection Sample_direction,
			  const blitz::Array<bool, 1>& Parameter_mask);
  %python_attribute(k_distort, const blitz::Array<double, 1>&);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("CameraRadialDistortion")
