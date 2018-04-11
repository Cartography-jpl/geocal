#include "unit_test_support.h"
#include "raw_raster_image.h"    
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(raw_raster_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
#ifdef HAVE_LIBRAW 
  std::string fname = test_data_dir() + "cannon_raw.cr2";
  RawRasterImage d(fname);
  BOOST_CHECK_EQUAL(d.number_line(), 3752);
  BOOST_CHECK_EQUAL(d.number_sample(), 5640);
  BOOST_CHECK_EQUAL(d(100,100), 1028);
#endif
}

BOOST_AUTO_TEST_SUITE_END()
