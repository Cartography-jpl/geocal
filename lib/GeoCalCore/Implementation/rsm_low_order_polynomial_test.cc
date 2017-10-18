#include "unit_test_support.h"
#include "rsm_low_order_polynomial.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_low_order_polynomial, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  RsmLowOrderPolynomial r;
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  double lmin = 0;
  double smin = 0;
  double lmax = rpc.line_offset * 2;
  double smax = rpc.sample_offset * 2;
  GeodeticConverter cconv;
  r.fit(*igc, cconv, hmin, hmax, lmin, lmax, smin, smax);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = r.image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<RsmLowOrderPolynomial> r
    = boost::make_shared<RsmLowOrderPolynomial>();
  double hmin = rpc.height_offset - rpc.height_scale;
  double hmax = rpc.height_offset + rpc.height_scale;
  double lmin = 0;
  double smin = 0;
  double lmax = rpc.line_offset * 2;
  double smax = rpc.sample_offset * 2;
  GeodeticConverter cconv;
  r->fit(*igc, cconv, hmin, hmax, lmin, lmax, smin, smax);
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmLowOrderPolynomial> rr = 
    serialize_read_string<RsmLowOrderPolynomial>(d);
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rr->image_coordinate(gp.longitude(), gp.latitude(),
					  gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}
BOOST_AUTO_TEST_SUITE_END()
