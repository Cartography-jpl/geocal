#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CoordinateConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
}

template<class Archive>
void GeodeticConverter::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(GeodeticConverter, CoordinateConverter);
}

GEOCAL_IMPLEMENT(CoordinateConverter);
GEOCAL_IMPLEMENT(GeodeticConverter);
#endif




