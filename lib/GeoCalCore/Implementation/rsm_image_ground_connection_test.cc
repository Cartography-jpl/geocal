#include "unit_test_support.h"
#include "rsm_image_ground_connection.h"
#include "geodetic.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
#include "rsm_fixture.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rsm_image_ground_connection, RsmFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<Dem> dem = boost::make_shared<SimpleDem>(1017);
  boost::shared_ptr<RasterImage> img =
    boost::make_shared<MemoryRasterImage>(100, 300, 0);
  boost::shared_ptr<Rsm> rsm = boost::make_shared<Rsm>(rp_from_rpc, cconv);
  RsmImageGroundConnection ig(rsm, dem, img);
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(ig.image_coordinate(g), ic_expect);
  BOOST_CHECK_CLOSE(ig.resolution_meter(ic_expect), 0.79234757774130682, 1e-4);
  boost::shared_ptr<GroundCoordinate> gcalc = 
    ig.ground_coordinate(ic_expect);
  BOOST_CHECK(distance(*gcalc, g) < 0.1);
  boost::shared_ptr<GroundCoordinate> gcalc2 = 
    ig.ground_coordinate_approx_height(ic_expect, 
				       g.height_reference_surface());
  BOOST_CHECK(distance(*gcalc2, g) < 0.1);
  Ecr ecr(g);
  blitz::Array<double, 2> jac = ig.image_coordinate_jac_cf(ecr);
  ImageCoordinate ic0 = ig.image_coordinate(ecr);
  double eps = 10;
  ImageCoordinate ic2;
  for(int i = 0; i < 3; ++i) {
    ecr.position[i] += eps;
    ic2 = ig.image_coordinate(ecr);
    ecr.position[i] -= eps;
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac(0, i), 1e-1);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac(1, i), 1e-1);
  }
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Dem> dem = boost::make_shared<SimpleDem>(1017);
  boost::shared_ptr<RasterImage> img =
    boost::make_shared<MemoryRasterImage>(100, 300, 0);
  boost::shared_ptr<Rsm> rsm = boost::make_shared<Rsm>(rp_from_rpc, cconv);
  boost::shared_ptr<RsmImageGroundConnection> ig =
    boost::make_shared<RsmImageGroundConnection>(rsm, dem, img);
  std::string d = serialize_write_string(ig);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmImageGroundConnection> igr = 
    serialize_read_string<RsmImageGroundConnection>(d);
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(igr->image_coordinate(g), ic_expect);
  BOOST_CHECK_CLOSE(igr->resolution_meter(ic_expect), 0.79234757774130682, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()

