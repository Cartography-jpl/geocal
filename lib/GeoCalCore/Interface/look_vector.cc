#include "look_vector.h"
#include "geocal_exception.h"
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
