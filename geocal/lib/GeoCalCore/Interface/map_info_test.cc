#include "unit_test_support.h"
#include "map_info.h"
#include <cmath>

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(map_info, GlobalFixture)

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
  BOOST_CHECK_CLOSE(mi.lrc_x(), ulc_x + x_pixel_res * number_x_pixel, 1e-4);
  BOOST_CHECK_CLOSE(mi.lrc_y(), ulc_y + y_pixel_res * number_y_pixel, 1e-4);
  BOOST_CHECK_EQUAL(mi.number_x_pixel(), number_x_pixel);
  BOOST_CHECK_EQUAL(mi.number_y_pixel(), number_y_pixel);
  BOOST_CHECK_CLOSE(mi.ulc_x(), ulc_x, 1e-4);
  BOOST_CHECK_CLOSE(mi.ulc_y(), ulc_y, 1e-4);
  BOOST_CHECK_CLOSE(mi.resolution_meter(), 55302.0924, 1e-4);
  Geodetic ptexpect(ulc_y, ulc_x, 0);
  boost::shared_ptr<GroundCoordinate>
    pt(mi.coordinate_converter().convert_from_coordinate(ulc_x, ulc_y));
  BOOST_CHECK(distance(ptexpect, *pt) < 1e-4);
  pt = mi.ground_coordinate(-0.5, -0.5);
  BOOST_CHECK(distance(ptexpect, *pt) < 1e-4);
  Geodetic ptexpect2(mi.lrc_y(), mi.lrc_x(), 0);
  pt = mi.ground_coordinate(number_x_pixel - 1 + 0.5, number_y_pixel - 1 + 0.5);
  BOOST_CHECK(distance(ptexpect2, *pt) < 1e-4);
  double xi, yi;
  mi.coordinate(ptexpect2, xi, yi);
  BOOST_CHECK_CLOSE(xi, number_x_pixel - 1 + 0.5, 1e-4);
  BOOST_CHECK_CLOSE(yi, number_y_pixel - 1 + 0.5, 1e-4);
  Geodetic p3(ulc_y + y_pixel_res / 2, ulc_x + x_pixel_res / 2, 0);
  mi.coordinate(p3, xi, yi);
  BOOST_CHECK(fabs(xi - 0) < 1e-4);
  BOOST_CHECK(fabs(yi - 0) < 1e-4);
  MapInfo sub = mi.subset(10, 20, 30, 40);
  BOOST_CHECK_CLOSE(sub.lrc_x(), ulc_x + x_pixel_res * 40, 1e-4);
  BOOST_CHECK_CLOSE(sub.lrc_y(), ulc_y + y_pixel_res * 60, 1e-4);
  BOOST_CHECK_EQUAL(sub.number_x_pixel(), 30);
  BOOST_CHECK_EQUAL(sub.number_y_pixel(), 40);
  BOOST_CHECK_CLOSE(sub.ulc_x(), ulc_x + 10 * x_pixel_res, 1e-4);
  BOOST_CHECK_CLOSE(sub.ulc_y(), ulc_y + 20 * y_pixel_res, 1e-4);
  std::vector<boost::shared_ptr<GroundCoordinate> > gp;
  gp.push_back(mi.ground_coordinate(10.1, 20.1));
  gp.push_back(mi.ground_coordinate(10 + 28.9, 20 + 38.9));
  MapInfo sub2 = mi.cover(gp);
  BOOST_CHECK_CLOSE(sub2.lrc_x(), ulc_x + x_pixel_res * 40, 1e-4);
  BOOST_CHECK_CLOSE(sub2.lrc_y(), ulc_y + y_pixel_res * 60, 1e-4);
  BOOST_CHECK_EQUAL(sub2.number_x_pixel(), 30);
  BOOST_CHECK_EQUAL(sub2.number_y_pixel(), 40);
  BOOST_CHECK_CLOSE(sub2.ulc_x(), ulc_x + 10 * x_pixel_res, 1e-4);
  BOOST_CHECK_CLOSE(sub2.ulc_y(), ulc_y + 20 * y_pixel_res, 1e-4);
  MapInfo mi2(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	      ulc_x + 10 * x_pixel_res, 
	      ulc_y - 20 * y_pixel_res, 
	      ulc_x + x_pixel_res * (number_x_pixel + 20), 
	      ulc_y + y_pixel_res * (number_y_pixel - 10), 
	      number_x_pixel + 10, number_y_pixel + 10);
  MapInfo mi3 = mi.intersection(mi2);
  BOOST_CHECK_EQUAL(mi3.number_x_pixel(), 90);
  BOOST_CHECK_EQUAL(mi3.number_y_pixel(), 190);
  BOOST_CHECK_CLOSE(mi3.ulc_x(), ulc_x + 10 * x_pixel_res, 1e-4);
  BOOST_CHECK_CLOSE(mi3.ulc_y(), ulc_y, 1e-4);
  MapInfo mi4(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	      ulc_x + 300 * x_pixel_res, 
	      ulc_y + 300 * y_pixel_res, 
	      ulc_x + 400 * x_pixel_res, 
	      ulc_y + 600 * y_pixel_res, 
	      number_x_pixel, number_y_pixel);
  MapInfo mi5 = mi4.intersection(mi);
  BOOST_CHECK_EQUAL(mi5.number_x_pixel(), 0);
  BOOST_CHECK_EQUAL(mi5.number_y_pixel(), 0);

  MapInfo mi6 = mi.scale(2, 5);
  BOOST_CHECK_CLOSE(mi6.lrc_x(), ulc_x + x_pixel_res * number_x_pixel, 1e-4);
  BOOST_CHECK_CLOSE(mi6.lrc_y(), ulc_y + y_pixel_res * number_y_pixel, 1e-4);
  BOOST_CHECK_EQUAL(mi6.number_x_pixel(), number_x_pixel / 2);
  BOOST_CHECK_EQUAL(mi6.number_y_pixel(), number_y_pixel / 5);
  BOOST_CHECK_CLOSE(mi6.ulc_x(), ulc_x, 1e-4);
  BOOST_CHECK_CLOSE(mi6.ulc_y(), ulc_y, 1e-4);
}

