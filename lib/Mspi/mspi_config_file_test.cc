#include "mspi_config_file.h"
#include "unit_test_support.h"
#include <iostream>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mspi_config_file, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  MspiConfigFile config(test_data_dir() + "mspi_config_file_test.txt");
  BOOST_CHECK_CLOSE(config.value<double>("parameter1"), 1.1, 1e-8);
  blitz::Array<double, 1> p2_expect(2);
  p2_expect = 2.1, -3.1;
  blitz::Array<double, 1> p2 = 
    config.value<blitz::Array<double, 1> >("parameter2");
  BOOST_CHECK_MATRIX_CLOSE(p2_expect, p2);
  blitz::Array<int, 1> p3_expect(3);
  p3_expect = 4, -5, 6;
  blitz::Array<int, 1> p3 = 
    config.value<blitz::Array<int, 1> >("parameter3");
  BOOST_CHECK_MATRIX_CLOSE(p3_expect, p3);
  // parameter 4
  BOOST_CHECK_CLOSE(config.value<double>("parameter6"), 1.00000001, 1e-8);
  std::vector<std::string> p7 = config.value<std::vector<std::string> >("parameter7");
  BOOST_CHECK(p7[0] == "alpha");
  BOOST_CHECK(p7[1] == "beta");
  BOOST_CHECK(p7[2] == "gamma");
  // array 1
  // signedlonglong
}

BOOST_AUTO_TEST_SUITE_END()
