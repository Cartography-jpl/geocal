#include "unit_test_support.h"
#include "argus_camera.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(argus_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
// Not actually used anymore, we hardwire the line and sample pitch.
//   std::string fname = std::string(getenv("srcdir")) + 
//     "/test_data/argus_camera6.jpg";
  ArgusCamera cam(10);
  BOOST_CHECK_CLOSE(cam.focal_length(), 400.0, 1e-4);
  BOOST_CHECK_CLOSE(cam.line_pitch(),   0.00641026, 1e-4);
  BOOST_CHECK_CLOSE(cam.sample_pitch(), 0.00641026, 1e-4);
  FrameCoordinate fc1(cam.number_line(0) / 2.0, cam.number_sample(0) / 2.0);
  BOOST_CHECK_CLOSE(fc1.line, cam.frame_coordinate(cam.sc_look_vector(fc1, 0), 
						   0).line, 1e-4);
  BOOST_CHECK_CLOSE(fc1.sample, 
		    cam.frame_coordinate(cam.sc_look_vector(fc1, 0), 
					 0).sample, 1e-4);
  ScLookVector slv = cam.sc_look_vector(fc1, 0);
  BOOST_CHECK_CLOSE(slv.direction()[0], 0.0,1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[1], 0.0,1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[2], 1.0,1e-4);
  FrameCoordinate fc2(1, 1);
  BOOST_CHECK_CLOSE(fc2.line, cam.frame_coordinate(cam.sc_look_vector(fc2, 0), 
						   0).line, 1e-4);
  BOOST_CHECK_CLOSE(fc2.sample, 
		    cam.frame_coordinate(cam.sc_look_vector(fc2, 0), 
					 0).sample, 1e-4);
  slv = cam.sc_look_vector(fc2, 0);
  BOOST_CHECK_CLOSE(slv.direction()[0], 0.0372854, 1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[1], -0.0390369, 1e-4);
  BOOST_CHECK_CLOSE(slv.direction()[2], 0.998542, 1e-4);
}


BOOST_AUTO_TEST_SUITE_END()

