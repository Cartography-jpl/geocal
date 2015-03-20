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

BOOST_AUTO_TEST_CASE(quat_to_euler_test)
{
  boost::math::quaternion<double> q = quat_rot("zyx", 
					       1 * Constant::deg_to_rad, 
					       2 * Constant::deg_to_rad, 
					       3 * Constant::deg_to_rad);
  double epsilon, beta, delta;
  quat_to_euler(q, epsilon, beta, delta);
  BOOST_CHECK_CLOSE(epsilon * Constant::rad_to_deg, 1, 1e-8);
  BOOST_CHECK_CLOSE(beta * Constant::rad_to_deg, 2, 1e-8);
  BOOST_CHECK_CLOSE(delta * Constant::rad_to_deg, 3, 1e-8);
}

BOOST_AUTO_TEST_CASE(determine_quat_rot_test)
{
  boost::array<double, 3> v1 = {{1,2,3}};
  boost::array<double, 3> v2 = {{4,5,6}};
  normalize(v1);
  normalize(v2);
  boost::math::quaternion<double> q = determine_quat_rot(v1, v2);
  boost::math::quaternion<double> v1_q(0, v1[0], v1[1], v1[2]);
  boost::math::quaternion<double> v2_q = q * v1_q * conj(q);
  boost::array<double, 3> v2_c = {{v2_q.R_component_2(),
				   v2_q.R_component_3(),
				   v2_q.R_component_4()}};
  for(int i = 0; i < 3; ++i)
    BOOST_CHECK_CLOSE(v2_c[0], v2[0], 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

