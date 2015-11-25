// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "spot_orbit.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::SpotOrbit);
namespace GeoCal {
class SpotOrbit : public Orbit {
public:
  SpotOrbit(const std::vector<Time>& Ephemeris_time, 
	    const blitz::Array<double, 2>& Ephemeris,
	    const std::vector<Time>& Attitude_time,
	    const blitz::Array<double, 2>& Ypr);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
};
}
