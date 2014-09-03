#include "unit_test_support.h"
#include "mars_coordinate.h"
#include "geocal_time.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mars_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(mars_fixed)
{
  MarsFixed mf(10, 20, 30);

}

BOOST_AUTO_TEST_SUITE_END()
