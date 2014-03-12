#include "mspi_camera.h"
#include "unit_test_support.h"
#include "constant.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiCamera cam(test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK(cam.file_name() == test_data_dir() + "mspi_camera_test.config");
  BOOST_CHECK_CLOSE(cam.epsilon(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.psi(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.theta(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.boresight_angle(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.yaw(), 180.0 * Constant::deg_to_rad, 1e-8);
  BOOST_CHECK_CLOSE(cam.pitch(), 90.0 * Constant::deg_to_rad, 1e-8);
  BOOST_CHECK_CLOSE(cam.roll(), 0, 1e-8);
  BOOST_CHECK_CLOSE(cam.focal_length(), 27.825088, 1e-8);
  BOOST_CHECK_CLOSE(cam.dx(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.ypitch(), 0.010, 1e-8);
  BOOST_CHECK_CLOSE(cam.dy(), 0.016, 1e-8);
  BOOST_CHECK_CLOSE(cam.s_origin(), 767.5, 1e-8);
  BOOST_CHECK(cam.line_direction() == 1);
  BOOST_CHECK(cam.pixel_order() == 1);
  BOOST_CHECK(cam.number_band() == 7);
  BOOST_CHECK(cam.number_line(0) == 1);
  BOOST_CHECK(cam.number_sample(0) == 1536);
  BOOST_CHECK_CLOSE(cam.parameter()(0), cam.yaw(), 1e-8);
  BOOST_CHECK_CLOSE(cam.parameter()(1), cam.pitch(), 1e-8);
  BOOST_CHECK_CLOSE(cam.parameter()(2), cam.roll(), 1e-8);
  ScLookVector v(27, -0.3, 0.4);
  const int nband = 7;
  // Results from old code unit test.
  double line_expect[nband] = {4.02840527542328388222e+01,
			       4.82840527542328388222e+01,
			       5.62840527542328388222e+01,
			       3.70840527542328430854e+01,
			       4.50840527542328359800e+01,
			       5.30840527542328430854e+01,
			       -1.07776474074074037901e+01};
  double sample_expect[nband] = {8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 8.00175853880662771189e+02,
				 7.98416764444444424953e+02};
  
  for(int b = 0; b < cam.number_band(); ++b) {
    FrameCoordinate fc = cam.frame_coordinate(v, b);
    BOOST_CHECK_CLOSE(fc.line, line_expect[b], 1e-8);
    BOOST_CHECK_CLOSE(fc.sample, sample_expect[b], 1e-8);
  }
}

BOOST_AUTO_TEST_SUITE_END()
