#include "coordinate_converter.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void CoordinateConverter::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
}

template<class Archive>
void GeodeticConverter::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(GeodeticConverter, CoordinateConverter);
}

template<class Archive>
void Geodetic360Converter::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(Geodetic360Converter, CoordinateConverter);
}

template<class Archive>
void GeodeticRadianConverter::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(GeodeticRadianConverter, CoordinateConverter);
}

template<class Archive>
void GeodeticRadian2piConverter::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(CoordinateConverter);
  GEOCAL_BASE(GeodeticRadian2piConverter, CoordinateConverter);
}

GEOCAL_IMPLEMENT(CoordinateConverter);
GEOCAL_IMPLEMENT(GeodeticConverter);
GEOCAL_IMPLEMENT(Geodetic360Converter);
GEOCAL_IMPLEMENT(GeodeticRadianConverter);
GEOCAL_IMPLEMENT(GeodeticRadian2piConverter);
#endif




