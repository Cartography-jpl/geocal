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
  // Turn off small velocity aberration correction. We are doing our
  // reasoning here ignoring this
  r.aberration_correction(QuaternionOrbitData::NO_CORRECTION);
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
  // Turn off small velocity aberration correction. We are doing our
  // reasoning here ignoring this
  r2.aberration_correction(QuaternionOrbitData::NO_CORRECTION);
  ecr_bx = r2.cf_look_vector(body_x);
  ecr_by = r2.cf_look_vector(body_y);
  ecr_bz = r2.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,0,-1)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,-1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,-1,0,0)) < 1e-8);

  // Check true heading = 90
  AircraftOrbitData r3(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       0.0, 0.0, 90.0);
  r3.aberration_correction(QuaternionOrbitData::NO_CORRECTION);
  ecr_bx = r3.cf_look_vector(body_x);
  ecr_by = r3.cf_look_vector(body_y);
  ecr_bz = r3.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,0,-1)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,-1,0,0)) < 1e-8);

  // Roll of 90 rotates z and y
  AircraftOrbitData r4(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       90.0, 0.0, 0.0);
  r4.aberration_correction(QuaternionOrbitData::NO_CORRECTION);
  ecr_bx = r4.cf_look_vector(body_x);
  ecr_by = r4.cf_look_vector(body_y);
  ecr_bz = r4.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,0,0,1)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,-1,0,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,0,-1,0)) < 1e-8);

  // Pitch of 90 rotates x and z
  AircraftOrbitData r5(Time::time_pgs(100.0), Geodetic(0, 0, 100), vel_fixed,
		       0.0, 90.0, 0.0);
  r5.aberration_correction(QuaternionOrbitData::NO_CORRECTION);
  ecr_bx = r5.cf_look_vector(body_x);
  ecr_by = r5.cf_look_vector(body_y);
  ecr_bz = r5.cf_look_vector(body_z);
  BOOST_CHECK(l1(ecr_bx.look_quaternion() - quat(0,1,0,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_by.look_quaternion() - quat(0,0,1,0)) < 1e-8);
  BOOST_CHECK(l1(ecr_bz.look_quaternion() - quat(0,0,0,1)) < 1e-8);
}

BOOST_AUTO_TEST_CASE(aircraft_orbit_data_conversion)
{
  boost::array<double, 3> vel_fixed = {{100.0, 200.0, 300.0}};
  boost::shared_ptr<QuaternionOrbitData> od1
    (new AircraftOrbitData(Time::time_pgs(100.0), Geodetic(30, 40, 100), 
			   vel_fixed, 5.0, 10.0, 20.0));
  AircraftOrbitData od2(*od1);
  BOOST_CHECK(distance(Geodetic(30, 40, 100), od2.position_geodetic()) < 0.1);
  BOOST_CHECK_CLOSE(od2.roll(), 5, 1e-8);
  BOOST_CHECK_CLOSE(od2.pitch(), 10, 1e-8);
  BOOST_CHECK_CLOSE(od2.heading(), 20, 1e-8);
}

BOOST_AUTO_TEST_CASE(aircraft_orbit_data_conversion2)
{
  double hpr[3] = {-1.4639910e+02,     1.6479492e-02,    -6.2622070e-01};
  boost::array<double, 3> vel_fixed = {{100.0, 200.0, 300.0}};
  boost::shared_ptr<QuaternionOrbitData> od1
    (new AircraftOrbitData(Time::time_pgs(100.0), Geodetic(30, 40, 100), 
			   vel_fixed, hpr[2], hpr[1], hpr[0]));
  AircraftOrbitData od2(*od1);
  BOOST_CHECK(distance(Geodetic(30, 40, 100), od2.position_geodetic()) < 0.1);
  BOOST_CHECK_CLOSE(od2.roll(), hpr[2], 1e-8);
  BOOST_CHECK_CLOSE(od2.pitch(), hpr[1], 1e-8);
  BOOST_CHECK_CLOSE(od2.heading(), hpr[0], 1e-8);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;

  boost::array<double, 3> vel_fixed = {{0.0, 0.0, 0.0}};
  boost::shared_ptr<AircraftOrbitData> od
    (new AircraftOrbitData(Time::time_pgs(100.0), Geodetic(0, 0, 100), 
			   vel_fixed, 0.0, 0.0, 0.0));
  std::string d = serialize_write_string(od);
  if(false)
    std::cerr << d;
  boost::shared_ptr<AircraftOrbitData> odr =
    serialize_read_string<AircraftOrbitData>(d);
}

BOOST_AUTO_TEST_SUITE_END()
