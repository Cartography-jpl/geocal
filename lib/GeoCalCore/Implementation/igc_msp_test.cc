#include "unit_test_support.h"
#include "igc_msp.h"
#include "geodetic.h"
#include "gdal_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(igc_msp, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic)
{
  if(!have_msp_supported())
    return;
  
  // Verify that we calculate the same value using a RPC and MSP. This
  // is the simplest test case, the RPC is well tested so this should
  // agree pretty close to exactly.
  GdalRasterImage img(test_data_dir() + "rpc.ntf");
  IgcMsp igc(test_data_dir() + "rpc.ntf");
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j) {
      ImageCoordinate ic(i, j);
      boost::shared_ptr<GroundCoordinate> gp1 = igc.ground_coordinate(ic);
      boost::shared_ptr<GroundCoordinate> gp2 =
	img.rpc().ground_coordinate(ic, 0);
      if(false)
	std::cerr << ic << "\n"
		  << Geodetic(*gp1) << "\n"
		  << Geodetic(*gp2) << "\n"
		  << GeoCal::distance(*gp1, *gp2) << "\n" << "\n";
      BOOST_CHECK(GeoCal::distance(*gp1, *gp2) < 0.01);
    }
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j) {
      ImageCoordinate ic(i, j);
      boost::shared_ptr<GroundCoordinate> gp1 =
	igc.ground_coordinate_approx_height(ic, 100);
      boost::shared_ptr<GroundCoordinate> gp2 =
	img.rpc().ground_coordinate(ic, 100);
      if(false)
	std::cerr << ic << "\n"
		  << Geodetic(*gp1) << "\n"
		  << Geodetic(*gp2) << "\n"
		  << GeoCal::distance(*gp1, *gp2) << "\n" << "\n";
      BOOST_CHECK(GeoCal::distance(*gp1, *gp2) < 0.01);
    }
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j) {
      ImageCoordinate ic(i, j);
      boost::shared_ptr<GroundCoordinate> gp1 = igc.ground_coordinate(ic);
      ImageCoordinate ic2 = igc.image_coordinate(*gp1);
      BOOST_CHECK(fabs(ic.line - ic2.line) < 0.01);
      BOOST_CHECK(fabs(ic.sample - ic2.sample) < 0.01);
    }
  Time texpect = Time::parse_time("2002-12-16T15:16:29.000000Z");
  BOOST_CHECK(fabs(igc.pixel_time(ImageCoordinate(10,10)) - texpect) < 1e-6);
}

BOOST_AUTO_TEST_CASE(generate_rsm_tre_test)
{
  if(!have_msp_supported())
    return;
  GdalRasterImage img(test_data_dir() + "rpc.ntf");
  IgcMsp igc(test_data_dir() + "rpc.ntf");
  if(false)
    std::cerr << igc.generate_rsm_tre("msp_rsm_generate_report.txt");
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  if(!have_msp_supported())
    return;
  boost::shared_ptr<IgcMsp> igc = boost::make_shared<IgcMsp>(test_data_dir() + "rpc.ntf");
  std::string d = serialize_write_string(igc);
  if(false)
    std::cerr << d;
  boost::shared_ptr<IgcMsp> igcr =
    serialize_read_string<IgcMsp>(d);
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j) {
      ImageCoordinate ic(i, j);
      boost::shared_ptr<GroundCoordinate> gp1 = igc->ground_coordinate(ic);
      boost::shared_ptr<GroundCoordinate> gp2 = igcr->ground_coordinate(ic);
      BOOST_CHECK(GeoCal::distance(*gp1, *gp2) < 0.01);
    }
}

BOOST_AUTO_TEST_CASE(rsm_ce90)
{
  // This depends on specific test data we don't in general have
  // around. Skip the test, but leave in place in case we want to run
  // again.
  return;

  // In all cases, need MSP support to run this unit test
  if(!have_msp_supported())
    return;
    
  // Test data that has a RSM and covariance stuff. We'll use this to
  // figure the interface in IgcMsp to get CE90 and LE90.
  // This is the output from the test_rsm_indirect_cov_msp found in
  // geocal_nitf_rsm_test.py
  auto dem = boost::make_shared<SimpleDem>();
  IgcMsp igc_msp("/home/smyth/Local/geocal-repo/python/rsm_with_cov.ntf",
		 dem, 0, "RSM", "RSM");
  boost::shared_ptr<GroundCoordinate> gc;
  blitz::Array<double, 2> gc_cov;
  blitz::Array<double, 2> ic_cov(2,2);
  ic_cov = 0;
  igc_msp.ground_coordinate_with_cov(ImageCoordinate(100,100),ic_cov,0,0,
				     gc, gc_cov);
  double ce90, le90;
  igc_msp.ce90_le90(ImageCoordinate(100,100), 0, ce90, le90);
  if(false) {
    std::cerr << igc_msp.covariance() << "\n";
    std::cerr << *igc_msp.ground_coordinate(ImageCoordinate(100,100)) << "\n";
    std::cerr << *gc << "\n"
	      << gc_cov << "\n";
    std::cerr << "CE90: " << ce90 << "\n"
	      << "LE90: " << le90 << "\n";
  }
}
  

BOOST_AUTO_TEST_SUITE_END()
