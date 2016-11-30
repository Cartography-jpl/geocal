// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "ground_mspi_orbit.h"
%}

%base_import(look_vector)
%base_import(orbit)

%geocal_shared_ptr(GeoCal::GroundMspiOrbit);
namespace GeoCal {
class GroundMspiOrbit : public Orbit {
public:
  GroundMspiOrbit(const Time& Start_time,
		  boost::shared_ptr<GroundCoordinate>& Pos,
		  double Azimuth,
		  double Start_elevation_angle,
		  double Rotation_rate);
  %python_attribute(start_time, Time);
  %python_attribute(position, boost::shared_ptr<GroundCoordinate>);
  %python_attribute(azimuth, double);
  %python_attribute(start_elevation_angle, double);
  %python_attribute(rotation_rate, double);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization_dir();
};
}
