// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_orbit.h"
%}

%base_import(orbit)
%import "aircraft_orbit_data.i"
%import "mspi_gimbal.i"
%import "geocal_datum.i"

%geocal_shared_ptr(GeoCal::AirMspiOrbit);
namespace GeoCal {
class AirMspiOrbit : public Orbit {
public:
  AirMspiOrbit(const std::string& Fname,
	       const boost::shared_ptr<MspiGimbal>& Gim);
  AirMspiOrbit(const std::string& Fname, 
	       const boost::shared_ptr<MspiGimbal>& Gim,
               const boost::shared_ptr<Datum>& D,
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);
  %python_attribute(data_version, std::string);
  %python_attribute(file_name, std::string);
  %python_attribute(datum, boost::shared_ptr<Datum>);
  %python_attribute(flight_description, std::string);
  %python_attribute(time_spacing, double);
  %python_attribute(vertical_def, AircraftOrbitData::VerticalDefinition);
  double gimbal_position(Time T) const;
  boost::shared_ptr<QuaternionOrbitData> orbit_data_index(int Index) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization();
};
}
