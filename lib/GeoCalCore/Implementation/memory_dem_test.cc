#include "unit_test_support.h"
#include "memory_dem.h"
#ifdef DO_SPARSE
#include <gmm/gmm.h>
#endif

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(memory_dem, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  double ulc_x = 50;
  double ulc_y = 60;
  double x_pixel_res = 0.25;
  double y_pixel_res = -0.50;
  int number_x_pixel = 100;
  int number_y_pixel = 300;
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     ulc_x, ulc_y, 
	     ulc_x + x_pixel_res * number_x_pixel, 
	     ulc_y + y_pixel_res * number_y_pixel, 
	     number_x_pixel, number_y_pixel);
  MemoryDem d(boost::shared_ptr<Datum>(new SimpleDatum()), mi);
  double v = 0;
  boost::multi_array<double, 2>::iterator t1 = d.elevation_data.begin();
  boost::multi_array<double, 2>::iterator t2 = d.elevation_data.end();
  for(double* i = d.elevation_data.data(); 
      i != d.elevation_data.data() + d.elevation_data.num_elements(); 
      ++i, ++v)
    *i = v;
  BOOST_CHECK_CLOSE(d.elevation(10, 20), d.elevation_data[10][20], 1e-4);
  boost::shared_ptr<GroundCoordinate> gc = mi.ground_coordinate(20, 10);
  BOOST_CHECK_CLOSE(d.height_reference_surface(*gc), d.elevation_data[10][20], 
		    1e-4);
#ifdef DO_SPARSE
  gmm::wsvector<double> j(100 * 200 + 10);
  gc = mi.ground_coordinate(20.7, 10.2);
  d.jacobian(*gc, j, 10);
  double h0 = d.height_reference_surface(*gc);
  for(gmm::wsvector<double>::const_iterator i = j.begin(); i != j.end(); ++i) {
    int ind = (*i).first - 10;
    d.elevation_data.data()[ind] += 1;
    double h1 = d.height_reference_surface(*gc);
    BOOST_CHECK_CLOSE(h1 - h0, (*i).second, 1e-4);
    d.elevation_data.data()[ind] -= 1;
  }
#endif
}

BOOST_AUTO_TEST_SUITE_END()
