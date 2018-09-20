#include "unit_test_support.h"
#include "rsm_adjustable_parameter_a.h"
#include "rsm_fixture.h"
using namespace GeoCal;
using namespace blitz;

  
BOOST_FIXTURE_TEST_SUITE(rsm_adjustable_parameter_a, RsmFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
}

BOOST_AUTO_TEST_CASE(tre)
{
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  // boost::shared_ptr<RsmRationalPolynomial> r =
  //   boost::make_shared<RsmRationalPolynomial>(3,3,3,3,3,3,3,3);
  // r->set_rpc_coeff(rpc);
  // std::string d = serialize_write_string(r);
  // if(false)
  //   std::cerr << d;
  // boost::shared_ptr<RsmRationalPolynomial> rr = 
  //   serialize_read_string<RsmRationalPolynomial>(d);
  // ImageCoordinate ic_expect = rpc.image_coordinate(gp);
  // ImageCoordinate ic = rr->image_coordinate(gp.longitude(), gp.latitude(),
  // 					    gp.height_reference_surface());
  // BOOST_CHECK_CLOSE(ic_expect.line, ic.line, 1e-4);
  // BOOST_CHECK_CLOSE(ic_expect.sample, ic.sample, 1e-4);
}
BOOST_AUTO_TEST_SUITE_END()
