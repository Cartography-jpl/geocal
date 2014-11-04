#include "unit_test_support.h"
#include "galileo_camera.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(galileo_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  GalileoCamera cam;
  BOOST_CHECK_CLOSE(cam.focal_length(), 1501.039, 1e-4);
  BOOST_CHECK_CLOSE(cam.line_pitch(),   15.24e-3, 1e-4);
  BOOST_CHECK_CLOSE(cam.sample_pitch(), 15.24e-3, 1e-4);
  BOOST_CHECK_CLOSE(cam.principal_point(0).line, 399, 1e-4);
  BOOST_CHECK_CLOSE(cam.principal_point(0).sample, 399, 1e-4);
  BOOST_CHECK_EQUAL(cam.number_line(0), 800);
  BOOST_CHECK_EQUAL(cam.number_sample(0), 800);
  BOOST_CHECK_EQUAL(cam.number_band(), 1);
  FrameCoordinate fc(0,0);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(399,399);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
  fc = FrameCoordinate(799,799);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).line -  fc.line) < 1e-6);
  BOOST_CHECK(fabs(cam.frame_coordinate(cam.sc_look_vector(fc, 0), 
	     			       0).sample -  fc.sample) < 1e-6);
}

BOOST_AUTO_TEST_SUITE_END()

