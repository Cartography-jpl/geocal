#include "look_vector.h"
#include "geocal_exception.h"
#include "ground_coordinate.h"
#include "ecr.h"
#include "constant.h"
#include <cmath>

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ScLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in spacecraft coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void ScLookVectorWithDerivative::print(std::ostream& Os) const
{
  boost::array<AutoDerivative<double>, 3> d = direction();
  Os << "Look vector in spacecraft coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianInertialLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Cartesian inertial coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void CartesianFixedLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Cartesian fixed coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void LnLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in local north coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

//-----------------------------------------------------------------------
/// Constructor going from a GroundCoordinate to a GroundCoordinate.
//-----------------------------------------------------------------------

CartesianFixedLookVector::CartesianFixedLookVector
(const GroundCoordinate& From,
 const GroundCoordinate& To)
{
  boost::shared_ptr<CartesianFixed> from_cf = From.convert_to_cf();
  boost::shared_ptr<CartesianFixed> to_cf = To.convert_to_cf();
  for(int i = 0; i < 3; ++i)
    look_vector[i] = to_cf->position[i] - from_cf->position[i];
}

//-----------------------------------------------------------------------
/// Return quaternion to go from ENU coordinates to CartesianFixed for
/// the given location.
//-----------------------------------------------------------------------

boost::math::quaternion<double> 
LnLookVector::enu_to_cf(const GroundCoordinate& Ref_pt)
{
  // This matrix is available from a variety of sources. See for
  // example the GroundMSPI L1B2 ATB.
  return quat_rot("zx", (Ref_pt.longitude() - 270) * Constant::deg_to_rad, 
		  (90 - Ref_pt.latitude()) * Constant::deg_to_rad);
}


//-----------------------------------------------------------------------
/// Return the solar look vector, which points from the center of the
/// the earth to the sun.
//-----------------------------------------------------------------------

CartesianFixedLookVector CartesianFixedLookVector::solar_look_vector
(const Time& T)
{
  return CartesianFixedLookVector(Ecr::sub_solar_point(T).position);
}

//-----------------------------------------------------------------------
/// Return the solar look vector, which points from the given
/// reference point to the sun in the local north coordinates. Can
/// calculate solar zenith and azimuth from this.
//-----------------------------------------------------------------------

LnLookVector LnLookVector::solar_look_vector
(const Time& T, 
 const GroundCoordinate& Ref_pt)
{
  return LnLookVector(CartesianFixedLookVector::solar_look_vector(T), Ref_pt);
}

//-----------------------------------------------------------------------
/// Print to given stream.
//-----------------------------------------------------------------------

void DcsLookVector::print(std::ostream& Os) const
{
  boost::array<double, 3> d = direction();
  Os << "Look vector in Detector Coordinate System coordinates: \n"
     << "  dir:    (" << d[0] << ", " << d[1] << ", " << d[2] << ")\n"
     << "  length: " << length() << " m\n";
}

