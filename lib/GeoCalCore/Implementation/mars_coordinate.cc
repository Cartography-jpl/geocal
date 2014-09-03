#include "mars_coordinate.h"
using namespace GeoCal;

//-----------------------------------------------------------------------
/// Create MarsFixed from GroundCoordinate
//-----------------------------------------------------------------------

MarsFixed::MarsFixed(const GroundCoordinate& Gc)
{
}

boost::shared_ptr<CartesianInertial> 
MarsFixed::convert_to_ci(const Time& T) const
{
}

double MarsFixed::height_reference_surface() const
{
}

double MarsFixed::min_radius_reference_surface() const
{
}

double MarsFixed::latitude() const
{
}

double MarsFixed::longitude() const
{
}

boost::shared_ptr<CartesianFixed>
MarsFixed::reference_surface_intersect_approximate
(const CartesianFixedLookVector& Cl, 
 double Height_reference_surface) const
{
}

void MarsFixed::print(std::ostream& Os) const
{
}

