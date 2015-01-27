#include "eci.h"
#include "ecr.h"
#include "wgs84_constant.h"
#include "geocal_matrix.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Eci::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianInertial);
}

GEOCAL_IMPLEMENT(Eci);
#endif

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> Eci::convert_to_cf(const Time& T) const
{
  boost::shared_ptr<CartesianFixed> res(new Ecr);
  CartesianFixed::toolkit_coordinate_interface->
    to_fixed(EARTH_NAIF_CODE, T, *this, *res);
  return res;
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void Eci::print(std::ostream& Os) const
{
  Os << "Eci: (" << position[0] << " m, " << position[1] << " m, "
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
Eci::reference_surface_intersect_approximate(
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
