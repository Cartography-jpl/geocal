#include "gdal_datum.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GdalDatum::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Datum);
  GEOCAL_BASE(GdalDatum, Datum);
  ar & GEOCAL_NVP_(map_file);
}

GEOCAL_IMPLEMENT(GdalDatum);
#endif
