#include "unit_test_support.h"
#include "vicar_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(vicar_raster_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  VicarRasterImage f(fname);
  BOOST_CHECK_EQUAL(f.number_line(), 10);
  BOOST_CHECK_EQUAL(f.number_sample(), 10);
  BOOST_CHECK_EQUAL(f.number_tile_line(), 10);
  BOOST_CHECK_EQUAL(f.number_tile_sample(), 10);
  
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j)
      BOOST_CHECK_EQUAL(f(i, j), (i + j));
  BOOST_CHECK(f.has_rpc());
  Rpc rpc = f.rpc();
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(rpc.image_coordinate(g), ic_expect);
  
  BOOST_CHECK(f.has_map_info());
  BOOST_CHECK_EQUAL(f.map_info().number_x_pixel(), 10);
  BOOST_CHECK_EQUAL(f.map_info().number_y_pixel(), 10);
  BOOST_CHECK_CLOSE(f.map_info().ulc_x(), 44.799913294865902, 1e-4);
  BOOST_CHECK_CLOSE(f.map_info().ulc_y(), 35.900027777778142, 1e-4);
}

BOOST_AUTO_TEST_CASE(create)
{
  VicarRasterImage out("vicar_out.img", "BYTE", 10, 11, 1);
  int val = 0;
  for(int i = 9; i >= 0; --i)
    for(int j = 0; j < 11; ++j, ++val)
      out.write(i, j, val);
  val = 0;
  for(int i = 9; i >= 0; --i)
    for(int j = 0; j < 11; ++j, ++val)
      BOOST_CHECK_EQUAL(out(i, j), val);
  val = 0;
  for(int i = 0; i <= 9; ++i)
    for(int j = 10; j >= 0; --j, ++val)
      out.write(i, j, val);
  val = 0;
  for(int i = 0; i <= 9; ++i)
    for(int j = 10; j >= 0; --j, ++val)
      BOOST_CHECK_EQUAL(out(i, j), val);
}

BOOST_AUTO_TEST_CASE(vicar_label)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = "test_out/vicar_copy.img";
  VicarRasterImage r(fname, 1, VicarFile::UPDATE);
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
  r.set_rpc(rpc);
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
  r.set_map_info(mi);
}

BOOST_AUTO_TEST_CASE(vicar_raster_image_point_vs_area)
{
  // Check correct handling of pixel area vs point.
  
  // These 2 files have the same tiepoint, but in one case this is 
  // for area and in the other point. For area, the geotiff (0,0)
  // corresponds to GeoCal (-0.5,-0.5) (since they use a different
  // convention of (0,0) - for geotiff this is the ulc of the first pixel,
  // for geocal this is the cneter of the first pixel). For pixel is
  // point, geotiff and Geocal both have (0,0) as the pixel.
  std::string pixel_is_area = test_data_dir() + "test_pixel_is_area.img";
  std::string pixel_is_point = test_data_dir() + "test_pixel_is_point.img";
  VicarRasterImage fpixel_is_area(pixel_is_area);
  VicarRasterImage fpixel_is_point(pixel_is_point);
  Geodetic ulc(20, 10);
  BOOST_CHECK_CLOSE(distance(*fpixel_is_area.ground_coordinate(-0.5, -0.5),
			     ulc), 0, 1e-4);
  BOOST_CHECK_CLOSE(distance(*fpixel_is_point.ground_coordinate(0, 0),
			     ulc), 0, 1e-4);

  // Check that we can force pixel as point to pixel as area. This is
  // really meant just as a workaround for the SRTM erroneously
  // marking the files as pixel is point when really it is pixel as
  // area, see Srtm class for details.
  VicarRasterImage fpixel_is_point_make_area
    (pixel_is_point, 1, VicarFile::READ, 100, 4, true);
  BOOST_CHECK_CLOSE
    (distance(*fpixel_is_point_make_area.ground_coordinate(-0.5, -0.5),
	      ulc), 0, 1e-4);
}

BOOST_AUTO_TEST_CASE(serialization)
{
#ifdef HAVE_BOOST_SERIALIZATON
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  std::string fname = test_data_dir() + "vicar.img";
  boost::shared_ptr<RasterImage> img(new VicarRasterImage(fname));
  oa << GEOCAL_NVP(img);
  if(false)
    std::cerr << os.str();
  
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<RasterImage> imgr;
  ia >> GEOCAL_NVP(imgr);
  BOOST_CHECK_EQUAL(imgr->number_line(), 10);
  BOOST_CHECK_EQUAL(imgr->number_sample(), 10);
  BOOST_CHECK_EQUAL(imgr->number_tile_line(), 10);
  BOOST_CHECK_EQUAL(imgr->number_tile_sample(), 10);
#endif
}

BOOST_AUTO_TEST_SUITE_END()


