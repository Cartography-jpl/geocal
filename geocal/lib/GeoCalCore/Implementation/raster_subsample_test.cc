#include "unit_test_support.h"
#include "raster_subsample.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(raster_subsample, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  RasterSubSample ra(data, 2, 3);
  BOOST_CHECK_EQUAL(ra.number_line(), 5);
  BOOST_CHECK_EQUAL(ra.number_sample(), 3);
  BOOST_CHECK_EQUAL(ra.high_resolution_image().number_line(), 11);
  BOOST_CHECK_EQUAL(ra.high_resolution_image_ptr()->number_line(), 11);
  BOOST_CHECK_EQUAL(ra.number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(ra.number_sample_per_pixel(), 3);
  BOOST_CHECK_EQUAL(ra(1, 2) , (*data)(1*2,2*3));
  boost::multi_array<int, 2> rarr = ra.read_array(1, 1, 1, 2);
  BOOST_CHECK_EQUAL(rarr[0][1], (*data)(1*2,2*3));
}

BOOST_AUTO_TEST_CASE(map_projected_averaged_test)
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
  RasterSubSample ra(data, 2, 3);
  BOOST_CHECK_EQUAL(ra.map_info().number_x_pixel(), 3);
  BOOST_CHECK_EQUAL(ra.map_info().number_y_pixel(), 5);
  BOOST_CHECK_CLOSE(ra.map_info().ulc_x(), ulc_x, 1e-4);
  BOOST_CHECK_CLOSE(ra.map_info().ulc_y(), ulc_y, 1e-4);
  BOOST_CHECK_CLOSE(ra.map_info().lrc_x(), ulc_x + 3 * x_pixel_res * 3, 
		    1e-4);
  BOOST_CHECK_CLOSE(ra.map_info().lrc_y(), ulc_y + 5 * y_pixel_res * 2, 
		    1e-4);
  BOOST_CHECK_EQUAL(ra.number_line(), 5);
  BOOST_CHECK_EQUAL(ra.number_sample(), 3);
  BOOST_CHECK_EQUAL(ra.high_resolution_image().number_line(), 11);
  BOOST_CHECK_EQUAL(ra.high_resolution_image_ptr()->number_line(), 11);
  BOOST_CHECK_EQUAL(ra.number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(ra.number_sample_per_pixel(), 3);
  BOOST_CHECK_EQUAL(ra(1, 2) , (*data)(1*2,2*3));
  boost::multi_array<int, 2> rarr = ra.read_array(1, 1, 1, 2);
  BOOST_CHECK_EQUAL(rarr[0][1], (*data)(1*2,2*3));
}

BOOST_AUTO_TEST_SUITE_END()
