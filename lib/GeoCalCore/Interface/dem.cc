#include "dem.h"
#include "geocal_matrix.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void Dem::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
}

GEOCAL_IMPLEMENT(Dem);
#endif

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
  return intersect_start_length(Cf, Lv, Resolution, start_length);
}

//-----------------------------------------------------------------------
/// Determine the intersection along a look vector from a starting
/// position with the Dem (e.g., what point on the ground does a
/// satellite see).
///
/// Resolution is the desired resolution in meters. 
///
/// The starting length along the look vector is given. This might
/// come from ray casting code, or it might come from the intersect
/// function of this class.
//-----------------------------------------------------------------------

boost::shared_ptr<CartesianFixed> 
Dem::intersect_start_length 
(const CartesianFixed& Cf,
 const CartesianFixedLookVector& Lv, 
 double Resolution, double Start_length) const
{
  boost::array<double, 3> ldir(Lv.direction());
  boost::shared_ptr<CartesianFixed> cf2 = Cf.create(Cf.position);
  for(int i = 0; i < 3; ++i)
    cf2->position[i] += Start_length * ldir[i];
  // Determine how far a unit move in Lv direction step along the earth.
  boost::array<double, 3> t = cf2->position;
  for(int i = 0; i < 3; ++i)
    t[i] += ldir[i];
  double d = distance(*cf2,*cf2->create(t));
  double step_size = Resolution / d;

  double hcurr = distance_to_surface(*cf2);
  double hlast;
  double lcurr = Start_length;
  double llast;
  int sign = (hcurr > 0 ? 1 : -1);
  int count = 0;
  double  hlast_at_count = hcurr;
  do {
    // Attempt to tell if we are not going to find a solution. This
    // is not perfect, but we assume that if the height doesn't reduce
    // after 100000 steps we aren't going to converge.
    count++;
    if(count > 100000) {
      if(fabs(hcurr) > fabs(hlast_at_count))
	throw ConvergenceFailure("Dem::intersect_start_length failed to converge. Often this is due to invalid data (e.g., look vector doesn't intersect the surface)");
      hlast_at_count = hcurr;
      count = 0;
    }
    llast = lcurr;
    lcurr += sign * step_size;
    hlast = hcurr;
    for(int i = 0; i < 3; ++i)
      cf2->position[i] += sign * step_size * ldir[i];
    hcurr = distance_to_surface(*cf2);
  } while (sign * hcurr > 0);
 
  double step_inter = (llast - lcurr) / (hlast - hcurr) * (0 - hcurr);
  for(int i = 0; i < 3; ++i)
    cf2->position[i] += step_inter * ldir[i];
  return cf2;

}
