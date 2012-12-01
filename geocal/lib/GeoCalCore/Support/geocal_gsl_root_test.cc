#include "unit_test_support.h"
#include "geocal_gsl_root.h"
#include <gsl/gsl_multiroots.h>
#include "geocal_gsl_matrix.h"

using namespace GeoCal;

BOOST_FIXTURE_TEST_SUITE(geocal_gsl_root, GlobalFixture)

// This example function comes from the GSL documentation. This is the
// Rosenbrock system of equations

class RosenbrockTest : public VFunctor {
public:
  RosenbrockTest(double av, double bv) : a(av), b(bv) {}
  virtual ~RosenbrockTest() {}
  blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X) const
  {
    blitz::Array<double, 1> res(2);
    res(0) = a * (1 - X(0));
    res(1) = b * (X(1) - X(0) * X(0));
    return res;
  }
private:
  double a, b;
};

class RosenbrockTestWDer : public VFunctorWithDerivative {
public:
  RosenbrockTestWDer(double av, double bv) : a(av), b(bv) {}
  virtual ~RosenbrockTestWDer() {}
  blitz::Array<double, 1> operator()(const blitz::Array<double, 1>& X) const
  {
    blitz::Array<double, 1> res(2);
    res(0) = a * (1 - X(0));
    res(1) = b * (X(1) - X(0) * X(0));
    return res;
  }
  blitz::Array<double, 2> df(const blitz::Array<double, 1>& X) const
  {
    blitz::Array<double, 2> res(2, 2);
    res = a,              0,
         -2 * b * X(0),   b;
    return res;
  }
private:
  double a, b;
};

class Test: public DFunctor {
public:
  Test() {}
  virtual	   ~Test() {}
  virtual double   operator()(const double& x) const
  { return (cos(x) - x) * (cos(x) - x) - 2; }
};

BOOST_AUTO_TEST_CASE(geocal_gsl_multi_root)
{
  RosenbrockTest r(1.0, 10.0);
  blitz::Array<double, 1> xin(2);
  xin = -10.0, -5.0;
  blitz::Array<double, 1> res = gsl_root(r, xin, 1e-7);
  blitz::Array<double, 1> res_expect(2);
  res_expect = 1, 1;
  BOOST_CHECK(blitz::all(fabs(res - res_expect) < 1e-8));
}

BOOST_AUTO_TEST_CASE(geocal_gsl_multi_root_with_der)
{
  RosenbrockTestWDer r(1.0, 10.0);
  blitz::Array<double, 1> xin(2);
  xin = -10.0, -5.0;
  blitz::Array<double, 1> res = gsl_root(r, xin, 1e-7);
  blitz::Array<double, 1> res_expect(2);
  res_expect = 1, 1;
  BOOST_CHECK(blitz::all(fabs(res - res_expect) < 1e-8));
}

BOOST_AUTO_TEST_CASE(geocal_gsl_1d_root)
{
  Test t;
  double xroot = gsl_root(t, 0, 2);
  BOOST_CHECK_CLOSE(xroot, 1.492465, 1e-4);
}

class Ftest : public DFunctor {
public:
  Ftest() {}
  virtual	   ~Ftest() {}
  virtual double   operator()(const double& X) const
  { return (X - 1) * (X - 1.5) * (X - 3) * (X - 5) * (X - 6); }
};

BOOST_AUTO_TEST_CASE(root_list)
{
  Ftest f;
  std::vector<double> res = GeoCal::root_list(f, 0, 7, 0.25);
  BOOST_CHECK_EQUAL(res.size(), (std::vector<double>::size_type) 5);
  BOOST_CHECK_CLOSE(res[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(res[1], 1.5, 1e-4);
  BOOST_CHECK_CLOSE(res[2], 3.0, 1e-4);
  BOOST_CHECK_CLOSE(res[3], 5.0, 1e-4);
  BOOST_CHECK_CLOSE(res[4], 6.0, 1e-4);
  res = GeoCal::root_list(f, 0, 5.5, 0.25);
  BOOST_CHECK_EQUAL(res.size(), (std::vector<double>::size_type) 4);
  BOOST_CHECK_CLOSE(res[0], 1.0, 1e-4);
  BOOST_CHECK_CLOSE(res[1], 1.5, 1e-4);
  BOOST_CHECK_CLOSE(res[2], 3.0, 1e-4);
  BOOST_CHECK_CLOSE(res[3], 5.0, 1e-4);
}

BOOST_AUTO_TEST_SUITE_END()
