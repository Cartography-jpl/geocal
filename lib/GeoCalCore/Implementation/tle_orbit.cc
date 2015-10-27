#include "tle_orbit.h"
#include "ostream_pad.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void TleOrbit::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(tle);
}
GEOCAL_IMPLEMENT(TleOrbit);
#endif

// See base class for description
boost::shared_ptr<OrbitData> TleOrbit::orbit_data(Time T) const
{
}

void TleOrbit::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "TleOrbit:\n"
     << "  TLE:\n";
  opad << tle() << "\n";
  opad.strict_sync();
}
