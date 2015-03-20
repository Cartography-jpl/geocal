#include "unit_test_support.h"
#include "orbit_data_image_ground_connection.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(orbit_data_image_ground_connection, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  Time tmin = Time::parse_time("2003-01-01T11:11:00Z");
  KeplerOrbit orb;
  boost::shared_ptr<OrbitData> od = orb.orbit_data(tmin);
  boost::shared_ptr<Camera> cam(new SimpleCamera);
  boost::shared_ptr<Dem> dem(new SimpleDem(100));
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(cam->number_line(0),
						   cam->number_sample(0)));
  OrbitDataImageGroundConnection igc(od, cam, dem, img);
  ImageCoordinate ic(1.0, 1504 / 2);
  Geodetic g(*igc.ground_coordinate(ic));
  BOOST_CHECK_CLOSE(g.latitude(), -4.8406639328445964, 1e-2);
  BOOST_CHECK_CLOSE(g.longitude(), 165.5531678459437, 1e-2);
  BOOST_CHECK_CLOSE(g.height_reference_surface(), 100, 1e-2);
  Geodetic g2(*igc.ground_coordinate_approx_height(ic, 100.0));
  BOOST_CHECK_CLOSE(g2.latitude(), -4.8406639329424825, 1e-2);
  BOOST_CHECK_CLOSE(g2.longitude(), 165.55316784592461, 1e-2);
  BOOST_CHECK_CLOSE(g2.height_reference_surface(), 100, 1e-2);
  ImageCoordinate ic_calc = igc.image_coordinate(g);
  BOOST_CHECK_CLOSE(ic_calc.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_calc.sample, ic.sample, 1e-2);
  Ecr ecr(g);
  blitz::Array<double, 2> jac = igc.image_coordinate_jac_cf(ecr);
  ImageCoordinate ic0 = igc.image_coordinate(ecr);
  double eps = 10;
  ImageCoordinate ic2;
  for(int i = 0; i < 3; ++i) {
    ecr.position[i] += eps;
    ic2 = igc.image_coordinate(ecr);
    ecr.position[i] -= eps;
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac(0, i), 1e-1);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac(1, i), 1e-1);
  }
}

BOOST_AUTO_TEST_CASE(include_refraction)
{
  Time tmin = Time::parse_time("2003-01-01T11:11:00Z");
  KeplerOrbit orb;
  boost::shared_ptr<OrbitData> od = orb.orbit_data(tmin);
  boost::shared_ptr<Camera> cam(new SimpleCamera);
  boost::shared_ptr<Dem> dem(new SimpleDem(100));
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(cam->number_line(0),
						   cam->number_sample(0)));
  boost::shared_ptr<Refraction> ref(new Refraction(0,0));
  OrbitDataImageGroundConnection igc(od, cam, dem, img, "Image", ref);
  ImageCoordinate ic(1.0, 1504 / 2);
  Geodetic g(*igc.ground_coordinate(ic));
  BOOST_CHECK_CLOSE(g.latitude(), -4.8400987761922023, 1e-2);
  BOOST_CHECK_CLOSE(g.longitude(), 165.55328228878236, 1e-2);
  BOOST_CHECK_CLOSE(g.height_reference_surface(), 100, 1e-2);
  Geodetic g2(*igc.ground_coordinate_approx_height(ic, 100));
  BOOST_CHECK_CLOSE(g2.latitude(), -4.840098776208313, 1e-2);
  BOOST_CHECK_CLOSE(g2.longitude(), 165.55328228878048, 1e-2);
  BOOST_CHECK_CLOSE(g2.height_reference_surface(), 100, 1e-2);
  ImageCoordinate ic_calc = igc.image_coordinate(g);
  BOOST_CHECK_CLOSE(ic_calc.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_calc.sample, ic.sample, 1e-2);
  Ecr ecr(g);
  blitz::Array<double, 2> jac = igc.image_coordinate_jac_cf(ecr);
  ImageCoordinate ic0 = igc.image_coordinate(ecr);
  double eps = 0.1;
  ImageCoordinate ic2;
  for(int i = 0; i < 3; ++i) {
    ecr.position[i] += eps;
    ic2 = igc.image_coordinate(ecr);
    ecr.position[i] -= eps;
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac(0, i), 1e-1);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac(1, i), 1e-1);
  }
}

BOOST_AUTO_TEST_SUITE_END()

