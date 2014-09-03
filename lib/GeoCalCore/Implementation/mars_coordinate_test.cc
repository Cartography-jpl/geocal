#include "unit_test_support.h"
#include "mars_coordinate.h"
#include "geocal_time.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mars_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(mars_fixed)
{
  MarsFixed mf(10, 20, 30);
  BOOST_CHECK_CLOSE(mf.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[2], 30.0, 1e-8);
  boost::shared_ptr<GroundCoordinate> gc = mf.convert_to_cf();
  MarsFixed mf2(*gc);
  BOOST_CHECK_CLOSE(mf2.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[2], 30.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.min_radius_reference_surface(), 3396000.0, 1e-8);
  // Need some data from MarsPlanetocentric to check
  // height_reference_surface, latitude, longitude, and 
  // reference_surface_intersect_approximate
}

BOOST_AUTO_TEST_SUITE_END()
