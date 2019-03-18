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

BOOST_AUTO_TEST_SUITE_END()
