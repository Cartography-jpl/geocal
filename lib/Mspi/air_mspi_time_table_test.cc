#include "air_mspi_time_table.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_time_table, GlobalFixture)

BOOST_AUTO_TEST_CASE(chain_test)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiTimeTable tt(test_data_dir() + "/mspi/3.l1b1.hdf5",
		      test_data_dir() + "/mspi/3.instrument_info.config");
  Time t;
  FrameCoordinate fc;
  tt.time(ImageCoordinate(0, 100), t, fc);
  std::cerr << t << fc << "\n";
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiTimeTable test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
// #ifdef HAVE_MSPI_SHARED
//   if(!have_serialize_supported())
//     return;
//   std::string fname = test_data_dir() + "cib_sample.img";
//   boost::shared_ptr<AirMspiIgc> 
//     igc(new AirMspiIgc(test_data_dir() + "/mspi/3.master.config", 
// 		       test_data_dir() + "/mspi/3.nav_gimbal.hdf4",
// 		       test_data_dir() + "/mspi/3.l1b1.hdf5",
// 		       1, 
// 		       test_data_dir() + "/mspi"));
//   std::string d = serialize_write_string(igc);
//   if(false)
//     std::cerr << d;
//   boost::shared_ptr<AirMspiIgc> igcr = 
//     serialize_read_string<AirMspiIgc>(d);
//   // Check band, camera, orbit, time_table, orbit_file_name,
//   // l1b1_file_name
//   BOOST_CHECK_EQUAL(igcr->base_directory(), igc->base_directory());
//   BOOST_CHECK_EQUAL(igcr->master_config_file(), igc->master_config_file());
// #else
//   BOOST_WARN_MESSAGE(false, "Skipping AirMspiIgc test, because we haven't build ith MSPI_SHARED support");
// #endif
}
BOOST_AUTO_TEST_SUITE_END()
