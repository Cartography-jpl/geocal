#include "scale_image.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ScaleImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(scale);
}

template<class Archive>
void ScaleImageMultiBand::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRasterMultiBand)
    & GEOCAL_NVP_(raw_data)
    & GEOCAL_NVP_(scale);
}

GEOCAL_IMPLEMENT(ScaleImage);
GEOCAL_IMPLEMENT(ScaleImageMultiBand);
#endif

