// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ypr_orbit_data.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::YprOrbitData);

namespace GeoCal {
class YprOrbitData : public QuaternionOrbitData {
public:
  YprOrbitData(const QuaternionOrbitData& Od,
               const std::string& Euler_order = "ZYX");

  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianFixed>& pos_cf,
               const boost::array<double, 3>& vel_fixed,
               double Yaw, double Pitch, double Roll,
               const std::string& Euler_order = "ZYX");

  YprOrbitData(const Time& Tm,
               const boost::shared_ptr<CartesianInertial>& pos_ci,
               const boost::array<double, 3>& vel_inertial,
               double Yaw, double Pitch, double Roll,
               const std::string& Euler_order = "ZYX");

  %python_attribute(euler_angle1, double)
  %python_attribute(euler_angle2, double)
  %python_attribute(euler_angle3, double)
  %python_attribute(yaw, double)
  %python_attribute(pitch, double)
  %python_attribute(roll, double)
  %python_attribute(euler_order, std::string)
  %python_attribute(orbital_to_eci, boost::math::quaternion<double>)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("YprOrbitData")
