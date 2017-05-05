#ifndef PLANET_COORDINATE_H
#define PLANET_COORDINATE_H
#include "ground_coordinate.h"
#include "geocal_matrix.h"
#include "spice_helper.h"
#include "orbit.h"
#include "simple_dem.h"
#include "coordinate_converter.h"

namespace GeoCal {
  class Planetocentric;
  
/****************************************************************//**
  Constants for a planet.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/
class PlanetConstant {
public:
  enum {MARS_NAIF_CODE=499, EUROPA_NAIF_CODE=502};
  static double a(int Naif_code) {return h(Naif_code).planet_a();}
  static double b(int Naif_code) {return h(Naif_code).planet_b(); }
  static double esq(int Naif_code) {return h(Naif_code).planet_esq(); }
  static std::string name(int Naif_code) { return h(Naif_code).planet_name();}

//-----------------------------------------------------------------------
/// Calculate flattening
//-----------------------------------------------------------------------

  static double flattening(int Naif_code)
  { return (PlanetConstant::a(Naif_code) - PlanetConstant::b(Naif_code)) /
      PlanetConstant::a(Naif_code); }
  
//-----------------------------------------------------------------------
/// Calculate inverse flattening.
//-----------------------------------------------------------------------
  static double inverse_flattening(int Naif_code)
  { return 1.0 / PlanetConstant::flattening(Naif_code); }    
private:
  static std::map<int, SpicePlanetConstant> h_map;
  static SpicePlanetConstant h(int Naif_code);
};

/****************************************************************//**
  This is a ground coordinate, expressed in fixed Planet coordinates.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/
class PlanetFixed : public CartesianFixed {
public:
  PlanetFixed(const GroundCoordinate& Gc);
  PlanetFixed(double X, double Y, double Z, int Naif_code);

//-----------------------------------------------------------------------
/// Create an PlanetFixed with the given position in meters.
//-----------------------------------------------------------------------

  PlanetFixed(const boost::array<double, 3>& Pos, int Naif_code)
    : naif_code_(Naif_code)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  PlanetFixed(int Naif_code = -1)  : naif_code_(Naif_code) {}

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~PlanetFixed() {}

  virtual boost::shared_ptr<CartesianInertial> 
  convert_to_ci(const Time& T) const;

//-----------------------------------------------------------------------
/// Create an instance of PlanetFixed.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianFixed> 
  create(boost::array<double, 3> P) const
  { return boost::shared_ptr<CartesianFixed>(new PlanetFixed(P, naif_code_)); }

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed. The transpose of this
/// will convert PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(naif_code_,
							   T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed including
/// velocity. The transpose of this
/// will convert PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void cf_to_ci_with_vel(const Time& T, double Cf_to_ci[6][6]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_inertial_with_vel
      (naif_code_, T, Cf_to_ci); 
  }

//-----------------------------------------------------------------------
/// Height above the reference ellipsoid.
//-----------------------------------------------------------------------
  virtual double height_reference_surface() const;
  virtual double min_radius_reference_surface() const
  {
    return std::min(PlanetConstant::a(naif_code_), 
		    PlanetConstant::b(naif_code_));
  }

//-----------------------------------------------------------------------
/// Return latitude in degrees. This is the Planetocentric. This is 
/// the planet equivalent of Geocentric, so relative to the center
/// (*not* normal to ellipsoid like Geodetic).
//-----------------------------------------------------------------------

  virtual double latitude() const
  {
    return atan2(position[2],
		 sqrt(position[0] * position[0] + position[1] * position[1]))
      * Constant::rad_to_deg;
  }

//-----------------------------------------------------------------------
/// Return longitude in degrees. This is the Planetocentric (which
/// actually isn't any different than Planetographic for longitude).
//-----------------------------------------------------------------------

  virtual double longitude() const
  {
    return atan2(position[1], position[0]) * Constant::rad_to_deg;
  }

  Planetocentric convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0)
    const;
//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------

  virtual int naif_code() const {return naif_code_;}
  
  virtual void print(std::ostream& Os) const;
  static PlanetFixed target_position
  (const std::string& Target_name, const Time& T, int Naif_code);
  static boost::shared_ptr<QuaternionOrbitData> orbit_data
  (const std::string& Target_name, 
   const std::string& Spacecraft_reference_frame_name, const Time& T,
   int Naif_code);
private:
  int naif_code_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is a Planet Intertial coordinate.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/

class PlanetInertial : public CartesianInertial {
public:
//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  PlanetInertial(int Naif_code=-1) : naif_code_(Naif_code) {}
  PlanetInertial(double X, double Y, double Z, int Naif_code);

//-----------------------------------------------------------------------
/// Create an PlanetInertial with the given position in meters.
//-----------------------------------------------------------------------

  PlanetInertial(const boost::array<double, 3>& Pos, int Naif_code)
    : naif_code_(Naif_code)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~PlanetInertial() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const;

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed. The transpose of this
/// will convert 
/// PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(naif_code_,
							   T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed including
/// velocity. The transpose of this
/// will convert PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed_with_vel
      (naif_code_, T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>
      (new PlanetInertial(P,naif_code_)); }

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
    const;

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  virtual int naif_code() const {return naif_code_;}
  virtual void print(std::ostream& Os) const;
private:
  int naif_code_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This is Planet coordinates as Planetocentric latitude, longitude,
  and height above the reference ellipsoid. This is the planet
  equivalent of Geocentric (*not* Geodetic). Height is relative to the
  ellipsoid, but latitude is relative to center of planet rather than
  normal of ellipsoid.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/

class Planetocentric : public GroundCoordinate {
public:
  Planetocentric(const GroundCoordinate& Gc);
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf() const;
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  virtual int naif_code() const {return naif_code_;}
  Planetocentric(double Latitude, double Longitude, double Height_ellipsoid,
		 int Naif_code);

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  Planetocentric(int Naif_code=-1) : naif_code_(Naif_code) {}

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
  int naif_code_;

//-----------------------------------------------------------------------
/// Radius of planet in meters at given Planetocentric Latitude (in
/// radians, since we've already converted.
//-----------------------------------------------------------------------

  double planet_radius(double Latitude_radians) const
  {
    double clat = cos(Latitude_radians);
    return PlanetConstant::b(naif_code_) / 
      sqrt(1 - PlanetConstant::esq(naif_code_) * clat * clat);
  }
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};


/****************************************************************//**
  CoordinateConverter that goes to and from Planetocentric coordinates.
*******************************************************************/

class PlanetocentricConverter : public CoordinateConverter {
public:
//-----------------------------------------------------------------------
/// Constructor.  
//-----------------------------------------------------------------------

