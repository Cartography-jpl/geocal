#ifndef GEODETIC_H
#define GEODETIC_H
#include "ground_coordinate.h"
#include "geocal_exception.h"

namespace GeoCal {
  class Ecr;			// Forward declaration.

/****************************************************************//**
  This is a ground coordinate, represented in Geodetic coordinates.
*******************************************************************/

class Geodetic : public GroundCoordinate {
public:
  Geodetic(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Make an Geodetic with the given latitude, longitude, and height.
/// Latitude and longitude are in degrees, height is in meters.
/// Longitude should be between -180 and 180 and latitude -90 and 90.
//-----------------------------------------------------------------------

  Geodetic(double Latitude, double Longitude, double Height_ellipsoid = 0)
  : lat_(Latitude), lon_(Longitude), height_ellipsoid_(Height_ellipsoid)
  {
    range_check(lat_, -90.0, 90.0);
    range_check(lon_, -180.0, 180.0);
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Geodetic() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Geodetic() {}


  virtual void lat_lon_height(double& Latitude, double& Longitude, 
		      double& Height_reference_surface) const
  { Latitude = latitude(); Longitude = longitude(); 
    Height_reference_surface = height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Height above ellipsoid, in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface() const {return height_ellipsoid_;}
    
//-----------------------------------------------------------------------
/// Return latitude in degrees. Latitude is -90 to 90.
//-----------------------------------------------------------------------

  virtual double latitude() const {return lat_;}
    
//-----------------------------------------------------------------------
/// Return longitude in degrees. Longitude is -180 to 180.
//-----------------------------------------------------------------------

  virtual double longitude() const {return lon_;}
private:
  double lat_;			///< Latitude, in degrees.
  double lon_;			///< Longitude, in degrees.
  double height_ellipsoid_;	///< Height above ellipsoid, in meters.
  friend class Ecr;		// This is a friend in order to
				// optimize the conversion of Ecr to
				// Geodetic, which it turns out is a
				// bottleneck in some programs.
};

/****************************************************************//**
  This is a ground coordinate, represented in Geocentric coordinates.
*******************************************************************/

class Geocentric : public GroundCoordinate {
public:
  Geocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Make an Geocentric with the given latitude, longitude, and height.
/// Latitude and longitude are in degrees, height is in meters.
/// Longitude should be between -180 and 180 and latitude -90 and 90.
//-----------------------------------------------------------------------

  Geocentric(double Latitude, double Longitude, double Height_ellipsoid = 0)
  : lat_(Latitude), lon_(Longitude), height_ellipsoid_(Height_ellipsoid)
  {
    range_check(lat_, -90.0, 90.0);
    range_check(lon_, -180.0, 180.0);
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Geocentric() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Geocentric() {}

//-----------------------------------------------------------------------
/// Height above ellipsoid, in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface() const {return height_ellipsoid_;}
    
//-----------------------------------------------------------------------
/// Return latitude in degrees. Latitude is -90 to 90.
//-----------------------------------------------------------------------

  virtual double latitude() const {return lat_;}
    
//-----------------------------------------------------------------------
/// Return longitude in degrees. Longitude is -180 to 180.
//-----------------------------------------------------------------------

  virtual double longitude() const {return lon_;}
private:
  double earth_radius(double Latitude) const;
  double lat_;			///< Latitude, in degrees.
  double lon_;			///< Longitude, in degrees.
  double height_ellipsoid_;	///< Height above ellipsoid, in meters.
};
}
#endif
