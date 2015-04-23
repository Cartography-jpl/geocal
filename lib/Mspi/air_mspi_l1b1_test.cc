#include "air_mspi_l1b1.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_l1b1, GlobalFixture)

BOOST_AUTO_TEST_CASE(air_mspi_l1b1_file)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiL1b1File f(test_data_dir() + "/mspi/3.l1b1.hdf5");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiL1b1 test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(air_mspi_l1b1)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiL1b1 f(test_data_dir() + "/mspi/3.l1b1.hdf5");
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiL1b1 test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_MSPI_SHARED
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<AirMspiL1b1> 
    l1b1(new AirMspiL1b1(test_data_dir() + "/mspi/3.l1b1.hdf5"));
  std::string d = serialize_write_string(l1b1);
  if(true)
    std::cerr << d;
  boost::shared_ptr<AirMspiL1b1> l1b1r =
    serialize_read_string<AirMspiL1b1>(d);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiL1b1 test, because we haven't build with MSPI_SHARED support");
#endif
}
BOOST_AUTO_TEST_SUITE_END()
