#include "orbit_time_shift.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include <boost/make_shared.hpp>

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitTimeShift::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(orbit_original)
    & GEOCAL_NVP_(time_shift);
}

GEOCAL_IMPLEMENT(OrbitTimeShift);
#endif

//-----------------------------------------------------------------------
/// Create an orbit that uses the data for the Orbit_original, shifted
/// by the given Time_shift.
//-----------------------------------------------------------------------

OrbitTimeShift::OrbitTimeShift(const boost::shared_ptr<Orbit> Orbit_original,
			       double Time_shift)
  : Orbit(std::max(Orbit_original->min_time()+Time_shift, Time::min_valid_time),
	  std::min(Orbit_original->max_time()+Time_shift, Time::max_valid_time)),
    orbit_original_(Orbit_original),
    time_shift_(Time_shift)
{
}

// See base class for description
boost::shared_ptr<OrbitData> OrbitTimeShift::orbit_data(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData> od = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orbit_original_->orbit_data(T - time_shift_));
  if(!od)
    throw Exception("OrbitTimeShift only works with orbits that return a QuaternionOrbitData");
  return boost::make_shared<QuaternionOrbitData>(T, od->position_cf(),
						 od->velocity_cf(),
						 od->sc_to_cf());
}

// Print to stream
void OrbitTimeShift::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitTimeShift:\n"
     << "  Time shift: " << time_shift() << " seconds\n"
     << "  Underlying orbit:\n";
  opad << *orbit_original() << "\n";
  opad.strict_sync();
}

