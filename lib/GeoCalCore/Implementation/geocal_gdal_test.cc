#include "unit_test_support.h"
#include "geocal_gdal.h"
#include "rpc_fixture.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_gdal, RpcFixture)

BOOST_AUTO_TEST_CASE(geocal_gdal_test)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  std::string fname2 = test_data_dir() + "rpc.ntf";
  BOOST_CHECK(gdal_has_map_info(fname));
  // Prior to GDAL 1.11, this had mapinfo. The new version of GDAL
  // doesn't create a map, but rather has 4 GCPs for the 4
  // corners. This is actually more correct, but it is
  // different. Right now, I don't think this causes problem
  // anywhere. But we'll leave this commented out to make it clear
  // that this behavior has been changed.
  // BOOST_CHECK(gdal_has_map_info(fname2));
  Gdal<GByte> g(fname.c_str());
  BOOST_CHECK(g.size()[0] ==200);
  BOOST_CHECK(g.size()[1] ==100);
  BOOST_CHECK(g.tile_size()[0] ==64);
  BOOST_CHECK(g.tile_size()[1] ==64);
  MapInfo m = g.map_info();
  BOOST_CHECK_CLOSE(m.ulc_x(), 44.7999133, 1e-6);
  BOOST_CHECK_CLOSE(m.ulc_y(), 35.9000278, 1e-6);
  BOOST_CHECK_CLOSE(m.lrc_x(), 44.8056936, 1e-6);
  BOOST_CHECK_CLOSE(m.lrc_y(), 35.8907685, 1e-6);
  BOOST_CHECK(m.number_x_pixel() == g.size()[1]);
  BOOST_CHECK(m.number_y_pixel() == g.size()[0]);
  GDALDriver* driver = (GDALDriver*) GDALGetDriverByName("hfa");
  BOOST_REQUIRE(driver != 0);
  boost::shared_ptr<GDALDataset> gd( 
    driver->Create("test_out/cib_sample.img",
		   30, 20, 1, GDT_Int16, 0));
  boost::shared_ptr<Gdal<GInt16> > g2(new Gdal<GInt16>(gd));
  MapInfo m2(boost::shared_ptr<CoordinateConverter>(new GeodeticConverter),
	     10, 60, 40, 20, 30, 20);
  g2->map_info(m2);
  gd.reset();
  BOOST_CHECK(g2->size()[0] ==20);
  BOOST_CHECK(g2->size()[1] ==30);
  GInt16 val = 0;
  for(int i = 0; i < g2->size()[0]; ++i)
    for(int j = 0; j < g2->size()[1]; ++j, ++val)
      (*g2)(i, j) = val;
  g2.reset();
  Gdal<GInt16> g3("test_out/cib_sample.img");
  BOOST_CHECK(g3.size()[0] ==20);
  BOOST_CHECK(g3.size()[1] ==30);
  val = 0;
  for(int i = 0; i < g3.size()[0]; ++i)
    for(int j = 0; j < g3.size()[1]; ++j, ++val)
      BOOST_CHECK(g3(i, j) == val);
  BOOST_CHECK_CLOSE(g3.map_info().ulc_x(), 10.0, 1e-6);
  BOOST_CHECK_CLOSE(g3.map_info().ulc_y(), 60.0, 1e-6);
  BOOST_CHECK_CLOSE(g3.map_info().lrc_x(), 40.0, 1e-6);
  BOOST_CHECK_CLOSE(g3.map_info().lrc_y(), 20.0, 1e-6);
  BOOST_CHECK(g3.map_info().number_x_pixel() == g3.size()[1]);
  BOOST_CHECK(g3.map_info().number_y_pixel() == g3.size()[0]);
  BOOST_CHECK(gdal_band_type(fname) ==GDT_Byte);
  
  Gdal<GUInt16> qbird(fname2);
  BOOST_CHECK_EQUAL(qbird.metadata<int>("LINE_OFF", "Rpc"),
		    (int) rpc.line_offset);
  BOOST_CHECK_CLOSE(qbird.metadata<double>("LONG_OFF", "Rpc"),
		    rpc.longitude_offset, 1e-4);
  BOOST_CHECK_EQUAL(qbird.metadata<std::string>("LINE_OFF", "Rpc"), "2881");
  boost::array<double, 20> t = 
    qbird.metadata<boost::array<double, 20> >("LINE_NUM_COEFF", "Rpc");
  for(int i = 0; i < 20; ++i)
    BOOST_CHECK_CLOSE(rpc.line_numerator[i], t[i], 1e-4);
  
  Rpc rpc2 = qbird.rpc();
  BOOST_CHECK_CLOSE(rpc.height_offset, rpc2.height_offset, 1e-4);
  BOOST_CHECK_CLOSE(rpc.height_scale, rpc2.height_scale, 1e-4);
  BOOST_CHECK_CLOSE(rpc.latitude_offset, rpc2.latitude_offset, 1e-4);
  BOOST_CHECK_CLOSE(rpc.latitude_scale, rpc2.latitude_scale, 1e-4);
  BOOST_CHECK_CLOSE(rpc.longitude_offset, rpc2.longitude_offset, 1e-4);
  BOOST_CHECK_CLOSE(rpc.longitude_scale, rpc2.longitude_scale, 1e-4);
  BOOST_CHECK_CLOSE(rpc.line_offset, rpc2.line_offset, 1e-4);
  BOOST_CHECK_CLOSE(rpc.line_scale, rpc2.line_scale, 1e-4);
  BOOST_CHECK_CLOSE(rpc.sample_offset, rpc2.sample_offset, 1e-4);
  BOOST_CHECK_CLOSE(rpc.sample_scale, rpc2.sample_scale, 1e-4);
  for(int i = 0; i < 20; ++i) {
    BOOST_CHECK_CLOSE(rpc.line_denominator[i], rpc2.line_denominator[i], 
		      1e-4);
    BOOST_CHECK_CLOSE(rpc.line_numerator[i], rpc2.line_numerator[i], 
		      1e-4);
    // Special case for rpc2 being zero, there is a small rounding
    // done that we want to avoid failing because of.
    if(fabs(rpc.sample_denominator[i]) > 1e-8 &&
       fabs(rpc2.sample_denominator[i]) > 1e-8)
      BOOST_CHECK_CLOSE(rpc.sample_denominator[i], rpc2.sample_denominator[i], 
			1e-4);
    BOOST_CHECK_CLOSE(rpc.sample_numerator[i], rpc2.sample_numerator[i], 
		      1e-4);
  }
}

BOOST_AUTO_TEST_SUITE_END()
