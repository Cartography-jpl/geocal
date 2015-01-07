// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "air_mspi_igc_collection.h"
%}

%base_import(igc_collection)
%import "image_ground_connection.i"

%geocal_shared_ptr(GeoCal::AirMspiIgcCollection);
namespace GeoCal {
class AirMspiIgcCollection : public IgcCollection {
public:
  AirMspiIgcCollection(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& L1b1_table);
  %python_attribute(number_image, virtual int);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int> Index_set) const;
};
}
