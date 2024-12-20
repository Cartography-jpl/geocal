#include "unit_test_support.h"
#include "raster_averaged.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(raster_averaged, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  RasterAveraged ra(data, 2, 3);
  BOOST_CHECK_EQUAL(ra.number_line(), 5);
  BOOST_CHECK_EQUAL(ra.number_sample(), 3);
  BOOST_CHECK_EQUAL(ra.high_resolution_image().number_line(), 11);
  BOOST_CHECK_EQUAL(ra.high_resolution_image_ptr()->number_line(), 11);
  BOOST_CHECK_EQUAL(ra.number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(ra.number_sample_per_pixel(), 3);
  int expected = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected += (*data)(2 + i, 6 + j);
  expected /= 6;
  BOOST_CHECK_EQUAL(ra(1, 2) , expected);
  boost::multi_array<int, 2> rarr = ra.read_array(1, 1, 1, 2);
  BOOST_CHECK_EQUAL(rarr[0][1], expected);
}

BOOST_AUTO_TEST_CASE(basic_mb_test)
{
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  boost::shared_ptr<RasterImage> data2(new MemoryRasterImage(11, 10));
  for(int i = 0; i < data2->number_line(); ++i)
    for(int j = 0; j < data2->number_sample(); ++j, ++val)
      data2->write(i, j, val);
  boost::shared_ptr<RasterImageMultiBandVariable> 
    mb(new RasterImageMultiBandVariable());
  mb->add_raster_image(data);
  mb->add_raster_image(data2);
  RasterAveragedMultiBand ra(mb, 2, 3);
  BOOST_CHECK_EQUAL(ra.raster_image(0).number_line(), 5);
  BOOST_CHECK_EQUAL(ra.raster_image(0).number_sample(), 3);
  BOOST_CHECK_EQUAL(ra.number_band(), 2);
  BOOST_CHECK_EQUAL(ra.raster_image(1).number_line(), 5);
  BOOST_CHECK_EQUAL(ra.raster_image(1).number_sample(), 3);
  BOOST_CHECK_EQUAL(ra.high_resolution_image().raster_image(0).number_line(), 11);
  BOOST_CHECK_EQUAL(ra.number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(ra.number_sample_per_pixel(), 3);
  double expected = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected += (*data)(2 + i, 6 + j);
  expected /= 6;
  BOOST_CHECK_CLOSE(ra.read_double(1, 2, 1, 1)(0,0,0) , expected, 1e-4);
  double expected2 = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected2 += (*data2)(2 + i, 6 + j);
  expected2 /= 6;
  BOOST_CHECK_CLOSE(ra.read_double(1, 2, 1, 1)(1,0,0) , expected2, 1e-4);
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
  RasterAveraged ra(data, 2, 3);
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
  int expected = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected += (*data)(2 + i, 6 + j);
  expected /= 6;
  BOOST_CHECK_EQUAL(ra(1, 2) , expected);
  boost::multi_array<int, 2> rarr = ra.read_array(1, 1, 1, 2);
  BOOST_CHECK_EQUAL(rarr[0][1], expected);
}

BOOST_AUTO_TEST_CASE(serialization_raster_averaged)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  boost::shared_ptr<RasterImage> img(new RasterAveraged(data, 2, 3));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterAveraged> imgr = 
    serialize_read_string<RasterAveraged>(d);
  BOOST_CHECK_EQUAL(imgr->number_line(), 5);
  BOOST_CHECK_EQUAL(imgr->number_sample(), 3);
  BOOST_CHECK_EQUAL(imgr->high_resolution_image().number_line(), 11);
  BOOST_CHECK_EQUAL(imgr->high_resolution_image_ptr()->number_line(), 11);
  BOOST_CHECK_EQUAL(imgr->number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(imgr->number_sample_per_pixel(), 3);
  int expected = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected += (*data)(2 + i, 6 + j);
  expected /= 6;
  BOOST_CHECK_EQUAL((*imgr)(1, 2) , expected);
}

BOOST_AUTO_TEST_CASE(serialization_raster_averaged_mb)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RasterImage> data(new MemoryRasterImage(11, 10));
  int val = 0;
  for(int i = 0; i < data->number_line(); ++i)
    for(int j = 0; j < data->number_sample(); ++j, ++val)
      data->write(i, j, val);
  boost::shared_ptr<RasterImage> data2(new MemoryRasterImage(11, 10));
  for(int i = 0; i < data2->number_line(); ++i)
    for(int j = 0; j < data2->number_sample(); ++j, ++val)
      data2->write(i, j, val);
  boost::shared_ptr<RasterImageMultiBandVariable> 
    mb(new RasterImageMultiBandVariable());
  mb->add_raster_image(data);
  mb->add_raster_image(data2);
  boost::shared_ptr<RasterImageMultiBand> img(new RasterAveragedMultiBand(mb, 2, 3));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterAveragedMultiBand> imgr = 
    serialize_read_string<RasterAveragedMultiBand>(d);
  BOOST_CHECK_EQUAL(imgr->raster_image(0).number_line(), 5);
  BOOST_CHECK_EQUAL(imgr->raster_image(0).number_sample(), 3);
  BOOST_CHECK_EQUAL(imgr->number_band(), 2);
  BOOST_CHECK_EQUAL(imgr->raster_image(1).number_line(), 5);
  BOOST_CHECK_EQUAL(imgr->raster_image(1).number_sample(), 3);
  BOOST_CHECK_EQUAL(imgr->high_resolution_image().raster_image(0).number_line(), 11);
  BOOST_CHECK_EQUAL(imgr->number_line_per_pixel(), 2);
  BOOST_CHECK_EQUAL(imgr->number_sample_per_pixel(), 3);
  int expected = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected += (*data)(2 + i, 6 + j);
  expected /= 6;
  BOOST_CHECK_CLOSE(imgr->read_double(1, 2, 1, 1)(0,0,0) , expected, 1e-4);
  double expected2 = 0;
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      expected2 += (*data2)(2 + i, 6 + j);
  expected2 /= 6;
  BOOST_CHECK_CLOSE(imgr->read_double(1, 2, 1, 1)(1,0,0) , expected2, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
