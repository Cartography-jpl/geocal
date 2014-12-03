#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include "geocal_serialize_common.h"
#include "auto_derivative.h"
#include "unit_test_support.h"

using namespace GeoCal;
using namespace blitz;

BOOST_FIXTURE_TEST_SUITE(auto_derivative, GlobalFixture)

BOOST_AUTO_TEST_CASE(basic_test)
{
  double xv = 2; double yv = 3;
  AutoDerivative<double> x(xv, 0, 2);
  AutoDerivative<double> y(yv, 1, 2);
  AutoDerivative<double> z;

  z = 2 * x + y;
  BOOST_CHECK_CLOSE(z.value(), 2 * xv + yv, 1e-8);
  Array<double, 1> grad(2);
  grad = 2, 1;
  BOOST_CHECK_MATRIX_CLOSE(z.gradient(), grad);

  // Check unary negation operator
  z = -z;
  BOOST_CHECK_CLOSE(z.value(), -(2 * xv + yv), 1e-8);
  grad = -2, -1;
  BOOST_CHECK_MATRIX_CLOSE(z.gradient(), grad);
  
  z = x * x + x / (x * x + 1);
  BOOST_CHECK_CLOSE(z.value(), xv * xv + xv / (xv * xv + 1), 1e-8);
  grad(0) = 2 * xv + 1 / (xv * xv + 1) - xv / ((xv * xv + 1) * (xv * xv + 1)) 
    * (2 * xv);
  grad(1) = 0;
  BOOST_CHECK_MATRIX_CLOSE(z.gradient(), grad);
}

