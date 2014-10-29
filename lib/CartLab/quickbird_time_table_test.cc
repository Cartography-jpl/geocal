#include "unit_test_support.h"
#include "quickbird_time_table.h"
#include <cmath>
#include <iostream>
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(quickbird_time_table, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = test_data_dir() + 
    "06JAN06033849-P1BS-005545519120_01_P001.IMD";
  QuickBirdTimeTable tt(fname);
  BOOST_CHECK_EQUAL(tt.file_name(), fname);

  // All these expected results come from directly looking at the
  // input file. This is in plain text, so it is easy to read by eye.

  BOOST_CHECK_EQUAL(tt.min_line(), 0);
  BOOST_CHECK_EQUAL(tt.max_line(), 29224);
  BOOST_CHECK(fabs(tt.min_time() -
		   Time::parse_time("2006-01-06T03:38:49.805725Z")) < 1e-3);
  BOOST_CHECK_CLOSE(tt.max_time() - tt.min_time(), 4.235362, 1e-4);
  ImageCoordinate ic = tt.image_coordinate(tt.min_time() + (tt.max_time() - tt.min_time()) / 2, FrameCoordinate(0, 100));
  BOOST_CHECK_CLOSE(ic.line, 29224 / 2.0, 1e-4);
  BOOST_CHECK_CLOSE(ic.sample, 100.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
