#include "aircraft_orbit_data.h"

using namespace GeoCal;

//-----------------------------------------------------------------------
/// Often with Aircraft data we don't have a direct measure of
/// velocity. This calculates a simple velocity by taking a second
/// position and a different time and assuming a constant velocity
/// between the values.
//-----------------------------------------------------------------------

AircraftOrbitData::AircraftOrbitData(const Time& Tm,
		    const GroundCoordinate& Position, 
		    const Time& Tm2,
		    const GroundCoordinate& Position2, 
		    double Roll, double Pitch,
				     double Heading, VerticalDefinition V)
{
  boost::array<double, 3> p1 = Position.convert_to_cf()->position;
  boost::array<double, 3> p2 = Position2.convert_to_cf()->position;
  boost::array<double, 3> vel;
  for(int i = 0; i < 3; ++i)
    vel[i] = (p2[i] - p1[i]) / (Tm2 - Tm);
  initialize(Tm, Position, vel, Roll, Pitch, Heading, V);
}

//-----------------------------------------------------------------------
/// Initialize data
//-----------------------------------------------------------------------

void AircraftOrbitData::initialize(const Time& Tm, 
				   const Geodetic& Position, 
				   const boost::array<double, 3>& Vel_fixed,
				   double Roll, double Pitch,
				   double Heading, VerticalDefinition V)
{
  roll_ = Roll;
  pitch_ = Pitch;
  heading_ = Heading;
  position_geodetic_ = Position;
  vertical_definition_ = V;

//-----------------------------------------------------------------------
// The transformations done in this function are documented in
// "AirMISR Navigation Data", MISR SDS DFM-0288-L. AirMISR isn't
// identical to Applanix, but they are pretty similar with navigation data.
//-----------------------------------------------------------------------

//-----------------------------------------------------------------------
// Determine aircraft coordinates z axis, which is in vertical direction,
// pointed downward. Also, determine S/N and E/W direction, This
// defines the local north coordinate system, which is X in S-N
// direction, Y in W-E direction, and Z pointed downward.
//
// The vertical can be defined one of three ways for the aircraft
// coordinate system:
//
//   1. Geocentric direction (e.g, to center of earth).
//   2. Geodetic direction (e.g., normal to surface of earth).
//   3. Local vertical (direction of gravity).
//
// I'm pretty sure 2 is the correct definition for Applanix. It is what
// is used by ossim library, and seems to match the actually data.
// 2 and 3 are pretty close together, but are different than 1.
//
//
// For geodetic, these coordinates are determined in ECR coordinate
// system by taking the derivative of the Geodetic to ECR equations
// (See "Theoretical Basis of the SDP toolkit Geolocation Package for
// the ECS Project", among other sources).
// 
// We have:
//
// Ecr_x = (NC + h) cos(latitude) cos(longitude)
// Ecr_y = (NC + h) cos(latitude) sin(longitude)
// Ecr_z = (NC * (1 - eccentricity^2) + h) sin(latitude)
// 
// where NC is radius of curvature in E-W ellipsoid. This gives a z
// vector by -d Ecr/dh ("-", because we define z to be downward
// pointing), which gives z = -(cos(latitude) cos(longitude), 
// cos(latitude) sin(longitude), sin(latitude)). y direction is
// (-sin(longitude), cos(longitude), 0). x is cross(y, z).
//
// For geocentric, we actually have exactly the same resulting
// equations, except the latitude and longitude are the geocentric
// latitude and longitude.
//
// Right now we hard code the Geodetic vertical choice. If we
// encounter a Geocentric case, we can generalize this by passing this
// in as an argument.
//-----------------------------------------------------------------------

  double latitude, longitude;
  if(vertical_definition_ == GEODETIC_VERTICAL) {
    latitude = position_geodetic_.latitude();
    longitude = position_geodetic_.longitude();
  } else {
    Geocentric pos_geoc(Position);
    latitude = pos_geoc.latitude();
    longitude = pos_geoc.longitude();
  }
  latitude *= Constant::deg_to_rad;
  longitude *= Constant::deg_to_rad;
  double sinlat = sin(latitude);
  double coslat = cos(latitude);
  double sinlon = sin(longitude);
  double coslon = cos(longitude);
  double m[3][3];
  // z_ecr
  m[0][2] = -coslat * coslon;
  m[1][2] = -coslat * sinlon;
  m[2][2] = -sinlat;
  // y_ecr
  m[0][1] = -sinlon;
  m[1][1] = coslon;
  m[2][1] = 0;
  // x_ecr = y_ecr x z_ecr
  m[0][0] = m[1][1] * m[2][2] - m[2][1] * m[1][2];
  m[1][0] = m[2][1] * m[0][2] - m[0][1] * m[2][2];
  m[2][0] = m[0][1] * m[1][2] - m[1][1] * m[0][2];
  boost::math::quaternion<double> local_north_to_ecr = matrix_to_quaternion(m);

// Pretty sure about the order here, this seems to be the standard
// order used by aircrafts.

  boost::math::quaternion<double> body_to_local_north = 
    quat_rot("ZYX", Heading * Constant::deg_to_rad, 
	     Pitch * Constant::deg_to_rad, Roll * Constant::deg_to_rad);
  boost::math::quaternion<double> body_to_ecr;
  body_to_ecr = local_north_to_ecr * body_to_local_north;
  
  QuaternionOrbitData::initialize(Tm, Position.convert_to_cf(), 
				  Vel_fixed, body_to_ecr);
}

//-----------------------------------------------------------------------
/// Print to stream. 
//-----------------------------------------------------------------------

void AircraftOrbitData::print(std::ostream& Os) const
{
  Os << "AircraftOrbitData:\n"
     << "  Time:     " << time() << "\n"
     << "  Position: " << position_geodetic() << "\n"
     << "  Roll:     " << roll() << " deg\n"
     << "  Pitch:    " << pitch() << " deg\n"
     << "  Heading:  " << heading() << " deg\n";
}
