#ifndef ORBIT_CORRECTION_H
#define ORBIT_CORRECTION_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This is a class that handles correcting an orbit. This handles the
  common behavior for having an independent position and attitude
  correction, derived classes need to supply the actual corrections 
  to use.
*******************************************************************/
class OrbitCorrection: public Orbit {
public:
  virtual ~OrbitCorrection() {}

//-----------------------------------------------------------------------
/// The uncorrected orbit.
//-----------------------------------------------------------------------
  boost::shared_ptr<Orbit> orbit_uncorrected() const 
  { return orb_uncorr; }

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual boost::shared_ptr<CartesianInertial> position_ci(Time T) const;
  virtual boost::array<AutoDerivative<double>, 3> 
  position_ci_with_derivative(const TimeWithDerivative& T) const;
  virtual boost::shared_ptr<CartesianFixed> position_cf(Time T) const;
  virtual boost::array<AutoDerivative<double>, 3> 
  position_cf_with_derivative(const TimeWithDerivative& T) const;
protected:
  OrbitCorrection(const boost::shared_ptr<Orbit> Orb_uncorr)
    : Orbit(Orb_uncorr->min_time(), Orb_uncorr->max_time()),
      orb_uncorr(Orb_uncorr),
      pos_corr_is_cf_cache_valid_(false){}
  OrbitCorrection() : pos_corr_is_cf_cache_valid_(false) {};
  boost::shared_ptr<Orbit> orb_uncorr;
  virtual boost::array<AutoDerivative<double>, 3 > 
  pcorr_with_derivative(const TimeWithDerivative& Tm, 
			const CartesianFixed& Pos_uncorr) const = 0;
  virtual boost::array<double, 3 > 
  pcorr(const Time& Tm,
	const CartesianFixed& Pos_uncorr) const = 0;
  virtual boost::math::quaternion<AutoDerivative<double> > 
  acorr_with_derivative(const TimeWithDerivative& T) const = 0;
  virtual boost::math::quaternion<double> 
  acorr(const Time& T) const = 0;
  bool pos_corr_is_cf() const;
private:
  mutable bool pos_corr_is_cf_cache_;
  mutable bool pos_corr_is_cf_cache_valid_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitCorrection);
#endif
