#include "unit_test_support.h"
#include "frame_coordinate.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(frame_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  FrameCoordinate f(10, 20);
  BOOST_CHECK_CLOSE(f.line, 10.0, 1e-4);
  BOOST_CHECK_CLOSE(f.sample, 20.0, 1e-4);
  BOOST_CHECK_EQUAL(f.print_to_string(), "Frame Coordinate: (10, 20)");
}

BOOST_AUTO_TEST_SUITE_END()

