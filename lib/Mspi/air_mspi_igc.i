// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_igc.h"
%}

%base_import(ipi_image_ground_connection)
%import "air_mspi_orbit.i"

%geocal_shared_ptr(GeoCal::AirMspiIgc);
namespace GeoCal {
class AirMspiIgc : public IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Orbit_file_name,
	     const std::string& Base_directory = ".");
  %python_attribute(base_directory, std::string);
  %python_attribute(master_config_file, std::string);
  %python_attribute(orbit, boost::shared_ptr<AirMspiOrbit>);
  %python_attribute(orbit_file_name, std::string);
  %pickle_init(1, self.master_config_file, self.orbit_file_name,
	       self.base_directory);
};
}
