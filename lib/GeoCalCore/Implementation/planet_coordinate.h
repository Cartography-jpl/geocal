#ifndef PLANET_COORDINATE_H
#define PLANET_COORDINATE_H
#include "ground_coordinate.h"
#include "geocal_matrix.h"
#include "spice_helper.h"
#include "orbit.h"
#include "simple_dem.h"
#include "coordinate_converter.h"

namespace GeoCal {
template<int NAIF_CODE> class Planetocentric;

/****************************************************************//**
  Constants for a planet.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/
template<int NCODE> class PlanetConstant {
public:
  enum {NAIF_CODE = NCODE };
  static double a() {return h.planet_a();}
  static double b() {return h.planet_b(); }
  static double esq() {return h.planet_esq(); }
  static std::string name() { return name_;}

//-----------------------------------------------------------------------
/// Calculate flattening
//-----------------------------------------------------------------------

  static double flattening()
  { return (PlanetConstant<NAIF_CODE>::a() - PlanetConstant<NAIF_CODE>::b()) / PlanetConstant<NAIF_CODE>::a(); }
  
//-----------------------------------------------------------------------
/// Calculate inverse flattening.
//-----------------------------------------------------------------------
  static double inverse_flattening()
  { return 1.0 / PlanetConstant<NAIF_CODE>::flattening(); }    

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  static int naif_code() {return NCODE;}
private:
  static SpicePlanetConstant h;
  static const char* name_;
};

/****************************************************************//**
  This is a ground coordinate, expressed in fixed Planet coordinates.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/
template<int NAIF_CODE> class PlanetFixed : public CartesianFixed {
public:
//-----------------------------------------------------------------------
/// Create PlanetFixed from GroundCoordinate
//-----------------------------------------------------------------------

  PlanetFixed(const GroundCoordinate& Gc)
  {
    if(const PlanetFixed<NAIF_CODE>* g = 
       dynamic_cast<const PlanetFixed<NAIF_CODE>*>(&Gc)) {
      *this = *g;
      return;
    }
    boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
    boost::shared_ptr<PlanetFixed<NAIF_CODE> > mf = 
      boost::dynamic_pointer_cast<PlanetFixed<NAIF_CODE> >(cf);
    if(!mf) {
      Exception e;
      e << "Cannot convert ground coordinate to "
	<< PlanetConstant<NAIF_CODE>::name()<< "Fixed\n"
	<< "Coordinate: " << Gc << "\n";
      throw e;
    }
    position = mf->position;
  }

//-----------------------------------------------------------------------
/// Make an PlanetFixed with the given position, in meters.
//-----------------------------------------------------------------------

  PlanetFixed(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an PlanetFixed with the given position in meters.
//-----------------------------------------------------------------------

  PlanetFixed(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Default constructor.
//-----------------------------------------------------------------------

  PlanetFixed() {}

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
  { return boost::shared_ptr<CartesianFixed>(new PlanetFixed<NAIF_CODE>(P)); }

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed. The transpose of this
/// will convert PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(NAIF_CODE,
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
      (NAIF_CODE, T, Cf_to_ci); 
  }

//-----------------------------------------------------------------------
/// Height above the reference ellipsoid.
//-----------------------------------------------------------------------
  virtual double height_reference_surface() const;
  virtual double min_radius_reference_surface() const
  {
    return std::min(PlanetConstant<NAIF_CODE>::a(), 
		    PlanetConstant<NAIF_CODE>::b());
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

  Planetocentric<NAIF_CODE> convert_to_planetocentric() const;
  virtual boost::shared_ptr<CartesianFixed>
  reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface = 0)
  const
  {
    double aph = PlanetConstant<NAIF_CODE>::a() + 
      Height_reference_surface;
    double bph = PlanetConstant<NAIF_CODE>::b() + 
      Height_reference_surface;
    boost::array<double, 3> dirci;
    dirci[0] = Cl.look_vector[0]/ aph;
    dirci[1] = Cl.look_vector[1]/ aph;
    dirci[2] = Cl.look_vector[2]/ bph;
    double t = norm(dirci);
    dirci[0] /= t;
    dirci[1] /= t;
    dirci[2] /= t;
    boost::array<double, 3> pci;
    pci[0] = position[0] / aph;
    pci[1] = position[1] / aph;
    pci[2] = position[2] / bph;
    double ddotp = dot(dirci, pci);
    double dl = -ddotp - sqrt(ddotp * ddotp + (1 - dot(pci, pci)));
    boost::array<double, 3> res;
    res[0] = (pci[0] + dirci[0] * dl) * aph;
    res[1] = (pci[1] + dirci[1] * dl) * aph;
    res[2] = (pci[2] + dirci[2] * dl) * bph;
    return create(res);
  }

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  static int naif_code() {return NAIF_CODE;}
  
  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::name()
       << "Fixed (" << position[0] << " m, " << position[1] << " m, "
       << position[2] << "m)";
  }

//-----------------------------------------------------------------------
/// Use spice to determine the position of the given body at the given
/// time.
//-----------------------------------------------------------------------

  static PlanetFixed<NAIF_CODE> target_position
  (const std::string& Target_name, const Time& T)
  {
    PlanetFixed<NAIF_CODE> res;
    boost::array<double, 3> vel;
    SpiceHelper::state_vector(NAIF_CODE, Target_name, T, res.position, vel);
    return res;
  }

//-----------------------------------------------------------------------
/// Return orbit data for the given target and spacecraft reference
/// frame.
//-----------------------------------------------------------------------

  static boost::shared_ptr<QuaternionOrbitData> orbit_data
  (const std::string& Target_name, 
   const std::string& Spacecraft_reference_frame_name, const Time& T)
  {
    boost::shared_ptr<PlanetFixed<NAIF_CODE> > 
      pos(new PlanetFixed<NAIF_CODE>());
    boost::array<double, 3> vel;
    SpiceHelper::state_vector(NAIF_CODE, Target_name, T, pos->position, vel);
    return boost::shared_ptr<QuaternionOrbitData>
      (new QuaternionOrbitData(T, pos, vel, 
          SpiceHelper::conversion_quaternion(Spacecraft_reference_frame_name, 
	     SpiceHelper::fixed_frame_name(NAIF_CODE), T)));
  }
};

/****************************************************************//**
  This is a Planet Intertial coordinate.

  Note that "Planet" also includes "Moon of planet", basically 
  anything with a NAIF_CODE
*******************************************************************/

template<int NAIF_CODE> class PlanetInertial : public CartesianInertial {
public:
//-----------------------------------------------------------------------
/// Default constructor, doesn't initialize position.
//-----------------------------------------------------------------------

  PlanetInertial() {}

//-----------------------------------------------------------------------
/// Make an PlanetInertial with the given position, in meters.
//-----------------------------------------------------------------------

  PlanetInertial(double X, double Y, double Z)
  {
    position[0] = X;
    position[1] = Y;
    position[2] = Z;
  }

//-----------------------------------------------------------------------
/// Create an PlanetInertial with the given position in meters.
//-----------------------------------------------------------------------

  PlanetInertial(const boost::array<double, 3>& Pos)
  {
    position = Pos;
  }

//-----------------------------------------------------------------------
/// Destructor.
//-----------------------------------------------------------------------

  virtual ~PlanetInertial() {}
  virtual boost::shared_ptr<CartesianFixed> convert_to_cf(const Time& T) 
    const
  {
    boost::shared_ptr<CartesianFixed> res(new PlanetFixed<NAIF_CODE>);
    CartesianFixed::toolkit_coordinate_interface->
      to_fixed(NAIF_CODE, T, *this, *res);
    return res;
  }

//-----------------------------------------------------------------------
/// Matrix to convert PlanetInertial to PlanetFixed. The transpose of this
/// will convert 
/// PlanetFixed to PlanetInertial.
//-----------------------------------------------------------------------

  virtual void ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
  { 
    CartesianFixed::toolkit_coordinate_interface->to_fixed(NAIF_CODE,
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
      (NAIF_CODE, T, Ci_to_cf); 
  }

//-----------------------------------------------------------------------
/// Create an instance of whatever type of CartesianInertial this is.
//-----------------------------------------------------------------------

  virtual boost::shared_ptr<CartesianInertial> 
    create(boost::array<double, 3> P) const 
  { return boost::shared_ptr<CartesianInertial>
      (new PlanetInertial<NAIF_CODE>(P)); }

  virtual boost::shared_ptr<CartesianInertial>
  reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface = 0) 
  const
  {
    double aph = PlanetConstant<NAIF_CODE>::a() + 
      Height_reference_surface;
    double bph = PlanetConstant<NAIF_CODE>::b() + 
      Height_reference_surface;
    boost::array<double, 3> dirci;
    dirci[0] = Cl.look_vector[0]/ aph;
    dirci[1] = Cl.look_vector[1]/ aph;
    dirci[2] = Cl.look_vector[2]/ bph;
    double t = norm(dirci);
    dirci[0] /= t;
    dirci[1] /= t;
    dirci[2] /= t;
    boost::array<double, 3> pci;
    pci[0] = position[0] / aph;
    pci[1] = position[1] / aph;
    pci[2] = position[2] / bph;
    double ddotp = dot(dirci, pci);
    double dl = -ddotp - sqrt(ddotp * ddotp + (1 - dot(pci, pci)));
    boost::array<double, 3> res;
    res[0] = (pci[0] + dirci[0] * dl) * aph;
    res[1] = (pci[1] + dirci[1] * dl) * aph;
    res[2] = (pci[2] + dirci[2] * dl) * bph;
    return create(res);
  }

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  static int naif_code() {return NAIF_CODE;}
  
//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::name()
       << "Inertial (" << position[0] << " m, " << position[1] << " m, "
       << position[2] << "m)";
  }
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
    PlanetFixed<NAIF_CODE> mf(Gc);
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
      (new PlanetFixed<NAIF_CODE>(r * cos(lat) * cos(lon),
				  r * cos(lat) * sin(lon),
				  r * sin(lat)));
  }
  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::name()
       << "Planetocentric: (" << latitude() << " deg, " 
       << longitude() << " deg, "
       << height_reference_surface() << " m)";
  }

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  static int naif_code() {return NAIF_CODE;}
  
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
    return PlanetConstant<NAIF_CODE>::b() / 
      sqrt(1 - PlanetConstant<NAIF_CODE>::esq() * clat * clat);
  }
};

