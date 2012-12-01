#ifndef ARGUS_ORBIT_H
#define ARGUS_ORBIT_H
#include "aircraft_orbit_data.h"
#include "orbit_quaternion_list.h"
#include "geodetic.h"
#include "gdal_raster_image.h"
#include <map>
#include <boost/shared_ptr.hpp>

namespace GeoCal {
/****************************************************************//**
  This is a single ARGUS navigation file record. In addition to the
  normal things in an OrbitData class, this contains some other
  metadata. This matches what is stored in the ARGUS CSV navigation
  file. The additional information is the file name of the JPEG image
  file collected by the camera and the camera number.

  Note that we don't have access to a direct measurement of the 
  velocity. Rather than trying to do some clever estimate of the
  velocity, we just set it to 0. 
*******************************************************************/

class ArgusOrbitData : public AircraftOrbitData {
public:
//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

  ArgusOrbitData(const Time& Tm,
		 const std::string& File_name, int Camera_number,
		 const Geodetic& Position, 
		 const boost::array<double, 3>& Vel_fixed,
		 double Roll, double Pitch,
		 double Heading)
    : AircraftOrbitData(Tm, Position, Vel_fixed, Roll, Pitch, Heading),
      camera_number_(Camera_number), file_name_(File_name)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~ArgusOrbitData() {}
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// File with JPEG data.
//-----------------------------------------------------------------------

  const std::string& file_name() const { return file_name_;}

  GdalRasterImage image(int band = 1) const;

  void save_ortho(const MapInfo& Mi, const Camera& Cam, const Dem& D,
		  const std::string& Fname,
		  const std::string& Type, int Border = 10,
		  int Grid_spacing = 1) const;
  void add_ortho_to_image(const boost::shared_ptr<RasterImage>& M1,
			  const boost::shared_ptr<RasterImage>& M2,
			  const boost::shared_ptr<RasterImage>& M3,
			  const Camera& Cam,
			  const Dem& D,
			  int Border = 10) const;
  static void mosaic(const std::vector<boost::shared_ptr<ArgusOrbitData> > 
		     &Od,
		     const std::vector<boost::shared_ptr<Camera> >& Cam,
		     const Dem& D,
		     const MapInfo& Mi, const std::string& Fname,
		     const std::string& Type,
		     int Border = 10);

//-----------------------------------------------------------------------
/// Camera number. This is 1 - 13.
//-----------------------------------------------------------------------

  int camera_number() const { return camera_number_;}

private:
  static void save_to_file(const std::string& Fname, 
			   const RasterImage& M1,
			   const RasterImage& M2, 
			   const RasterImage& M3, 
			   const std::string& Type);
  int camera_number_;
  std::string file_name_;
  mutable boost::shared_ptr<GDALDataset> gdal_data_;
};

/****************************************************************//**
  This read a CSV navigation file and uses it to generate a Orbit. 
  We access the data in two ways. The first is the standard time 
  index, giving a OrbitData for that time (interpolating if needed).
  The second is by "row" and camera number. All the orbit data for a
  given row were acquired at nearly the same time. A row contains data
  for 1 or more cameras - once the instrument is fully working a row
  has data for cameras 1 through 13. We return a ArgusOrbitData, which
  in addition to having the normal OrbitData stuff has additional
  metadata information.
*******************************************************************/

class ArgusOrbit : public OrbitQuaternionList {
public:
  ArgusOrbit(const std::string& Fname);
  virtual void print(std::ostream& Os) const;

  boost::shared_ptr<ArgusOrbitData> nav(int row, int camera_num) const;

  double focal_length(int camera_num) const;

//-----------------------------------------------------------------------
/// Number of rows of data we have.
//-----------------------------------------------------------------------

  int number_row() const { return number_row_; }
  const std::string& file_name() const {return fname;}
private:
  std::string fname;
  static double row_time_tolerance;

  /// Same data as in orbit_data_map, but arranged by "row, camera". Data
  /// for different cameras that are within row_time_tolerance are
  /// considered to be in the same row.

  typedef std::map<std::pair<int, int>, 
		   boost::shared_ptr<ArgusOrbitData> > row_map;
  row_map row_data;
  /// Number of rows we have.
  int number_row_;
};
}
#endif
