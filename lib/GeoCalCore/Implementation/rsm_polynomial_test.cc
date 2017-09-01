#include "unit_test_support.h"
#include "rsm_polynomial.h"
#include <boost/make_shared.hpp>
using namespace GeoCal;
using namespace blitz;

class RsmPolynomialFixture : public GlobalFixture {
public:
  RsmPolynomialFixture()
    : r1(2,3,4,false,4), r2(2,3,4,true,4), r3(2,3,4),
      cexpect1(3,4,5), cexpect2(3,4,5), cexpect3(3,4,5)
  {
  x = 2;
  y = 3;
  z = 4;
  int size_expect1 = 0;
  int size_expect2 = 0;
  int size_expect3 = cexpect3.size();
  for(int i = 0; i <= 2; ++i)
    for(int j = 0; j <= 3; ++j)
      for(int k = 0; k <= 4; ++k)
	if(i + j + k <= 4)
	  ++size_expect1;
  size_expect2 = size_expect1 - 1;
  vexpect1 = 0;
  // Start with leading 1.0 constant term.
  vexpect2 = 1.0;
  vexpect3 = 0;
  fexpect1.resize(size_expect1);
  fexpect2.resize(size_expect2);
  fexpect3.resize(size_expect3);
  double val1 = 0.1;
  double val2 = 0.1;
  double val3 = 0.1;
  int ind1 = 0;
  int ind2 = 0;
  int ind3 = 0;
  for(int i = 0; i <= 2; ++i)
    for(int j = 0; j <= 3; ++j)
      for(int k = 0; k <= 4; ++k) {
	fexpect3(ind3++) = val3;
	cexpect3(i, j, k) = val3;
	vexpect3 += val3 * pow(x, i) * pow(y, j) * pow(z, k);
	val3 += 0.1;
	if(i + j + k <= 4) {
	  fexpect1(ind1++) = val1;
	  cexpect1(i, j, k) = val1;
	  vexpect1 += val1 * pow(x, i) * pow(y, j) * pow(z, k);
	  val1 += 0.1;
	  if(i == 0 && j == 0 && k == 0) {
	    cexpect2(i,j,k) = 1;
	  } else {
	    fexpect2(ind2++) = val2;
	    cexpect2(i, j, k) = val2;
	    vexpect2 += val2 * pow(x, i) * pow(y, j) * pow(z, k);
	    val2 += 0.1;
	  }
	} else {
	  cexpect1(i, j, k) = 0;
	  cexpect2(i, j, k) = 0;
	}
      }
  }
  RsmPolynomial r1,r2,r3;
  double vexpect1,vexpect2,vexpect3;
  double x, y, z;
  Array<double, 3> cexpect1,cexpect2,cexpect3;
  Array<double, 1> fexpect1,fexpect2,fexpect3;
};
  
BOOST_FIXTURE_TEST_SUITE(rsm_polynomial, RsmPolynomialFixture)
BOOST_AUTO_TEST_CASE(basic_test)
{
  r1.fitted_coefficent(fexpect1);
  BOOST_CHECK_MATRIX_CLOSE(r1.fitted_coefficent(), fexpect1);
  BOOST_CHECK_MATRIX_CLOSE(r1.coefficient(), cexpect1);
  BOOST_CHECK_CLOSE(r1(x,y,z), vexpect1, 1e-6);
  r2.fitted_coefficent(fexpect2);
  BOOST_CHECK_MATRIX_CLOSE(r2.fitted_coefficent(), fexpect2);
  BOOST_CHECK_MATRIX_CLOSE(r2.coefficient(), cexpect2);
  BOOST_CHECK_CLOSE(r2(x,y,z), vexpect2, 1e-6);
  r3.fitted_coefficent(fexpect3);
  BOOST_CHECK_MATRIX_CLOSE(r3.fitted_coefficent(), fexpect3);
  BOOST_CHECK_MATRIX_CLOSE(r3.coefficient(), cexpect3);
  BOOST_CHECK_CLOSE(r3(x,y,z), vexpect3, 1e-6);
  blitz::Array<double, 1> xvec(3), yvec(3), zvec(3);
  blitz::Array<double, 1> r1vecexp(3), r2vecexp(3), r3vecexp(3);
  xvec = x, 10, 20;
  yvec = y, 30, 40;
  zvec = z, 15, 18;
  r1vecexp = vexpect1, r1(10,30,15), r1(20,40,18);
  r2vecexp = vexpect2, r2(10,30,15), r2(20,40,18);
  r3vecexp = vexpect3, r3(10,30,15), r3(20,40,18);
  BOOST_CHECK_MATRIX_CLOSE(r1(xvec, yvec, zvec), r1vecexp);
  BOOST_CHECK_MATRIX_CLOSE(r2(xvec, yvec, zvec), r2vecexp);
  BOOST_CHECK_MATRIX_CLOSE(r3(xvec, yvec, zvec), r3vecexp);
  // Check that we can call jacobian. We can't easily check that this
  // is right, but we verify that at a higher level when we fit for
  // RsmRationalPolynomial in that unit test.
  blitz::Array<double, 2> jac1(r1.jacobian(xvec, yvec, zvec));
  blitz::Array<double, 2> jac2(r2.jacobian(xvec, yvec, zvec));
  blitz::Array<double, 2> jac3(r3.jacobian(xvec, yvec, zvec));
  BOOST_CHECK_EQUAL(jac1.rows(), xvec.rows());
  BOOST_CHECK_EQUAL(jac2.rows(), xvec.rows());
  BOOST_CHECK_EQUAL(jac3.rows(), xvec.rows());
  BOOST_CHECK_EQUAL(jac1.cols(), fexpect1.rows()); 
  BOOST_CHECK_EQUAL(jac2.cols(), fexpect2.rows()); 
  BOOST_CHECK_EQUAL(jac3.cols(), fexpect3.rows()); 
}

BOOST_AUTO_TEST_CASE(serialize)
{
  if(!have_serialize_supported())
    return;
  r1.fitted_coefficent(fexpect1);
  boost::shared_ptr<RsmPolynomial> r = boost::make_shared<RsmPolynomial>(r1);
  std::string d = serialize_write_string(r);
  if(false)
    std::cerr << d;
  boost::shared_ptr<RsmPolynomial> rr = 
    serialize_read_string<RsmPolynomial>(d);
  BOOST_CHECK_MATRIX_CLOSE(r1.coefficient(), rr->coefficient());
  BOOST_CHECK_EQUAL(r1.is_denominator(), rr->is_denominator());
  BOOST_CHECK_EQUAL(r1.max_order(), rr->max_order());
}
BOOST_AUTO_TEST_SUITE_END()
