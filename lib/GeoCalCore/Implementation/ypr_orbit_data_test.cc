#include "unit_test_support.h"
#include "ypr_orbit_data.h"
#include "ecr.h"

using namespace GeoCal;
using namespace blitz;

class YprOrbitDataFixture : public GlobalFixture {
public:
  YprOrbitDataFixture() {
    // Create test position and velocity using concrete Ecr/Eci classes
    t = Time::time_pgs(100.0);

    // Position on X-axis at ~7000 km altitude
    boost::array<double, 3> pos = {{7000e3, 0, 0}};
    pos_cf.reset(new Ecr(pos[0], pos[1], pos[2]));

    // Convert to ECI for alternate constructor
    pos_ci = pos_cf->convert_to_ci(t);

    // Typical orbital velocity
    vel_cf[0] = 0;
    vel_cf[1] = 7500;  // ~7.5 km/s
    vel_cf[2] = 0;

    // Convert velocity to CI
    boost::shared_ptr<CartesianInertial> pos_ci_tmp;
    convert_position_and_velocity(t, *pos_cf, vel_cf, pos_ci_tmp, vel_ci);

    // Test Euler angles in degrees (use distinct values to catch index errors)
    yaw = 10.0;
    pitch = 20.0;
    roll = 30.0;

    // Create sc_to_orbital quaternion from Euler angles for comparison
    double yaw_rad = yaw * Constant::deg_to_rad;
    double pitch_rad = pitch * Constant::deg_to_rad;
    double roll_rad = roll * Constant::deg_to_rad;
    quat_sc_to_orb = quat_rot("ZYX", yaw_rad, pitch_rad, roll_rad);
  }

  Time t;
  boost::shared_ptr<CartesianFixed> pos_cf;
  boost::shared_ptr<CartesianInertial> pos_ci;
  boost::array<double, 3> vel_cf, vel_ci;
  double yaw, pitch, roll;
  boost::math::quaternion<double> quat_sc_to_orb;
};

BOOST_FIXTURE_TEST_SUITE(ypr_orbit_data, YprOrbitDataFixture)

BOOST_AUTO_TEST_CASE(basic_construction_cf)
{
  // Construct with CartesianFixed and default "ZYX" order
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);

  // Check that Euler angles are stored correctly
  BOOST_CHECK_CLOSE(ypr.euler_angle1(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle2(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle3(), roll, 1e-6);

  // Check that yaw/pitch/roll accessors work
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);

  // Check euler order
  BOOST_CHECK_EQUAL(ypr.euler_order(), "ZYX");

  // Check position is preserved
  boost::shared_ptr<CartesianFixed> pos_result = ypr.position_cf();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_cf->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_cf->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_cf->position[2], 1e-6);

  // Check prefer_cf
  BOOST_CHECK_EQUAL(ypr.prefer_cf(), true);
}

BOOST_AUTO_TEST_CASE(basic_construction_ci)
{
  // Construct with CartesianInertial
  YprOrbitData ypr(t, pos_ci, vel_ci, yaw, pitch, roll);

  // Check that Euler angles are stored correctly
  BOOST_CHECK_CLOSE(ypr.euler_angle1(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle2(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.euler_angle3(), roll, 1e-6);

  // Check that yaw/pitch/roll accessors work
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);

  // Check position
  boost::shared_ptr<CartesianInertial> pos_result = ypr.position_ci();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_ci->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_ci->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_ci->position[2], 1e-6);

  // Check prefer_cf
  BOOST_CHECK_EQUAL(ypr.prefer_cf(), false);
}

BOOST_AUTO_TEST_CASE(conversion_from_quaternion_cf)
{
  // Create a YprOrbitData first to get the orbital frame
  YprOrbitData ypr_temp(t, pos_ci, vel_ci, yaw, pitch, roll);
  boost::math::quaternion<double> orbital_to_eci = ypr_temp.orbital_to_eci();

  // Combine with sc_to_orbital to get sc_to_eci
  boost::math::quaternion<double> sc_to_eci = orbital_to_eci * quat_sc_to_orb;

  // Create QuaternionOrbitData with this attitude
  QuaternionOrbitData qod(t, pos_ci, vel_ci, sc_to_eci);

  // Convert to YprOrbitData
  YprOrbitData ypr(qod);

  // Check that Euler angles match original (within tolerance for round-trip)
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-4);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-4);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-4);

  // Check position is preserved
  boost::shared_ptr<CartesianInertial> pos_result = ypr.position_ci();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_ci->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_ci->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_ci->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(round_trip_conversion)
{
  // Create YprOrbitData
  YprOrbitData ypr1(t, pos_cf, vel_cf, yaw, pitch, roll);

  // Convert to QuaternionOrbitData (implicit via base class)
  QuaternionOrbitData qod(ypr1);

  // Convert back to YprOrbitData
  YprOrbitData ypr2(qod);

  // Check that Euler angles are preserved (looser tolerance for double round-trip)
  BOOST_CHECK_CLOSE(ypr2.yaw(), yaw, 1e-3);
  BOOST_CHECK_CLOSE(ypr2.pitch(), pitch, 1e-3);
  BOOST_CHECK_CLOSE(ypr2.roll(), roll, 1e-3);
}

