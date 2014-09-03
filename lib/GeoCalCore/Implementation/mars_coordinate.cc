#include "mars_coordinate.h"
#include "geocal_matrix.h"
using namespace GeoCal;

// We probably want these in the class/template somehow, but for now
// just put these here

inline double sqr(double x) { return x * x; }

bool MarsConstant::filled_in = false;
double MarsConstant::a;
double MarsConstant::b;
double MarsConstant::esq;

void MarsConstant::calc_data()
{
  MarsConstant::a = 3396000.0;
  MarsConstant::b = 3396000.0;
  MarsConstant::esq = (sqr(MarsConstant::a) - sqr(MarsConstant::b)) / 
    sqr(MarsConstant::a);
				// Eccentricity squared. From CRC.
}

//-----------------------------------------------------------------------
/// Create MarsFixed from GroundCoordinate
//-----------------------------------------------------------------------

MarsFixed::MarsFixed(const GroundCoordinate& Gc)
{
  if(const MarsFixed* g = 
     dynamic_cast<const MarsFixed*>(&Gc)) {
    *this = *g;
    return;
  }
  boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
  boost::shared_ptr<MarsFixed> mf = 
    boost::dynamic_pointer_cast<MarsFixed>(cf);
  if(!mf) {
    Exception e;
    e << "Cannot convert ground coordinate to MarsFixed\n"
      << "Coordinate: " << Gc << "\n";
    throw e;
  }
  position = mf->position;
}

boost::shared_ptr<CartesianInertial> 
MarsFixed::convert_to_ci(const Time& T) const
{
  boost::shared_ptr<CartesianInertial> res(new MarsInertial);
  CartesianFixed::toolkit_coordinate_interface->
    to_inertial((int) MARS_NAIF_CODE, T, *this, *res);
  return res;
}

//-----------------------------------------------------------------------
/// Height above the reference ellipsoid.
//-----------------------------------------------------------------------

double MarsFixed::height_reference_surface() const
{
  return MarsPlanetocentric(*this).height_reference_surface();
}

double MarsFixed::min_radius_reference_surface() const
{
  return std::min(MarsConstant::planet_a(), MarsConstant::planet_b());
}

//-----------------------------------------------------------------------
/// Return latitude in degrees. This is the Planetocentric. This is 
/// the planet equivalent of Geocentric, so relative to the center
/// (*not* normal to ellipsoid like Geodetic).
//-----------------------------------------------------------------------

double MarsFixed::latitude() const
{
  return atan2(position[2],
	       sqrt(position[0] * position[0] + position[1] * position[1]))
    * Constant::rad_to_deg;
}

//-----------------------------------------------------------------------
/// Return longitude in degrees. This is the Planetocentric (which
/// actually isn't any different than Planetographic for longitude).
//-----------------------------------------------------------------------

double MarsFixed::longitude() const
{
  return atan2(position[1], position[0]) * Constant::rad_to_deg;
}

boost::shared_ptr<CartesianFixed>
MarsFixed::reference_surface_intersect_approximate
(const CartesianFixedLookVector& Cl, 
 double Height_reference_surface) const
{
  double aph = MarsConstant::planet_a() + Height_reference_surface;
  double bph = MarsConstant::planet_b() + Height_reference_surface;
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

MarsPlanetocentric MarsFixed::convert_to_planetocentric() const
{
  return MarsPlanetocentric(latitude(), longitude(), 
			    height_reference_surface());
}

void MarsFixed::print(std::ostream& Os) const
{
  Os << "MarsFixed (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << "m)";
}

boost::shared_ptr<CartesianFixed> MarsInertial::convert_to_cf
(const Time& T) const
{
  boost::shared_ptr<CartesianFixed> res(new MarsFixed);
  CartesianFixed::toolkit_coordinate_interface->
    to_fixed(MARS_NAIF_CODE, T, *this, *res);
  return res;
}

boost::shared_ptr<CartesianInertial>
MarsInertial::reference_surface_intersect_approximate
(const CartesianInertialLookVector& Cl, 
 double Height_reference_surface) const
{
  double aph = MarsConstant::planet_a() + Height_reference_surface;
  double bph = MarsConstant::planet_b() + Height_reference_surface;
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

void MarsInertial::print(std::ostream& Os) const
{
  Os << "MarsIntertial (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << "m)";
}

//-----------------------------------------------------------------------
/// Convert from GroundCoor.
//-----------------------------------------------------------------------

MarsPlanetocentric::MarsPlanetocentric(const GroundCoordinate& Gc)
{
  if(const MarsPlanetocentric* g = 
     dynamic_cast<const MarsPlanetocentric*>(&Gc)) {
    *this = *g;
    return;
  }
  MarsFixed mf(Gc);
  lon_ = mf.longitude();
  lat_ = mf.latitude();
  height_ellipsoid_ = 
    norm(mf.position) - planet_radius(lat_ * Constant::deg_to_rad);
}

boost::shared_ptr<CartesianFixed> MarsPlanetocentric::convert_to_cf() const
{
  double lat = lat_ * Constant::deg_to_rad;
  double lon = lon_ * Constant::deg_to_rad;
  double r = planet_radius(lat) + height_ellipsoid_;
  return boost::shared_ptr<CartesianFixed>
    (new MarsFixed(r * cos(lat) * cos(lon),
		   r * cos(lat) * sin(lon),
		   r * sin(lat)));
}

void MarsPlanetocentric::print(std::ostream& Os) const
{
  Os << "MarsPlanetocentric: (" << latitude() << " deg, " 
     << longitude() << " deg, "
     << height_reference_surface() << " m)";
}

//-----------------------------------------------------------------------
/// Radius of planet in meters at given Planetocentric Latitude (in
/// radians, since we've already converted.
//-----------------------------------------------------------------------

double MarsPlanetocentric::planet_radius(double Latitude_radians)const
{
  return MarsConstant::planet_b() / 
    sqrt(1 - MarsConstant::planet_esq() * sqr(cos(Latitude_radians)));
}
