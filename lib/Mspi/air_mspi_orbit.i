// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_orbit.h"
%}

%base_import(orbit)
%import "aircraft_orbit_data.i"
%import "mspi_camera.i"
%import "geocal_datum.i"

%geocal_shared_ptr(GeoCal::AirMspiOrbit);
namespace GeoCal {
class AirMspiOrbit : public Orbit {
public:
  AirMspiOrbit(const std::string& Fname, 
	       const blitz::Array<double, 1>& Gimbal_angle,
	       const blitz::Array<double, 1>& Ypc_corr);
  AirMspiOrbit(const std::string& Fname, 
	       const blitz::Array<double, 1>& Gimbal_angle,
	       const blitz::Array<double, 1>& Ypc_corr,
               const boost::shared_ptr<Datum>& D,
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);
  AirMspiOrbit(const std::string& Fname, 
	       const blitz::Array<double, 1>& Gimbal_angle);
  AirMspiOrbit(const std::string& Fname, 
	       const blitz::Array<double, 1>& Gimbal_angle,
               const boost::shared_ptr<Datum>& D,
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);
  AirMspiOrbit(const std::string& Fname);
  AirMspiOrbit(const std::string& Fname,
	       const boost::shared_ptr<MspiCamera>& Cam);
  AirMspiOrbit(const std::string& Fname, 
               const boost::shared_ptr<Datum>& D,
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);
  AirMspiOrbit(const std::string& Fname, 
	       const boost::shared_ptr<MspiCamera>& Cam,
               const boost::shared_ptr<Datum>& D,
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);
  %python_attribute(data_version, std::string);
  %python_attribute(file_name, std::string);
  %python_attribute(datum, boost::shared_ptr<Datum>);
  %python_attribute(flight_description, std::string);
  %python_attribute(gimbal_angle, blitz::Array<double, 1>);
  %python_attribute(ypr_corr, blitz::Array<double, 1>);
  %python_attribute(time_spacing, double);
  %python_attribute(vertical_def, AircraftOrbitData::VerticalDefinition);
  boost::shared_ptr<QuaternionOrbitData> orbit_data_index(int Index) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  %pickle_serialization();
};
}
