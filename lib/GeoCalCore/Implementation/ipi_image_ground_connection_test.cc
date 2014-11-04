#include "unit_test_support.h"
#include "ipi_image_ground_connection.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ipi_image_ground_connection, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 10000 * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  boost::shared_ptr<Camera> cam(new SimpleCamera);
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(tmin, tmax));
  int band = 0;
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, band, tmin, tmax, tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(100,
					   cam->number_sample(0)));
  IpiImageGroundConnection igc(ipi, dem, img);

  ImageCoordinate ic(999, 30);
  boost::shared_ptr<GroundCoordinate> gc = igc.ground_coordinate(ic);
  ImageCoordinate ic2 = igc.image_coordinate(*gc);
  BOOST_CHECK(fabs(ic2.line - ic.line) < 1.0 / 16);
  BOOST_CHECK(fabs(ic2.sample - ic.sample) < 1.0 / 16);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    igc.ground_coordinate_approx_height(ic, 100);
  ImageCoordinate ic3 = igc.image_coordinate(*gc2);
  BOOST_CHECK(fabs(ic3.line - ic.line) < 1.0 / 16);
  BOOST_CHECK(fabs(ic3.sample - ic.sample) < 1.0 / 16);
  BOOST_CHECK_CLOSE(gc2->height_reference_surface(), 100.0, 1e-2);
  double line_res, samp_res;
  igc.footprint_resolution(0, 0, line_res, samp_res);
  BOOST_CHECK_CLOSE(line_res, 711.43, 1e-2);
  BOOST_CHECK_CLOSE(samp_res, 276.297, 1e-2);
}

BOOST_AUTO_TEST_SUITE_END()
