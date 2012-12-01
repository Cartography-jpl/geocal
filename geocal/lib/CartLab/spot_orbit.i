// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "spot_orbit.h"
%}

%geocal_shared_ptr(SpotOrbit);
namespace GeoCal {
class SpotOrbit : public Orbit {
public:
  SpotOrbit(const std::vector<Time>& Ephemeris_time, 
	    const blitz::Array<double, 2>& Ephemeris,
	    const std::vector<Time>& Attitude_time,
	    const blitz::Array<double, 2>& Ypr);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
};
}
