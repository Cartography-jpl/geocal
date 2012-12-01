#include "unit_test_support.h"
#include "sub_raster_image.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(sub_raster_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<RasterImage> ri(new MemoryRasterImage(5, 10));
  SubRasterImage im(ri,1,3,2,4);
  BOOST_CHECK_EQUAL(im.number_line(), 2);
  BOOST_CHECK_EQUAL(im.number_sample(), 4);
  int val = 0;
  for(int i = 0; i < ri->number_line(); ++i)
    for(int j = 0; j < ri->number_sample(); ++j, ++val)
      ri->write(i, j, val);
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), (*ri)(i + 1, j + 3));
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      im.write(i, j, val);
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), val);
}

BOOST_AUTO_TEST_CASE(basic_map)
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
  boost::shared_ptr<RasterImage> ri(new MemoryRasterImage(mi));
  SubRasterImage im(ri,1,3,2,4);
  BOOST_CHECK_EQUAL(im.number_line(), 2);
  BOOST_CHECK_EQUAL(im.number_sample(), 4);
  int val = 0;
  for(int i = 0; i < ri->number_line(); ++i)
    for(int j = 0; j < ri->number_sample(); ++j, ++val)
      ri->write(i, j, val);
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), (*ri)(i + 1, j + 3));
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      im.write(i, j, val);
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), val);
}

BOOST_AUTO_TEST_SUITE_END()

