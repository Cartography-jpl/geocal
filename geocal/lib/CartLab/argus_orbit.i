// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "argus_orbit.h"
#include "ecr.h"
#include "image_ground_connection.h"
#include "raster_image_multi_band_variable.h"
%}
%base_import(orbit_quaternion_list)
%base_import(aircraft_orbit_data)
%import "map_info.i"
%import "gdal_raster_image.i"
%geocal_shared_ptr(GeoCal::ArgusOrbitData);
%geocal_shared_ptr(GeoCal::ArgusOrbit);
namespace GeoCal {
class ArgusOrbitData : public AircraftOrbitData {
public:
  ArgusOrbitData(const Time& Tm,
		 const std::string& File_name, int Camera_number,
		 const Geodetic& Position, 
		 const boost::array<double, 3>& Vel_fixed,
		 double Roll, double Pitch,
		 double Heading);
  void save_ortho(const MapInfo& Mi, const Camera& Cam, const Dem& D,
		  const std::string& Fname,
		  const std::string& Type, int Border = 10,
   	          int Grid_spacing = 1) const;
  void add_ortho_to_image(const boost::shared_ptr<RasterImage>& M1,
			  const boost::shared_ptr<RasterImage>& M2,
			  const boost::shared_ptr<RasterImage>& M3,
			  const Camera& Cam,
			  const Dem& D, int Border = 10) const;
  static void mosaic(const std::vector<boost::shared_ptr<ArgusOrbitData> > 
		     &Od,
		     const std::vector<boost::shared_ptr<Camera> >& Cam,
		     const Dem& D,
		     const MapInfo& Mi, const std::string& Fname,
		     const std::string& Type,
		     int Border = 10);
  boost::shared_ptr<GdalRasterImage> image(int band = 1) const;
  %python_attribute(file_name, std::string)
  %python_attribute(camera_number, int)
  %pickle_init(1, self.time, self.file_name, self.camera_number,
	       self.position_geodetic, self.vector_cf, self.roll,
	       self.pitch, self.heading)
};

class ArgusOrbit : public OrbitQuaternionList {
public:
  ArgusOrbit(const std::string& Fname);
  double focal_length(int camera_num) const;
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  %python_attribute(number_row, int)
  boost::shared_ptr<ArgusOrbitData> nav(int row, int camera_num) const;
  %python_attribute(file_name, std::string)
  %pickle_init(1, self.file_name)
};
}

%template(Vector_ArgusOrbitData) std::vector<boost::shared_ptr<GeoCal::ArgusOrbitData> >;
