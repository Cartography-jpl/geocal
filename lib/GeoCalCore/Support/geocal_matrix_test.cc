#include "unit_test_support.h"
#include "geocal_matrix.h"    

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_matrix, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  double m[3][3] = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};
  boost::array<double, 3> x = {{10, 11, 12}};
  boost::array<double, 3> y;
  mul(m, x, y);
  BOOST_CHECK_CLOSE(y[0], 1.0 * 10 + 2 * 11 + 3 * 12, 1e-4);
  BOOST_CHECK_CLOSE(y[1], 4.0 * 10 + 5 * 11 + 6 * 12, 1e-4);
  BOOST_CHECK_CLOSE(y[2], 7.0 * 10 + 8 * 11 + 9 * 12, 1e-4);
  mul_t(m, x, y);
  BOOST_CHECK_CLOSE(y[0], 1.0 * 10 + 4 * 11 + 7 * 12, 1e-4);
  BOOST_CHECK_CLOSE(y[1], 2.0 * 10 + 5 * 11 + 8 * 12, 1e-4);
  BOOST_CHECK_CLOSE(y[2], 3.0 * 10 + 6 * 11 + 9 * 12, 1e-4);
  boost::array<double, 3>  z = {{13, 14, 15}};
  BOOST_CHECK_CLOSE(dot(x, z), 10.0 * 13 + 11 * 14 + 12 * 15, 1e-4);
  BOOST_CHECK_CLOSE(norm(x), sqrt(10.0 * 10 + 11 * 11 + 12 * 12), 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

