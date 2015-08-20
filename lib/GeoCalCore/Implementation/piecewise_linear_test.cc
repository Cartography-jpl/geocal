#include "unit_test_support.h"
#include "piecewise_linear.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(piecewise_linear, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;

  // boost::shared_ptr<PiecewiseLinear> pl
  //   (new PiecewiseLinear());
  // std::string d = serialize_write_string(pl);
  // if(false)
  //   std::cerr << d;
  // boost::shared_ptr<PiecewiseLinear> plr =
  //   serialize_read_string<PiecewiseLinear>(d);
}

BOOST_AUTO_TEST_SUITE_END()
