#ifndef ORBIT_PIECEWISE_CORRECTION_H
#define ORBIT_PIECEWISE_CORRECTION_H
#include "orbit_correction.h"
#include "piecewise_linear.h"

namespace GeoCal {
/****************************************************************//**
  This class gives an orbit that tries to correct errors in another
  underlying orbit. 

  The correction is a piecewise correction to the position, in local
  ENU coordinate system.

  The underlying orbit should return a QuaternionOrbitData orbit data, 
  since this is currently the only type supported.
*******************************************************************/
class OrbitPiecewiseCorrection: public OrbitCorrection {
public:
  OrbitPiecewiseCorrection(const boost::shared_ptr<Orbit> Orb_uncorr,
			   const PiecewiseLinear& E_corr,
			   const PiecewiseLinear& N_corr,
			   const PiecewiseLinear& U_corr)
    : OrbitCorrection(Orb_uncorr),
      e_corr(E_corr), n_corr(N_corr), u_corr(U_corr)
  { }
  virtual ~OrbitPiecewiseCorrection() {}
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
  acorr_with_derivative(const TimeWithDerivative& T) const
  { return boost::math::quaternion<AutoDerivative<double> >(1,0,0,0); }
  virtual boost::math::quaternion<double> 
  acorr(const Time& T) const
  { return boost::math::quaternion<double>(1,0,0,0); }
private:
  PiecewiseLinear e_corr, n_corr, u_corr;
  OrbitPiecewiseCorrection() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitPiecewiseCorrection);
#endif
