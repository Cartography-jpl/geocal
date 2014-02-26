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
  boost::math::quaternion<double> q(-0.3087939999536268,    
				    -0.8376664361461180,     
				    0.3189198092995629,    
				    -0.3182001926197278);
  double m2[3][3];
  quaternion_to_matrix(q, m2);
  boost::math::quaternion<double> q2 = matrix_to_quaternion(m2);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  // Shuffle the largest value around the quaternion, so we try all
  // branches in matrix_to_quaternion.
  q = boost::math::quaternion<double>(-0.8376664361461180,     
				      -0.3087939999536268,    
				      0.3189198092995629,    
				      -0.3182001926197278);
  quaternion_to_matrix(q, m2);
  q2 = matrix_to_quaternion(m2);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  q = boost::math::quaternion<double>(-0.3087939999536268,    
				      0.3189198092995629,    
				      -0.8376664361461180,     
				      -0.3182001926197278);
  quaternion_to_matrix(q, m2);
  q2 = matrix_to_quaternion(m2);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  q = boost::math::quaternion<double>(-0.3087939999536268,    
				      0.3189198092995629,    
				      -0.3182001926197278,
				      -0.8376664361461180);
  quaternion_to_matrix(q, m2);
  q2 = matrix_to_quaternion(m2);
  BOOST_CHECK(l1(q - q2) < 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()

