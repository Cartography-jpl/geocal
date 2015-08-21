#include "unit_test_support.h"
#include "piecewise_linear.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(piecewise_linear, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  Array<Time, 1> x(7);
  x = Time::time_pgs(10),Time::time_pgs(15),Time::time_pgs(20),
    Time::time_pgs(25),Time::time_pgs(35),Time::time_pgs(40),Time::time_pgs(45);
  Array<int, 1> t(7-1);
  t = PiecewiseLinear::CONSTANT, 
    PiecewiseLinear::LINEAR,
    PiecewiseLinear::LINEAR,
    PiecewiseLinear::CONSTANT, 
    PiecewiseLinear::CONSTANT,     
    PiecewiseLinear::LINEAR_TO_ZERO;
  Array<double, 1> p(3);
  p = 1,2,3;
  PiecewiseLinear s(x, p, t);
  BOOST_CHECK_CLOSE(s.value(x(0)), p(0), 1e-4);
  BOOST_CHECK_CLOSE(s.value(x(1)), p(0), 1e-4);
  BOOST_CHECK_CLOSE(s.value(x(2)), p(1), 1e-4);
  BOOST_CHECK_CLOSE(s.value(x(3)), p(2), 1e-4);
  BOOST_CHECK_CLOSE(s.value(x(5)), p(2), 1e-4);
  BOOST_CHECK_CLOSE(s.value(x(6)), 0.0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  Array<Time, 1> x(7);
  x = Time::time_pgs(10),Time::time_pgs(15),Time::time_pgs(20),
    Time::time_pgs(25),Time::time_pgs(35),Time::time_pgs(40),Time::time_pgs(45);
  Array<int, 1> t(7-1);
  t = PiecewiseLinear::CONSTANT, 
    PiecewiseLinear::LINEAR,
    PiecewiseLinear::LINEAR,
    PiecewiseLinear::CONSTANT, 
    PiecewiseLinear::CONSTANT,     
    PiecewiseLinear::LINEAR_TO_ZERO;
  Array<double, 1> p(3);
  p = 1,2,3;
  boost::shared_ptr<PiecewiseLinear> pl
    (new PiecewiseLinear(x,p,t));
  std::string d = serialize_write_string(pl);
  if(false)
    std::cerr << d;
  boost::shared_ptr<PiecewiseLinear> plr =
    serialize_read_string<PiecewiseLinear>(d);
}

BOOST_AUTO_TEST_SUITE_END()
