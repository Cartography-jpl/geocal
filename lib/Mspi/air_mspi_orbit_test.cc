#include "air_mspi_orbit.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  AirMspiOrbit orb(test_data_dir() + "airmspi_orbit_file_test.hdf");
}

BOOST_AUTO_TEST_SUITE_END()
