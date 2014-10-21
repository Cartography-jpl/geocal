#include "unit_test_support.h"
#include "igc_rolling_shutter.h"
#include "quaternion_camera.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_rolling_shutter, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  Time tmin = Time::parse_time("2003-01-01T11:11:00Z");
  boost::shared_ptr<Orbit> orb(new KeplerOrbit());
  boost::shared_ptr<Camera> cam
    (new QuaternionCamera(boost::math::quaternion<double>(1,0,0,0),
			  3375, 3648, 1.0 / 2500000, 1.0 / 2500000,
			  1.0, FrameCoordinate(1688.0, 1824.5),
			  QuaternionCamera::LINE_IS_Y));
  boost::shared_ptr<Dem> dem(new SimpleDem(100));
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(cam->number_line(0),
						   cam->number_sample(0)));
  double tspace = 1e-3;
  boost::shared_ptr<TimeTable> tt
    (new ConstantSpacingTimeTable(tmin, tmin + cam->number_line(0) * tspace,
				  tspace));
  IgcRollingShutter igc(orb, tt, cam, dem, img);
}

BOOST_AUTO_TEST_SUITE_END()

