// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "argus_orbit.h"
%}

%geocal_shared_ptr(ArgusOrbitData);
%geocal_shared_ptr(ArgusOrbit);
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
  GdalRasterImage image(int band = 1) const;
  %python_attribute(file_name, std::string)
  %python_attribute(camera_number, int)
  %pickle_init(self.time, self.file_name, self.camera_number,
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
  %pickle_init(self.file_name)
};
}

%template(Vector_ArgusOrbitData) std::vector<boost::shared_ptr<GeoCal::ArgusOrbitData> >;
