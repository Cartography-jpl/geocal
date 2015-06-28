#include "unit_test_support.h"
#include "covariance.h"
#include <iostream>

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

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Covariance> c(new Covariance());
  c->add(1.0, 2.05);
  c->add(1.1, 2.2);
  c->add(1.2, 1.9);
  c->add(1.3, 1.8);
  c->add(0.9, 2.1);
  BOOST_CHECK_EQUAL(c->count(), 5);
  BOOST_CHECK_CLOSE(c->mean1(), 1.1, 1e-8);
  BOOST_CHECK_CLOSE(c->mean2(), 2.01, 1e-8);
  BOOST_CHECK_CLOSE(c->covariance(), -0.01875, 1e-8);
  std::string d = serialize_write_string(c);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<Covariance> cr =
    serialize_read_string<Covariance>(d);
  BOOST_CHECK_EQUAL(cr->count(), 5);
  BOOST_CHECK_CLOSE(cr->mean1(), 1.1, 1e-8);
  BOOST_CHECK_CLOSE(cr->mean2(), 2.01, 1e-8);
  BOOST_CHECK_CLOSE(cr->covariance(), -0.01875, 1e-8);
}


BOOST_AUTO_TEST_SUITE_END()