BOOST_AUTO_TEST_CASE(different_euler_order)
{
  // Test with XYZ order
  // Constructor always takes (yaw, pitch, roll) regardless of order
  // euler_order controls how they're combined into quaternion
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll, "XYZ");

  // Check euler order
  BOOST_CHECK_EQUAL(ypr.euler_order(), "XYZ");

  // Check physical accessors return what was given
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);

  // Check generic accessors map based on order
  // For "XYZ": euler_angle1=roll(X), euler_angle2=pitch(Y), euler_angle3=yaw(Z)
  BOOST_CHECK_CLOSE(ypr.euler_angle1(), roll, 1e-6);   // X rotation first
  BOOST_CHECK_CLOSE(ypr.euler_angle2(), pitch, 1e-6);  // Y rotation second
  BOOST_CHECK_CLOSE(ypr.euler_angle3(), yaw, 1e-6);    // Z rotation third
}

BOOST_AUTO_TEST_CASE(numeric_euler_order)
{
  // Test with numeric order "321" (equivalent to "ZYX")
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll, "321");

  BOOST_CHECK_EQUAL(ypr.euler_order(), "321");
  BOOST_CHECK_CLOSE(ypr.yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll, 1e-6);
}

BOOST_AUTO_TEST_CASE(zero_angles)
{
  // Test with zero angles (identity rotation)
  YprOrbitData ypr(t, pos_cf, vel_cf, 0.0, 0.0, 0.0);

  BOOST_CHECK_CLOSE(ypr.yaw(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), 0.0, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), 0.0, 1e-6);

  // Check that orbital_to_eci quaternion is calculated
  boost::math::quaternion<double> orb_to_eci = ypr.orbital_to_eci();
  // Just verify it's not NaN
  BOOST_CHECK(orb_to_eci.R_component_1() == orb_to_eci.R_component_1());
}

BOOST_AUTO_TEST_CASE(large_angles)
{
  // Test with large angles (distinct values to catch mixing)
  double yaw_large = 100.0;
  double pitch_large = 80.0;
  double roll_large = -45.0;

  YprOrbitData ypr(t, pos_cf, vel_cf, yaw_large, pitch_large, roll_large);

  BOOST_CHECK_CLOSE(ypr.yaw(), yaw_large, 1e-6);
  BOOST_CHECK_CLOSE(ypr.pitch(), pitch_large, 1e-6);
  BOOST_CHECK_CLOSE(ypr.roll(), roll_large, 1e-6);
}

BOOST_AUTO_TEST_CASE(look_vector_conversion)
{
  // Create YprOrbitData
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);

  // Create a test spacecraft look vector
  ScLookVector sl(1.0, 0.0, 0.0);  // Looking along +X in spacecraft frame

  // Convert to CartesianFixed look vector
  CartesianFixedLookVector cfl = ypr.cf_look_vector(sl);
  BOOST_CHECK(cfl.length() > 0.99 && cfl.length() < 1.01);

  // Convert back to spacecraft frame
  ScLookVector sl_back = ypr.sc_look_vector(cfl);
  BOOST_CHECK_CLOSE(sl_back.direction()[0], sl.direction()[0], 1e-4);
  // Y and Z components can have small numerical errors when original values are zero
  BOOST_CHECK_SMALL(sl_back.direction()[1] - sl.direction()[1], 1e-9);
  BOOST_CHECK_SMALL(sl_back.direction()[2] - sl.direction()[2], 1e-9);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;

  boost::shared_ptr<YprOrbitData> ypr(new YprOrbitData(t, pos_cf, vel_cf, yaw, pitch, roll));

  std::string d = serialize_write_string(ypr);
  if(false)
    std::cerr << d;

  boost::shared_ptr<YprOrbitData> ypr_restored =
    serialize_read_string<YprOrbitData>(d);

  // Check all values are preserved
  BOOST_CHECK_CLOSE(ypr_restored->yaw(), yaw, 1e-6);
  BOOST_CHECK_CLOSE(ypr_restored->pitch(), pitch, 1e-6);
  BOOST_CHECK_CLOSE(ypr_restored->roll(), roll, 1e-6);
  BOOST_CHECK_EQUAL(ypr_restored->euler_order(), "ZYX");

  boost::shared_ptr<CartesianFixed> pos_result = ypr_restored->position_cf();
  BOOST_CHECK_CLOSE(pos_result->position[0], pos_cf->position[0], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[1], pos_cf->position[1], 1e-6);
  BOOST_CHECK_CLOSE(pos_result->position[2], pos_cf->position[2], 1e-6);
}

BOOST_AUTO_TEST_CASE(print)
{
  YprOrbitData ypr(t, pos_cf, vel_cf, yaw, pitch, roll);

  std::ostringstream os;
  os << ypr;
  std::string output = os.str();

  // Check that output contains expected strings
  BOOST_CHECK(output.find("YprOrbitData") != std::string::npos);
  BOOST_CHECK(output.find("Euler Order") != std::string::npos);
  BOOST_CHECK(output.find("Yaw") != std::string::npos);
  BOOST_CHECK(output.find("Pitch") != std::string::npos);
  BOOST_CHECK(output.find("Roll") != std::string::npos);
  BOOST_CHECK(output.find("deg") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()
