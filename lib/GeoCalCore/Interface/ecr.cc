#include "ecr.h"
#include "eci.h"
#include "constant.h"
#include "wgs84_constant.h"
#include "geodetic.h"
#include "geocal_matrix.h"
#include <cmath>
using namespace GeoCal;

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
//-----------------------------------------------------------------------
// This is the equatorial radius of and eccentricity squared of the
// WGS84 ellipsoid.
//-----------------------------------------------------------------------

  const double a = Constant::wgs84_a;
  const double esq = Constant::wgs84_esq;

//-----------------------------------------------------------------------
// This is the tolerance for the calculation of Q. This gives a
// accuracy of about 1mm.
//-----------------------------------------------------------------------

  const double q_tol = 1e-9;

//-----------------------------------------------------------------------
// We bypass the constructor of Geodetic, to avoid doing the range
// checking (which is unnecessary for us and is pretty time
// consuming. 
//-----------------------------------------------------------------------

  Geodetic res;			
  double rho = sqrt(sqr(position[0]) +
		    sqr(position[1]));
  if(rho > 0.0)
    res.lon_ = atan2(position[1], position[0]) * Constant::rad_to_deg;
  else
    res.lon_ = 0;
  double normrho = rho / a;
  double normz = position[2] / a;
  double signn = ((sqr(normrho) + sqr(normz) / (1 - esq)) 
		  > 1 ? 1 : -1);
  double q  = 0.0;
  double qold;
  int n = 0;
  double t1 = sqr(normz) * (1 - esq);
  do {
    qold = q;
    q = esq  * (normrho - qold) / sqrt(sqr(normrho - qold) + t1);
  } while (fabs(qold  - q) > q_tol &&
 	   ++n < 10);
  if(n ==10)
    throw Exception("Number of iterations exceeded");
  res.lat_ = atan2(normz, normrho - q) * Constant::rad_to_deg;
  double t3 = normrho - q;
  double r = sqrt(sqr(normz) + sqr(t3));
  double coslat = t3 / r;
  double sinlat = normz / r;
  double t = 1.0 / sqrt(1 - esq * sinlat * sinlat);
  double normrhofoot = coslat * t;
  double normzfoot = (1 - esq) * sinlat * t;
  res.height_ellipsoid_ = signn * 
    sqrt(sqr(normrhofoot - normrho) + sqr(normzfoot - normz)) * a;
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
