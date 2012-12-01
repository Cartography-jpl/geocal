#include "unit_test_support.h"
#include "ipi_image_ground_connection.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include <cmath>
#include "config.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ipi_image_ground_connection, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
#ifdef HAVE_TIME_TOOLKIT
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 10000 * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  boost::shared_ptr<PushBroomCamera> cam(new SimplePushBroomCamera);
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
#endif
}

BOOST_AUTO_TEST_SUITE_END()
