#include "unit_test_support.h"
#include "igc_simulated_ray_caster.h"
#include "ipi_image_ground_connection.h"
#include "geodetic.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "gdal_raster_image.h"

using namespace GeoCal;

class IgcSimulatedRayCasterFixture : public GlobalFixture {
public:
  IgcSimulatedRayCasterFixture()
  {
    // The 100 * 60 is to get us off the dateline. MapInfo cover
    // function has a bug where it doesn't handle the date line
    // correctly. We need to fix that, but this unit test shouldn't
    // wait on that.
    Time tmin = Time::parse_time("2003-01-01T11:00:00Z") + 100 * 60;
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
    igc.reset(new IpiImageGroundConnection(ipi, dem, img));
    double ulc_x = 50;
    double ulc_y = 60;
    double x_pixel_res = 0.25;
    double y_pixel_res = -0.50;
    int number_x_pixel = 100;
    int number_y_pixel = 200;
    MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	       ulc_x, ulc_y, 
	       ulc_x + x_pixel_res * number_x_pixel, 
	       ulc_y + y_pixel_res * number_y_pixel, 
	       number_x_pixel, number_y_pixel);
    double scl = igc->resolution_meter() / mi.resolution_meter();
    MapInfo mi2 = mi.scale(scl, scl);
    MapInfo mi3 = igc->cover(mi2);
    gimg.reset(new MemoryRasterImage(mi3));
    int val = 0;
    for(int i = 0; i < gimg->number_line(); ++i)
      for(int j = 0; j < gimg->number_sample(); ++j, ++val)
	gimg->write(i, j, val);
  }
  boost::shared_ptr<ImageGroundConnection> igc;
  boost::shared_ptr<MemoryRasterImage> gimg;
};

BOOST_FIXTURE_TEST_SUITE(igc_simulated_ray_caster, IgcSimulatedRayCasterFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  IgcSimulatedRayCaster rcast(igc, gimg);
  blitz::Array<double, 2> r = rcast.read_double(10, 0, 10, 10);
  // Not really any easy test here, we just check that we can create
  // the object and then successfully use it to generate results. 
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;

  // Can't use the img from the fixture, because it can't be
  // serialized. So just grab a random test image so we have
  // this present.
  std::string fname = test_data_dir() + "cib_sample.img";
  boost::shared_ptr<RasterImage> img2(new GdalRasterImage(fname));
  boost::shared_ptr<IgcSimulatedRayCaster> 
    rcast(new IgcSimulatedRayCaster(igc, img2));
  std::string d = serialize_write_string(rcast);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IgcSimulatedRayCaster> rcastd =
    serialize_read_string<IgcSimulatedRayCaster>(d);
}

BOOST_AUTO_TEST_SUITE_END()

