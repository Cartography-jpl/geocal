#include "unit_test_support.h"
#include "orbit_des.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(orbit_des, GlobalFixture)

BOOST_AUTO_TEST_CASE(pos_csephb)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  PosCsephb p(korb, 1.0);
  BOOST_CHECK_CLOSE(p.min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(p.max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(0), -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(1), -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(p.pos_vel(t)(2), -16811.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(orbit_des)
{
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  OrbitDes orb(boost::make_shared<PosCsephb>(korb, 1.0));
  for(Time ti = orb.min_time(); ti < orb.max_time(); ti += 0.5)
    BOOST_CHECK(GeoCal::distance(*orb.position_cf(ti), *korb.position_cf(ti)) < 1.0);
}

BOOST_AUTO_TEST_CASE(serialization_pos_csephb)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  boost::shared_ptr<PosCsephb> p = boost::make_shared<PosCsephb>(korb, 1.0);
  std::string d = serialize_write_string(p);
  if(false)
    std::cerr << d;
  boost::shared_ptr<PosCsephb> pr = 
    serialize_read_string<PosCsephb>(d);
  BOOST_CHECK_CLOSE(pr->min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(0), -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(1), -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(pr->pos_vel(t)(2), -16811.0, 1e-3);
}

BOOST_AUTO_TEST_CASE(serialization_orbit_des)
{
  if(!have_serialize_supported())
    return;
  Time t = Time::parse_time("1998-06-30T10:51:28.32Z");
  KeplerOrbit korb(t, t + 100.0);
  boost::shared_ptr<Orbit> orb =
    boost::make_shared<OrbitDes>(boost::make_shared<PosCsephb>(korb, 1.0));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<Orbit> orbr = 
    serialize_read_string<Orbit>(d);
  BOOST_CHECK_CLOSE(orbr->min_time() - t, 0.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->max_time() - t, 100.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[0], -1788501.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[1], -6854177.0, 1e-4);
  BOOST_CHECK_CLOSE(orbr->position_ci(t)->position[2], -16811.0, 1e-3);
}

BOOST_AUTO_TEST_SUITE_END()

