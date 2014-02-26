#include "eci_tod.h"
#include "ecr.h"
#include "wgs84_constant.h"
#include "geocal_matrix.h"
#include "spice_helper.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
// Convert from Eci to EciTod.
//-----------------------------------------------------------------------

EciTod::EciTod(const Eci& Eci_coor, const Time& T)
{
  SpiceHelper::conversion("J2000", "ECI_TOD", T, Eci_coor.position, position);
}

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> EciTod::convert_to_cf(const Time& T) const
{
  boost::shared_ptr<CartesianFixed> res(new Ecr);
  SpiceHelper::conversion("ECI_TOD", "ITRF93", T, position, res->position);
  return res;
}

//-----------------------------------------------------------------------
/// Matrix to convert EciTod to Ecr. The transpose of this will convert
/// Ecr to EciTod.
//-----------------------------------------------------------------------

void EciTod::ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
{ 
  SpiceHelper::conversion_matrix("ECI_TOD", "ITRF93", T);
  mat_copy(SpiceHelper::m, Ci_to_cf);
}

//-----------------------------------------------------------------------
// Convert from EciTod to Eci.
//-----------------------------------------------------------------------

Eci EciTod::to_eci(const Time& T)
{
  Eci res;
  SpiceHelper::conversion("ECI_TOD", "J2000", T, position, res.position);
  return res;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void EciTod::print(std::ostream& Os) const
{
  Os << "EciTod: (" << position[0] << " m, " << position[1] << " m, "
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

boost::shared_ptr<CartesianInertial>
EciTod::reference_surface_intersect_approximate(
  const CartesianInertialLookVector& Cl, double Height_reference_surface) 
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
