// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "air_mspi_igc_collection.h"
%}

%base_import(igc_collection)
%import "image_ground_connection.i"
%import "air_mspi_orbit.i"
%import "mspi_camera.i"
%import "time_table.i"

%geocal_shared_ptr(GeoCal::AirMspiIgcCollection);
namespace GeoCal {
class AirMspiIgcCollection : public IgcCollection {
public:
  AirMspiIgcCollection(const std::string& Master_config_file,
		       const std::string& Orbit_file_name,
		       const std::string& L1b1_table,
		       const std::string& Swath_to_use = "660-I",
		       const std::string& Base_directory = ".");
  AirMspiIgcCollection(const boost::shared_ptr<Orbit>& Orb,
   		       const boost::shared_ptr<MspiCamera>& Cam,
   		       const boost::shared_ptr<MspiGimbal>& Gim,
   		       const boost::shared_ptr<Dem>& D,
   		       const std::vector<std::string>& L1b1_file_name,
		       const std::string& Swath_to_use = "660-I",
		       int Dem_resolution = 10,
		       const std::string& Base_directory = ".");
  AirMspiIgcCollection(const boost::shared_ptr<Orbit>& Orb,
   		       const boost::shared_ptr<MspiCamera>& Cam,
   		       const boost::shared_ptr<MspiGimbal>& Gim,
   		       const boost::shared_ptr<Dem>& D,
		       const std::string& Master_config_file,
   		       const std::vector<std::string>& L1b1_file_name,
		       const std::string& Swath_to_use = "660-I",
		       int Dem_resolution = 10,
		       const std::string& Base_directory = ".");
  %python_attribute(number_image, virtual int);
  virtual boost::shared_ptr<ImageGroundConnection> 
  image_ground_connection(int Image_index) const;
  virtual boost::shared_ptr<IgcCollection> 
  subset(const std::vector<int>& Index_set) const;

  bool have_config(int Index, const std::string& Keyword) const;
  // Just have the types we happen to have needed. We can extend this
  // as needed.
  %extend {
    double config_value_double(int Index, const std::string& Key) const
    { return $self->config_value<double>(Index,Key); }
    int config_value_int(int Index, const std::string& Key) const
    { return $self->config_value<int>(Index, Key); }
    std::string config_value_string(int Index, const std::string& Key) const
    { return $self->config_value<std::string>(Index, Key); }
  }
  void set_config_value(int Index, const std::string& Keyword,
			const std::string& Value);
  boost::shared_ptr<Orbit> orbit(int Index) const;
  std::string l1b1_file_name(int Index) const;
  void set_orbit(const boost::shared_ptr<Orbit>& Orb);
  boost::shared_ptr<MspiCamera> camera(int Index) const;
  void set_camera(const boost::shared_ptr<MspiCamera>& Can);
  boost::shared_ptr<MspiGimbal> gimbal(int Index) const;
  void set_gimbal(const boost::shared_ptr<MspiGimbal>& Gim);
  boost::shared_ptr<TimeTable> time_table(int Index) const;
  int number_band(int Index);
  int band(int Index);
  void band(int Index, int B);
  int min_l1b1_line(int Index) const;
  int max_l1b1_line(int Index) const;
  int view_number_to_image_index(int View_number) const;
  void replace_view_config(const std::string& Master_config_file,
			   const std::string& L1b1_table);
%pickle_serialization_dir();
%pythoncode {
def __getitem__(self, i):
  self.config_value_string(*i)

def __setitem__(self, i, v):
  self.set_config_value(i[0], i[1], v)
}
};
}

// List of things "import *" will include
%python_export("AirMspiIgcCollection")
