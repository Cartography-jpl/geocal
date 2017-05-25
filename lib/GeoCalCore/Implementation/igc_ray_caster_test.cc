#include "unit_test_support.h"
#include "igc_ray_caster.h"
#include "ipi_image_ground_connection.h"
#include "eci.h"
#include "geocal_time.h"
#include "geodetic.h"
#include "wgs84_constant.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "constant_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_ray_caster, GlobalFixture)
BOOST_AUTO_TEST_CASE(aftward_camera)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + (10000 - 1) * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  // Have camera only 10 samples, just so unit test runs faster
  boost::shared_ptr<Camera> 
    cam(new SimpleCamera(-58*Constant::deg_to_rad,
			 -2.7*Constant::deg_to_rad,
			 0, 123.8e-3, 18e-6, 21e-6, 1, 10));
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(tmin, tmax));
  int band = 0;
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, band, tmin, tmax, tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(100,
					   cam->number_sample(0)));
  boost::shared_ptr<ImageGroundConnection> 
    igc(new IpiImageGroundConnection(ipi, dem, img));
  IgcRayCaster rcast(igc);
  BOOST_CHECK(rcast.start_position() == 0);
  BOOST_CHECK(!rcast.last_position());
  BOOST_CHECK(rcast.number_position() == 10000);
  blitz::Array<double, 6> r = rcast.next_position();
  BOOST_CHECK(rcast.current_position() == 10000 - 1);
  r.reference(rcast.next_position());
  BOOST_CHECK(rcast.current_position() == 10000 - 2);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + (10000 - 1) * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  // Have camera only 10 samples, just so unit test runs faster
  boost::shared_ptr<Camera> 
    cam(new SimpleCamera(-58*Constant::deg_to_rad,
			 -2.7*Constant::deg_to_rad,
			 0, 123.8e-3, 18e-6, 21e-6, 1, 10));
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(tmin, tmax));
  int band = 0;
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, band, tmin, tmax, tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  // Use ConstantRasterImage rather than RasterImage here because
  // otherwise we get a valgrind error about using uninitialized memory.
  boost::shared_ptr<RasterImage> img(new ConstantRasterImage(100,
				     cam->number_sample(0), 10));
  boost::shared_ptr<ImageGroundConnection> 
    igc(new IpiImageGroundConnection(ipi, dem, img));
  boost::shared_ptr<RayCaster> rcast(new IgcRayCaster(igc));
  std::string d = serialize_write_string(rcast);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IgcRayCaster> rcastr =
    serialize_read_string<IgcRayCaster>(d);
  BOOST_CHECK(rcastr->start_position() == 0);
  BOOST_CHECK(!rcastr->last_position());
  BOOST_CHECK(rcastr->number_position() == 10000);
  blitz::Array<double, 6> r = rcastr->next_position();
  BOOST_CHECK(rcastr->current_position() == 10000 - 1);
  r.reference(rcastr->next_position());
  BOOST_CHECK(rcastr->current_position() == 10000 - 2);
}

BOOST_AUTO_TEST_CASE(forward_camera)
{
  Time tmin = Time::parse_time("2003-01-01T11:00:00Z");
  Time tmax = tmin + (10000 - 1) * 40.8e-3;
  boost::shared_ptr<Orbit> orb(new KeplerOrbit);
  // Have camera only 10 samples, just so unit test runs faster
  boost::shared_ptr<Camera> 
    cam(new SimpleCamera(58*Constant::deg_to_rad,
			 -2.7*Constant::deg_to_rad,
			 0, 123.8e-3, 18e-6, 21e-6, 1, 10));
  boost::shared_ptr<TimeTable> tt(new ConstantSpacingTimeTable(tmin, tmax));
  int band = 0;
  boost::shared_ptr<Ipi> ipi(new Ipi(orb, cam, band, tmin, tmax, tt));
  boost::shared_ptr<Dem> dem(new SimpleDem());
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(100,
					   cam->number_sample(0)));
  boost::shared_ptr<ImageGroundConnection> 
    igc(new IpiImageGroundConnection(ipi, dem, img));
  IgcRayCaster rcast(igc);
  BOOST_CHECK(rcast.start_position() == 0);
  BOOST_CHECK(!rcast.last_position());
  BOOST_CHECK(rcast.number_position() == 10000);
  blitz::Array<double, 6> r = rcast.next_position();
  BOOST_CHECK(rcast.current_position() == 0);
  r.reference(rcast.next_position());
  BOOST_CHECK(rcast.current_position() == 1);
}

BOOST_AUTO_TEST_SUITE_END()

