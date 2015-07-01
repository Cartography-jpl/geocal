#include "ray_caster.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RayCaster::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RayCaster);
}

GEOCAL_IMPLEMENT(RayCaster);
#endif

