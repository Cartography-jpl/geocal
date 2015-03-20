#include "igc_collection.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void IgcCollection::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(IgcCollection);
  GEOCAL_GENERIC_BASE(WithParameter);
  GEOCAL_BASE(IgcCollection, WithParameter);
}

GEOCAL_IMPLEMENT(IgcCollection);
#endif