BOOST_AUTO_TEST_CASE(math_function)
{
  AutoDerivative<double> x(5,0,1);
  AutoDerivative<double> r;
  r = sqrt(x * x);
  BOOST_CHECK_CLOSE(r.value(), sqrt(5.0 * 5.0), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 5.0 / (2 * sqrt(5.0*5.0)), 1e-8); 
  r = log(x * x * x);
  BOOST_CHECK_CLOSE(r.value(), log(5.0 * 5.0 * 5.0), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 3 / 5.0, 1e-8);
  r = log10(x * x * x);
  BOOST_CHECK_CLOSE(r.value(), log10(5.0 * 5.0 * 5.0), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 3 / 5.0 * M_LOG10E, 1e-8);
  r = exp(x * x);
  BOOST_CHECK_CLOSE(r.value(), exp(5.0 * 5.0), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 5.0 * r.value(), 1e-8); 

  // Use something within the range of these functions
  AutoDerivative<double> y(0.5,0,1);

  r = sin(y*y);
  BOOST_CHECK_CLOSE(r.value(), sin(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 0.5 * cos(0.5*0.5), 1e-8); 

  r = asin(y*y);
  BOOST_CHECK_CLOSE(r.value(), asin(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 0.5 / sqrt(1-0.5*0.5*0.5*0.5), 1e-8); 

  r = cos(y*y);
  BOOST_CHECK_CLOSE(r.value(), cos(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), -2 * 0.5 * sin(0.5*0.5), 1e-8); 

  r = acos(y*y);
  BOOST_CHECK_CLOSE(r.value(), acos(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), -2 * 0.5 / sqrt(1-0.5*0.5*0.5*0.5), 1e-8); 

  r = tan(y*y);
  BOOST_CHECK_CLOSE(r.value(), tan(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 0.5 / (cos(0.5*0.5)*cos(0.5*0.5)), 1e-8); 

  r = atan(y*y);
  BOOST_CHECK_CLOSE(r.value(), atan(0.5 * 0.5), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 0.5 / (0.5*0.5*0.5*0.5 + 1), 1e-8); 

  r = pow(y*y,2);
  BOOST_CHECK_CLOSE(r.value(), pow(0.5 * 0.5, 2), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), 2 * 0.5 * 2 * pow(0.5*0.5, 1), 1e-8); 
  
  r = y*y*y*y;
  AutoDerivative<double> r2 = pow(y,4);
  BOOST_CHECK_CLOSE(r.value(), r2.value(), 1e-8);
  BOOST_CHECK_CLOSE(r.gradient()(0), r2.gradient()(0), 1e-8); 

}

template<class T> T f(const T& X, const T& Y)
{
  return X * X + X * Y + 2 * Y * Y;
}

BOOST_AUTO_TEST_CASE(func)
{
  double xval = 1;
  double yval = 2;
  double fexpect = xval * xval + xval * yval + 2 * yval * yval;
  BOOST_CHECK_CLOSE(f(xval, yval), fexpect, 1e-8);
  AutoDerivative<double> xauto(xval, 0, 2);
  AutoDerivative<double> yauto(yval, 1, 2);
  AutoDerivative<double> fauto = f(xauto, yauto);
  BOOST_CHECK_CLOSE(fauto.value(), fexpect, 1e-8);
  double dfdx_expect = 2 * xval + yval;
  double dfdy_expect = xval + 4 * yval;
  BOOST_CHECK_CLOSE(fauto.gradient()(0), dfdx_expect, 1e-8);
  BOOST_CHECK_CLOSE(fauto.gradient()(1), dfdy_expect, 1e-8);
}

BOOST_AUTO_TEST_CASE(array)
{
  Array<AutoDerivative<double>, 1> a(2);
  a(0) = AutoDerivative<double>(1, 0, 2);
  a(1) = 2 * AutoDerivative<double>(2, 1, 2);
  AutoDerivative<double> sm;
  sm = sum(a);
  BOOST_CHECK_CLOSE(sm.value(), 1 + 2 * 2, 1e-8);
  BOOST_CHECK_CLOSE(sm.gradient()(0), 1, 1e-8);
  BOOST_CHECK_CLOSE(sm.gradient()(1), 2, 1e-8);
  Array<AutoDerivative<double>, 1> av(log(a));
  BOOST_CHECK_CLOSE(av(1).value(), log(4), 1e-8);
  BOOST_CHECK_CLOSE(av(1).gradient()(1), 2.0 / 4, 1e-8);
  av = exp(a);
  BOOST_CHECK_CLOSE(av(1).value(), exp(4), 1e-8);
  BOOST_CHECK_CLOSE(av(1).gradient()(1), exp(4) * 2, 1e-8);
}

BOOST_AUTO_TEST_CASE(array_assignment)
{
  Array<AutoDerivative<double>, 1> a(2);
  Array<double, 1> b(2);
  b = 1, 2;
  a = 2;
  a = a + b;
  a = cast<AutoDerivative<double> >(b);
}

BOOST_AUTO_TEST_CASE(serialize)
{
  AutoDerivative<double> x(3, 0, 2);
  AutoDerivative<double> y(11);
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);
  oa << BOOST_SERIALIZATION_NVP(x) << BOOST_SERIALIZATION_NVP(y);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  AutoDerivative<double> xr, yr;
  ia >> BOOST_SERIALIZATION_NVP(xr) >> BOOST_SERIALIZATION_NVP(yr);
  BOOST_CHECK_CLOSE(xr.value(), x.value(), 1e-8);
  BOOST_CHECK_CLOSE(yr.value(), y.value(), 1e-8);
  BOOST_CHECK(yr.is_constant());
  BOOST_CHECK_EQUAL(xr.number_variable(), x.number_variable());
  BOOST_CHECK_MATRIX_CLOSE(xr.gradient(), x.gradient());
}

BOOST_AUTO_TEST_CASE(serialize_blitz_array)
{
  blitz::Array<double, 1> d(3);
  d = 1, 2, 3;
  std::ostringstream os;
  boost::archive::xml_oarchive oa(os);
  oa << BOOST_SERIALIZATION_NVP(d);
  if(false)
    // Can dump to screen, if we want to see the text
    std::cerr << os.str();

  std::istringstream is(os.str());
  boost::archive::xml_iarchive ia(is);
  blitz::Array<double, 1> dr;
  ia >> BOOST_SERIALIZATION_NVP(dr);
  BOOST_CHECK_EQUAL(d.rows(), dr.rows());
  BOOST_CHECK_MATRIX_CLOSE(d, dr);
}

BOOST_AUTO_TEST_SUITE_END()

