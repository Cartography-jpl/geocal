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
  GEOCAL_GENERIC_BASE(VelocityAberrationExact);
  // Dummy placeholder, just so we can have derived classes call
  // serialization of this. We use to have derived classes "know"
  // that the base class doesn't have anything. But seems better to
  // *always* have base classes do something, so we can add stuff in
  // the future w/o breaking a bunch of code.
  std::string p = "empty";
  ar & GEOCAL_NVP2("placeholder", p);
}

GEOCAL_IMPLEMENT(VelocityAberrationExact);
#endif

CartesianFixedLookVector VelocityAberrationExact::aberration_calc
(const QuaternionOrbitData& Od,
 const GroundCoordinate& Gc_before_correction,
 bool Forward_calc) const
{
}

void VelocityAberrationExact::print(std::ostream& Os) const
{
  Os << "VelocityAberrationExact";
}

