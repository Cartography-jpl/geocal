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
		       const std::string& L1b1_table,
		       const std::string& Base_directory = ".");
  %python_attribute(number_image, virtual int);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;
  int view_number(int Index) const;
  std::string view_name(int Index) const;
  std::string view_time(int Index) const;
  std::string l1b1_file_name(int Index) const;
  std::string l1b1_granule_id(int Index) const;
  std::string geolocation_stage(int Index) const;
  std::string target_type(int Index) const;
  int min_l1b1_line(int Index) const;
  int max_l1b1_line(int Index) const;
  double view_resolution(int Index) const;
  %python_attribute(max_view_resolution, double);
  %python_attribute(l1b2_hdf_chunk_size_x, int);
  %python_attribute(l1b2_hdf_chunk_size_y, int);
};
}
