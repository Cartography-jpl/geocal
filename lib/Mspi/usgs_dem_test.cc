#include "usgs_dem.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(usgs_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(usgs_dem_data)
{
  UsgsDemData(test_data_dir() + "usgs_dem", false);
}

BOOST_AUTO_TEST_SUITE_END()
