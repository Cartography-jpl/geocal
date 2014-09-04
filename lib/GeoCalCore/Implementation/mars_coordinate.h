#ifndef MARS_COORDINATE_H
#define MARS_COORDINATE_H
#include "ground_coordinate.h"
#include "geocal_matrix.h"
#include "spice_helper.h"

namespace GeoCal {
template<int NAIF_CODE> class Planetocentric;

template<int NCODE> class PlanetConstant {
public:
  enum {NAIF_CODE = NCODE };
  static double planet_a() {return h.planet_a();}
  static double planet_b() {return h.planet_b(); }
  static double planet_esq() {return h.planet_esq(); }
  static std::string planet_name() { return name;}
private:
  static SpicePlanetConstant h;
  static const char* name;
};

/****************************************************************//**
  This is a ground coordinate, expressed in fixed Mars coordinates.
*******************************************************************/
class MarsFixed : public CartesianFixed {
public:
  enum { NAIF_CODE = 499 };
  MarsFixed(const GroundCoordinate& Gc);

//-----------------------------------------------------------------------
/// Make an MarsFixed with the given position, in meters.
//-----------------------------------------------------------------------

  MarsFixed(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an MarsFixed with the given position in meters.
//-----------------------------------------------------------------------

  MarsFixed(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  MarsFixed() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MarsFixed() {}

  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;

//-----------------------------------------------------------------------
/// Create an instance of MarsFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
  create(boost::array<double, 3> P) const
  { return boost::shared_ptr<CartesianFixed>(new MarsFixed(P)); }

//-----------------------------------------------------------------------
/// Matrix to convert MarsInertial to MarsFixed. The transpose of this
/// will convert MarsFixed to MarsInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(NAIF_CODE,
							   T, Ci_to_cf); 
  }

  virtual double height_reference_surface() const;
  virtual double min_radius_reference_surface() const;
  virtual double latitude() const;
  virtual double longitude() const;
  Planetocentric<NAIF_CODE> convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0) 
  const;
  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is a MarsIntertial coordinate (J2000)
*******************************************************************/

class MarsInertial : public CartesianInertial {
public:
  enum { NAIF_CODE = 499 };

//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  MarsInertial() {}

//-----------------------------------------------------------------------
/// Make an MarsInertial with the given position, in meters.
//-----------------------------------------------------------------------

  MarsInertial(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an MarsInertial with the given position in meters.
//-----------------------------------------------------------------------

  MarsInertial(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~MarsInertial() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;

//-----------------------------------------------------------------------
/// Matrix to convert MarsInertial to MarsFixed. The transpose of this
/// will convert 
/// MarsFixed to MarsInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(NAIF_CODE,
							   T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>(new MarsInertial(P)); }

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const;
};

/****************************************************************//**
  This is Planet coordinates as Planetocentric latitude, longitude,
  and height above the reference ellipsoid. This is the planet
  equivalent of Geocentric (*not* Geodetic). Height is relative to the
  ellipsoid, but latitude is relative to center of planet rather than
  normal of ellipsoid.
*******************************************************************/

template<int NAIF_CODE> class Planetocentric : public GroundCoordinate {
public:
//-----------------------------------------------------------------------
/// Convert from GroundCoor.
//-----------------------------------------------------------------------

  Planetocentric(const GroundCoordinate& Gc)
  {
    if(const Planetocentric<NAIF_CODE>* g = 
       dynamic_cast<const Planetocentric<NAIF_CODE>*>(&Gc)) {
      *this = *g;
      return;
    }
    MarsFixed mf(Gc);
    lon_ = mf.longitude();
    lat_ = mf.latitude();
    height_ellipsoid_ = 
      norm(mf.position) - planet_radius(lat_ * Constant::deg_to_rad);
  }
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const
  {
    double lat = lat_ * Constant::deg_to_rad;
    double lon = lon_ * Constant::deg_to_rad;
    double r = planet_radius(lat) + height_ellipsoid_;
    return boost::shared_ptr<CartesianFixed>
      (new MarsFixed(r * cos(lat) * cos(lon),
		     r * cos(lat) * sin(lon),
		     r * sin(lat)));
  }
  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::planet_name()
       << "Planetocentric: (" << latitude() << " deg, " 
       << longitude() << " deg, "
       << height_reference_surface() << " m)";
  }

//-----------------------------------------------------------------------
/// Make an Planetocentric with the given latitude, longitude, and height.
/// Latitude and longitude are in degrees, height is in meters.
/// Longitude should be between -180 and 180 and latitude -90 and 90.
//-----------------------------------------------------------------------

  Planetocentric(double Latitude, double Longitude, double Height_ellipsoid = 0)
  : lat_(Latitude), lon_(Longitude), height_ellipsoid_(Height_ellipsoid)
  {
    range_check(lat_, -90.0, 90.0);
    range_check(lon_, -180.0, 180.0);
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Planetocentric() {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~Planetocentric() {}

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
  double height_ellipsoid_;	///< Height above ellipsoid, in
				///meters.

//-----------------------------------------------------------------------
/// Radius of planet in meters at given Planetocentric Latitude (in
/// radians, since we've already converted.
//-----------------------------------------------------------------------

  double planet_radius(double Latitude_radians) const
  {
    double clat = cos(Latitude_radians);
    return PlanetConstant<NAIF_CODE>::planet_b() / 
      sqrt(1 - PlanetConstant<NAIF_CODE>::planet_esq() * clat * clat);
  }
};

typedef PlanetConstant<499> MarsConstant;
typedef Planetocentric<499> MarsPlanetocentric;


}
#endif
