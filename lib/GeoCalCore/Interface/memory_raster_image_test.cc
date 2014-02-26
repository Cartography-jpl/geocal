#include "unit_test_support.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(memory_raster, GlobalFixture)

BOOST_AUTO_TEST_CASE(memory_raster_image)
{
  MemoryRasterImage im(5, 10);
  BOOST_CHECK_EQUAL(im.number_line(), 5);
  BOOST_CHECK_EQUAL(im.number_sample(), 10);
  int val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      im.write(i, j, val);
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), val);
  boost::multi_array<int, 2> sub = im.read_array(1, 4, 2, 5);
  blitz::Array<int, 2> sub2 = im.read(1, 4, 2, 5);
  for(int i = 1; i < 3; ++i) 
    for(int j = 4; j < 9; ++j) {
      BOOST_CHECK_EQUAL(im(i, j), sub[i - 1][j - 4]);
      BOOST_CHECK_EQUAL(im(i, j), sub2(i - 1, j - 4));
    }
  blitz::Array<double, 2> res = im.interpolate(2.3, 3.4, 2, 3);
  for(int i = 0; i < res.extent(0); ++i)
    for(int j = 0; j < res.extent(1); ++j)
      BOOST_CHECK_CLOSE(res(i, j), im.interpolate(2.3 + i, 3.4 + j), 1e-4);
  MemoryRasterImage im2(im, 2);
  BOOST_CHECK_EQUAL(im2.number_line(), 5);
  BOOST_CHECK_EQUAL(im2.number_sample(), 10);
  val = 0;
  for(int i = 0; i < im2.number_line(); ++i)
    for(int j = 0; j < im2.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im2(i, j), val);

  MemoryRasterImage im3(5, 10);
  copy(im, im3);
  for(int i = 0; i < im3.number_line(); ++i)
    for(int j = 0; j < im3.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im3(i, j), im(i, j));
}

BOOST_AUTO_TEST_CASE(memory_map_projected_image)
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
  MemoryRasterImage im(mi);
  BOOST_CHECK_EQUAL(im.number_line(), 5);
  BOOST_CHECK_EQUAL(im.number_sample(), 10);
  int val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      im.write(i, j, val);
  val = 0;
  for(int i = 0; i < im.number_line(); ++i)
    for(int j = 0; j < im.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im(i, j), val);
  boost::multi_array<int, 2> sub = im.read_array(1, 4, 2, 5);
  blitz::Array<int, 2> sub2 = im.read(1, 4, 2, 5);
  for(int i = 1; i < 3; ++i) 
    for(int j = 4; j < 9; ++j) {
      BOOST_CHECK_EQUAL(im(i, j), sub[i - 1][j - 4]);
      BOOST_CHECK_EQUAL(im(i, j), sub2(i - 1, j - 4));
    }
  blitz::Array<double, 2> res = im.interpolate(2.3, 3.4, 2, 3);
  for(int i = 0; i < res.extent(0); ++i)
    for(int j = 0; j < res.extent(1); ++j)
      BOOST_CHECK_CLOSE(res(i, j), im.interpolate(2.3 + i, 3.4 + j), 1e-4);
  boost::array<double, 2> ind = im.interpolate_derivative(2.3, 3.4);
  BOOST_CHECK_CLOSE(ind[0], (im.interpolate(2.3 + 0.1, 3.4) - 
			     im.interpolate(2.3, 3.4)) / 0.1, 1e-2);
  BOOST_CHECK_CLOSE(ind[1], (im.interpolate(2.3, 3.4 + 0.1) - 
			     im.interpolate(2.3, 3.4)) / 0.1, 1e-2);
  Geodetic ptexpect(ulc_y + (1.5) * y_pixel_res, ulc_x + 2.5 * x_pixel_res, 0);
  ImageCoordinate ic_expect(1, 2);
  BOOST_CHECK_EQUAL(im.coordinate(ptexpect), ic_expect);
  BOOST_CHECK(distance(ptexpect, *im.ground_coordinate(ic_expect)) < 1e-4);
  BOOST_CHECK_CLOSE(im.grid_center_line_resolution(), 55693.0, 1.0);
  BOOST_CHECK_CLOSE(im.grid_center_sample_resolution(), 14472.0, 1.0);
  
  MemoryRasterImage im2(im);
  BOOST_CHECK_EQUAL(im2.number_line(), 5);
  BOOST_CHECK_EQUAL(im2.number_sample(), 10);
  val = 0;
  for(int i = 0; i < im2.number_line(); ++i)
    for(int j = 0; j < im2.number_sample(); ++j, ++val)
      BOOST_CHECK_EQUAL(im2(i, j), val);
  BOOST_CHECK_EQUAL(im2.coordinate(ptexpect), ic_expect);
  BOOST_CHECK(distance(ptexpect, *im2.ground_coordinate(ic_expect)) < 1e-4);


  std::vector<boost::shared_ptr<GroundCoordinate> > gp;
  gp.push_back(im.ground_coordinate(ImageCoordinate(2.1, 3.1)));
  gp.push_back(im.ground_coordinate(ImageCoordinate(2 + 1.9, 3 + 3.9)));
  // SubMapProjectedImage im3 = im.cover(gp);
  // BOOST_CHECK_EQUAL(im3.number_line(), 3);
  // BOOST_CHECK_EQUAL(im3.number_sample(), 5);
  // for(int i = 0; i < im3.number_line(); ++i)
  //   for(int j = 0; j < im3.number_sample(); ++j) {
  //     BOOST_CHECK_EQUAL(im3(i, j), im(i + 2, j + 3));
  //     BOOST_CHECK(distance(*im3.ground_coordinate(ImageCoordinate(i, j)),
  //  	          *im.ground_coordinate(ImageCoordinate(i + 2, j + 3))) < 1e-4);
  //   }
}

BOOST_AUTO_TEST_SUITE_END()

