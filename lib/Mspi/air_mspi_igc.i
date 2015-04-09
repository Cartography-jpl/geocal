// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_igc.h"
%}

%base_import(ipi_image_ground_connection)
%import "air_mspi_orbit.i"
%import "air_mspi_time_table.i"
%import "mspi_camera.i"

%geocal_shared_ptr(GeoCal::AirMspiIgc);
namespace GeoCal {
class AirMspiIgc : public IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Orbit_file_name,
	     const std::string& L1b1_file_name,
	     int Band,
	     const std::string& Base_directory = ".");
  %python_attribute_with_set(band, int);
  %python_attribute(orbit, boost::shared_ptr<AirMspiOrbit>);
  %python_attribute(time_table, boost::shared_ptr<AirMspiTimeTable>);
  %python_attribute(camera, boost::shared_ptr<MspiCamera>);
  %python_attribute(l1b1_file_name, std::string);
  %pickle_serialization();
};
}