BOOST_AUTO_TEST_CASE(rotated)
{
  
  double ulc_x = 50;
  double ulc_y = 60;
  Array<double, 1> param(6);
  param = 50, 0.02, 0.01,
    60, -0.01, 0.02;
  int number_x_pixel = 100;
  int number_y_pixel = 200;
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     param,
	     number_x_pixel, number_y_pixel);
  BOOST_CHECK_EQUAL(mi.number_x_pixel(), number_x_pixel);
  BOOST_CHECK_EQUAL(mi.number_y_pixel(), number_y_pixel);
  BOOST_CHECK_CLOSE(mi.ulc_x(), ulc_x, 1e-8);
  BOOST_CHECK_CLOSE(mi.ulc_y(), ulc_y, 1e-8);
  BOOST_CHECK_CLOSE(mi.lrc_x(), ulc_x + number_x_pixel * 0.02 + 
		    0.01 * number_y_pixel,
		    1e-8);
  BOOST_CHECK_CLOSE(mi.lrc_y(), ulc_y - 0.01 * number_x_pixel + 
		    0.02 * number_y_pixel,
		    1e-8);
  double xindex = 10, yindex = 15;
  double x, y;
  mi.index_to_coordinate(xindex, yindex, x, y);
  BOOST_CHECK_CLOSE(x, 50.365, 1e-4);
  BOOST_CHECK_CLOSE(y, 60.205, 1e-4);
  double xindex2, yindex2;
  mi.coordinate_to_index(x, y, xindex2, yindex2);
  BOOST_CHECK_CLOSE(xindex2, xindex, 1e-4);
  BOOST_CHECK_CLOSE(yindex2, yindex, 1e-4);
  MapInfo mi2 = mi.subset(20, 30, 50, 60);
  BOOST_CHECK_EQUAL(mi2.number_x_pixel(), 50);
  BOOST_CHECK_EQUAL(mi2.number_y_pixel(), 60);
  mi.coordinate(*mi2.ground_coordinate(5.5,6.25), xindex, yindex);
  BOOST_CHECK_CLOSE(xindex, 20 + 5.5, 1e-4);
  BOOST_CHECK_CLOSE(yindex, 30 + 6.25, 1e-4);
  mi2.coordinate(*mi.ground_coordinate(25.5,36.25), xindex, yindex);
  BOOST_CHECK_CLOSE(xindex, 5.5, 1e-4);
  BOOST_CHECK_CLOSE(yindex, 6.25, 1e-4);
  MapInfo mi3 = mi.scale(2, 5);
  mi.coordinate(*mi3.ground_coordinate(5.5,6.25), xindex, yindex);
  BOOST_CHECK_CLOSE(xindex, (5.5 + 0.5) * 2 - 0.5, 1e-8);
  BOOST_CHECK_CLOSE(yindex, (6.25 + 0.5) * 5 - 0.5, 1e-8);
  BOOST_CHECK_EQUAL(mi3.number_x_pixel(), 100 / 2);
  BOOST_CHECK_EQUAL(mi3.number_y_pixel(), 200 / 5);
  mi3.coordinate(*mi.ground_coordinate(5.5,6.25), xindex, yindex);
  BOOST_CHECK_CLOSE(xindex, (5.5 + 0.5) / 2 - 0.5, 1e-8);
  BOOST_CHECK_CLOSE(yindex, (6.25  + 0.5) / 5 - 0.5, 1e-8);
}

BOOST_AUTO_TEST_SUITE_END()
