#ifndef RSM_FIXTURE_H
#define RSM_FIXTURE_H
#include "unit_test_support.h"
#include "rpc_fixture.h"
#include "rpc_image_ground_connection.h"
#include "coordinate_converter.h"
#include "memory_raster_image.h"
#include "simple_dem.h"
#include "rsm.h"
#include "rsm_rational_polynomial.h"
#include "rsm_grid.h"
#include "rsm_low_order_polynomial.h"
#include <boost/make_shared.hpp>

// This is a fixture that sets up a reasonable rpc, igc based on that
// and test point in the image. This is used in several unit tests.

namespace GeoCal {
class RsmFixture : public RpcFixture {
public:
  RsmFixture()
  {
    rp_from_rpc = boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3);
    rp_from_rpc->set_rpc_coeff(rpc);
    boost::shared_ptr<MemoryRasterImage> image =
      boost::make_shared<MemoryRasterImage>(rpc.line_offset * 2,
					    rpc.sample_offset * 2, 0);
    igc = boost::make_shared<RpcImageGroundConnection>
      (rpc, boost::make_shared<SimpleDem>(), image);
    cconv = boost::make_shared<GeodeticConverter>();
    rsm = boost::make_shared<Rsm>(rp_from_rpc, cconv);
    rsm->fill_in_ground_domain_vertex(*igc,
				      rpc.height_offset - rpc.height_scale,
				      rpc.height_offset + rpc.height_scale);
  }
  boost::shared_ptr<CoordinateConverter> cconv;
  boost::shared_ptr<RsmRationalPolynomial> rp_from_rpc;
  boost::shared_ptr<Rsm> rsm;
  boost::shared_ptr<RpcImageGroundConnection> igc;
};

class RsmFixtureRadian : public RsmFixture {
public:
  RsmFixtureRadian()
  {
    cconv = boost::make_shared<GeodeticRadianConverter>();
    double hmin = rpc.height_offset - rpc.height_scale;
    double hmax = rpc.height_offset + rpc.height_scale;
    double lmin = 0;
    double smin = 0;
    double lmax = rpc.line_offset * 2;
    double smax = rpc.sample_offset * 2;
    rp_from_rpc->fit(*igc, *cconv, hmin, hmax, lmin, lmax, smin, smax);
    rsm = boost::make_shared<Rsm>(rp_from_rpc, cconv);
    rsm->fill_in_ground_domain_vertex(*igc,
				      rpc.height_offset - rpc.height_scale,
				      rpc.height_offset + rpc.height_scale);
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
    rlop->fit(*igc, *cconv, hmin, hmax, lmin, lmax, smin, smax);
  }
  boost::shared_ptr<RsmLowOrderPolynomial> rlop;
};

class RsmGridFixture : public RsmFixture {
public:
  RsmGridFixture()
  {
    rg_from_rpc = boost::make_shared<RsmGrid>(40,40,4, false, 7,7);
    double hmin = rpc.height_offset - rpc.height_scale;
    double hmax = rpc.height_offset + rpc.height_scale;
    double lmin = 0;
    double smin = 0;
    double lmax = rpc.line_offset * 2;
    double smax = rpc.sample_offset * 2;
    rg_from_rpc->fit(*igc, *cconv, hmin, hmax, lmin, lmax, smin, smax);
    rsm = boost::make_shared<Rsm>(rg_from_rpc, cconv);
  }
  boost::shared_ptr<RsmGrid> rg_from_rpc;
};
}

#endif
