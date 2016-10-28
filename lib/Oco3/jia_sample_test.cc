#include "unit_test_support.h"
#include "jia_sample.h"

using namespace GeoCal;

// We use the boost unit test framework
BOOST_FIXTURE_TEST_SUITE(jia_sample, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  JiaSample js(10);
  BOOST_CHECK_EQUAL(js.blah(), 10);
  js.jia_func();
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<JiaSample> js(new JiaSample(10));
  std::string d = serialize_write_string(js);
  if(true)
    std::cerr << d;
  boost::shared_ptr<JiaSample> jsr = 
    serialize_read_string<JiaSample>(d);
  BOOST_CHECK_EQUAL(js->blah(), 10);
}
BOOST_AUTO_TEST_SUITE_END()

  

