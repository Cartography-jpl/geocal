#include "unit_test_support.h"
#include "ipi_image_ground_connection.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "rsm_base.h"
#include "planet_coordinate.h"
#include <cmath>

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ipi_image_ground_connection, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 1000 * 40.8e-3;
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
  BOOST_CHECK_CLOSE(line_res, 707.97523894017218, 1e-2);
  BOOST_CHECK_CLOSE(samp_res, 275.9268228817025, 1e-2);
}

BOOST_AUTO_TEST_CASE(pushframe_test)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + 1000 * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  int nline_framelet = 10;
  boost::shared_ptr<Camera> cam =
    boost::make_shared<SimpleCamera>(0,0,0,123.8e-3,18e-6,21e-6,
				     nline_framelet, 1500);
  boost::shared_ptr<TimeTable> tt =
    boost::make_shared<ConstantSpacingFrameletTimeTable>(tmin, tmax,
			 nline_framelet, 0.17);
  int band = 0;
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, band, tmin, tmax, tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(100,
					   cam->number_sample(0)));
  auto igc = boost::make_shared<IpiImageGroundConnection>(ipi, dem, img);
  
  ImageCoordinate ic(999, 30);
  boost::shared_ptr<GroundCoordinate> gc = igc->ground_coordinate(ic);
  ImageCoordinate ic2 = igc->image_coordinate(*gc);
  BOOST_CHECK(fabs(ic2.line - ic.line) < 1.0 / 16);
  BOOST_CHECK(fabs(ic2.sample - ic.sample) < 1.0 / 16);
  boost::shared_ptr<GroundCoordinate> gc2 = 
    igc->ground_coordinate_approx_height(ic, 100);
  ImageCoordinate ic3 = igc->image_coordinate(*gc2);
  BOOST_CHECK(fabs(ic3.line - ic.line) < 1.0 / 16);
  BOOST_CHECK(fabs(ic3.sample - ic.sample) < 1.0 / 16);
  BOOST_CHECK_CLOSE(gc2->height_reference_surface(), 100.0, 1e-2);
}

BOOST_AUTO_TEST_CASE(timing_test)
{
  // Don't normally run, this depends on specific test data that isn't
  // available other than on pistol
  return;
  // This has been slow to run. Put this is a unit test so we can look
  // at this with valgrind and try to speed up.
  boost::shared_ptr<IpiImageGroundConnection> igc =
    serialize_read<IpiImageGroundConnection>("/home/smyth/Local/MarsRsm/ctx1_igc.xml");
  //const int nline = 100;
  const int nline = 500;
  blitz::Array<double, 4> t = RsmBase::generate_data(*igc, PlanetocentricConverter(PlanetConstant::MARS_NAIF_CODE), -19.55, -19.02, 23.148, 23.202, 0, 1000, nline, 100, 20);
}

BOOST_AUTO_TEST_SUITE_END()
