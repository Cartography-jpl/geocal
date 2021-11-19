#include "geocal_config.h"
#include "refraction.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Refraction::serialize
(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Refraction);
  // Dummy placeholder, just so we can have derived classes call
  // serialization of this. We use to have derived classes "know"
  // that the base class doesn't have anything. But seems better to
  // *always* have base classes do something, so we can add stuff in
  // the future w/o breaking a bunch of code.
  std::string p = "empty";
  ar & GEOCAL_NVP2("placeholder", p);
}

GEOCAL_IMPLEMENT(Refraction);
#endif
