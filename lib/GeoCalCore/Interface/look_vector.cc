#include "look_vector.h"
#include "geocal_exception.h"
#include "ground_coordinate.h"
#include "ecr.h"
#include "constant.h"
#include <cmath>

using namespace GeoCal;


//-----------------------------------------------------------------------
/// This is the direction, as a unit vector.
//-----------------------------------------------------------------------

boost::array<double, 3> LookVector::direction() const 
{
  boost::array<double, 3> res;
  double l = length();
  res[0] = look_vector[0] / l;
  res[1] = look_vector[1] / l;
  res[2] = look_vector[2] / l;
  return res;
}


//-----------------------------------------------------------------------
/// Length of look vector, in meters.
//-----------------------------------------------------------------------

inline double sqr(double x) {return x * x;}
double LookVector::length() const
{
  return sqrt(sqr(look_vector[0]) + sqr(look_vector[1]) + sqr(look_vector[2]));
}

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
/// Return the solar look vector, which points from the given
/// reference point to the sun in the local north coordinates. Can
/// calculate solar zenith and azimuth from this.
//-----------------------------------------------------------------------

LnLookVector LnLookVector::solar_look_vector
(const Time& T, 
 const GroundCoordinate& Ref_pt)
{
  CartesianFixedLookVector lv(Ecr::sub_solar_point(T).position);
  return LnLookVector(lv, Ref_pt);
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

