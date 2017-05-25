#include "unit_test_support.h"
#include "spice_planet_orbit.h"
#include "spice_helper.h"
#include "planet_coordinate.h"
#include "galileo_camera.h"
#include "vicar_lite_file.h"
#include "orbit_data_image_ground_connection.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(spice_planet_orbit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  std::vector<std::string> klist;
  klist.push_back(test_data_dir() + "/galileo_kernel/galileo.ker");
  SpicePlanetOrbit orb("GLL", "GLL_SCAN_PLANE", klist,
		       PlanetConstant::EUROPA_NAIF_CODE);
  BOOST_CHECK_EQUAL(orb.target_name(), "GLL");
  BOOST_CHECK_EQUAL(orb.spacecraft_reference_frame_name(), "GLL_SCAN_PLANE");
  BOOST_CHECK_EQUAL(orb.naif_id(), (int) PlanetConstant::EUROPA_NAIF_CODE);
  BOOST_CHECK_EQUAL(orb.kernel_list()[0], klist[0]);
  double day_to_sec = 24 * 60 * 60;
  Time tm3800 = Time::parse_time("1997-01-01T12:18:20.130Z") + 
    (350 - 1) * day_to_sec + 1;
  boost::shared_ptr<OrbitData> od = orb.orbit_data(tm3800);
  BOOST_CHECK_CLOSE(od->position_cf()->latitude(), -2.61439, 5e-4);
  BOOST_CHECK_CLOSE(od->position_cf()->longitude(), -152.651, 5e-4);
  boost::shared_ptr<Dem> dem(new PlanetSimpleDem(PlanetConstant::EUROPA_NAIF_CODE));
  boost::shared_ptr<RasterImage> 
    img(new VicarLiteRasterImage(test_data_dir() + "3800r.img"));
  boost::shared_ptr<Camera> cam(new GalileoCamera());
  OrbitDataImageGroundConnection igc(od, cam, dem, img);
  Planetocentric gp(*igc.ground_coordinate(ImageCoordinate(399,399)));
  BOOST_CHECK_CLOSE(gp.latitude(), -15.8989, 1e-3);
  BOOST_CHECK_CLOSE(gp.longitude(), 164.426, 1e-3);
  BOOST_CHECK_EQUAL(gp.naif_code(), (int) PlanetConstant::EUROPA_NAIF_CODE);
  SpicePlanetOrbit orb2("GLL", "GLL_SCAN_PLANE",
			PlanetConstant::EUROPA_NAIF_CODE);
  boost::shared_ptr<OrbitData> od2 =
    orb2.orbit_data(TimeWithDerivative(tm3800));
  BOOST_CHECK_CLOSE(od2->position_cf()->latitude(), -2.61439, 5e-4);
  BOOST_CHECK_CLOSE(od2->position_cf()->longitude(), -152.651, 5e-4);
  // Exercise printing, although we don't actually check anything
  // here. Just make sure no errors occur
  std::ostringstream os;
  os << orb << "\n";
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!SpiceHelper::spice_available()) {
    BOOST_WARN_MESSAGE(false, "Not configured to use SPICE library, so skipping Spice tests.");
    return;
  }
  if(!have_serialize_supported())
    return;
  std::vector<std::string> klist;
  klist.push_back(test_data_dir() + "/galileo_kernel/galileo.ker");
  boost::shared_ptr<Orbit> orb
    (new SpicePlanetOrbit("GLL", "GLL_SCAN_PLANE", klist,
			  PlanetConstant::EUROPA_NAIF_CODE));
  std::string d = serialize_write_string(orb);
  if(false)
    std::cerr << d;
  boost::shared_ptr<SpicePlanetOrbit> orbr = 
    serialize_read_string<SpicePlanetOrbit>(d);
  BOOST_CHECK_EQUAL(orbr->target_name(), "GLL");
  BOOST_CHECK_EQUAL(orbr->spacecraft_reference_frame_name(), "GLL_SCAN_PLANE");
  BOOST_CHECK_EQUAL(orbr->naif_id(), (int) PlanetConstant::EUROPA_NAIF_CODE);
  BOOST_CHECK_EQUAL(orbr->kernel_list()[0], klist[0]);
}

BOOST_AUTO_TEST_SUITE_END()

