#include "dem_map_info_offset.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemMapInfoOffset::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(dem)
    & GEOCAL_NVP_(height_offset);
}

GEOCAL_IMPLEMENT(DemMapInfoOffset);
#endif

  
