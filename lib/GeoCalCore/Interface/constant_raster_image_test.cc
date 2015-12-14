#include "unit_test_support.h"
#include "constant_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(constant_raster_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(raster_image)
{
  ConstantRasterImage im(5, 10, 20);
  BOOST_CHECK_EQUAL(im.number_line(), 5);
  BOOST_CHECK_EQUAL(im.number_sample(), 10);
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j)
      BOOST_CHECK_EQUAL(im(i, j), 20);
}

BOOST_AUTO_TEST_CASE(map_projected_image)
{
  double ulc_x = 50;
  double ulc_y = 60;
  double x_pixel_res = 0.25;
  double y_pixel_res = -0.50;
  int number_x_pixel = 10;
  int number_y_pixel = 5;
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     ulc_x, ulc_y, 
	     ulc_x + x_pixel_res * number_x_pixel, 
	     ulc_y + y_pixel_res * number_y_pixel, 
	     number_x_pixel, number_y_pixel);
  ConstantRasterImage im(mi, 20);
  BOOST_CHECK_EQUAL(im.number_line(), 5);
  BOOST_CHECK_EQUAL(im.number_sample(), 10);
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j)
      BOOST_CHECK_EQUAL(im(i, j), 20);
}

BOOST_AUTO_TEST_SUITE_END()

