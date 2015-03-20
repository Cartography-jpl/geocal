#include "rolling_shutter_constant_time_table.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void RollingShutterConstantTimeTable::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_BASE(RollingShutterConstantTimeTable, TimeTable);
  GEOCAL_GENERIC_BASE(TimeTable);
  ar & GEOCAL_NVP(min_t) & GEOCAL_NVP(max_l) & GEOCAL_NVP(tspace);
}

GEOCAL_IMPLEMENT(RollingShutterConstantTimeTable);
#endif

