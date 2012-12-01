#include "unit_test_support.h"
#include "spot_orbit.h"

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(spot_orbit, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  // This data is test data from a particular Dimap file we used as a
  // test case. No particularly significance, other than these are
  // reasonable values.
  std::vector<Time> t(2);
  blitz::Array<double, 2> eph(2,6);
  blitz::Array<double, 2> ypr(2,3);
  t[0] = Time::time_unix(1268937566.0);
  t[1] = Time::time_unix(1268937596.0);
  eph = -2085112.0269, -4633286.4834, 5100226.2779, 
    -3914.121458, -3891.038691, -5122.558975,
    -2201773.2664, -4747531.396, 4944120.8796,
    -3862.554142, -3724.717958, -5283.635313;
  ypr = 8.3039658465e-04, -4.3067777528e-04, 2.7552616219e-04,
    8.3045120787e-04, -4.3021004885e-04, 2.7427261489e-04;
    
  SpotOrbit orb(t, eph, t, ypr);
  BOOST_CHECK_CLOSE(orb.min_time() - t[0], 0, 1e-6);
  BOOST_CHECK_CLOSE(orb.max_time() - t[1], 0, 1e-6);
  Time tv = t[0] + (t[1] - t[0]) / 2;
  BOOST_CHECK_CLOSE(orb.position_cf(tv)->position[0],
		    -2143442.64665, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_cf(tv)->position[1],
		    -4690408.9397, 1e-4);
  BOOST_CHECK_CLOSE(orb.position_cf(tv)->position[2],
		    5022173.5787499994, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
