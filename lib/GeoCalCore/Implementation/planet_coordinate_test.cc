#include "unit_test_support.h"
#include "planet_coordinate.h"
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

#include "SpiceUsr.h"

BOOST_FIXTURE_TEST_SUITE(galileo, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  SpiceHelper::add_kernel(test_data_dir() + "/galileo_kernels",
			  "galileo.ker");
  double day_to_sec = 24 * 60 * 60;
  // We can write a general conversion from the time given in VICAR
  // to time. But this is "day 350". The -1 is because we start with
  // day 1 (not 0), and the +1 is because we had a leapsecond in
  // 1997. 
  Time tm3800 = Time::parse_time("1997-01-01T12:18:20.130Z") + 
    (350 - 1) * day_to_sec + 1;
  Time tm2828 = Time::parse_time("1997-01-01T12:08:31.202Z") + 
    (350 - 1) * day_to_sec + 1;
  Time tm9400 = Time::parse_time("1996-01-01T19:50:52.696Z") + 
    (311 - 1) * day_to_sec;
  std::cerr << tm3800 << "\n";
  std::cerr << tm2828 << "\n";
  std::cerr << tm9400 << "\n";
  double state[6], lt;
  //spkezr_c("-77036", tm.et(), "IAU_EUROPA", "NONE", "EUROPA", state, &lt);
  spkezr_c("-77", tm3800.et(), "IAU_EUROPA", "NONE", "EUROPA", state, &lt);
  SpiceHelper::spice_error_check();
  std::cerr << "Distance: " << sqrt(state[0] * state[0] + state[1] * state[1] + state[2] * state[2]) << "\n";
  EuropaFixed p3800(state[0] * 1000.0, state[1] * 1000, state[2] * 1000);

  spkezr_c("-77", tm2828.et(), "IAU_EUROPA", "NONE", "EUROPA", state, &lt);
  SpiceHelper::spice_error_check();
  std::cerr << "Distance: " << sqrt(state[0] * state[0] + state[1] * state[1] + state[2] * state[2]) << "\n";
  EuropaFixed p2828(state[0] * 1000.0, state[1] * 1000, state[2] * 1000);

  spkezr_c("-77", tm9400.et(), "IAU_EUROPA", "NONE", "EUROPA", state, &lt);
  SpiceHelper::spice_error_check();
  std::cerr << "Distance: " << sqrt(state[0] * state[0] + state[1] * state[1] + state[2] * state[2]) << "\n";
  EuropaFixed p9400(state[0] * 1000.0, state[1] * 1000, state[2] * 1000);
  std::cerr << EuropaPlanetocentric(p3800) << "\n"
	    << EuropaPlanetocentric(p2828) << "\n"
	    << EuropaPlanetocentric(p9400) << "\n";
}
BOOST_AUTO_TEST_SUITE_END()
