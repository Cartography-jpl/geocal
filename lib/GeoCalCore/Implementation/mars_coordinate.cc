#include "mars_coordinate.h"
using namespace GeoCal;

// Constants for mars
template<> SpicePlanetConstant MarsConstant::h(MarsConstant::NAIF_CODE);
template<> const char* MarsConstant::name = "Mars";

boost::shared_ptr<CartesianFixed> MarsInertial::convert_to_cf
(const Time& T) const
{
  boost::shared_ptr<CartesianFixed> res(new MarsFixed);
  CartesianFixed::toolkit_coordinate_interface->
    to_fixed(NAIF_CODE, T, *this, *res);
  return res;
}

boost::shared_ptr<CartesianInertial>
MarsInertial::reference_surface_intersect_approximate
(const CartesianInertialLookVector& Cl, 
 double Height_reference_surface) const
{
  double aph = PlanetConstant<NAIF_CODE>::planet_a() + Height_reference_surface;
  double bph = PlanetConstant<NAIF_CODE>::planet_b() + Height_reference_surface;
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
  Os << PlanetConstant<NAIF_CODE>::planet_name()
     << "Inertial (" << position[0] << " m, " << position[1] << " m, "
     << position[2] << "m)";
}

