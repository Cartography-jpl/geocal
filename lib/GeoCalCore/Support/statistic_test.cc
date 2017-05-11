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

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Statistic> stat(new Statistic);
  *stat += 10;
  for(int i = -100 ; i > -110 ; i--)
    *stat += i;
  std::string d = serialize_write_string(stat);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Statistic> statr = 
    serialize_read_string<Statistic>(d);
  BOOST_CHECK_CLOSE(statr->min(), -109.0, 1e-8);
  BOOST_CHECK_CLOSE(statr->max(), 10.0, 1e-8);
  BOOST_CHECK_CLOSE(statr->mean(), -94.09090909090909300000, 1e-8);
  BOOST_CHECK_CLOSE(statr->mean_abs(), 95.90909090909090909090, 1e-8);
  BOOST_CHECK_CLOSE(statr->sigma(), 34.64232828622967700000, 1e-8);
  BOOST_CHECK_EQUAL(statr->min_index(), 10);
  BOOST_CHECK_EQUAL(statr->max_index(), 0);
  BOOST_CHECK_EQUAL(statr->count(), 11);
}  
  

BOOST_AUTO_TEST_SUITE_END()
