#include "unit_test_support.h"
#include "quick_mosaic.h"
#include "memory_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(quick_mosaic, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
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
  MapInfo mi1 = mi.subset(10,20,50,60);
  MapInfo mi2 = mi.subset(40,50,30,20);
  boost::shared_ptr<MemoryRasterImage> img1 =
    boost::make_shared<MemoryRasterImage>(mi1);
  boost::shared_ptr<MemoryRasterImage> img2 =
    boost::make_shared<MemoryRasterImage>(mi2);
  int val = 0;
  for(int i = 0; i < img1->number_line(); ++i)
    for(int j = 0; j < img1->number_sample(); ++j)
      img1->write(i,j, ++val);
  img1->write(45,45,0);
  for(int i = 0; i < img2->number_line(); ++i)
    for(int j = 0; j < img2->number_sample(); ++j)
      img2->write(i,j, ++val);
  std::vector<boost::shared_ptr<RasterImage> > img_list;
  img_list.push_back(img1);
  img_list.push_back(img2);
  QuickMosaic mos(mi, img_list);
  for(int i = 0; i < mos.number_line(); ++i)
    for(int j = 0; j < mos.number_sample(); ++j) {
      if(i == 45+20 && j == 45+10) {
	BOOST_CHECK_EQUAL(mos(i,j), (*img2)(i-50,j-40));
	continue;
      }
      if(i >= 20 && i < 20 + img1->number_line() &&
	 j >= 10 && j < 10 + img1->number_sample()) {
	BOOST_CHECK_EQUAL(mos(i,j), (*img1)(i-20,j-10));
	continue;
      }
      if(i >= 50 && i < 50 + img2->number_line() &&
	 j >= 40 && j < 40 + img2->number_sample()) {
	BOOST_CHECK_EQUAL(mos(i,j), (*img2)(i-50,j-40));
	continue;
      }
      BOOST_CHECK_EQUAL(mos(i,j), 0);
    }
      
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
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
  MapInfo mi1 = mi.subset(10,20,50,60);
  MapInfo mi2 = mi.subset(40,50,30,20);
  boost::shared_ptr<MemoryRasterImage> img1 =
    boost::make_shared<MemoryRasterImage>(mi1);
  boost::shared_ptr<MemoryRasterImage> img2 =
    boost::make_shared<MemoryRasterImage>(mi2);
  int val = 0;
  for(int i = 0; i < img1->number_line(); ++i)
    for(int j = 0; j < img1->number_sample(); ++j)
      img1->write(i,j, ++val);
  img1->write(45,45,0);
  for(int i = 0; i < img2->number_line(); ++i)
    for(int j = 0; j < img2->number_sample(); ++j)
      img2->write(i,j, ++val);
  std::vector<boost::shared_ptr<RasterImage> > img_list;
  img_list.push_back(img1);
  img_list.push_back(img2);
  boost::shared_ptr<QuickMosaic> img =
    boost::make_shared<QuickMosaic>(mi, img_list);
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterImage> imr = serialize_read_string<RasterImage>(d);
  BOOST_CHECK_EQUAL(imr->number_line(), img->number_line());
  BOOST_CHECK_EQUAL(imr->number_sample(), img->number_sample());
  for(int i = 0; i < imr->number_line(); ++i)
    for(int j = 0; j < imr->number_sample(); ++j)
      BOOST_CHECK_EQUAL((*imr)(i, j), (*img)(i,j));
}

BOOST_AUTO_TEST_SUITE_END()
