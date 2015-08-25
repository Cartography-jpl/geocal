#include "unit_test_support.h"
#include "rpc_image_ground_connection.h"
#include "geodetic.h"
#include "simple_dem.h"
#include "memory_raster_image.h"
#include "ecr.h"
using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(rpc_image_ground_connection, GlobalFixture)
BOOST_AUTO_TEST_CASE(basic)
{
  Rpc rpc;
  rpc.error_bias = 0;
  rpc.error_random = 0;
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
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08}};
  rpc.sample_denominator = t4;

  boost::shared_ptr<Dem> dem(new SimpleDem(1017));
  boost::shared_ptr<RasterImage> ri(new MemoryRasterImage(100, 300));
  RpcImageGroundConnection ig(rpc, dem, ri);
  ig.fit_height_offset(true);
  ImageCoordinate ic_expect(5729.22, 27561.36);
  Geodetic g(35.8399968, 45.0770183, 1017);
  BOOST_CHECK_EQUAL(ig.image_coordinate(g), ic_expect);
  BOOST_CHECK_CLOSE(ig.resolution_meter(ic_expect), 0.7923528069944753, 1e-4);
  boost::shared_ptr<GroundCoordinate> gcalc = 
    ig.ground_coordinate(ic_expect);
  BOOST_CHECK(distance(*gcalc, g) < 0.1);
  boost::shared_ptr<GroundCoordinate> gcalc2 = 
    ig.ground_coordinate_approx_height(ic_expect, 
				       g.height_reference_surface());
  BOOST_CHECK(distance(*gcalc2, g) < 0.1);
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
  eps = 1e-4;
  blitz::Array<double, 2> jac2 = ig.image_coordinate_jac_parm(g);
  ic0 = ig.image_coordinate(g);
  for(int i = 0; i < p0.rows(); ++i) {
    p0(i) += eps;
    ig.parameter_subset(p0);
    ic2 = ig.image_coordinate(g);
    BOOST_CHECK_CLOSE((ic2.line - ic0.line) / eps, jac2(0, i), 1e-2);
    BOOST_CHECK_CLOSE((ic2.sample - ic0.sample) / eps, jac2(1, i), 1e-2);
    p0(i) -= eps;
  }
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  Rpc rpc;
  rpc.error_bias = 0;
  rpc.error_random = 0;
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
      1e-9, -2.33038e-07, 1.86413e-08, -1.35637e-08}};
  rpc.sample_denominator = t4;

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

