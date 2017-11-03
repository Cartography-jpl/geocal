#include "unit_test_support.h"
#include "rsm_low_order_polynomial.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_low_order_polynomial, RsmLowOrderPolynomialFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  ImageCoordinate ic = rlop->image_coordinate(gp.longitude(), gp.latitude(),
					      gp.height_reference_surface());
  BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-2);
  BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-2);
}

BOOST_AUTO_TEST_CASE(tre)
{
  std::stringstream in(rlop->tre_string());
  // Don't actually check this, minor differences on machines might
  // have the exact polynomial coefficients vary
  // BOOST_CHECK_EQUAL(rlop->tre_string(), "Blah");
  RsmLowOrderPolynomial r;
  r.read_tre_string(in);
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
  std::string d = serialize_write_string(rlop);
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
