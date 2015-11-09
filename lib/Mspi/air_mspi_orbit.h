#ifndef AIRMSPI_ORBIT_H
#define AIRMSPI_ORBIT_H
#include "orbit.h"		// Definition of Orbit
#include "mspi_gimbal.h"	// Definition of MspiGimbal.
#include "gdal_raster_image.h"	// Definition of GdalRasterImage
#include "geocal_datum.h"	// Definition of Datum
#include "aircraft_orbit_data.h"
				// Definition of VerticalDefinition.
#include "air_mspi_file.h"

namespace GeoCal {
/****************************************************************//**
  Simple structure that describes navigation data at a point in time.
*******************************************************************/
class AirMspiNavData : public Printable<AirMspiNavData> {
public:
  AirMspiNavData() {}
  AirMspiNavData(const blitz::Array<double, 1>& Raw_data,
		 const Datum& datum,
		 bool Old_format);
  Geodetic position;
  /// Velocity in m/s
  boost::array<double, 3> velocity;
  /// YPR in radians
  boost::array<double, 3> ypr;
  /// YPR rate in radians/second
  boost::array<double, 3> ypr_rate;
  // Gimbal position in radians;
  double gimbal_pos;
  // Gimbal velocity in radians/second
  double gimbal_vel;
  void print(std::ostream& Os) const;

  static AirMspiNavData interpolate(const AirMspiNavData& N1, 
				    const AirMspiNavData& N2,
				    double f);
private:
  static double interpolate_angle(double v1, double v2, double f);
};

/****************************************************************//**
  This reads the AirMSPI orbit file.

  As an implementation detail, we use GDAL to read the data. This
  isn't really an image, but by doing this we don't need to bother
  developing all the mechanism for reading/writing HDF files. May
  want to revisit this if we actually do develop that, since there 
  is no reason other than convenience to use GDAL here.
*******************************************************************/

class AirMspiOrbit : public Orbit, public Observer<MspiGimbal> {
public:
  AirMspiOrbit(const std::string& Fname, 
	       const boost::shared_ptr<MspiGimbal>& Gim,
	       const boost::shared_ptr<Datum>& D
	       = boost::shared_ptr<Datum>(new NoDatum()),
	       AircraftOrbitData::VerticalDefinition Def = 
	       AircraftOrbitData::GEODETIC_VERTICAL);

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~AirMspiOrbit() {}

//-----------------------------------------------------------------------
/// The data version.
//-----------------------------------------------------------------------

  std::string data_version() const 
  { return data->metadata<std::string>("AIRMISR_NAVIGATION_DATA_VERSION"); }

//-----------------------------------------------------------------------
/// File name for data.
//-----------------------------------------------------------------------

  std::string file_name() const
  { return file_name_; }

//-----------------------------------------------------------------------
/// Datum the raw height is relative to.
//-----------------------------------------------------------------------

  const boost::shared_ptr<Datum>& datum() const { return datum_;}

//-----------------------------------------------------------------------
/// Flight description.
//-----------------------------------------------------------------------

  std::string flight_description() const 
  { return data->metadata<std::string>("flight_description"); }

  AirMspiNavData nav_data(int Index) const;
  AirMspiNavData nav_data(Time T) const;

//-----------------------------------------------------------------------
/// Return the gimbal position at time T in degrees.
//-----------------------------------------------------------------------
  
  double gimbal_position(Time T) const
  { return nav_data(T).gimbal_pos * GeoCal::Constant::rad_to_deg; }
    
  boost::shared_ptr<QuaternionOrbitData> orbit_data_index(int Index) const;
  boost::shared_ptr<QuaternionOrbitData> orbit_data_index_with_derivative(int Index) const;
  boost::shared_ptr<CartesianFixed> position_cf(Time T) const;

//-----------------------------------------------------------------------
/// Spacing in time between measurements. This is in seconds.
//-----------------------------------------------------------------------
  
  double time_spacing() const {return tspace_;}

//-----------------------------------------------------------------------
/// Definition of vertical. This should almost always be
/// GEODETIC_VERTICAL, but we have this in place to be able to match
/// old test data.
//-----------------------------------------------------------------------

  AircraftOrbitData::VerticalDefinition vertical_def() const
  { return vdef_; }

  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;

  virtual void print(std::ostream& Os) const;

  virtual void notify_update(const MspiGimbal& G) 
  { empty_cache(); };
private:
  blitz::Array<double, 1> raw_data(int Index) const;
  boost::shared_ptr<GdalRasterImage> data;
  std::string file_name_;
  mutable std::vector<boost::shared_ptr<QuaternionOrbitData> > cache_;
  mutable std::vector<blitz::Array<double, 2> > data_cache_;
  mutable std::vector<blitz::Array<double, 2> >::iterator next_swap_;
  int tile_number_line;
  boost::shared_ptr<Datum> datum_;
  AircraftOrbitData::VerticalDefinition vdef_;
  double tspace_;
  bool old_format;
  boost::shared_ptr<MspiGimbal> gimbal;
  void empty_cache();
  AirMspiOrbit();
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}
GEOCAL_EXPORT_KEY(AirMspiOrbit);
#endif

