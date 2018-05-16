// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "aircraft_orbit_data.h"
%}
%base_import(orbit)
%import "geodetic.i"
%geocal_shared_ptr(GeoCal::AircraftOrbitData);
namespace GeoCal {
class AircraftOrbitData : public QuaternionOrbitData {
public:
  enum VerticalDefinition { GEODETIC_VERTICAL, GEOCENTRIC_VERTICAL };
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const boost::array<double, 3>& Vel_fixed,
		    double Roll, double Pitch,
		    double Heading,
		    VerticalDefinition V = GEODETIC_VERTICAL);
  AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const Time& Tm2,
		    const GroundCoordinate& Position2, 
		    double Roll, double Pitch,
		    double Heading,
		    VerticalDefinition V = GEODETIC_VERTICAL);
  AircraftOrbitData(const QuaternionOrbitData& Od,
		    VerticalDefinition V = GEODETIC_VERTICAL);
  %python_attribute(position_geodetic, Geodetic)
  %python_attribute(roll, double)
  %python_attribute(pitch, double)
  %python_attribute(heading, double)
  %python_attribute(vertical_definition, VerticalDefinition)
  %pickle_serialization();
};
}

// List of things "import *" will include
%python_export("AircraftOrbitData")

