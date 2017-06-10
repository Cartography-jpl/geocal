#include "unit_test_support.h"
#include <cstdlib>
#include "geocal_time.h"    
#include "boost/date_time/posix_time/posix_time.hpp"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_time, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time t = Time::time_pgs(100.0);
  BOOST_CHECK_CLOSE(t.pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE((t + 200.0).pgs(), 100.0 + 200.0, 1e-6);
  BOOST_CHECK_CLOSE((t - 200.0).pgs(), 100.0 - 200.0, 1e-6);
  t = Time::time_gps(100.0);
  BOOST_CHECK_CLOSE(t.gps(), 100.0, 1e-6);
  Time  t2 = t;
  t2 += 10;
  BOOST_CHECK(t < t2);
  BOOST_CHECK(t2 > t);
  BOOST_CHECK(t2 <= t2);
  BOOST_CHECK(t2 >= t2);
  BOOST_CHECK_CLOSE(t2 - t, 10.0, 1e-6);
  Time r = Time::parse_time("1996-07-03T04:13:57.987654Z");
  BOOST_CHECK_CLOSE(r.pgs(), 110520840.987654, 1e-6);
  r += 2.0 * 365 * 24 * 60 * 60; // This goes through a leapsecond
  BOOST_CHECK_EQUAL(r.to_string(), 
		    std::string("1998-07-03T04:13:56.987654Z"));

  // This example was extracted from the unit test for the vicar
  // proc sc2rpc.
  t = Time::time_acs(215077459.472);
  BOOST_CHECK_CLOSE(t.j2000(), 215077524.656, 1e-6);
  BOOST_CHECK_CLOSE(t.acs(), 215077459.472, 1e-6);

  // J2000 is the same system used the SPICE kernel, so make sure
  // these agree
  BOOST_CHECK_CLOSE(Time::time_j2000(1).et(), 1.0, 1e-6);
  BOOST_CHECK_CLOSE(Time::time_et(100).j2000(), 100.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(time_with_derivative)
{
  AutoDerivative<double> tm(100.0, 0, 10);
  TimeWithDerivative t = TimeWithDerivative::time_pgs(tm);
  BOOST_CHECK_CLOSE(t.value().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE((t + 200.0).value().pgs(), 100.0 + 200.0, 1e-6);
  BOOST_CHECK_CLOSE((t - 200.0).value().pgs(), 100.0 - 200.0, 1e-6);
  t = TimeWithDerivative::time_gps(tm);
  BOOST_CHECK_CLOSE(t.gps().value(), 100.0, 1e-6);
  TimeWithDerivative  t2 = t;
  t2 += 10;
  BOOST_CHECK(t < t2);
  BOOST_CHECK(t2 > t);
  BOOST_CHECK(t2 <= t2);
  BOOST_CHECK(t2 >= t2);
  BOOST_CHECK_CLOSE((t2 - t).value(), 10.0, 1e-6);
}

BOOST_AUTO_TEST_CASE(unix_time_interface)
{
  // Even if we have a different toolkit available, good to test the
  // unix interface to make sure it is working.
  ToolkitTimeInterface* original_toolkit = Time::toolkit_time_interface;  
  try {
    Time::toolkit_time_interface = Time::_unix_toolkit_time_interface;
    Time t = Time::time_pgs(100.0);
    BOOST_CHECK_CLOSE(t.pgs(), 100.0, 1e-6);
    BOOST_CHECK_CLOSE((t + 200.0).pgs(), 100.0 + 200.0, 1e-6);
    BOOST_CHECK_CLOSE((t - 200.0).pgs(), 100.0 - 200.0, 1e-6);
    t = Time::time_gps(100.0);
    BOOST_CHECK_CLOSE(t.gps(), 100.0, 1e-6);
    Time  t2 = t;
    t2 += 10;
    BOOST_CHECK(t < t2);
    BOOST_CHECK(t2 > t);
    BOOST_CHECK(t2 <= t2);
    BOOST_CHECK(t2 >= t2);
    BOOST_CHECK_CLOSE(t2 - t, 10.0, 1e-6);
    Time r = Time::parse_time("1996-07-03T04:13:57.987654Z");
    BOOST_CHECK_CLOSE(r.pgs(), 110520840.987654, 1e-6);
    BOOST_CHECK_EQUAL(r.to_string(), "1996-07-03T04:13:57.987654Z");
    r += 2.0 * 365 * 24 * 60 * 60; // This goes through a leapsecond
    BOOST_CHECK_EQUAL(r.to_string(), 
		      std::string("1998-07-03T04:13:56.987654Z"));
  } catch(...) {
    Time::toolkit_time_interface = original_toolkit;
    throw;
  }
  Time::toolkit_time_interface = original_toolkit;
}

BOOST_AUTO_TEST_CASE(timing_test)
{
  Time t;
  for(int i = 0; i < 200000; ++i)
    t = Time::time_acs(215077459.472 + i);

  t = Time::time_acs(215077459.472);
  BOOST_CHECK_CLOSE(t.j2000(), 215077524.656, 1e-6);
  BOOST_CHECK_CLOSE(t.acs(), 215077459.472, 1e-6);
}

BOOST_AUTO_TEST_CASE(problem_time)
{
  // We encountered some times where str2et_c parsed slightly
  // incorrectly, because of minor rounding in going from et to pgs
  // time we have Time in. Have a unit test the illustrates the old
  // problem, which we've tried to fix by explicitly handling
  // fractional seconds separately.
  BOOST_CHECK_EQUAL(Time::parse_time("2015-01-24T20:43:46.428490Z").to_string(),
		    std::string("2015-01-24T20:43:46.428490Z"));
}

BOOST_AUTO_TEST_SUITE_END()

