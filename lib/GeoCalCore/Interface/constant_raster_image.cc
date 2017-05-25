#include "constant_raster_image.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void ConstantRasterImage::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(RasterImageVariable);
  ar & GEOCAL_NVP_(data);
}

GEOCAL_IMPLEMENT(ConstantRasterImage);
#endif
