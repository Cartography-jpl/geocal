#include "unit_test_support.h"
#include "orbit_des.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(orbit_des, GlobalFixture)

BOOST_AUTO_TEST_CASE(pos_csephb)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  PosCsephb p(orb, 1.0);
  // BOOST_CHECK_CLOSE(orb.min_time() - t, 0.0, 1e-4);
  // BOOST_CHECK_CLOSE(orb.max_time() - t, 100.0, 1e-4);
  // BOOST_CHECK_CLOSE(orb.position_ci(t)->position[0], -1788501.0, 1e-4);
  // BOOST_CHECK_CLOSE(orb.position_ci(t)->position[1], -6854177.0, 1e-4);
  // BOOST_CHECK_CLOSE(orb.position_ci(t)->position[2], -16811.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(serialization_pos_csephb)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit orb(t, t + 100.0);
  boost::shared_ptr<PosCsephb> p(new PosCsephb(orb, 1.0));
  std::string d = serialize_write_string(p);
  if(false)
    std::cerr << d;
  boost::shared_ptr<PosCsephb> pr = 
    serialize_read_string<PosCsephb>(d);
  // BOOST_CHECK_CLOSE(orbr->min_time() - t, 0.0, 1e-4);
  // BOOST_CHECK_CLOSE(orbr->max_time() - t, 100.0, 1e-4);
  // BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[0], -1788501.0, 1e-4);
  // BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[1], -6854177.0, 1e-4);
  // BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[2], -16811.0, 1e-3);
}
BOOST_AUTO_TEST_SUITE_END()

