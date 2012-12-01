#include "simple_dem.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Return distance to surface directly above/below the given point.
/// Distance is in meters. Positive means Gp is above the surface, 
/// negative means below.
//-----------------------------------------------------------------------

double SimpleDem::distance_to_surface(const GroundCoordinate& Gp) const
{ 
  return Gp.height_reference_surface() - h_; 
}

//-----------------------------------------------------------------------
/// Return a GroundCoordinate on the surface directly above or below
/// the given point.
//-----------------------------------------------------------------------

boost::shared_ptr<GroundCoordinate> 
SimpleDem::surface_point(const GroundCoordinate& Gp) const
{
  Geodetic g(Gp);
  return boost::shared_ptr<GroundCoordinate>(
     new Geodetic(g.latitude(), g.longitude(), h_));
}