template<int NAIF_CODE> inline boost::shared_ptr<CartesianInertial> 
PlanetFixed<NAIF_CODE>::convert_to_ci(const Time& T) const
{
  boost::shared_ptr<CartesianInertial> res(new PlanetInertial<NAIF_CODE>);
  CartesianFixed::toolkit_coordinate_interface->
    to_inertial((int) NAIF_CODE, T, *this, *res);
  return res;
}

template<int NAIF_CODE> inline double PlanetFixed<NAIF_CODE>::height_reference_surface() const
{
  return Planetocentric<NAIF_CODE>(*this).height_reference_surface();
}

template<int NAIF_CODE> inline Planetocentric<NAIF_CODE> 
PlanetFixed<NAIF_CODE>::convert_to_planetocentric() const
{
  return Planetocentric<NAIF_CODE>(latitude(), longitude(), 
				   height_reference_surface());
}

/****************************************************************//**
  CoordinateConverter that goes to and from Planetocentric coordinates.
*******************************************************************/

template<int NAIF_CODE> class PlanetocentricConverter : 
    public CoordinateConverter {
public:
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
      (new Planetocentric<NAIF_CODE>(Y, X, Height));
  }

//-----------------------------------------------------------------------
/// Test if two CoordinateConverters are the same coordinate system.
//-----------------------------------------------------------------------

  virtual bool is_same(const CoordinateConverter& Conv) const
  {
    return dynamic_cast<const PlanetocentricConverter<NAIF_CODE>*>(&Conv);
  }


