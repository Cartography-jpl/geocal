#include "unit_test_support.h"
#include "spot_camera.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(spot_camera, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::vector<double> psi_x(3), psi_y(3);
  psi_x[0] = -0.0095544910301;
  psi_x[1] = -0.0095545017004;
  psi_x[2] = -0.0095545133715;
  psi_y[0] = 0.067955961852;
  psi_y[1] = 0.067967964881;
  psi_y[2] = 0.067979966909;  
  SpotCamera c(psi_x, psi_y);
  FrameCoordinate f1(1, 1.5);
  ScLookVector sl = c.sc_look_vector(f1, 0);
  FrameCoordinate f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_EQUAL(c.number_band(), 1);
  BOOST_CHECK_EQUAL(c.number_line(0), 1);
  BOOST_CHECK_EQUAL(c.number_sample(0), 3);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  f1.line = -1;
  f1.sample = -1;
  sl = c.sc_look_vector(f1,0);
  f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
  f1.sample = 4;
  sl = c.sc_look_vector(f1,0);
  f2 = c.frame_coordinate(sl, 0);
  BOOST_CHECK_CLOSE(f2.line, f1.line, 1e-4);
  BOOST_CHECK_CLOSE(f2.sample, f1.sample, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
