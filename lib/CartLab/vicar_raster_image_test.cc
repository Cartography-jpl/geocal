#include "unit_test_support.h"
#include "vicar_raster_image.h"
#include "rpc_fixture.h"

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
  if(!VicarFile::vicar_available())
    return;
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

BOOST_FIXTURE_TEST_CASE(vicar_label, RpcFixture)
{
  if(!VicarFile::vicar_available())
    return;
  std::string fname = "test_out/vicar_copy.img";
  VicarRasterImage r(fname, 1, VicarFile::UPDATE);
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
  if(!VicarFile::vicar_available())
    return;
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
  BOOST_CHECK(fpixel_is_point.map_info().is_point());
  BOOST_CHECK(!fpixel_is_area.map_info().is_point());

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

BOOST_AUTO_TEST_CASE(write_pixel_is_point)
{
  // Test data where we have data as point instead of image. Already
  // have test above for reading, this checks that we can write this
  // correctly. 
  if(!VicarFile::vicar_available())
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
	     number_x_pixel, number_y_pixel, true);
  VicarRasterImage d("test_out/vicar_point.img", mi, "HALF");
  int val = 0;
  for(int i = 0; i < d.number_line(); ++i)
    for(int j = 0; j < d.number_sample(); ++j)
      d.write(i,j,++val);
  d.close();
  VicarFile f2("test_out/vicar_point.img");
  VicarRasterImage d2("test_out/vicar_point.img");
  BOOST_CHECK(d.map_info().is_point());
  BOOST_CHECK(d2.map_info().is_point());
  BOOST_CHECK(distance(*d.ground_coordinate(0,0), *d2.ground_coordinate(0,0))
	      < 0.1);
  BOOST_CHECK(distance(*d.ground_coordinate(50,70),
		       *d2.ground_coordinate(50,70)) < 0.1);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  if(!VicarFile::vicar_available())
    return;
  std::string fname = test_data_dir() + "vicar.img";
  boost::shared_ptr<VicarRasterImage> f(new VicarRasterImage(fname));
  std::string d = serialize_write_string(f);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<VicarRasterImage> fr =
    serialize_read_string<VicarRasterImage>(d);
  BOOST_CHECK_EQUAL(fr->number_line(), 10);
  BOOST_CHECK_EQUAL(fr->number_sample(), 10);
  BOOST_CHECK_EQUAL(fr->number_tile_line(), 10);
  BOOST_CHECK_EQUAL(fr->number_tile_sample(), 10);
}

BOOST_AUTO_TEST_SUITE_END()


