#include "unit_test_support.h"
#include "rpc_image_ground_connection.h"
#include "coordinate_converter.h"
#include "memory_raster_image.h"
#include "simple_dem.h"
#include "rsm_rational_polynomial.h"
#include "rsm_grid.h"
#include "rsm_low_order_polynomial.h"
#include "geocal_rpc.h"
#include "geodetic.h"
#include <boost/make_shared.hpp>

// This is a fixture that sets up a reasonable rpc, igc based on that
// and test point in the image. This is used in several unit tests.

namespace GeoCal {
class RsmFixture : public GlobalFixture {
public:
  RsmFixture()
  {
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
  gp = Geodetic(35.8399968, 45.0770183, 1017);
  rp_from_rpc = boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3);
  rp_from_rpc->set_rpc_coeff(rpc);
  boost::shared_ptr<MemoryRasterImage> image =
    boost::make_shared<MemoryRasterImage>(rpc.line_offset * 2,
					  rpc.sample_offset * 2, 0);
  igc = boost::make_shared<RpcImageGroundConnection>
    (rpc, boost::make_shared<SimpleDem>(), image);
  cconv = boost::make_shared<GeodeticConverter>();
  }
  boost::shared_ptr<CoordinateConverter> cconv;
  Rpc rpc;
  boost::shared_ptr<RsmRationalPolynomial> rp_from_rpc;
  Geodetic gp;
  boost::shared_ptr<RpcImageGroundConnection> igc;
};

class RsmFixtureRadian : public RsmFixture {
public:
  RsmFixtureRadian()
  {
  cconv = boost::make_shared<GeodeticRadianConverter>();
  }
};
  
class RsmLowOrderPolynomialFixture : public RsmFixture {
public:
  RsmLowOrderPolynomialFixture()
  {
    rlop = boost::make_shared<RsmLowOrderPolynomial>();
    double hmin = rpc.height_offset - rpc.height_scale;
    double hmax = rpc.height_offset + rpc.height_scale;
    double lmin = 0;
    double smin = 0;
    double lmax = rpc.line_offset * 2;
    double smax = rpc.sample_offset * 2;
    GeodeticConverter cconv;
    rlop->fit(*igc, cconv, hmin, hmax, lmin, lmax, smin, smax);
  }
  boost::shared_ptr<RsmLowOrderPolynomial> rlop;
};

class RsmGridFixture : public RsmFixture {
public:
  RsmGridFixture()
  {
    rg_from_rpc = boost::make_shared<RsmGrid>(60,60,20);
    double hmin = rpc.height_offset - rpc.height_scale;
    double hmax = rpc.height_offset + rpc.height_scale;
    double lmin = 0;
    double smin = 0;
    double lmax = rpc.line_offset * 2;
    double smax = rpc.sample_offset * 2;
    GeodeticConverter cconv;
    rg_from_rpc->fit(*igc, cconv, hmin, hmax, lmin, lmax, smin, smax);
  }
  boost::shared_ptr<RsmGrid> rg_from_rpc;
};
}

