#include "unit_test_support.h"
#include "eci_tod_burl.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "ecr.h"
#include "wgs84_constant.h"
#include "geocal_config.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(eci_tod_burl, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
#ifndef HAVE_CARTO
  return;
#endif
  boost::array<double, 3> p = {{10, 20, 30}};
  EciTodBurl e1(p);
  EciTodBurl e2(10, 20, 30);
  BOOST_CHECK(e1.position == p);
  BOOST_CHECK(e2.position == p);

  // This is from sc2rpc test. 

  Time t = Time::time_acs(215077459.471879);
  EciTodBurl::delta_ut1 = 0.1128609;
  EciTodBurl eci_tod(3435100.496, 945571.538, -6053387.573);
  boost::shared_ptr<GroundCoordinate> gc(eci_tod.convert_to_cf(t));
  Ecr ecr_exp(2508132.035257665440440, 2530473.205708642024547,
	      -6053387.572999999858439);
  BOOST_CHECK(distance(ecr_exp, *gc) < 0.01);
}

BOOST_AUTO_TEST_SUITE_END()