  PlanetocentricConverter(int Naif_code = -1)
    : naif_code_(Naif_code)
  { }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~PlanetocentricConverter() {}

//-----------------------------------------------------------------------
/// Convert to Planetocentric. X and Y are longitude and latitude in
/// degrees, and height is in meters.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const
  {
    return boost::shared_ptr<GroundCoordinate>
      (new Planetocentric(Y, X, Height, naif_code_));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    const PlanetocentricConverter* t =
      dynamic_cast<const PlanetocentricConverter*>(&Conv);
    if(t && t->naif_code() == naif_code())
      return true;
    return false;
  }

//-----------------------------------------------------------------------
/// Convert to Planetocentric. X and Y are longitude and latitude in
/// degrees, and height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const
  {
    if(Gc.naif_code() != naif_code_) {
      Exception e;
      e << "Gp has the wrong naif_code(). Got " << Gc.naif_code()
	<< " but expected " << naif_code_;
      throw e;
    }
    Planetocentric gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Height = gd.height_reference_surface();
  }
  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
				     double& Y, double& Height) const
  {
    if(Gc.naif_code() != naif_code_) {
      Exception e;
      e << "Gp has the wrong naif_code(). Got " << Gc.naif_code()
	<< " but expected " << naif_code_;
      throw e;
    }
    Planetocentric gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Height = gd.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  virtual int naif_code() const { return naif_code_; }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << PlanetConstant::name(naif_code_)
       << "Planetocentric Converter"; }
private:
  int naif_code_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

class PlanetSimpleDem: public Dem {
public:
//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

  PlanetSimpleDem(double H, int Naif_code) : h_(H), naif_code_(Naif_code) {}

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  PlanetSimpleDem(int Naif_code=-1) : h_(0), naif_code_(Naif_code) {}

//-----------------------------------------------------------------------
/// Destructor
//-----------------------------------------------------------------------

  virtual ~PlanetSimpleDem() {}

//-----------------------------------------------------------------------
/// Return height of surface above/below the reference surface (e.g.,
/// WGS-84 for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const {
    if(Gp.naif_code() != naif_code_) {
      Exception e;
      e << "Gp has the wrong naif_code(). Got " << Gp.naif_code()
	<< " but expected " << naif_code_;
      throw e;
    }
    return h_;
  }

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

  virtual double distance_to_surface(const GroundCoordinate& Gp) 
    const
  {
    if(Gp.naif_code() != naif_code_) {
      Exception e;
      e << "Gp has the wrong naif_code(). Got " << Gp.naif_code()
	<< " but expected " << naif_code_;
      throw e;
    }
    Planetocentric g(Gp);
    return g.height_reference_surface() - h_;
  }

  virtual boost::shared_ptr<GroundCoordinate> 
    surface_point(const GroundCoordinate& Gp) const
  {
    if(Gp.naif_code() != naif_code_) {
      Exception e;
      e << "Gp has the wrong naif_code(). Got " << Gp.naif_code()
	<< " but expected " << naif_code_;
      throw e;
    }
    Planetocentric g(Gp);
    return boost::shared_ptr<GroundCoordinate>(
       new Planetocentric(g.latitude(), g.longitude(), h_, naif_code_));
  }

//-----------------------------------------------------------------------
/// Print to stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const 
  { Os << "Planet Simple Dem"
       << "  Height: " << h_ << "m \n"
       << "  Planet: " << PlanetConstant::name(naif_code_) << "\n";
  }

//-----------------------------------------------------------------------
/// Return height value used by this object.
//-----------------------------------------------------------------------

  double h() const {return h_;}

//-----------------------------------------------------------------------
/// Set height value used by this object.
//-----------------------------------------------------------------------

  void h(double Hnew) {h_ = Hnew;}

//-----------------------------------------------------------------------
/// Naif code for planet
//-----------------------------------------------------------------------

  int naif_code() const {return naif_code_;}

//-----------------------------------------------------------------------
/// Set Naif code for planet
//-----------------------------------------------------------------------

  void naif_code(int Naif_code) {naif_code_ = Naif_code;}
private:
  double h_;
  int naif_code_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PlanetFixed);
GEOCAL_EXPORT_KEY(PlanetInertial);
GEOCAL_EXPORT_KEY(Planetocentric);
GEOCAL_EXPORT_KEY(PlanetSimpleDem);
GEOCAL_EXPORT_KEY(PlanetocentricConverter);
#endif
