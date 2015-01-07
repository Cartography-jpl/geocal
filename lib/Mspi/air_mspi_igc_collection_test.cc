#include "air_mspi_igc_collection.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_igc_collection, GlobalFixture)

BOOST_AUTO_TEST_CASE(chain_test)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiIgcCollection
    igcol(test_data_dir() + "mspi/3.master.config", 
	  test_data_dir() + "mspi/3.nav_gimbal.hdf4",
	  test_data_dir() + "mspi/3.l1b1_table",
	  test_data_dir() + "mspi");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgcCollection test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_MSPI_SHARED
  if(!have_serialize_supported())
    return;
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgcCollection test, because we haven't build ith MSPI_SHARED support");
#endif
}
BOOST_AUTO_TEST_SUITE_END()
