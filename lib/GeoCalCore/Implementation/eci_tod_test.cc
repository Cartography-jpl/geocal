#include "unit_test_support.h"
#include "eci_tod.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "ecr.h"
#include "wgs84_constant.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(eci_tod, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::array<double, 3> p = {{10, 20, 30}};
  EciTod e1(p);
  EciTod e2(10, 20, 30);
  BOOST_CHECK(e1.position == p);
  BOOST_CHECK(e2.position == p);

  Geodetic g(10, 20, 10000);
  Time t = Time::parse_time("2003-01-01T10:30:00Z");
  boost::shared_ptr<CartesianInertial> e3 = 
    g.convert_to_cf()->convert_to_ci(t);
  boost::array<double, 3> d = {{10.0, 11.0, 12.0}};
  CartesianInertialLookVector lv(d);
  boost::shared_ptr<CartesianInertial> e4 =
    e3->reference_surface_intersect_approximate(lv, 100);

// Check that surface point is in the direction of the lv.

  d[0] = e4->position[0] - e3->position[0];
  d[1] = e4->position[1] - e3->position[1];
  d[2] = e4->position[2] - e3->position[2];
  CartesianInertialLookVector lv2(d);
  BOOST_CHECK_CLOSE(lv2.direction()[0], lv.direction()[0], 1e-2);
  BOOST_CHECK_CLOSE(lv2.direction()[1], lv.direction()[1], 1e-2);
  BOOST_CHECK_CLOSE(lv2.direction()[2], lv.direction()[2], 1e-2);

// Check that height is near desired height.

  BOOST_CHECK(fabs(e4->convert_to_cf(t)->height_reference_surface() - 100) < 1);


  // This is from sc2rpc test. 
  // Note that we get an answer that is about 7 meters different than
  // sc2rpc. My current guess is that SPICE has a better model for
  // nutation, precision, etc (i.e., I'm right and sc2rpc is wrong).
  // It is possible that there is something I'm not understanding and 
  // the mistake is mine. For now, we'll accept this, but we'll come
  // back and fix this if it turns out I'm wrong.

  t = Time::time_acs(215077459.472);
  EciTod eci_tod(3435100.496, 945571.538, -6053387.573);
  boost::shared_ptr<GroundCoordinate> gc(eci_tod.convert_to_cf(t));
  Ecr ecr_exp(2508132.035257665440440, 2530473.205708642024547,
	      -6053387.572999999858439);
  BOOST_CHECK(distance(ecr_exp, *gc) < 10);
  Ecr ecr(*gc);
  BOOST_CHECK_CLOSE(ecr.position[0], 2508131.6186824911, 1e-3);
  BOOST_CHECK_CLOSE(ecr.position[1], 2530480.4869565335, 1e-3);
  BOOST_CHECK_CLOSE(ecr.position[2], -6053384.7018460119, 1e-3);

  Eci eci(eci_tod.to_eci(t));
  EciTod eci_tod2(eci, t);
  boost::shared_ptr<GroundCoordinate> gc2(eci_tod2.convert_to_cf(t));
  BOOST_CHECK(distance(*gc, *gc2) < 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
