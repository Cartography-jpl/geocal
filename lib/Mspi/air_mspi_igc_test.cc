#include "air_mspi_igc.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_igc, GlobalFixture)

BOOST_AUTO_TEST_CASE(chain_test)
{
#ifdef HAVE_MSPI_SHARED
  // Skip test if data isn't available
  if(!boost::filesystem::is_directory("/home/smyth/Local/MSPI-Ground/chain_test/input"))
    return;
  // Grab data from Mike's chain test. We'll either modify this or
  // copy it, but for now grab from Mike's area
  AirMspiIgc 
    igc("/home/smyth/Local/MSPI-Ground/chain_test/input/3.master.config", 
	"/home/smyth/Local/MSPI-Ground/chain_test/input/3.nav_gimbal.hdf4",
	"/home/smyth/Local/MSPI-Ground/chain_test/3.l1b1.hdf5",
	1, 
	"/home/smyth/Local/MSPI-Ground");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgc test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_SUITE_END()
