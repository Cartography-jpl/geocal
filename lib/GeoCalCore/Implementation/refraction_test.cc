#include "unit_test_support.h"
#include "orbit.h"
#include "refraction.h"
#include "simple_dem.h"
#include "constant.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(refraction_test, GlobalFixture)

BOOST_AUTO_TEST_CASE(displacement_test)
{
  Refraction r(0, 0);
  // These expected results come from the original refraction
  // documentation.
  BOOST_CHECK_CLOSE(r.surface_zenith(10 * Constant::deg_to_rad) *
		    Constant::rad_to_deg, 9.997066, 1e-4);
  BOOST_CHECK_CLOSE(r.surface_zenith(80 * Constant::deg_to_rad) *
		    Constant::rad_to_deg, 79.906069, 1e-4);
  BOOST_CHECK_CLOSE(r.index_refraction_surface(), 1.0002905, 1e-5);
  BOOST_CHECK_CLOSE(r.displacement(10 * Constant::deg_to_rad) * 6371000, 
		    0.5678776, 1e-4);
  BOOST_CHECK_CLOSE(r.displacement(80 * Constant::deg_to_rad) * 6371000, 
		    448.95736, 1e-4);
}

BOOST_AUTO_TEST_CASE(refraction_test)
{
  // Nominal orbit, with a fairly steep angle (MISR DF camera)
  Time tmin = Time::parse_time("2003-01-01T11:11:00Z");
  KeplerOrbit orb;
  boost::shared_ptr<OrbitData> od = orb.orbit_data(tmin);
  SimplePushBroomCamera cam;
  FrameCoordinate fc(0, 1504 / 2);
  SimpleDem dem;
  boost::shared_ptr<GroundCoordinate> gc = od->surface_intersect(cam, fc, dem);
  Refraction ref(0.0, gc->latitude());
  boost::shared_ptr<GroundCoordinate> gc_corr = 
    ref.refraction_apply(*od->position_cf(), *gc);

  // Simple check that we have signs correct. Refraction should bring
  // the ground point closer to the satellite.
  BOOST_CHECK(distance(*gc_corr, *od->position_cf()) <
	      distance(*gc, *od->position_cf()));
  // From table in 6-5 in "Theoretical Basis of the SDP
  // Toolkit Geolocation Package for the ECS" the distance of the
  // correction should be about 58 meters (for space zenith angle of
  // 70.000). This particular test has a space zenith of 70.7622. We
  // get a distance of 63.5987, which seems like a reasonable result.
  BOOST_CHECK_CLOSE(distance(*gc, *gc_corr), 63.5987, 1e-2);

  boost::shared_ptr<GroundCoordinate> gc_reverse = 
    ref.refraction_reverse(*od->position_cf(), *gc_corr);
  BOOST_CHECK(distance(*gc_reverse, *gc) < 0.02);
}

BOOST_AUTO_TEST_SUITE_END()
