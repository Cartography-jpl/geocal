#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "unit_test_support.h"
#include "gdal_raster_image.h"    
using namespace GeoCal;
BOOST_CLASS_EXPORT(GeoCal::RasterImage);
BOOST_CLASS_EXPORT(GeoCal::GdalRasterImage);

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
    GdalRasterImage::read_all(fname2);
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

BOOST_AUTO_TEST_CASE(create)
{
  GdalRasterImage out("test_out/gdalrpc.tif", "gtiff", 10, 11, 1, 
		      GdalRasterImage::Byte);
  Rpc rpc;
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
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);

  std::string fname = test_data_dir() + "cib_sample.img";
  boost::shared_ptr<RasterImage> img(new GdalRasterImage(fname));
  oa << BOOST_SERIALIZATION_NVP(img);
  if(false)
    std::cerr << os.str();
  
  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  boost::shared_ptr<RasterImage> imgr;
  ia >> BOOST_SERIALIZATION_NVP(imgr);
  BOOST_CHECK_EQUAL(imgr->number_tile_line(), 64);
  BOOST_CHECK_EQUAL(imgr->number_tile_sample(), 64);
  BOOST_CHECK_EQUAL(imgr->number_line(), 200);
  BOOST_CHECK_EQUAL(imgr->number_sample(), 100);
  BOOST_CHECK_EQUAL((*imgr)(10, 20), 58);
}

BOOST_AUTO_TEST_SUITE_END()
