#include "unit_test_support.h"
#include "spice_orbit.h"
#include "spice_helper.h"
#include "geodetic.h"
#include "camera.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(spice_orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  SpiceOrbit orb(SpiceOrbit::ISS_ID, test_data_dir() + "iss_kernel/iss.ker");
  // This expected results comes from the TleOrbit test.
  Time t = Time::parse_time("2015-10-27T00:05:10Z");
  Geodetic pexpect(-51.7532613384, 20.0670430907, 425523.669914);
  std::cerr << Geodetic(*orb.position_cf(t)) << "\n";
  std::cerr << distance(*orb.position_cf(t), pexpect) << "\n";
  SimpleCamera ncam(0,0,0);
  FrameCoordinate fc(0.5,1504.0/2);
  std::cerr << ncam.sc_look_vector(fc, 0) << "\n";
  std::cerr << Geodetic(*orb.reference_surface_intersect_approximate(t, ncam, fc)) << "\n";
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Orbit> orb
    (new SpiceOrbit(SpiceOrbit::ISS_ID, test_data_dir() + 
		    "iss_kernel/iss.ker"));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<SpiceOrbit> orbr = 
    serialize_read_string<SpiceOrbit>(d);
}

BOOST_AUTO_TEST_SUITE_END()

