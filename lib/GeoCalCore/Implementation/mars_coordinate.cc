#include "mars_coordinate.h"
#include "geocal_matrix.h"
using namespace GeoCal;

// We probably want these in the class/template somehow, but for now
// just put these here

const double mars_a = 3396000.0;
				// Equatorial radius in meters.
const double mars_b = 3396000.0;
				// Polar radius in meters.
const double mars_esq = (mars_a * mars_a - mars_b * mars_b) / 
                         (mars_a * mars_a);
				// Eccentricity squared. From CRC.

//-----------------------------------------------------------------------
/// Create MarsFixed from GroundCoordinate
//-----------------------------------------------------------------------

MarsFixed::MarsFixed(const GroundCoordinate& Gc)
{
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

double MarsFixed::height_reference_surface() const
{
  // We use a reference sphere instead of ellipsoid. We'll need to
  // see if this needs to be generalized.
  return norm(position) - mars_a;
}

double MarsFixed::min_radius_reference_surface() const
{
  return mars_a;
}

double MarsFixed::latitude() const
{
  return atan2(position[2],
	       sqrt(position[0] * position[0] + position[1] * position[1]));
}

double MarsFixed::longitude() const
{
  return atan2(position[1], position[0]);
}

boost::shared_ptr<CartesianFixed>
MarsFixed::reference_surface_intersect_approximate
(const CartesianFixedLookVector& Cl, 
 double Height_reference_surface) const
{
  double aph = mars_a + Height_reference_surface;
  double bph = mars_b + Height_reference_surface;
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
  double aph = mars_a + Height_reference_surface;
  double bph = mars_b + Height_reference_surface;
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

