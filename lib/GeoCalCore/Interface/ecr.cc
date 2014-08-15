#include "ecr.h"
#include "eci.h"
#include "constant.h"
#include "wgs84_constant.h"
#include "geodetic.h"
#include "geocal_matrix.h"
#include <cmath>
using namespace GeoCal;

//-----------------------------------------------------------------------
/// This code is from the paper D. K. Olson "Converting Earth-Centered,
/// Earth-Fixed Coordinates to Geodetic Coordinates.", IEEE
/// Transactions on Aerospace and Electronic Systems, 32 (1996)
/// 473-476. 
///
/// The claim of the paper is that this runs faster than all the other
/// methods he compared to. We have cut and pasted the code from that
/// paper here, and then slightly adapted this.
//-----------------------------------------------------------------------

void latlon(double x, double y, double z, double& lat, double& lon, double& ht)
{
  const double a = Constant::wgs84_a;
  const double e2 = Constant::wgs84_esq;
  const double a1 = a * e2;
  const double a2 = a1 * a1;
  const double a3 = a1 * e2 / 2;
  const double a4 = 2.5 * a2;
  const double a5 = a1 + a3;
  const double a6 = 1 - e2;
  double zp = fabs(z);
  double w2 = x * x + y * y;
  double w = sqrt(w2);
  double z2 = z * z;
  double r2 = w2 + z2;
  double r = sqrt(r2);
  if(r < 1e5) {
    lat = 0;
    lon = 0;
    ht = -1e7;
    return;
  }
  lon = atan2(y, x);
  double s2 = z2 / r2;
  double c2 = w2 / r2;
  double u = a2 / r;
  double v = a3 - a4 / r;
  double s, ss, c;
  if(c2 > 0.3) {
    s = (zp / r) * (1 + c2 * (a1 + u + s2 * v) / r);
    lat = asin(s);
    ss = s * s;
    c = sqrt(1 - ss);
  } else {
    c = (w / r) * (1 - s2 * (a5 - u - c2 * v) / r);
    lat = acos(c);
    ss = 1 - c * c;
    s = sqrt(ss);
  }
  double g = 1 - e2 * ss;
  double rg = a / sqrt(g);
  double rf = a6 * rg;
  u = w - rg * c;
  v = zp - rf * s;
  double f = c * u + s * v;
  double m = c * v - s * u;
  double p = m / (rf / g + f);
  lat = lat + p;
  ht = f + m * p / 2;
  if(z < 0)
    lat = -lat;
}

//-----------------------------------------------------------------------
/// Convert from GroundCoor.
//-----------------------------------------------------------------------

Ecr::Ecr(const GroundCoordinate& Gc)
{
  boost::shared_ptr<CartesianFixed> cf = Gc.convert_to_cf();
  Ecr* ecr = dynamic_cast<Ecr*>(cf.get());
  if(!ecr)
    throw Exception("Ground coordinate cannot be converted to an Ecr");
  position = ecr->position;
}

inline double sqr(double x) {return x * x;}

//-----------------------------------------------------------------------
/// Convert to CartesianInertial.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianInertial> Ecr::convert_to_ci(const Time& T) const
{ 
  boost::shared_ptr<CartesianInertial> res(new Eci);
  CartesianFixed::toolkit_coordinate_interface->
    to_inertial((int) EARTH_NAIF_CODE, T, *this, *res);
  return res;
}

//-----------------------------------------------------------------------
/// Convert to Geodetic coordinates.
//-----------------------------------------------------------------------

Geodetic Ecr::convert_to_geodetic() const
{
  Geodetic res;
  latlon(position[0], position[1], position[2], res.lat_, res.lon_,
	 res.height_ellipsoid_);
  res.lat_ *= Constant::rad_to_deg;
  res.lon_ *= Constant::rad_to_deg;
  return res;
}

//-----------------------------------------------------------------------
/// Return height above/below the reference surface (e.g., WGS-84
/// for the earth). Positive means above, negative below. This is 
/// in meters.
//-----------------------------------------------------------------------

double Ecr::height_reference_surface() const
{
  return convert_to_geodetic().height_reference_surface();
}

//-----------------------------------------------------------------------
/// This is the radius of the largest sphere that fits completely
/// into the reference surface (e.g., radius for a reference
/// sphere, minimum of a and b for a ellipsoid).
//-----------------------------------------------------------------------

double Ecr::min_radius_reference_surface() const
{
  return Constant::wgs84_a;
}
  
//-----------------------------------------------------------------------
/// Return latitude in degrees. By default we just convert to 
/// CartesianFixed and then to latitude, but derived classes can supply
/// more efficient versions of these if needed. Latitude is -90 to 90.
//-----------------------------------------------------------------------

double Ecr::latitude() const
{
  return convert_to_geodetic().latitude();
}

//-----------------------------------------------------------------------
/// Return longitude in degrees. By default we just convert to 
/// CartesianFixed and then to longitude, but derived classes can supply
/// more efficient versions of these if needed. Longitude is -180 to 180.
//-----------------------------------------------------------------------

double Ecr::longitude() const
{
  return convert_to_geodetic().longitude();
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void Ecr::print(std::ostream& Os) const
{
  Os << "Ecr: (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << " m)";
}

//-----------------------------------------------------------------------
/// Find the intersection with the surface at the given height
/// starting at this point and looking in the given direction. The
/// intersection is done in an approximate but quicker manner.
///
/// We find the intersection with the ellipsoid that has the given
/// height added to the equatorial and polar axis. This is not the
/// same as the figure with a height h above the ellipsoid (which
/// isn't an ellipsoid), but for many purposes this is close enough.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed>
Ecr::reference_surface_intersect_approximate(
  const CartesianFixedLookVector& Cl, double Height_reference_surface) 
  const
{
  double aph = Constant::wgs84_a + Height_reference_surface;
  double bph = Constant::wgs84_b + Height_reference_surface;
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
/// Return the subsolar point for the given time. This is the point on
/// the surface of the reference ellipsoid that lies on the line
/// from the center of the earth to the sun.
//-----------------------------------------------------------------------

Ecr Ecr::sub_solar_point(const Time& T)
{
  Ecr res;
  CartesianFixed::toolkit_coordinate_interface->
    sub_solar_point((int) EARTH_NAIF_CODE, T, res);
  return res;
}

//-----------------------------------------------------------------------
/// Solar distance at given time. This is AU (which is defined to be
/// exactly 149597870700 meter).
//-----------------------------------------------------------------------

double Ecr::solar_distance(const Time& T)
{
  const double one_au = 149597870700;
  return CartesianFixed::toolkit_coordinate_interface->
    solar_distance((int) EARTH_NAIF_CODE, T) / one_au;
}
