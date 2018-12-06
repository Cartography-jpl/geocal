#include "unit_test_support.h"
#include "rpc_image_ground_connection.h"
#include "rpc_fixture.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rpc_image_ground_connection, RpcFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  boost::shared_ptr<Dem> dem(new SimpleDem(1017));
  boost::shared_ptr<RasterImage> ri(new MemoryRasterImage(100, 300));
  RpcImageGroundConnection ig(rpc, dem, ri);
  ig.fit_height_offset(true);
  ImageCoordinate ic_expect(5729.22, 27561.36);
  BOOST_CHECK_EQUAL(ig.image_coordinate(gp), ic_expect);
  BOOST_CHECK_CLOSE(ig.resolution_meter(ic_expect), 0.7923528069944753, 1e-4);
  boost::shared_ptr<GroundCoordinate> gcalc = 
    ig.ground_coordinate(ic_expect);
  BOOST_CHECK(distance(*gcalc, gp) < 0.1);
  boost::shared_ptr<GroundCoordinate> gcalc2 = 
    ig.ground_coordinate_approx_height(ic_expect, 
				       gp.height_reference_surface());
  BOOST_CHECK(distance(*gcalc2, gp) < 0.1);
  ig.rpc().fit_line_numerator[0] = true;
  ig.rpc().fit_line_numerator[2] = true;
  ig.rpc().fit_sample_numerator[0] = true;
  ig.rpc().fit_sample_numerator[5] = true;
  blitz::Array<double, 1> p0 = ig.parameter_subset();
  BOOST_CHECK_CLOSE(ig.parameter_subset()(0), ig.rpc().line_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(1), ig.rpc().line_numerator[2], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(2), ig.rpc().sample_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(3), ig.rpc().sample_numerator[5], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(4), ig.rpc().height_offset, 1e-4);
  BOOST_CHECK_EQUAL(ig.parameter_name_subset()[0], "RPC Line Numerator Parameter 0");
  BOOST_CHECK_EQUAL(ig.parameter_name_subset()[1], "RPC Line Numerator Parameter 2");
  BOOST_CHECK_EQUAL(ig.parameter_name_subset()[2], "RPC Sample Numerator Parameter 0");
  BOOST_CHECK_EQUAL(ig.parameter_name_subset()[3], "RPC Sample Numerator Parameter 5");
  BOOST_CHECK_EQUAL(ig.parameter_name_subset()[4], "RPC Height Offset");
  blitz::Array<double, 1> pnew(5);
  pnew = 1, 2, 3, 4, 5;
  ig.parameter_subset(pnew);
  BOOST_CHECK_CLOSE(pnew(0), ig.rpc().line_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(pnew(1), ig.rpc().line_numerator[2], 1e-4);
  BOOST_CHECK_CLOSE(pnew(2), ig.rpc().sample_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(pnew(3), ig.rpc().sample_numerator[5], 1e-4);
  BOOST_CHECK_CLOSE(pnew(4), ig.rpc().height_offset, 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(0), ig.rpc().line_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(1), ig.rpc().line_numerator[2], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(2), ig.rpc().sample_numerator[0], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(3), ig.rpc().sample_numerator[5], 1e-4);
  BOOST_CHECK_CLOSE(ig.parameter_subset()(4), ig.rpc().height_offset, 1e-4);
  ig.parameter_subset(p0);
  Ecr ecr(gp);
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
  eps = 1e-4;
  blitz::Array<double, 2> jac2 = ig.image_coordinate_jac_parm(gp);
  ic0 = ig.image_coordinate(gp);
  for(int i = 0; i < p0.rows(); ++i) {
    p0(i) += eps;
    ig.parameter_subset(p0);
    ic2 = ig.image_coordinate(gp);
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac2(0, i), 1e-2);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac2(1, i), 1e-2);
    p0(i) -= eps;
  }
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<Dem> dem(new SimpleDem(1017));
  boost::shared_ptr<RasterImage> ri;
  boost::shared_ptr<RpcImageGroundConnection> igc(new RpcImageGroundConnection(rpc, dem, ri));
  igc->fit_height_offset(true);
  std::string d = serialize_write_string(igc);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RpcImageGroundConnection> igcr = 
    serialize_read_string<RpcImageGroundConnection>(d);
}

BOOST_AUTO_TEST_SUITE_END()

