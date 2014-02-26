#include "unit_test_support.h"
#include "pan_sharpen.h"
#include "gdal_raster_image.h"
#include "gdal_multi_band.h"
#include "sub_raster_image.h"
#include "simple_dem.h"
#include <boost/progress.hpp>

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(pan_sharpen, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  // We need to subset this data and store in test area.
  boost::shared_ptr<RasterImage> pan
    (new GdalRasterImage(test_data_dir() + "pan.tif"));
  boost::shared_ptr<RasterImageMultiBand> mul
    (new GdalMultiBand(test_data_dir() + "mul.tif"));
  PanSharpen ps(pan, mul, false, false);

  // Check that the various raster image objects we will be using are
  // the same size and overlap on the ground.

  BOOST_CHECK_EQUAL(ps.psmooth->number_line(), ps.pansub->number_line());
  BOOST_CHECK_EQUAL(ps.psmooth->number_sample(), ps.pansub->number_sample());
  BOOST_CHECK_EQUAL(ps.mag->number_band(), mul->number_band());
  for(int i = 0; i < ps.mag->number_band(); ++i) {
    BOOST_CHECK_EQUAL(ps.psmooth->number_line(), 
		      ps.mag->raster_image(i).number_line());
    BOOST_CHECK_EQUAL(ps.psmooth->number_sample(), 
		      ps.mag->raster_image(i).number_sample());
  }
  SimpleDem d;
  ImageCoordinate ulc(-0.5,-0.5);
  ImageCoordinate lrc(ps.psmooth->number_line() - 0.5, 
		      ps.psmooth->number_sample() - 0.5);
  BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(ulc, d),
       ps.pansub->rpc().ground_coordinate(ulc, d)) < 0.2);
  BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(lrc, d),
       ps.pansub->rpc().ground_coordinate(lrc, d)) < 0.2);
  for(int i = 0; i < ps.mag->number_band(); ++i) {
    BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(ulc, d),
	 ps.mag->raster_image(i).rpc().ground_coordinate(ulc, d)) < 0.2);
    BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(lrc, d),
	 ps.mag->raster_image(i).rpc().ground_coordinate(lrc, d)) < 0.2);
  }

  // Check set up of ps itself.
  BOOST_CHECK_EQUAL(ps.number_band(), mul->number_band());
  for(int i = 0; i < ps.mag->number_band(); ++i) {
    BOOST_CHECK_EQUAL(ps.raster_image(i).number_line(),
		      ps.psmooth->number_line());
    BOOST_CHECK_EQUAL(ps.raster_image(i).number_sample(),
		      ps.psmooth->number_sample());
    BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(ulc, d),
	 ps.raster_image(i).rpc().ground_coordinate(ulc, d)) < 0.2);
    BOOST_CHECK(GeoCal::distance(ps.psmooth->rpc().ground_coordinate(lrc, d),
	 ps.raster_image(i).rpc().ground_coordinate(lrc, d)) < 0.2);
  }

  BOOST_CHECK_EQUAL(ps.psq_stat.count(), 
		    ps.psmooth->number_line() * ps.psmooth->number_sample());
  BOOST_CHECK_EQUAL(ps.isq_stat.count(), 
		    ps.mulsub->raster_image(0).number_line() * 
		    ps.mulsub->raster_image(0).number_sample());

  Array<double, 3> pan_sharp = 
    ps.read_double(0,0,ps.raster_image(0).number_line(),
		   ps.raster_image(0).number_sample());

  // There is no easy way to know what the answer should be, but I've
  // visually analyzed the results and they look good. Check a point
  // just to make sure that nothing has broken due to changes.

  BOOST_CHECK_CLOSE(pan_sharp(0,10,10), 372.86, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(1,10,10), 315.35, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(2,10,10), 309.95, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(3,10,10), 283.62, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(4,10,10), 162.15, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(5,10,10), 252.51, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(6,10,10), 206.43, 1e-1);
  BOOST_CHECK_CLOSE(pan_sharp(7,10,10), 198.23, 1e-1);
}

BOOST_AUTO_TEST_CASE(timing)
{
  return;	// Don't normally run this
  // For timing we depend on a full size image, which is much too
  // large to put into our source tree.
  boost::shared_ptr<RasterImage> 
    pan(new GdalRasterImage("/Users/smyth/Cloudy/12MAR_Pan.NTF"));
  boost::shared_ptr<RasterImageMultiBand> mul
    (new GdalMultiBand("/Users/smyth/Cloudy/12MAR.NTF"));
  PanSharpen ps(pan, mul, false, true);
  int count = 0;
  for(RasterImageTileIterator i(ps.raster_image(0)); !i.end(); ++i)
    ++count;
  boost::progress_display disp(count);
  for(RasterImageTileIterator i(ps.raster_image(0)); !i.end(); ++i) {
    Array<double, 3> d(ps.read_double(i.istart(), i.jstart(), 
				      i.number_line(), i.number_sample()));
    disp += 1;
  }
}

BOOST_AUTO_TEST_SUITE_END()
