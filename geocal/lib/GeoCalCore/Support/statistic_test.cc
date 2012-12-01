#include "unit_test_support.h"
#include "statistic.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(statistic, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Statistic stat;
  BOOST_CHECK_CLOSE(stat.min(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.max(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean_abs(), 0.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.sigma(), 0.0, 1e-8);
  BOOST_CHECK_EQUAL(stat.min_index(), 0);
  BOOST_CHECK_EQUAL(stat.max_index(), 0);
  BOOST_CHECK_EQUAL(stat.count(), 0);
  stat += 10;
  BOOST_CHECK_CLOSE(stat.min(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.max(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean_abs(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.sigma(), 0.0, 1e-8);
  BOOST_CHECK_EQUAL(stat.min_index(), 0);
  BOOST_CHECK_EQUAL(stat.max_index(), 0);
  BOOST_CHECK_EQUAL(stat.count(), 1);
  for(int i = -100 ; i > -110 ; i--)
    stat += i;
  BOOST_CHECK_CLOSE(stat.min(), -109.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.max(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean(), -94.09090909090909300000, 1e-8);
  BOOST_CHECK_CLOSE(stat.mean_abs(), 95.90909090909090909090, 1e-8);
  BOOST_CHECK_CLOSE(stat.sigma(), 34.64232828622967700000, 1e-8);
  BOOST_CHECK_EQUAL(stat.min_index(), 10);
  BOOST_CHECK_EQUAL(stat.max_index(), 0);
  BOOST_CHECK_EQUAL(stat.count(), 11);
}

BOOST_AUTO_TEST_SUITE_END()
