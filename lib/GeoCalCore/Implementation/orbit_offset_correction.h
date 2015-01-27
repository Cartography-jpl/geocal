#ifndef ORBIT_OFFSET_CORRECTION_H
#define ORBIT_OFFSET_CORRECTION_H
#include "orbit.h"
#include <map>

namespace GeoCal {
/****************************************************************//**
  This class gives an orbit that tries to correct errors in another
  underlying orbit. This uses a simple error model which captures a
  common set of orbit errors.

  This adds a fixed offset to the inertial position of the orbit,
  modeling a static error in the ephemeris.

  A time dependent correction is added to the spacecraft to Cartesian
  inertial system. This correction supplies a yaw, pitch, and roll
  correction at fixed time values. For times in between we interpolate
  the quaternion correction.

  The position offset is in meters. Right now, the attitude correction
  is in arcseconds. We may change that.

  The velocity is left unchanged.

  The underlying orbit should return a QuaternionOrbitData orbit data, 
  since this is currently the only type supported.
*******************************************************************/
class OrbitOffsetCorrection: public Orbit {
public:
  OrbitOffsetCorrection(const boost::shared_ptr<Orbit> Orb_uncorr,
			bool Outside_is_error = false,
			bool Fit_position = true,
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
  bool fit_position() const { return fit_position_; }
  void fit_position(bool V) { fit_position_ = V; }

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

//-----------------------------------------------------------------------
/// The uncorrected orbit.
//-----------------------------------------------------------------------
  boost::shared_ptr<Orbit> orbit_uncorrected() const 
  { return orb_uncorr; }
  std::vector<Time> time_point() const;

//-----------------------------------------------------------------------
/// Add a time pointer where we are going to do an attitude correction.
//-----------------------------------------------------------------------
  void insert_time_point(Time T_pt)
  {
    att_corr[T_pt] = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  }
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual ArrayAd<double, 1> parameter_with_derivative() const;
  virtual void parameter_with_derivative(const ArrayAd<double, 1>& Parm);
  virtual std::vector<std::string> parameter_name() const;
  virtual blitz::Array<bool, 1> parameter_mask() const;
  virtual void print(std::ostream& Os) const;
private:
  boost::shared_ptr<Orbit> orb_uncorr;
  typedef std::map<Time, boost::math::quaternion<AutoDerivative<double> > > map_type;
  typedef std::pair<Time, boost::math::quaternion<AutoDerivative<double> > > map_pair_type;
  map_type att_corr;
  boost::array<AutoDerivative<double>, 3> pos_corr;
  bool outside_is_error_;
  bool fit_position_, fit_yaw_, fit_pitch_, fit_roll_;
  OrbitOffsetCorrection() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitOffsetCorrection);
#endif

