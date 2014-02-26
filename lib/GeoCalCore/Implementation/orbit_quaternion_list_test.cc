#include "unit_test_support.h"
#include "orbit_quaternion_list.h"
#include "aircraft_orbit_data.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(orbit_quaternion_list, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  Time t1 = Time::time_pgs(100.0);
  Time t2 = Time::time_pgs(102.0);
  Geodetic p1(10, 20, 200);
  Geodetic p2(12, 22, 400);
  boost::shared_ptr<AircraftOrbitData> od1
    (new AircraftOrbitData(t1, p1, t2, p2, 10, 20, 30));
  boost::shared_ptr<AircraftOrbitData> od2
    (new AircraftOrbitData(t2, p2, t1, p1, 20, 30, 20));
  std::vector<boost::shared_ptr<QuaternionOrbitData> > v;
  v.push_back(od2);
  v.push_back(od1);
  OrbitQuaternionList orb(v);
  BOOST_CHECK_CLOSE(orb.min_time().pgs(), 100.0, 1e-6);
  BOOST_CHECK_CLOSE(orb.max_time().pgs(), 102.0, 1e-6);
  BOOST_CHECK(distance(*orb.orbit_data(Time::time_pgs(100.0))->position_cf(),
		       od1->position_geodetic()) < 10);
  Geodetic g1(*orb.orbit_data(Time::time_pgs(101.999))->position_cf());
  BOOST_CHECK(distance(*orb.orbit_data(Time::time_pgs(101.99999))->position_cf(),
		       od2->position_geodetic()) < 10);
  Geodetic g2(*orb.orbit_data(Time::time_pgs(101))->position_cf());
  BOOST_CHECK_CLOSE(orb.orbit_data(Time::time_pgs(101))->position_cf()->
		    latitude(), 11.0, 1.0);
  BOOST_CHECK_CLOSE(orb.orbit_data(Time::time_pgs(101))->position_cf()->
		    longitude(), 21.0, 1.0);
}

BOOST_AUTO_TEST_SUITE_END()
