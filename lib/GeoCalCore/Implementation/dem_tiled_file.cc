#include "dem_tiled_file.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DemTiledFile::serialize(Archive & ar, const unsigned int version)
{
  // Not sure if this serialization will ever be used, the classes
  // that use DemTiledFile have there own split serialization (e.g.,
  // GdalDem) and calls DemTiledFile initialize directly. But make
  // this available in case it ever gets used. 
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(DemMapInfo)
    & GEOCAL_NVP_(data)
    & GEOCAL_NVP_(scale);
}

GEOCAL_IMPLEMENT(DemTiledFile);
#endif
