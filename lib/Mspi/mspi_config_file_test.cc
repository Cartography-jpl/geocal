#include "mspi_config_file.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_config_file, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test.txt");
  BOOST_CHECK_CLOSE(config.value<double>("parameter1"), 1.1, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
