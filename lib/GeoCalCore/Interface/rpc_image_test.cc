#include "unit_test_support.h"
#include "rpc_image.h"
#include "geodetic.h"
#include "memory_raster_image.h"
#include "simple_dem.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rpc_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  Rpc rpc;
  rpc.error_bias = 0;
  rpc.error_random = 0;
  rpc.rpc_type = Rpc::RPC_B;
  rpc.line_offset = 2881;
  rpc.line_scale = 2921;
  rpc.sample_offset = 13763;
  rpc.sample_scale = 14238;
  rpc.latitude_offset = 35.8606;
  rpc.latitude_scale = 0.0209;
  rpc.longitude_offset = 44.9534;
  rpc.longitude_scale = 0.1239;
  rpc.height_offset = 1017;
  rpc.height_scale = 634;
  boost::array<double, 20> t1 = 
    {{ 0.0132748, -0.14751, -1.13465, -0.0138959, 0.0020018,
      6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
      3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
      3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
       1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08 }};
  rpc.line_numerator = t1;
  boost::array<double, 20> t2 = 
    {{ 1, 0.00380256, 0.00643151, 0.00031479,
      1.84958e-05, -1.98243e-06, -1.16422e-06,
      -1.92637e-05, 7.224010000000001e-05, -1.61779e-05,
      4.95049e-06, 1.26794e-06, 0.000190771, -1.57429e-07,
      2.46815e-05, 0.00049166, -5.41022e-07, 3.209e-07,
       1.81401e-05, 1.43592e-07}};
  rpc.line_denominator = t2;
  boost::array<double, 20> t3 = 
    {{ -0.0104025, 0.96885, -0.000487887, -0.0325142,
      -0.000710444, 0.000217572, -6.549690000000001e-05,
      0.0107384, -5.19453e-06, -1.76761e-05, -1.21058e-06,
      0.000106017, 5.41226e-06, -3.8968e-06, 1.03307e-05,
      5.84016e-05, 3.80777e-08, 9.01765e-06, 1.65988e-06,
       -1.19374e-07}};
  rpc.sample_numerator = t3;
  boost::array<double, 20> t4 = 
    {{1, -0.000319831, 0.000681092, -0.000549762,
     -2.67977e-06, -6.19388e-06, 2.67975e-08, 4.76371e-06,
     -1.47985e-05, -4.23457e-06, 1.44009e-08, -1.07213e-06,
     1.1059e-07, 4.10217e-08, -1.69482e-07, 1.08104e-06,
      0, -2.33038e-07, 1.86413e-08, -1.35637e-08}};
  rpc.sample_denominator = t4;
  boost::shared_ptr<RasterImage> img(new MemoryRasterImage(200, 100));
  int v = 0;
  for(int i = 0; i < img->number_line(); ++i)
    for(int j = 0; j < img->number_sample(); ++j, ++v)
      img->write(i, j, v);
  boost::shared_ptr<Dem> dem(new SimpleDem);
  double ulc_x = 44.829624277520701;
  double ulc_y = 35.881509259259623;
  double x_pixel_res = 0.000057803468200;
  double y_pixel_res = -0.000046296296296;
  ulc_x -= 150 * x_pixel_res;	// Empirical, this gives us data.
  int number_x_pixel = 100;
  int number_y_pixel = 200;
  MapInfo mi(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter), 
	     ulc_x, ulc_y, 
	     ulc_x + x_pixel_res * number_x_pixel, 
	     ulc_y + y_pixel_res * number_y_pixel, 
	     number_x_pixel, number_y_pixel);
  RpcImage ri(img, rpc, dem, mi);
  BOOST_CHECK_CLOSE(ri.rpc().line_offset, rpc.line_offset, 1e-4);
  BOOST_CHECK_EQUAL(&ri.raw_image(), &(*img));
  BOOST_CHECK_EQUAL(&ri.dem(), &(*dem));
  BOOST_CHECK_EQUAL(ri(0, 29), 9756);
  BOOST_CHECK_CLOSE(ri.value(0, 29), 9756.62411, 1e-4);
  BOOST_CHECK_CLOSE(ri.value(10, 29, 1), 17144.41856, 1e-4);
  blitz::Array<int, 2> rexp(2, 3);
  for(int i = 0; i < 2; ++i)
    for(int j = 0; j < 3; ++j)
      rexp(i, j) = ri(i, 28 + j);
  BOOST_CHECK(blitz::all(rexp == ri.read(0, 28, 2, 3)));
  MapInfo mi2 = ri.map_info_bounding();
  BOOST_CHECK_EQUAL(mi2.number_x_pixel(), 19);
  BOOST_CHECK_EQUAL(mi2.number_y_pixel(), 30);
  ImageCoordinate iexpect(-14.5, 25.5);
  BOOST_CHECK_EQUAL(ri.coordinate(Geodetic(mi2.ulc_y(), mi2.ulc_x())),
		    iexpect);
}

BOOST_AUTO_TEST_SUITE_END()



