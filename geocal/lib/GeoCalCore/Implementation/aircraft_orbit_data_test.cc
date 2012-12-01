#include "unit_test_support.h"
#include "aircraft_orbit_data.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(aircraft_orbit_data, GlobalFixture)
BOOST_AUTO_TEST_CASE(aircraft_orbit_data)
{
  // Check a few simple cases to see if we are calculating body_to_ecr 
  // correctly.
  // At latitude 0, longitude 0 body z points in -x direction, body, x
  // points in +z direction, y in +y direction.

  boost::array<double, 3> vel_fixed = {{0.0, 0.0, 0.0}};
  AircraftOrbitData r(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed, 
		      0.0, 0.0, 0.0);
  typedef boost::math::quaternion<double> quat;
  ScLookVector body_x(1, 0, 0);
  ScLookVector body_y(0, 1, 0);
  ScLookVector body_z(0, 0, 1);
  CartesianFixedLookVector ecr_bx, ecr_by, ecr_bz;
  ecr_bx = r.cf_look_vector(body_x);
  ecr_by = r.cf_look_vector(body_y);
  ecr_bz = r.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,0,1)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,-1,0,0)) < 1e-8);

  // With true heading = 180, x and y should change sign
  AircraftOrbitData r2(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       0.0, 0.0, 180.0);
  ecr_bx = r2.cf_look_vector(body_x);
  ecr_by = r2.cf_look_vector(body_y);
  ecr_bz = r2.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,0,-1)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,-1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,-1,0,0)) < 1e-8);

  // Check true heading = 90
  AircraftOrbitData r3(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       0.0, 0.0, 90.0);
  ecr_bx = r3.cf_look_vector(body_x);
  ecr_by = r3.cf_look_vector(body_y);
  ecr_bz = r3.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,0,-1)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,-1,0,0)) < 1e-8);

  // Roll of 90 rotates z and y
  AircraftOrbitData r4(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       90.0, 0.0, 0.0);
  ecr_bx = r4.cf_look_vector(body_x);
  ecr_by = r4.cf_look_vector(body_y);
  ecr_bz = r4.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,0,1)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,-1,0,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,0,-1,0)) < 1e-8);

  // Pitch of 90 rotates x and z
  AircraftOrbitData r5(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       0.0, 90.0, 0.0);
  ecr_bx = r5.cf_look_vector(body_x);
  ecr_by = r5.cf_look_vector(body_y);
  ecr_bz = r5.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,1,0,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,0,0,1)) < 1e-8);
}
BOOST_AUTO_TEST_SUITE_END()
