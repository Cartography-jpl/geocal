#include "unit_test_support.h"
#include "look_vector.h"
#include "geocal_time.h"
#include "geodetic.h"
#include <cmath>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(look_vector, GlobalFixture)

BOOST_AUTO_TEST_CASE(sc_look_vector)
{
  boost::array<double, 3> d = {{10.0, 0.0, 0.0}};
  ScLookVector lv(d);
  BOOST_CHECK_CLOSE(lv.length(), 10.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[1], 0.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[2], 0.0, 1e-4);
  BOOST_CHECK_EQUAL(lv.print_to_string(), "Look vector in spacecraft coordinatesg: \n  dir:    (1, 0, 0)\n  length: 10 m\n");
}

BOOST_AUTO_TEST_CASE(solar_angle)
{
  Time t = Time::parse_time("1996-07-03T04:13:57.987654Z") + 10;
  Geodetic p(50, 60, 1);
  LnLookVector ln = LnLookVector::solar_look_vector(t, p);
  // The results are from an old unit test in MSPI code. We don't
  // expect this to exactly match because the MSPI code used the SDP
  // toolkit. But should be close.
  BOOST_CHECK_CLOSE(ln.view_zenith(), 9.06966388e-01 * Constant::rad_to_deg, 
		    0.1);
  BOOST_CHECK_CLOSE(ln.view_azimuth(), 4.87556601e+00 * Constant::rad_to_deg, 
		    0.1);
}

BOOST_AUTO_TEST_CASE(cartesian_inertial_look_vector)
{
  boost::array<double, 3> d = {{10.0, 0.0, 0.0}};
  CartesianInertialLookVector lv(d);
  BOOST_CHECK_CLOSE(lv.length(), 10.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[1], 0.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[2], 0.0, 1e-4);
  BOOST_CHECK_EQUAL(lv.print_to_string(), "Look vector in Cartesian inertial coordinates: \n  dir:    (1, 0, 0)\n  length: 10 m\n");
}

BOOST_AUTO_TEST_CASE(cartesian_fixed_look_vector)
{
  boost::array<double, 3> d = {{10.0, 0.0, 0.0}};
  CartesianFixedLookVector lv(d);
  BOOST_CHECK_CLOSE(lv.length(), 10.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[1], 0.0, 1e-4);
  BOOST_CHECK_CLOSE(lv.direction()[2], 0.0, 1e-4);
  BOOST_CHECK_EQUAL(lv.print_to_string(), "Look vector in Cartesian fixed coordinates: \n  dir:    (1, 0, 0)\n  length: 10 m\n");
}

BOOST_AUTO_TEST_SUITE_END()
