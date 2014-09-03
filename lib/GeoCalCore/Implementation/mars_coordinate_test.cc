#include "unit_test_support.h"
#include "mars_coordinate.h"
#include "geocal_time.h"
#include "spice_helper.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(mars_coordinate, GlobalFixture)

BOOST_AUTO_TEST_CASE(mars_constant)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  BOOST_CHECK_CLOSE(MarsConstant::planet_a(), 3396190.0, 1e-8);
  BOOST_CHECK_CLOSE(MarsConstant::planet_b(), 3376200.0, 1e-8);
  BOOST_CHECK_CLOSE(MarsConstant::planet_esq(), 0.0117373700261, 1e-8);
}

BOOST_AUTO_TEST_CASE(mars_fixed)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsFixed mf(10, 20, 30);
  BOOST_CHECK_CLOSE(mf.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.position[2], 30.0, 1e-8);
  boost::shared_ptr<GroundCoordinate> gc = mf.convert_to_cf();
  MarsFixed mf2(*gc);
  BOOST_CHECK_CLOSE(mf2.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mf2.position[2], 30.0, 1e-8);
  BOOST_CHECK_CLOSE(mf.min_radius_reference_surface(), 3376200.0, 1e-8);

  // Results from an old unit test in the old version of GeoCal.
  MarsFixed e3mf(0.5, 0.75, 0.90);
  Time t = Time::parse_time("1991-01-01T11:29:30.123211Z") + 3600.0;
  boost::shared_ptr<CartesianInertial> m3mi = e3mf.convert_to_ci(t);
  BOOST_CHECK(boost::dynamic_pointer_cast<MarsInertial>(m3mi));
  BOOST_CHECK_CLOSE(m3mi->position[0], 1.1578655, 1e-4);
  BOOST_CHECK_CLOSE(m3mi->position[1], -0.4738199, 1e-4);
  BOOST_CHECK_CLOSE(m3mi->position[2], 0.23946256, 1e-4);

  MarsPlanetocentric mp(10, 20, 30);
  MarsFixed mf4(mp);
  BOOST_CHECK_CLOSE(mf4.latitude(), 10, 1e-8);
  BOOST_CHECK_CLOSE(mf4.longitude(), 20, 1e-8);
  BOOST_CHECK_CLOSE(mf4.height_reference_surface(), 30, 1e-8);
}

BOOST_AUTO_TEST_CASE(mars_inertial)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsInertial mi(10, 20, 30);
  BOOST_CHECK_CLOSE(mi.position[0], 10.0, 1e-8);
  BOOST_CHECK_CLOSE(mi.position[1], 20.0, 1e-8);
  BOOST_CHECK_CLOSE(mi.position[2], 30.0, 1e-8);

  // Results from an old unit test in the old version of GeoCal.
  MarsInertial m3mi(0.5, 0.75, 0.90);
  Time t = Time::parse_time("1991-01-01T11:29:30.123211Z") + 3600.0;
  boost::shared_ptr<CartesianFixed> m3mf = m3mi.convert_to_cf(t);
  BOOST_CHECK(boost::dynamic_pointer_cast<MarsFixed>(m3mf));
  BOOST_CHECK_CLOSE(m3mf->position[0], -0.992024, 1e-4);
  BOOST_CHECK_CLOSE(m3mf->position[1], 0.48329419, 1e-4);
  BOOST_CHECK_CLOSE(m3mf->position[2], 0.636251, 1e-4);
}

BOOST_AUTO_TEST_CASE(mars_planetocentric)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  MarsPlanetocentric mp(10, 20, 30);
  BOOST_CHECK_CLOSE(mp.height_reference_surface(), 30, 1e-8);
  BOOST_CHECK_CLOSE(mp.latitude(), 10, 1e-8);
  BOOST_CHECK_CLOSE(mp.longitude(), 20, 1e-8);
  MarsFixed mf(mp);
  MarsPlanetocentric mp2(mf);
  MarsFixed mf2(mp2);
  BOOST_CHECK(distance(mp, mp2) < 1e-8);
  BOOST_CHECK(distance(mf, mf2) < 1e-8);
}
BOOST_AUTO_TEST_SUITE_END()
