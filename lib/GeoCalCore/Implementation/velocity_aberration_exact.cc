#include "geocal_config.h"
#include "velocity_aberration_exact.h"
#include "geocal_serialize_support.h"

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

CartesianFixedLookVector VelocityAberrationExact::aberration_calc
(const GroundCoordinate& Spacecraft_pos,
 const GroundCoordinate& Gc_uncorrected,
 const boost::array<double, 3> &Velocity_cf,
 bool Forward_calc) const
{
  boost::math::quaternion<double> vel_cf(0, Velocity_cf[0], Velocity_cf[1],
					 Velocity_cf[2]);
  CartesianFixedLookVector lv(Spacecraft_pos, Gc_uncorrected);
  auto cf = lv.look_quaternion();
  double k = lv.length() / Constant::speed_of_light;
  if(Forward_calc)
    cf -= k * vel_cf;
  else
    cf += k * vel_cf;
  return CartesianFixedLookVector(cf);
}


