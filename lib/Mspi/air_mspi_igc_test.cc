#include "air_mspi_igc.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_igc, GlobalFixture)

BOOST_AUTO_TEST_CASE(chain_test)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiIgc 
    igc(test_data_dir() + "/mspi/3.master.config", 
	test_data_dir() + "/mspi/3.nav_gimbal.hdf4",
	test_data_dir() + "/mspi/3.l1b1.hdf5",
	1, 
	test_data_dir() + "/mspi");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgc test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_SUITE_END()
