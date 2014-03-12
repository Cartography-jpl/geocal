#include "mspi_camera.h"
#include "unit_test_support.h"
#include <iostream>
#include "geocal_matrix.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiCamera cam(test_data_dir() + "mspi_camera_test.config");
  ScLookVector v(27, -0.3, 0.4);
  std::cerr << cam.detector_look(v) / v.length() << "\n";
}

BOOST_AUTO_TEST_SUITE_END()
