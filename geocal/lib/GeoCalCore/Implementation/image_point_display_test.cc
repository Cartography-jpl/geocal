#include "unit_test_support.h"
#include "image_point_display.h"    
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(image_point_display, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = "test_out/image_point_display.img";
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
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName("hfa");
  if(driver) {			// Skip test if we are on a system
				// with ERDAS support in GDAL
    boost::shared_ptr<GDALDataset> gd(
	    driver->Create(fname.c_str(), mi.number_x_pixel(),
  	    mi.number_y_pixel(), 1, GDT_Byte, 0));
    ImagePointDisplay id(gd);
    id.set_map_info(mi);
    for(int i = 1; i <= 16; ++i)
      id.image_point(ImageCoordinate(10 * i, 50), i);
  }
}

BOOST_AUTO_TEST_SUITE_END()
