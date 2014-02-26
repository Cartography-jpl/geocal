#include "unit_test_support.h"
#include "covariance.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(covariance, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Covariance c;
  BOOST_CHECK_EQUAL(c.count(), 0);
  BOOST_CHECK_CLOSE(c.mean1(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(c.mean2(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(c.covariance(), 0.0, 1e-8);
  c.add(1.0, 2.05);
  BOOST_CHECK_EQUAL(c.count(), 1);
  BOOST_CHECK_CLOSE(c.mean1(), 1.0, 1e-8);
  BOOST_CHECK_CLOSE(c.mean2(), 2.05, 1e-8);
  BOOST_CHECK_CLOSE(c.covariance(), 0.0, 1e-8);
  c.add(1.1, 2.2);
  c.add(1.2, 1.9);
  c.add(1.3, 1.8);
  c.add(0.9, 2.1);
  BOOST_CHECK_EQUAL(c.count(), 5);
  BOOST_CHECK_CLOSE(c.mean1(), 1.1, 1e-8);
  BOOST_CHECK_CLOSE(c.mean2(), 2.01, 1e-8);
  BOOST_CHECK_CLOSE(c.covariance(), -0.01875, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
