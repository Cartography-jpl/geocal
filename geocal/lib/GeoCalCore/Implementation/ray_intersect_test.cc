#include "unit_test_support.h"
#include "ray_intersect.h"
#include "simple_dem.h"
#include "vicar_lite_file.h"
#include "rpc_image_ground_connection.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(ray_intersect, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  double initial_height = 1291;
  boost::shared_ptr<Dem> dem(new SimpleDem(initial_height));
  boost::shared_ptr<RasterImage> 
    img1(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-1.img"));
  boost::shared_ptr<RasterImage> 
    img2(new VicarLiteRasterImage(stereo_test_data_dir() + "10MAY21-2.img"));
  boost::shared_ptr<ImageGroundConnection>
    igc1(new RpcImageGroundConnection(img1->rpc(), dem, img1));
  boost::shared_ptr<ImageGroundConnection>
    igc2(new RpcImageGroundConnection(img2->rpc(), dem, img2));
  RayIntersect ri(igc1, igc2);
  boost::shared_ptr<CartesianFixed> res;
  double dist;
  ri.two_ray_intersect(ImageCoordinate(550, 550), 
		       ImageCoordinate(553.659, 553.99), res, dist);
  Geodetic expect(36.7754303197, -116.118981291, 1290.64);
  BOOST_CHECK(distance(*res, expect) < 0.01);
  BOOST_CHECK_CLOSE(dist, 0.359056, 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()
