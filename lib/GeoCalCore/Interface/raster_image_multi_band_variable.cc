#include "raster_image_multi_band_variable.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterImageMultiBandVariable::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImageMultiBand);
  GEOCAL_BASE(RasterImageMultiBandVariable, RasterImageMultiBand);
  ar & GEOCAL_NVP_(raster_image);
}

GEOCAL_IMPLEMENT(RasterImageMultiBandVariable);
#endif
