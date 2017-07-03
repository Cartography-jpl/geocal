#include "dem_to_raster.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemToRaster::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CalcRaster)
    & GEOCAL_NVP(dem);
}

GEOCAL_IMPLEMENT(DemToRaster);
#endif
