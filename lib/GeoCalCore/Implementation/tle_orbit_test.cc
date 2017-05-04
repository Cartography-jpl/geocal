#include "unit_test_support.h"
#include "tle_orbit.h"
#include "spice_helper.h"
#include "geodetic.h"

using namespace GeoCal;

// TLE comes from http://www.n2yo.com/satellite/?s=25544, and this is
// for ISS
const std::string test_tle =
"1 25544U 98067A   15299.90708094  .00011146  00000-0  17215-3 0  9995\n"
"2 25544  51.6444 146.6664 0006740  82.5539   7.0440 15.54629204968532\n";

BOOST_FIXTURE_TEST_SUITE(tle_orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  TleOrbit orb(test_tle);
  Time tepoch_expect = Time::parse_time("2015-10-26T21:46:11.793215Z");
  BOOST_CHECK(fabs(orb.epoch() - tepoch_expect)  < 1.0);
  Time t = Time::parse_time("2015-10-27T00:05:10Z");
  Geodetic pexpect(-51.75326134, 20.06286501, 425523.669914);
  BOOST_CHECK(distance(*orb.position_cf(t), pexpect) < 1.0);
  BOOST_CHECK_EQUAL(orb.revolution_number_at_epoch(), 96853);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  boost::shared_ptr<Orbit> orb(new TleOrbit(test_tle));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<TleOrbit> orbr = 
    serialize_read_string<TleOrbit>(d);
  BOOST_CHECK_EQUAL(orbr->tle(), test_tle);
}

BOOST_AUTO_TEST_SUITE_END()

