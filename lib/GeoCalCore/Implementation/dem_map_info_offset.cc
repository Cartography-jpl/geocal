#include "dem_map_info_offset.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemMapInfoOffset::save(Archive & ar, const unsigned int version) const
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(dem)
    & GEOCAL_NVP_(height_offset);
}

template<class Archive>
void DemMapInfoOffset::load(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(dem)
    & GEOCAL_NVP_(height_offset);
  map_info_ = dem_->map_info();
}

GEOCAL_IMPLEMENT(DemMapInfoOffset);
#endif

  
