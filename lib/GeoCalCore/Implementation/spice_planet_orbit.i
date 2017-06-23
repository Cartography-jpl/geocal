// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "spice_planet_orbit.h"
%}
%base_import(orbit)
%geocal_shared_ptr(GeoCal::SpicePlanetOrbit);

namespace GeoCal {
class SpicePlanetOrbit : public Orbit {
public:
  SpicePlanetOrbit(const std::string& Target_name, 
		   const std::string& Spacecraft_reference_frame_name,
		   int Naif_id);
  SpicePlanetOrbit(const std::string& Target_name, 
		   const std::string& Spacecraft_reference_frame_name,
		   const std::vector<std::string>& Kernel_list,
		   int Naif_id);
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(const TimeWithDerivative& T) 
    const;
  void set_min_time(const Time& Tmin);
  void set_max_time(const Time& Tmax);
  %python_attribute(naif_id, int);
  %python_attribute(target_name, std::string);
  %python_attribute(spacecraft_reference_frame_name, std::string);
  %python_attribute(kernel_list, std::vector<std::string>);
  %pickle_serialization();
};
}


