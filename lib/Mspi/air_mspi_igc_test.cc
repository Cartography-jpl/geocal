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
	"660-I", 
	test_data_dir() + "/mspi");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgc test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_MSPI_SHARED
  if(!have_serialize_supported())
    return;
  std::string fname = test_data_dir() + "cib_sample.img";
  boost::shared_ptr<AirMspiIgc> 
    igc(new AirMspiIgc(test_data_dir() + "/mspi/3.master.config", 
		       test_data_dir() + "/mspi/3.nav_gimbal.hdf4",
		       test_data_dir() + "/mspi/3.l1b1.hdf5",
		       "660-I", 
		       test_data_dir() + "/mspi"));
  std::string d = serialize_write_string(igc);
  if(false)
    std::cerr << d;
  boost::shared_ptr<AirMspiIgc> igcr = 
    serialize_read_string<AirMspiIgc>(d);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgc test, because we haven't build ith MSPI_SHARED support");
#endif
}
BOOST_AUTO_TEST_SUITE_END()
