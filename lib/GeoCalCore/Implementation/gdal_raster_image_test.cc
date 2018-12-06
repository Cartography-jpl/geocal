#include "unit_test_support.h"
#include "gdal_raster_image.h"
#include "rpc_fixture.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(gdal_raster_image, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  GdalRasterImage d(fname);
  BOOST_CHECK_EQUAL(d.number_tile_line(), 64);
  BOOST_CHECK_EQUAL(d.number_tile_sample(), 64);
  BOOST_CHECK_EQUAL(d.number_line(), 200);
  BOOST_CHECK_EQUAL(d.number_sample(), 100);
  BOOST_CHECK_EQUAL(d(10, 20), 58);
  boost::multi_array<int, 2> r = d.read_array(10, 20, 30, 40);
  BOOST_CHECK_EQUAL(r.shape()[0], (boost::multi_array<int, 2>::size_type) 30);
  BOOST_CHECK_EQUAL(r.shape()[1], (boost::multi_array<int, 2>::size_type) 40);
  BOOST_CHECK_EQUAL(r[0][0], d(10, 20));
  GdalRasterImage d2("test_out/test.tif", "gtiff", 100, 200, 1, GDT_Byte,
     "TILED=YES BLOCKXSIZE=16 BLOCKYSIZE=32 COMPRESS=JPEG JPEG_QUALITY=90");

  std::string fname2 = test_data_dir() + "argus_camera6.jpg";
  GdalRasterImage d3(fname2);
  BOOST_CHECK_CLOSE(d3.metadata<double>("EXIF_FocalPlaneXResolution"),
		    3957.72, 1e-4);
  BOOST_CHECK_CLOSE(d3.metadata<double>("EXIF_FocalPlaneYResolution"),
		    3961.9, 1e-4);
  BOOST_CHECK_CLOSE(d3.metadata<double>("EXIF_FocalLength"),
		    400.0, 1e-4);
  std::vector<boost::shared_ptr<RasterImage> > a = 
    GdalRasterImage::read_all_bands(fname2);
  BOOST_CHECK_EQUAL((int) a.size(), 3);
  BOOST_CHECK_EQUAL(a[0]->number_line(), 3744);
  BOOST_CHECK_EQUAL(a[0]->number_sample(), 5616);
  BOOST_CHECK_EQUAL((*a[0])(99, 100), 45);
  BOOST_CHECK_EQUAL((*a[1])(99, 100), 48);
  BOOST_CHECK_EQUAL((*a[2])(99, 100), 81);
  GdalRasterImage::save_to_erdas("test_out/cib_sample1.img", d);
  GdalRasterImage::save_to_erdas("test_out/cib_sample2.img", d, d, d);
  GdalRasterImage::save("test_out/cib_sample1.tif", "GTiff", d, 
			      GDT_Byte, "", true);
  GdalRasterImage::save("test_out/cib_sample2.tif", "GTiff", d, d, d,
			      GDT_Byte, "PHOTOMETRIC=RGB", true);
}

BOOST_AUTO_TEST_CASE(rotated_image)
{
  // Prior to GDAL 1.11, this had mapinfo. The new version of GDAL
  // doesn't create a map, but rather has 4 GCPs for the 4
  // corners. This is actually more correct, but it is
  // different. Right now, I don't think this causes problem
  // anywhere. But we'll leave this commented out to make it clear
  // that this behavior has been changed.
  // std::string f = test_data_dir() + "rpc.ntf";
  // GdalRasterImage g(f);
  // MapInfo m = g.map_info();
  // BOOST_CHECK_CLOSE(m.ulc_x(), 44.826373456790115, 1e-7);
  // BOOST_CHECK_CLOSE(m.ulc_y(), 35.881944444444443, 1e-7);
  // BOOST_CHECK_CLOSE(m.lrc_x(), 44.826682098765467, 1e-7);
  // BOOST_CHECK_CLOSE(m.lrc_y(), 35.881944444444443, 1e-7);
}

BOOST_FIXTURE_TEST_CASE(create, RpcFixture)
{
  GdalRasterImage out("test_out/gdalrpc.tif", "gtiff", 10, 11, 1, 
		      GdalRasterImage::Byte);
  out.set_rpc(rpc);
}

