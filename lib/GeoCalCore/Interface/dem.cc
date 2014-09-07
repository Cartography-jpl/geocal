#include "dem.h"
#include "geocal_matrix.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Determine the intersection along a look vector from a starting
/// position with the Dem (e.g., what point on the ground does a
/// satellite see).
///
/// This is brute force, so it won't mistakenly pick a plain behind a
/// mountain.
///
/// Resolution is the desired resolution in meters. Max_height is the
/// maximum height we will encounter in the Dem - the default of 9 km
/// is higher than the highest point in the world.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> Dem::intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height) const
{
  // Find the starting length for the ray.
  boost::shared_ptr<CartesianFixed> cf2 = 
    Cf.reference_surface_intersect_approximate(Lv, Max_height);
  boost::array<double, 3> t(cf2->position);
  for(int i = 0; i < 3; ++i)
    t[i] -= Cf.position[i];
  double start_length = norm(t);

  // Determine how far a unit move in Lv direction step along the earth.
  t = cf2->position;
  for(int i = 0; i < 3; ++i)
    t[i] += Lv.direction()[i];
  double d = distance(*cf2,*cf2->create(t));
  double step_size = Resolution / d;

  double hcurr = distance_to_surface(*cf2);
  double hlast;
  double lcurr = start_length;
  double llast;
  int sign = (hcurr > 0 ? 1 : -1);
  do {
    llast = lcurr;
    lcurr += sign * step_size;
    hlast = hcurr;
    for(int i = 0; i < 3; ++i)
      cf2->position[i] += sign * step_size * Lv.direction()[i];
    hcurr = distance_to_surface(*cf2);
  } while (sign * hcurr > 0);
 
  double step_inter = (llast - lcurr) / (hlast - hcurr) * (0 - hcurr);
  for(int i = 0; i < 3; ++i)
    cf2->position[i] += step_inter * Lv.direction()[i];
  return cf2;
}
