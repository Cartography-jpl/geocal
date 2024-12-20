#include "unit_test_support.h"
#include "geocal_gsl_fit.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_gsl_fit, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  blitz::Array<double, 2> x(3, 2);
  x = 1, 2,
      3, 4,
      5, 6;
  blitz::Array<double, 1> c_expect(2);
  c_expect = 7, 8;
  blitz::firstIndex i1;
  blitz::secondIndex i2;
  blitz::Array<double, 1> y(sum(x(i1, i2) * c_expect(i2), i2));
  y(0) += 0.01;
  y(1) -= 0.01;
  GslMatrix cov;
  double chisq;
  GslVector c;
  gsl_fit(x, y, c, cov, chisq);
  BOOST_CHECK(blitz::all(fabs(c.blitz_array() - c_expect) < 1e-2));
  blitz::Array<double, 2> m(2,2);
  m = 1, 2,
    3, 4;
  GslMatrix minv;
  gsl_invert(GslMatrix(m), minv);
  blitz::Array<double, 2> minv_exp(2,2);
  minv_exp = -2, 1,
    1.5, -0.5;
  BOOST_CHECK_MATRIX_CLOSE(minv.blitz_array(), minv_exp);
  BOOST_CHECK_MATRIX_CLOSE(gsl_invert(m), minv_exp);
}

BOOST_AUTO_TEST_SUITE_END()
