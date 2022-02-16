#include "geocal_config.h"
#include "velocity_aberration.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VelocityAberration::serialize
(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(VelocityAberration);
  // Dummy placeholder, just so we can have derived classes call
  // serialization of this. We use to have derived classes "know"
  // that the base class doesn't have anything. But seems better to
  // *always* have base classes do something, so we can add stuff in
  // the future w/o breaking a bunch of code.
  std::string p = "empty";
  ar & GEOCAL_NVP2("placeholder", p);
}

template<class Archive>
void NoVelocityAberration::serialize
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VelocityAberration);
}

template<class Archive>
void VelocityAberrationFirstOrder::serialize
(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(VelocityAberration);
}

GEOCAL_IMPLEMENT(VelocityAberration);
GEOCAL_IMPLEMENT(NoVelocityAberration);
GEOCAL_IMPLEMENT(VelocityAberrationFirstOrder);
#endif
