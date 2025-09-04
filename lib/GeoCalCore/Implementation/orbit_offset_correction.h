#ifndef ORBIT_OFFSET_CORRECTION_H
#define ORBIT_OFFSET_CORRECTION_H
#include "orbit_correction.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This class gives an orbit that tries to correct errors in another
  underlying orbit. This uses a simple error model which captures a
  common set of orbit errors.

  A time dependent correction is added to the position of the
  orbit. This correction supplies a correction at fixed time
  values. For times in between, we interpolate the position
  correction. 
  
  A time dependent correction is added to the spacecraft to Cartesian
  inertial system. This correction supplies a yaw, pitch, and roll
  correction at fixed time values. For times in between we interpolate
  the quaternion correction. Note that in general the time points used
  in the attitude correction do not match the time points used in the
  position correction.

  The position offset is in meters. Right now, the attitude correction
  is in arcseconds. We may change that.

  The velocity is left unchanged.

  The underlying orbit should return a QuaternionOrbitData orbit data, 
  since this is currently the only type supported.
*******************************************************************/
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
  virtual ~OrbitOffsetCorrection() {}

//-----------------------------------------------------------------------
/// If true, then calling for times outside of the time points we have
/// is treated as an error, otherwise we just extrapolate past the
/// ends. 
//-----------------------------------------------------------------------

  bool outside_is_error() const { return outside_is_error_; }
  void outside_is_error(bool Outside_is_error) 
  { outside_is_error_ = Outside_is_error; }

//-----------------------------------------------------------------------
/// If true, fit for the position correction.
//-----------------------------------------------------------------------
  bool fit_position_x() const { return fit_position_x_; }
  void fit_position_x(bool V) { fit_position_x_ = V; }
  bool fit_position_y() const { return fit_position_y_; }
  void fit_position_y(bool V) { fit_position_y_ = V; }
  bool fit_position_z() const { return fit_position_z_; }
  void fit_position_z(bool V) { fit_position_z_ = V; }

//-----------------------------------------------------------------------
/// Synonym of fit_position_x, more descriptive name if we are using
/// local coordinates for offset.
//-----------------------------------------------------------------------

  bool fit_position_e() const { return fit_position_x_; }
  void fit_position_e(bool V) { fit_position_x_ = V; }
  bool fit_position_n() const { return fit_position_y_; }
  void fit_position_n(bool V) { fit_position_y_ = V; }
  bool fit_position_u() const { return fit_position_z_; }
  void fit_position_u(bool V) { fit_position_z_ = V; }

//-----------------------------------------------------------------------
/// If true, use local north coordinate (ENU) for position offset,
/// rather that ECR/ECI. This is often more appropriate for airplane data.
//-----------------------------------------------------------------------

  bool use_local_north_coordinate() const {return use_local_north_coordinate_;}
  void use_local_north_coordinate(bool V) 
  { use_local_north_coordinate_ = V; }

//-----------------------------------------------------------------------
/// If true, fit for the yaw correction.
//-----------------------------------------------------------------------
  bool fit_yaw() const { return fit_yaw_; }
  void fit_yaw(bool V) { fit_yaw_ = V; }

//-----------------------------------------------------------------------
/// If true, fit for the pitch correction.
//-----------------------------------------------------------------------
  bool fit_pitch() const { return fit_pitch_; }
  void fit_pitch(bool V) { fit_pitch_ = V; }

//-----------------------------------------------------------------------
/// If true, fit for the roll correction.
//-----------------------------------------------------------------------
  bool fit_roll() const { return fit_roll_; }
  void fit_roll(bool V) { fit_roll_ = V; }

  std::vector<Time> attitude_time_point() const;
  std::vector<Time> position_time_point() const;

  void orbit_correction_parameter(std::vector<boost::shared_ptr<Time> >&
				  Attitude_time_point,
				  blitz::Array<double, 2>& Attitude_corr,
				  std::vector<boost::shared_ptr<Time> >&
				  Position_time_point,
				  blitz::Array<double, 2>& Position_corr);
  
//-----------------------------------------------------------------------
/// Directly update the quaternion at time_point i. This is
/// occasionally more convenient that updating the parameters.
//-----------------------------------------------------------------------
  void update_quaterion(int Ind, const boost::math::quaternion<double>& Q)
  {
    range_check(Ind, 0, (int) attitude_time_point().size());
    att_corr[attitude_time_point()[Ind]] = Q;
    notify_update();
  }

//-----------------------------------------------------------------------
/// Add a time point where we are going to do an attitude correction.
//-----------------------------------------------------------------------
  void insert_attitude_time_point(Time T_pt)
  {
    att_corr[T_pt] = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
    notify_update();
  }

//-----------------------------------------------------------------------
/// Add a time point where we are going to do a position correction.
//-----------------------------------------------------------------------
  void insert_position_time_point(Time T_pt)
  {
    boost::array<AutoDerivative<double>, 3> pc;
    pc[0] = 0;
    pc[1] = 0;
    pc[2] = 0;
    pos_corr[T_pt] = pc;
    notify_update();
  }
  blitz::Array<double, 1> att_parm_to_match(const Time& Tm) const;
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const;
  virtual void print(std::ostream& Os) const;
protected:
  virtual void notify_update()
  {
    notify_update_do(*this);
  }
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
private:
  typedef std::map<Time, boost::math::quaternion<AutoDerivative<double> > > att_map_type;
  typedef std::pair<Time, boost::math::quaternion<AutoDerivative<double> > > att_map_pair_type;
  typedef std::map<Time, boost::array<AutoDerivative<double>, 3 > > pos_map_type;
  typedef std::pair<Time, boost::array<AutoDerivative<double>, 3 > > pos_map_pair_type;
  att_map_type att_corr;
  pos_map_type pos_corr;
  bool outside_is_error_;
  bool fit_position_x_, fit_position_y_, fit_position_z_, fit_yaw_, 
    fit_pitch_, fit_roll_;
  bool use_local_north_coordinate_;
  OrbitOffsetCorrection() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitOffsetCorrection);
#endif
