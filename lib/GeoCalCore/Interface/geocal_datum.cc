#include "geocal_datum.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Datum::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(Datum);
}

template<class Archive>
void SimpleDatum::serialize(Archive & ar, const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(SimpleDatum, Datum);
  ar & GEOCAL_NVP_(u);
}

GEOCAL_IMPLEMENT(Datum);
BOOST_CLASS_EXPORT_IMPLEMENT(SimpleDatum);
#endif
