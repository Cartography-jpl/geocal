#include "unit_test_support.h"
#include "orbit.h"
#include "refraction_msp.h"
#include "simple_dem.h"
#include "constant.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(refraction_msp, GlobalFixture)

BOOST_AUTO_TEST_CASE(refraction_msp)
{
  // Nominal orbit, with a fairly steep angle (MISR DF camera)
  Time tmin = Time::parse_time("2003-01-01T11:11:00Z");
  KeplerOrbit orb;
  boost::shared_ptr<OrbitData> od = orb.orbit_data(tmin);
  SimpleCamera cam;
  FrameCoordinate fc(0, 1504 / 2);
  SimpleDem dem;
  boost::shared_ptr<GroundCoordinate> gc = od->surface_intersect(cam, fc, dem);
  RefractionMsp ref;
  boost::shared_ptr<GroundCoordinate> gc_corr =
    dem.intersect(*od->position_cf(), 
		  ref.refraction_apply(*od->position_cf(), *gc), 10);

  // Simple check that we have signs correct. Refraction should bring
  // the ground point closer to the satellite.
  BOOST_CHECK(distance(*gc_corr, *od->position_cf()) <
   	      distance(*gc, *od->position_cf()));
  // From table in 6-5 in "Theoretical Basis of the SDP
  // Toolkit Geolocation Package for the ECS" the distance of the
  // correction should be about 58 meters (for space zenith angle of
  // 70.000). This particular test has a space zenith of 70.7622. We
  // also have a more direct comparison against the MSP library in
  // python (See test_refraction_gfm in glas_gfm_test.py). Have the
  // results we previously got here, that we check to make sure is
  // unchanged. This is about 6m off of RefractionSdp. Possible there
  // is a small error in RefractionSdp, but this refraction code is
  // more sophisticated (but slower) than the SDP toolkit so the
  // difference might be that.
  
  BOOST_CHECK_CLOSE(distance(*gc, *gc_corr), 57.60567529796468, 1e-2);

  boost::shared_ptr<GroundCoordinate> gc_reverse =
    dem.intersect(*od->position_cf(), 
		  ref.refraction_reverse(*od->position_cf(), *gc_corr), 10);
  BOOST_CHECK(distance(*gc_reverse, *gc) < 0.01);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RefractionMsp> r = boost::make_shared<RefractionMsp>();
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RefractionMsp> rr = 
    serialize_read_string<RefractionMsp>(d);
  BOOST_CHECK_CLOSE(rr->wavelength(), r->wavelength(), 1e-6);
  BOOST_CHECK_CLOSE(rr->temperature(), r->temperature(), 1e-6);
  BOOST_CHECK_CLOSE(rr->pressure(), r->pressure(), 1e-6);
}

BOOST_AUTO_TEST_SUITE_END()
