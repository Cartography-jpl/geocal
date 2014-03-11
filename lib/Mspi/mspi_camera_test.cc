#include "mspi_camera.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_camera, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiCamera cam(test_data_dir() + "mspi_camera_test.config");
}

BOOST_AUTO_TEST_SUITE_END()
