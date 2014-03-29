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
  blitz::Array<bool, 2> p4 =
    config.value<blitz::Array<bool, 2> >("parameter4");
  blitz::Array<bool, 2> p4_expect(2, 6);
  p4_expect = 
    true, false, true, false, true, false,
    true, false, true, false, true, false;
  BOOST_CHECK(p4_expect.rows() == p4.rows());
  BOOST_CHECK(p4_expect.cols() == p4.cols());
  for(int i = 0; i < p4.rows(); ++i)
    for(int j = 0; j < p4.cols(); ++j)
      BOOST_CHECK(p4(i,j) == p4_expect(i,j));
  BOOST_CHECK_CLOSE(config.value<double>("parameter6"), 1.00000001, 1e-8);
  std::vector<std::string> p7 = config.value<std::vector<std::string> >("parameter7");
  BOOST_CHECK(p7[0] == "alpha");
  BOOST_CHECK(p7[1] == "beta");
  BOOST_CHECK(p7[2] == "gamma");
  blitz::Array<int, 2> a1 = config.value<blitz::Array<int, 2> >("array1");
  blitz::Array<int, 2> a1_expect(3, 4);
  a1_expect = 
    100, 101, 102, 103,
    200, 201, 202, 203,
    300, 301, 302, 303;
  BOOST_CHECK_MATRIX_CLOSE(a1, a1_expect);
  // The original unit test called this "signedlonglong". However this
  // is actually unsigned, the value used it too large for
  // signed. lexical_cast correctly gives an error when we try to
  // convert to signed long long. I don't think this is actually
  // needed any where other than Level 0 stuff (which isn't even read
  // from a text file, so this probably doesn't matter. But we can
  // revisit this if needed.
  std::vector<unsigned long long> sll = config.value<std::vector<unsigned long long> >("signedlonglong");
  BOOST_CHECK(sll[0] == 18446744073709551615ULL);
  BOOST_CHECK(sll[1] == 0);
  BOOST_CHECK(sll[2] == (unsigned long long) -2);
}

BOOST_AUTO_TEST_SUITE_END()