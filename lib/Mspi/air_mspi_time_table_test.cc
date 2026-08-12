#include "air_mspi_time_table.h"
#include "unit_test_support.h"
#include <iostream>
#include <boost/filesystem.hpp>

using namespace GeoCal;
BOOST_FIXTURE_TEST_SUITE(air_mspi_time_table, GlobalFixture)

BOOST_AUTO_TEST_CASE(chain_test)
{
#ifdef HAVE_MSPI_SHARED
  AirMspiTimeTable tt(test_data_dir() + "/mspi/3.l1b1.hdf5");
  Time t, t2;
  FrameCoordinate fc;
  tt.time(ImageCoordinate(0, 100), t, fc);
  BOOST_CHECK_CLOSE(fc.line, 0, 1e-4);
  BOOST_CHECK_CLOSE(fc.sample, 100, 1e-4);
  tt.time(ImageCoordinate(1, 100), t2, fc);
  BOOST_CHECK_CLOSE(t2 - t, 0.044528007507, 1e-4);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiTimeTable test, because we haven't build ith MSPI_SHARED support");
#endif
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_MSPI_SHARED
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<TimeTable> 
    tt(new AirMspiTimeTable(test_data_dir() + "/mspi/3.l1b1.hdf5"));
  std::string d = serialize_write_string(tt);
  if(false)
    std::cerr << d;
  boost::shared_ptr<TimeTable> ttr = 
    serialize_read_string<TimeTable>(d);
  Time t, t2;
  FrameCoordinate fc, fc2;
  tt->time(ImageCoordinate(0, 100), t, fc);
  ttr->time(ImageCoordinate(0, 100), t2, fc2);
  BOOST_CHECK(fabs(t - t2) < 1e-4);
#else
  BOOST_WARN_MESSAGE(false, "Skipping AirMspiTimeTable test, because we haven't build with MSPI_SHARED support");
#endif
}
BOOST_AUTO_TEST_SUITE_END()
