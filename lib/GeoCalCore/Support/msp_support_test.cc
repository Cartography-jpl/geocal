#include "unit_test_support.h"
#include "msp_support.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(msp_support, GlobalFixture)

BOOST_AUTO_TEST_CASE(msp_terrain_point_test)
{
  std::cerr << msp_terrain_point(test_data_dir() + "rpc.ntf",
				 ImageCoordinate(5,5)) << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
