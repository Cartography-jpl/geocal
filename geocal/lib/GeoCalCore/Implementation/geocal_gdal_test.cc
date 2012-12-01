#include "unit_test_support.h"
#include "geocal_gdal.h"    
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_gdal, GlobalFixture)

BOOST_AUTO_TEST_CASE(geocal_gdal_test)
{
  std::string fname = test_data_dir() + "cib_sample.img";
  std::string fname2 = test_data_dir() + "rpc.ntf";
  BOOST_CHECK(gdal_has_map_info(fname));
  BOOST_CHECK(gdal_has_map_info(fname2));
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
  BOOST_CHECK_EQUAL(qbird.metadata<int>("RPC_LINE_OFF"), 2881);
  BOOST_CHECK_CLOSE(qbird.metadata<double>("RPC_LONG_OFF"), 44.9534, 1e-4);
  BOOST_CHECK_EQUAL(qbird.metadata<std::string>("RPC_LINE_OFF"), "2881");
  boost::array<double, 20> texpect = 
    {{ 0.0132748, -0.14751, -1.13465, -0.0138959, 0.0020018,
      6.35242e-05, 0.000115861, -0.00286551, -0.00778726, 
      3.88308e-06, -1.26487e-06, 7.881069999999999e-06, 
      3.65929e-05, 2.32154e-06, -2.25421e-05, -2.08933e-05, 
       1.8091e-05, 3.6393e-07, -9.39815e-07, -4.31269e-08 }};
  boost::array<double, 20> t = 
    qbird.metadata<boost::array<double, 20> >("RPC_LINE_NUM_COEFF");
  for(int i = 0; i < 20; ++i)
    BOOST_CHECK_CLOSE(texpect[i], t[i], 1e-4);
  
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
    BOOST_CHECK_CLOSE(rpc.sample_denominator[i], rpc2.sample_denominator[i], 
		      1e-4);
    BOOST_CHECK_CLOSE(rpc.sample_numerator[i], rpc2.sample_numerator[i], 
		      1e-4);
  }
}

BOOST_AUTO_TEST_SUITE_END()
