// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_offset_correction.h"
%}
%base_import(orbit_correction)
%base_import(with_parameter)
%geocal_shared_ptr(GeoCal::OrbitOffsetCorrection);
namespace GeoCal {
class OrbitOffsetCorrection: public OrbitCorrection {
public:
  OrbitOffsetCorrection(const boost::shared_ptr<Orbit> Orb_uncorr,
			bool Outside_is_error = false,
			bool Use_local_north_coordinate = false,
			bool Fit_position_x = true,
			bool Fit_position_y = true,
			bool Fit_position_z = true,
			bool Fit_yaw = true,
			bool Fit_pitch = true,
			bool Fit_roll = true);
  %python_attribute_with_set(outside_is_error, bool);
  %python_attribute_with_set(fit_position_x, bool);
  %python_attribute_with_set(fit_position_y, bool);
  %python_attribute_with_set(fit_position_z, bool);
  %python_attribute_with_set(fit_position_e, bool);
  %python_attribute_with_set(fit_position_n, bool);
  %python_attribute_with_set(fit_position_u, bool);
  %python_attribute_with_set(use_local_north_coordinate, bool);
  %python_attribute_with_set(fit_yaw, bool);
  %python_attribute_with_set(fit_pitch, bool);
  %python_attribute_with_set(fit_roll, bool);
  void update_quaterion(int Ind, const boost::math::quaternion<double>& Q);
  void insert_attitude_time_point(Time T_pt);
  void insert_position_time_point(Time T_pt);
  %pickle_serialization();
};
}

