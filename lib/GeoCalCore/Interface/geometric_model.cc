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
GEOCAL_IMPLEMENT(GeometricTiePoints);
#endif

//-----------------------------------------------------------------------
/// Add a point
//-----------------------------------------------------------------------

void GeometricTiePoints::add_point
(const ImageCoordinate& Resampled_ic,
 const ImageCoordinate& Original_ic)
{
  if(replace_point_ >= (int) itie.size())
    replace_point_ = -1;
  if(replace_point_ < 0) {
    itie.push_back(Original_ic);
    otie.push_back(Resampled_ic);
    return;
  }
  itie[replace_point_] = Original_ic;
  otie[replace_point_] = Resampled_ic;
  ++replace_point_;
}
