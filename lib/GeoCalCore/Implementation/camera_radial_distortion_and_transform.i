// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_time.h"
#include "camera_radial_distortion_and_transform.h"
%}

%base_import(camera_radial_distortion)

%geocal_shared_ptr(GeoCal::CameraRadialDistortionAndTransform);
namespace GeoCal {
class CameraRadialDistortionAndTransform : public CameraRadialDistortion {
public:
  CameraRadialDistortionAndTransform
  (boost::math::quaternion<double> Frame_to_sc_q,
   const blitz::Array<double, 1>& K_distort,
   double Number_line, double Number_sample,
   double Line_pitch, double Sample_pitch,
   double Focal_length,
   int Bin_mode,
   const blitz::Array<double, 1>& Ccd_off,
   const blitz::Array<double, 1>& Ccd_cen,
   const blitz::Array<double, 1>& T_off,
   const blitz::Array<double, 2>& T_m,
   const blitz::Array<double, 1>& Tinv_off,
   const blitz::Array<double, 2>& Tinv_m);
  %python_attribute_with_set(bin_mode, int);
  %python_attribute_with_set(ccd_off, const blitz::Array<double, 1>&);
  %python_attribute_with_set(ccd_cen, const blitz::Array<double, 1>&);
  %python_attribute_with_set(t_off, const blitz::Array<double, 1>&);
  %python_attribute_with_set(tinv_off, const blitz::Array<double, 1>&);
  %python_attribute_with_set(t_m, const blitz::Array<double, 2>&);
  %python_attribute_with_set(tinv_m, const blitz::Array<double, 2>&);
  %pickle_serialization();
};
}
// List of things "import *" will include
%python_export("CameraRadialDistortionAndTransform")
