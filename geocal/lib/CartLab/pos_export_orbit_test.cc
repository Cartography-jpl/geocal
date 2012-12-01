#include "unit_test_support.h"
#include "pos_export_orbit.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(pos_export_orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time epoch = Time::time_pgs(100);
  PosExportOrbit 
    orb(test_data_dir() + "s251_sbet_subset.txt", epoch);
  BOOST_CHECK_CLOSE(orb.min_time() - epoch, 380430.32656, 1e-4);
  BOOST_CHECK_CLOSE(orb.max_time() - epoch, 380431.99657, 1e-4);
  Time t = epoch + 380431.99158;
  const AircraftOrbitData& od = orb.aircraft_orbit_data(t);
  BOOST_CHECK_CLOSE(od.roll(), 0.080, 1e-2);
  BOOST_CHECK_CLOSE(od.pitch(), 0.183, 1e-2);
  BOOST_CHECK_CLOSE(od.heading(), 271.101, 1e-2);
  BOOST_CHECK(distance(od.position_geodetic(), *orb.position_cf(t)) < 1);
}

BOOST_AUTO_TEST_SUITE_END()

