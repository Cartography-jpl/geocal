#include "unit_test_support.h"
#include "camera.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(simple_pushbroom_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  SimplePushBroomCamera c;
  FrameCoordinate f1(1, 2);
  ScLookVector sl = c.sc_look_vector(f1, 0);
  FrameCoordinate f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_EQUAL(c.number_band(), 1);
  BOOST_CHECK_EQUAL(c.number_line(0), 1);
  BOOST_CHECK_EQUAL(c.number_sample(0), 1504);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  BOOST_CHECK_CLOSE(c.frame_line_coordinate(sl, 0), f1.line, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
