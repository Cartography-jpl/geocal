#include "geocal_config.h"
#include "velocity_aberration_exact.h"
#include "geocal_serialize_support.h"
#include "ecr.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VelocityAberrationExact::serialize
(Archive & ar, const unsigned int UNUSED(version))
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VelocityAberration);
}

GEOCAL_IMPLEMENT(VelocityAberrationExact);
#endif

inline double dotb(const double x[], 
		   const double y[])
{
  return x[0] * y[0] + x[1] * y[1] + x[2] * y[2];
}

inline double normb(const double x[])
{
  return std::sqrt(dotb(x, x));
}

inline void cross2
(const double x[],
 const double y[],
 double res[])
{
  res[0] = x[1] * y[2] - x[2] * y[1];
  res[1] = x[2] * y[0] - x[0] * y[2];
  res[2] = x[0] * y[1] - x[1] * y[0];
}

CartesianFixedLookVector VelocityAberrationExact::aberration_calc
(const GroundCoordinate& Spacecraft_pos,
 const GroundCoordinate& Gc_uncorrected,
 const boost::array<double, 3> &Velocity_cf,
 bool Forward_calc) const
{
  if(Spacecraft_pos.naif_code() != Ecr::EARTH_NAIF_CODE)
    throw Exception("VelocityAberrationExact only works with Earth data");
  // Turns out this function can be a bit of a bottle neck. We use
  // raw C arrays here for speed, even though it is a little less
  // clear than blitz arrays.

  // Velocity of earth
  const double omega[3] = {0,0,7.292115e-5};
  
  double d[3], vel_a[3], p_hat[3], v_hat[3],
    k_hat[3], i_hat[3], p_hat_prime[3];
  CartesianFixedLookVector lv(Spacecraft_pos, Gc_uncorrected);
  d[0] = -lv.look_vector[0];
  d[1] = -lv.look_vector[1];
  d[2] = -lv.look_vector[2];
  cross2(omega,d, vel_a);
  vel_a[0] += Velocity_cf[0];
  vel_a[1] += Velocity_cf[1];
  vel_a[2] += Velocity_cf[2];
  auto t = lv.direction();
  p_hat[0] = t[0];
  p_hat[1] = t[1];
  p_hat[2] = t[2];
  double v = normb(vel_a);
  v_hat[0] = vel_a[0] / v;
  v_hat[1] = vel_a[1] / v;
  v_hat[2] = vel_a[2] / v;
  double theta = acos(dotb(p_hat,v_hat));
  double theta_prime = acos((cos(theta)-v/Constant::speed_of_light) /
			    (1-v/Constant::speed_of_light*cos(theta)));
  cross2(p_hat, v_hat, k_hat);
  double stheta = sin(theta);
  k_hat[0] /= stheta;
  k_hat[1] /= stheta;
  k_hat[2] /= stheta;
  cross2(v_hat, k_hat, i_hat);
  double ctheta_prime = cos(theta_prime);
  double stheta_prime = sin(theta_prime);
  p_hat_prime[0] = v_hat[0] * ctheta_prime + i_hat[0] * stheta_prime;
  p_hat_prime[1] = v_hat[1] * ctheta_prime + i_hat[1] * stheta_prime;
  p_hat_prime[2] = v_hat[2] * ctheta_prime + i_hat[2] * stheta_prime;
  if(Forward_calc)
    return CartesianFixedLookVector(p_hat_prime[0], p_hat_prime[1],
				    p_hat_prime[2]);
  else
    throw Exception("Not implemented yet");
}


