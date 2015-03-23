#include "raster_image_variable.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RasterImageVariable::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(RasterImage);
  GEOCAL_BASE(RasterImageVariable, RasterImage);
  ar & GEOCAL_NVP_(number_line)
    & GEOCAL_NVP_(number_sample)
    & GEOCAL_NVP_(number_tile_line)
    & GEOCAL_NVP_(number_tile_sample)
    & GEOCAL_NVP_(map_info)
    & GEOCAL_NVP_(rpc);
}

GEOCAL_IMPLEMENT(RasterImageVariable);
#endif
