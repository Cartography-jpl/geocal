#include "unit_test_support.h"
#include "map_reprojected_image.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(map_reprojected, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  double ulc_x = 50;
  double ulc_y = 60;
  double x_pixel_res = 0.25;
  double y_pixel_res = -0.50;
  int number_x_pixel = 10;
  int number_y_pixel = 11;
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     ulc_x, ulc_y, 
	     ulc_x + x_pixel_res * number_x_pixel, 
	     ulc_y + y_pixel_res * number_y_pixel, 
	     number_x_pixel, number_y_pixel);
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(mi));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  MapInfo mi2(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	      ulc_x + x_pixel_res * 1.8, ulc_y + y_pixel_res * 1.8, 
	      ulc_x + x_pixel_res * 1.8 * number_x_pixel, 
	     ulc_y + y_pixel_res * 1.8 * number_y_pixel, 
	     number_x_pixel - 1, number_y_pixel - 1);
  MapReprojectedImage mri(data, mi2);
  BOOST_CHECK_EQUAL(mri(9, 8), 0);
  BOOST_CHECK_EQUAL(mri(2, 3), 65);
  blitz::Array<int, 2> rb = mri.read(0, 0, 5, 6);
  for(int i = 0; i < 5; ++i)
    for(int j = 0; j < 6; ++j)
      BOOST_CHECK_EQUAL(mri(i, j), rb(i, j));
}

BOOST_AUTO_TEST_SUITE_END()