//-----------------------------------------------------------------------
/// Convert to Planetocentric. X and Y are longitude and latitude in
/// degrees, and height is in meters.
//-----------------------------------------------------------------------

  virtual void convert_to_coordinate(const GroundCoordinate& Gc, double& X, 
			       double& Y, double& Height) const
  {
    Planetocentric<NAIF_CODE> gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Height = gd.height_reference_surface();
  }
  virtual void convert_to_coordinate(const Geodetic& Gc, double& X, 
			       double& Y, double& Height) const
  {
    Planetocentric<NAIF_CODE> gd(Gc);
    X = gd.longitude();
    Y = gd.latitude();
    Height = gd.height_reference_surface();
  }

//-----------------------------------------------------------------------
/// Return NAIF code.
//-----------------------------------------------------------------------
  
  static int naif_code() { return NAIF_CODE; }

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << PlanetConstant<NAIF_CODE>::name()
       << "Planetocentric Converter"; }
};

typedef PlanetConstant<499> MarsConstant;
typedef Planetocentric<499> MarsPlanetocentric;
typedef PlanetFixed<499> MarsFixed;
typedef PlanetInertial<499> MarsInertial;
typedef SimpleDemT<MarsPlanetocentric> MarsSimpleDem;
typedef PlanetocentricConverter<499> MarsPlanetocentricConverter;

typedef PlanetConstant<502> EuropaConstant;
typedef Planetocentric<502> EuropaPlanetocentric;
typedef PlanetFixed<502> EuropaFixed;
typedef PlanetInertial<502> EuropaInertial;
typedef SimpleDemT<EuropaPlanetocentric> EuropaSimpleDem;
typedef PlanetocentricConverter<502> EuropaPlanetocentricConverter;

}
#endif
