// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "orbit_offset_correction.h"
%}
%base_import(orbit)
%base_import(with_parameter)
%geocal_shared_ptr(GeoCal::OrbitOffsetCorrection);
namespace GeoCal {
class OrbitOffsetCorrection: public Orbit {
public:
  OrbitOffsetCorrection(const boost::shared_ptr<Orbit> Orb_uncorr,
			bool Outside_is_error = false,
			bool Fit_position = true,
			bool Fit_yaw = true,
			bool Fit_pitch = true,
			bool Fit_roll = true);
  %python_attribute_with_set(outside_is_error, bool);
  %python_attribute_with_set(fit_position, bool);
  %python_attribute_with_set(fit_yaw, bool);
  %python_attribute_with_set(fit_pitch, bool);
  %python_attribute_with_set(fit_roll, bool);
  %python_attribute(orbit_uncorrected, boost::shared_ptr<Orbit>);
  void update_quaterion(int Ind, const boost::math::quaternion<double>& Q);
  void insert_time_point(Time T_pt);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization();
};
}

