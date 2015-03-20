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
  BOOST_CHECK_EQUAL(igcol.number_image(), 2);
  BOOST_CHECK_EQUAL(igcol.number_band(0), 14);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgcCollection test, because we haven't build with MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_MSPI_SHARED
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<IgcCollection> igcol
    (new  AirMspiIgcCollection (test_data_dir() + "mspi/3.master.config", 
				test_data_dir() + "mspi/3.nav_gimbal.hdf4",
				test_data_dir() + "mspi/3.l1b1_table",
				test_data_dir() + "mspi"));
  std::string d = serialize_write_string(igcol);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<AirMspiIgcCollection> igcol2 = 
    serialize_read_string<AirMspiIgcCollection>(d);
  BOOST_CHECK_EQUAL(igcol2->number_image(), 2);
  BOOST_CHECK_EQUAL(igcol2->number_band(0), 14);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgcCollection test, because we haven't build with MSPI_SHARED support");
#endif
}
BOOST_AUTO_TEST_SUITE_END()
