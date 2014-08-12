// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_igc.h"
%}

%base_import(ipi_image_ground_connection)

%geocal_shared_ptr(GeoCal::AirMspiIgc);
namespace GeoCal {
class AirMspiIgc : public IpiImageGroundConnection {
public:
  AirMspiIgc(const std::string& Master_config_file,
	     const std::string& Base_directory = ".");
    %python_attribute(base_directory, std::string);
    %python_attribute(master_config_file, std::string);
};
}
