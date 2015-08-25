#include "array_ad.h"
#include "unit_test_support.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(array_ad, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  ArrayAd<double, 2> ad(2,3,4);
  ad(0, 1) = AutoDerivative<double>(3, 1, 4);
  ad(0, 2) = 5;
  BOOST_CHECK_CLOSE(ad.value()(0,1), 3.0, 1e-8);
  BOOST_CHECK_CLOSE(ad.value()(0,2), 5.0, 1e-8);
  Array<double, 1> grad_expect(4);
  grad_expect = 0, 1, 0, 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,1,Range::all()), grad_expect);
  grad_expect = 0;
  BOOST_CHECK_MATRIX_CLOSE(ad.jacobian()(0,2,Range::all()), grad_expect);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  boost::shared_ptr<ArrayAd<double, 1> > ad(new ArrayAd<double, 1>(2, 4));
  (*ad)(0) = AutoDerivative<double>(3, 1, 4);
  (*ad)(1) = AutoDerivative<double>(5, 2, 4);
  std::string d = serialize_write_string(ad);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << d;
  boost::shared_ptr<ArrayAd<double, 1> > adr =
    serialize_read_string<ArrayAd<double, 1> >(d);
  BOOST_CHECK_MATRIX_CLOSE(adr->value(), ad->value());
  BOOST_CHECK_MATRIX_CLOSE(adr->jacobian(), ad->jacobian());
}
 

BOOST_AUTO_TEST_SUITE_END()
