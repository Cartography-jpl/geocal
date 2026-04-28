#include "spice_kepler_orbit.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void SpiceKeplerOrbit::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit);
}

GEOCAL_IMPLEMENT(SpiceKeplerOrbit);
#endif

//-----------------------------------------------------------------------
/// Create an kepler orbit
//-----------------------------------------------------------------------

SpiceKeplerOrbit::SpiceKeplerOrbit()
  : Orbit(Time::min_valid_time, Time::max_valid_time)
{
}

// See base class for description
boost::shared_ptr<OrbitData> SpiceKeplerOrbit::orbit_data(Time T) const
{
  throw Exception("Not implemented yet");
}

// Print to stream
void SpiceKeplerOrbit::print(std::ostream& Os) const
{
  Os << "SpiceKeplerOrbit:\n";
}

