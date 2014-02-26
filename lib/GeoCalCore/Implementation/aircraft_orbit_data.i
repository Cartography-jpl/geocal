// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "aircraft_orbit_data.h"
%}
%base_import(orbit)
%import "geodetic.i"
%geocal_shared_ptr(GeoCal::AircraftOrbitData);
namespace GeoCal {
class AircraftOrbitData : public QuaternionOrbitData {
public:
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const boost::array<double, 3>& Vel_fixed,
		    double Roll, double Pitch,
		    double Heading);
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const Time& Tm2,
		    const GroundCoordinate& Position2, 
		    double Roll, double Pitch,
		    double Heading);
  %python_attribute(position_geodetic, Geodetic)
  %python_attribute(roll, double)
  %python_attribute(pitch, double)
  %python_attribute(heading, double)
  %pickle_init(1, self.time, self.position_geodetic, self.velocity_cf,
	       self.roll, self.pitch, self.heading)
};
}

