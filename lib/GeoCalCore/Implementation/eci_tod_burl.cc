#include "eci_tod_burl.h"
#include "ecr.h"
#include "wgs84_constant.h"
#include "geocal_matrix.h"
#include "geocal_serialize_support.h"
#include "geocal_config.h"
#ifdef HAVE_CARTO
extern "C" {
#include "carto/eos_coords.h"
}
#endif

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void EciTodBurl::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(CartesianInertial);
}

GEOCAL_IMPLEMENT(EciTodBurl);
#endif

double EciTodBurl::delta_ut1 = 0;

//-----------------------------------------------------------------------
/// Convert to CartesianFixed.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> EciTodBurl::convert_to_cf(const Time& T) const
{
  boost::shared_ptr<CartesianFixed> res(new Ecr);
  double ci_to_cf_m[3][3];
  ci_to_cf(T, ci_to_cf_m);
  mul(ci_to_cf_m, position, res->position);
  return res;
}

//-----------------------------------------------------------------------
/// Matrix to convert EciTodBurl to Ecr. The transpose of this will convert
/// Ecr to EciTodBurl.
//-----------------------------------------------------------------------

void EciTodBurl::ci_to_cf(const Time& T, double Ci_to_cf[3][3]) const
{
#ifdef HAVE_CARTO
  double ci_to_j2000[9];
  eos_coords(T.et(), T.acs() + delta_ut1, ci_to_j2000, &Ci_to_cf[0][0]);
#else
  throw Exception("Need to compile with carto library to use EciTodBurl");
#endif    
}

//-----------------------------------------------------------------------
/// Matrix to convert EciTodBurl to Ecr. The transpose of this will convert
/// Ecr to EciTodBurl.
//-----------------------------------------------------------------------

void EciTodBurl::ci_to_cf_with_vel(const Time& T, double Ci_to_cf[6][6]) const
{ 
  // Mike burl doesn't actually implement this. But we can get the
  // motion of the earth from spice, and then use the rotation matrix
  // from Mike Burl.
  for(int i = 0; i < 6; ++i)
    for(int j = 0; j < 6; ++j)
      Ci_to_cf[i][j] = 0;
  double m[3][3];
  ci_to_cf(T, m);
  for(int i = 0; i < 3; ++i)
    for(int j = 0; j < 3; ++j) {
      Ci_to_cf[i][j] = m[i][j];
      Ci_to_cf[i+3][j+3] = m[i][j];
    }
  Eci e(1,2,3);
  double m2[6][6];
  e.ci_to_cf_with_vel(T, m2);
  for(int i = 3; i < 6; ++i)
    for(int j = 0; j < 3; ++j)
      Ci_to_cf[i][j] = m2[i][j];
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void EciTodBurl::print(std::ostream& Os) const
{
  Os << "EciTodBurl: (" << position[0] << " m, " << position[1] << " m, "
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
EciTodBurl::reference_surface_intersect_approximate(
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
