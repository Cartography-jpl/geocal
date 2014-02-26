#include "unit_test_support.h"
#include "look_vector.h"
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
  BOOST_CHECK_EQUAL(lv.print_to_string(), "Look vector in spacecraft coordinates: \n  dir:    (1, 0, 0)\n  length: 10 m\n");
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
