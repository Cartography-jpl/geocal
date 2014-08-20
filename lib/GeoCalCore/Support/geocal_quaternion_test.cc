#include "unit_test_support.h"
#include "geocal_quaternion.h"    
#include "constant.h"
using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(geocal_quaternion, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  boost::math::quaternion<double> q(-0.3087939999536268,    
				    -0.8376664361461180,     
				    0.3189198092995629,    
				    -0.3182001926197278);
  Array<double, 2> m = quaternion_to_matrix(q);
  boost::math::quaternion<double> q2 = matrix_to_quaternion(m);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  // Shuffle the largest value around the quaternion, so we try all
  // branches in matrix_to_quaternion.
  q = boost::math::quaternion<double>(-0.8376664361461180,     
				      -0.3087939999536268,    
				      0.3189198092995629,    
				      -0.3182001926197278);
  m = quaternion_to_matrix(q);
  q2 = matrix_to_quaternion(m);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  q = boost::math::quaternion<double>(-0.3087939999536268,    
				      0.3189198092995629,    
				      -0.8376664361461180,     
				      -0.3182001926197278);
  m = quaternion_to_matrix(q);
  q2 = matrix_to_quaternion(m);
  BOOST_CHECK(l1(q - q2) < 1e-8);
  q = boost::math::quaternion<double>(-0.3087939999536268,    
				      0.3189198092995629,    
				      -0.3182001926197278,
				      -0.8376664361461180);
  m = quaternion_to_matrix(q);
  q2 = matrix_to_quaternion(m);
  BOOST_CHECK(l1(q - q2) < 1e-8);
}

BOOST_AUTO_TEST_CASE(quat_to_ypr_test)
{
  boost::math::quaternion<double> q = quat_rot("xyz", 
					       1 * Constant::deg_to_rad, 
					       2 * Constant::deg_to_rad, 
					       3 * Constant::deg_to_rad);
  double yaw, pitch, roll;
  quat_to_ypr(q, yaw, pitch, roll);
  BOOST_CHECK_CLOSE(pitch * Constant::rad_to_deg, 1, 1e-8);
  BOOST_CHECK_CLOSE(roll * Constant::rad_to_deg, 2, 1e-8);
  BOOST_CHECK_CLOSE(yaw * Constant::rad_to_deg, 3, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()

