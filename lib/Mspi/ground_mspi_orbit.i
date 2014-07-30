// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "ground_mspi_orbit.h"
%}

%base_import(orbit)
%import "look_vector.i"

%geocal_shared_ptr(GeoCal::GroundMspiOrbitData);
%geocal_shared_ptr(GeoCal::GroundMspiOrbit);
namespace GeoCal {

class GroundMspiOrbitData : public QuaternionOrbitData {
public:
  GroundMspiOrbitData(const Time& Tm, const GroundCoordinate& Pos,
		      double Azimuth, double Zenith);
  LnLookVector ln_look_vector(const ScLookVector& Sl) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianInertialLookVector& Ci) const;
  virtual ScLookVector 
  sc_look_vector(const CartesianFixedLookVector& Cf) const;
  virtual ScLookVector sc_look_vector(const LnLookVector& Ln) const;
};

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
  %pickle_init(1, self.start_time, self.position, self.azimuth,
	       self.start_elevation_angle, self.rotation_rate);
};
}