BOOST_AUTO_TEST_CASE(nitf_corner)
{
#ifndef HAVE_VICAR_GDALPLUGIN
  // Test requires VICAR gdalplugin, so skip if we don't have this.
  return;
#endif
  // File without NITF corner, should return null pointer.
  std::string f = test_data_dir() + "rpc.ntf";
  GdalRasterImage g(f);
  boost::shared_ptr<MapInfo> m = g.map_info_from_nitf_corner();
  BOOST_CHECK(!m.get());
  // File with NITF corner
  f = test_data_dir() + "Stereo/10MAY21-1.img";
  GdalRasterImage g2(f);
  boost::shared_ptr<MapInfo> m2 = g2.map_info_from_nitf_corner();
  BOOST_CHECK(m2.get());
  Geodetic gc_ulc(g2.metadata<double>("NITF_CORNERLAT1"),
		  g2.metadata<double>("NITF_CORNERLON1"));
  Geodetic gc_urc(g2.metadata<double>("NITF_CORNERLAT2"),
		  g2.metadata<double>("NITF_CORNERLON2"));
  Geodetic gc_lrc(g2.metadata<double>("NITF_CORNERLAT3"),
		  g2.metadata<double>("NITF_CORNERLON3"));
  Geodetic gc_llc(g2.metadata<double>("NITF_CORNERLAT4"),
		  g2.metadata<double>("NITF_CORNERLON4"));
  boost::shared_ptr<GroundCoordinate> gc = m2->ground_coordinate(0.5, 0.5);
  BOOST_CHECK(distance(gc_ulc, *gc) < 0.5);
  gc = m2->ground_coordinate(g2.number_sample() - 0.5, 0.5);
  BOOST_CHECK(distance(gc_urc, *gc) < 0.5);
  gc = m2->ground_coordinate(g2.number_sample() - 0.5, 
			     g2.number_line() - 0.5);
  BOOST_CHECK(distance(gc_lrc, *gc) < 0.5);
  gc = m2->ground_coordinate(0.5, 
			     g2.number_line() - 0.5);
  BOOST_CHECK(distance(gc_llc, *gc) < 0.5);
}

BOOST_AUTO_TEST_CASE(serialization)
{
  if(!have_serialize_supported())
    return;
  std::string fname = test_data_dir() + "cib_sample.img";
  boost::shared_ptr<RasterImage> img(new GdalRasterImage(fname));
  std::string d = serialize_write_string(img);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RasterImage> imgr = serialize_read_string<RasterImage>(d);
  BOOST_CHECK_EQUAL(imgr->number_tile_line(), 64);
  BOOST_CHECK_EQUAL(imgr->number_tile_sample(), 64);
  BOOST_CHECK_EQUAL(imgr->number_line(), 200);
  BOOST_CHECK_EQUAL(imgr->number_sample(), 100);
  BOOST_CHECK_EQUAL((*imgr)(10, 20), 58);
}

BOOST_AUTO_TEST_CASE(handle_point)
{
  // Test data where we have data as point instead of image.
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
  GdalRasterImage d("test_out/gdal_point.tif", "gtiff", mi, 1,
		    GdalRasterImage::Int16);
  int val = 0;
  for(int i = 0; i < d.number_line(); ++i)
    for(int j = 0; j < d.number_sample(); ++j)
      d.write(i,j,++val);
  d.close();
  GdalRasterImage d2("test_out/gdal_point.tif");
  BOOST_CHECK(d2.map_info().is_point());
  BOOST_CHECK(distance(*d.ground_coordinate(0,0), *d2.ground_coordinate(0,0))
	      < 0.1);
  BOOST_CHECK(distance(*d.ground_coordinate(50,70),
		       *d2.ground_coordinate(50,70)) < 0.1);
}

BOOST_AUTO_TEST_CASE(handle_point_read)
{
  // Normally skip, since this depends on specific file. Note that the
  // functionality of this is really tested already by the unit test
  // above.
  return;
  GdalRasterImage f("/data/smyth/Temp/N30E075.hgt");
  BOOST_CHECK(f.map_info().is_point());
}

BOOST_AUTO_TEST_SUITE_END()
