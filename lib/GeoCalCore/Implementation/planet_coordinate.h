#ifndef PLANET_COORDINATE_H
#define PLANET_COORDINATE_H
#include "ground_coordinate.h"
#include "geocal_matrix.h"
#include "spice_helper.h"

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
  static double planet_a() {return h.planet_a();}
  static double planet_b() {return h.planet_b(); }
  static double planet_esq() {return h.planet_esq(); }
  static std::string planet_name() { return name;}
private:
  static SpicePlanetConstant h;
  static const char* name;
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
	<< PlanetConstant<NAIF_CODE>::planet_name()<< "Fixed\n"
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
/// Height above the reference ellipsoid.
//-----------------------------------------------------------------------
  virtual double height_reference_surface() const;
  virtual double min_radius_reference_surface() const
  {
    return std::min(PlanetConstant<NAIF_CODE>::planet_a(), 
		    PlanetConstant<NAIF_CODE>::planet_b());
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
    double aph = PlanetConstant<NAIF_CODE>::planet_a() + 
      Height_reference_surface;
    double bph = PlanetConstant<NAIF_CODE>::planet_b() + 
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

  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::planet_name()
       << "Fixed (" << position[0] << " m, " << position[1] << " m, "
       << position[2] << "m)";
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
    double aph = PlanetConstant<NAIF_CODE>::planet_a() + 
      Height_reference_surface;
    double bph = PlanetConstant<NAIF_CODE>::planet_b() + 
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
/// Print to given stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  {
    Os << PlanetConstant<NAIF_CODE>::planet_name()
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

typedef PlanetConstant<499> MarsConstant;
typedef Planetocentric<499> MarsPlanetocentric;
typedef PlanetFixed<499> MarsFixed;
typedef PlanetInertial<499> MarsInertial;

typedef PlanetConstant<502> EuropaConstant;
typedef Planetocentric<502> EuropaPlanetocentric;
typedef PlanetFixed<502> EuropaFixed;
typedef PlanetInertial<502> EuropaInertial;

}
#endif
