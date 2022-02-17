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

template<class T> inline T dotb(const blitz::Array<T, 1>& x, 
				const blitz::Array<T, 1>& y)
{
  return sum(x * y);
}

template<class T> inline T normb(const blitz::Array<T, 1>& x)
{
  return std::sqrt(dotb(x, x));
}

template<class T> inline blitz::Array<T, 1> cross2
(const blitz::Array<T, 1>& x,
 const blitz::Array<T, 1>& y)
{
  blitz::Array<T, 1> res(3);
  res(0) = x(1) * y(2) - x(2) * y(1);
  res(1) = x(2) * y(0) - x(0) * y(2);
  res(2) = x(0) * y(1) - x(1) * y(0);
  return res;
}

CartesianFixedLookVector VelocityAberrationExact::aberration_calc
(const GroundCoordinate& Spacecraft_pos,
 const GroundCoordinate& Gc_uncorrected,
 const boost::array<double, 3> &Velocity_cf,
 bool Forward_calc) const
{
  if(Spacecraft_pos.naif_code() != Ecr::EARTH_NAIF_CODE)
    throw Exception("VelocityAberrationExact only works with Earth data");
  // Velocity of earth
  const double omega_v = 7.292115e-5;
  blitz::Array<double, 1> omega(3), d(3), vel_a(3), p_hat(3), v_hat(3),
    k_hat(3), i_hat(3), p_hat_prime(3);
  omega = 0,0,omega_v;
  CartesianFixedLookVector lv(Spacecraft_pos, Gc_uncorrected);
  d = -lv.look_vector[0],-lv.look_vector[1],-lv.look_vector[2];
  vel_a = Velocity_cf[0], Velocity_cf[1], Velocity_cf[2];
  vel_a += cross2(omega,d);
  auto t = lv.direction();
  p_hat = t[0],t[1],t[2];
  double v = normb(vel_a);
  v_hat = vel_a / v;
  double theta = acos(dotb(p_hat,v_hat));
  double theta_prime = acos((cos(theta)-v/Constant::speed_of_light) /
			    (1-v/Constant::speed_of_light*cos(theta)));
  k_hat = cross2(p_hat, v_hat) / sin(theta);
  i_hat = cross2(v_hat, k_hat);
  p_hat_prime = v_hat * cos(theta_prime) + i_hat * sin(theta_prime);
  if(Forward_calc)
    return CartesianFixedLookVector(p_hat_prime(0), p_hat_prime(1),
				    p_hat_prime(2));
  else
    throw Exception("Not implemented yet");
}


