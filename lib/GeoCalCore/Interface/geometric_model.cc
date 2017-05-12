#include "geometric_model.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void GeometricModel::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GeometricModel);
}

template<class Archive>
void GeometricTiePoints::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(GeometricTiePoints);
  ar & GEOCAL_NVP(itie)
    & GEOCAL_NVP(otie);
}

GEOCAL_IMPLEMENT(GeometricModel);
#endif
