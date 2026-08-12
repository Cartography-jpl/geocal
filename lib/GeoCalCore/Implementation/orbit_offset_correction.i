// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

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
  blitz::Array<double, 1> att_parm_to_match(const Time& Tm) const;
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
  void orbit_correction_parameter
  (std::vector<boost::shared_ptr<Time> >& OUTPUT,
   blitz::Array<double, 2>& OUTPUT,
   std::vector<boost::shared_ptr<Time> >& OUTPUT,
   blitz::Array<double, 2>& OUTPUT);
  %pickle_serialization();
  virtual boost::array<AutoDerivative<double>, 3 > 
  pcorr_with_derivative(const TimeWithDerivative& Tm, 
			const CartesianFixed& Pos_uncorr) const;
  virtual boost::array<double, 3 > 
  pcorr(const Time& Tm,
	const CartesianFixed& Pos_uncorr) const;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  acorr_with_derivative(const TimeWithDerivative& T) const;
  virtual boost::math::quaternion<double> 
  acorr(const Time& T) const;
  
};
}
// List of things "import *" will include
%python_export("OrbitOffsetCorrection")

