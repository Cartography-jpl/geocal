#include "unit_test_support.h"
#include "msp_support.h"
#include "geodetic.h"
#include "gdal_raster_image.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(msp_support, GlobalFixture)

BOOST_AUTO_TEST_CASE(msp_terrain_point_test)
{
  // Verify that we calculate the same value using a RPC and MSP. This
  // is the simplest test case, the RPC is well tested so this should
  // agree pretty close to exactly.
  GdalRasterImage img(test_data_dir() + "rpc.ntf");
  for(int i = 0; i < 10; ++i)
    for(int j = 0; j < 10; ++j) {
      ImageCoordinate ic(i, j);
      Ecr gp1 = msp_terrain_point(test_data_dir() + "rpc.ntf", ic);
      boost::shared_ptr<GroundCoordinate> gp2 =
	img.rpc().ground_coordinate(ic, 0);
      if(false)
	std::cerr << ic << "\n"
		  << Geodetic(gp1) << "\n"
		  << Geodetic(*gp2) << "\n"
		  << GeoCal::distance(gp1, *gp2) << "\n" << "\n";
      BOOST_CHECK(GeoCal::distance(gp1, *gp2) < 0.01);
    }
}

BOOST_AUTO_TEST_SUITE_END()
