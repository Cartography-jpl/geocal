#include "geocal_gsl_root.h"
#include "geocal_gsl_matrix.h"
#include "geocal_exception.h"
#include <gsl/gsl_multiroots.h>
#include <gsl/gsl_roots.h>
// We will replace with boost tools and rename things
#include <boost/math/tools/roots.hpp>
using namespace GeoCal;
using namespace blitz;

namespace GeoCal {
//-----------------------------------------------------------------------
// Wrapper around fsolver, so make sure it is cleanup properly.
//-----------------------------------------------------------------------
class GslMultiFsolver {
public:
  GslMultiFsolver(size_t n)
  { fsolver = gsl_multiroot_fsolver_alloc(gsl_multiroot_fsolver_hybrids,
					  n); }
  ~GslMultiFsolver() {gsl_multiroot_fsolver_free(fsolver);}
  gsl_multiroot_fsolver* fsolver;
};

class GslMultiFdfsolver {
public:
  GslMultiFdfsolver(size_t n)
  { fsolver = gsl_multiroot_fdfsolver_alloc(gsl_multiroot_fdfsolver_hybridsj,
					    n); }
  ~GslMultiFdfsolver() {gsl_multiroot_fdfsolver_free(fsolver);}
  gsl_multiroot_fdfsolver* fsolver;
};

class GslFsolver {
public:
  GslFsolver()
  { fsolver = gsl_root_fsolver_alloc(gsl_root_fsolver_brent); }
  ~GslFsolver() {gsl_root_fsolver_free(fsolver);}
  gsl_root_fsolver* fsolver;
};

int vfunctor_adapter(const gsl_vector* x, void* params, gsl_vector* f) 
{
  VFunctor* vf = static_cast<VFunctor*>(params);
  GslVector xv(const_cast<gsl_vector*>(x), false);
  GslVector y(f, false);
  y.blitz_array() = (*vf)(xv.blitz_array());
  return GSL_SUCCESS;
}

int vdffunctor_adapter(const gsl_vector* x, void* params, gsl_matrix* j) 
{
  VFunctorWithDerivative* vf = static_cast<VFunctorWithDerivative*>(params);
  GslVector xv(const_cast<gsl_vector*>(x), false);
  GslMatrix jac(j, false);
  jac.blitz_array() = vf->df(xv.blitz_array());
  return GSL_SUCCESS;
}

int vdffunctor_adapter2(const gsl_vector* x, void* params, gsl_vector* f, 
			gsl_matrix* j) 
{
  VFunctorWithDerivative* vf = static_cast<VFunctorWithDerivative*>(params);
  GslVector xv(const_cast<gsl_vector*>(x), false);
  Array<double, 1> ft;
  Array<double, 2> jt;
  vf->f_and_df(xv.blitz_array(), ft, jt);
  GslVector y(f, false);
  GslMatrix jac(j, false);
  y.blitz_array() = ft;
  jac.blitz_array() = jt;
  return GSL_SUCCESS;
}

double dfunctor_adapter(double x, void* params) 
{
  DFunctor* df = static_cast<DFunctor*>(params);
  return (*df)(x);
}
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// This finds the root of a Vector -> Vector function, without a
/// Jacobian available. The size of the input and output Vector should
/// be the same (i.e., same number of equations as we have variables).
///
/// If we can't find a solution, we throw a ConvergenceFailure
/// exception. 
//-----------------------------------------------------------------------

blitz::Array<double, 1> 
GeoCal::gsl_root(const VFunctor& F, const blitz::Array<double, 1>& Initial,
		 double Residual)
{
  GslMultiFsolver w(Initial.extent(0));
  GslVector x(const_cast<Array<double, 1>& > (Initial));
  gsl_multiroot_function gf;
  gf.f = &vfunctor_adapter;
  gf.n = Initial.extent(0);
  gf.params = static_cast<void*>(const_cast<VFunctor*>(&F));
  int status;
  status = gsl_multiroot_fsolver_set(w.fsolver, &gf, x.gsl());
  gsl_check(status);
  status = GSL_CONTINUE;
  for(int i = 0; i < 1000 && status ==GSL_CONTINUE; ++i) {
    status = gsl_multiroot_fsolver_iterate(w.fsolver);
    if(status)
      throw ConvergenceFailure("gsl_multiroot_fsolver_iterate is stuck");
    status = gsl_multiroot_test_residual(w.fsolver->f, Residual);
  }
  if(status != GSL_SUCCESS)
    throw 
      ConvergenceFailure("gsl_root exceeded the maximum number of iterations");
  Array<double, 1> res(Initial.shape());
  GslVector resv(w.fsolver->x, false);
  res = resv.blitz_array();
  return res;
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// This finds the root of a Vector -> Vector function, with a
/// Jacobian available. The size of the input and output Vector should
/// be the same (i.e., same number of equations as we have variables).
///
/// If we can't find a solution, we throw a ConvergenceFailure
/// exception. 
//-----------------------------------------------------------------------

blitz::Array<double, 1> 
GeoCal::gsl_root(const VFunctorWithDerivative& F, 
		 const blitz::Array<double, 1>& Initial,
		 double Residual)
{
  GslMultiFdfsolver w(Initial.extent(0));
  GslVector x(const_cast<Array<double, 1>& > (Initial));
  gsl_multiroot_function_fdf gf;
  gf.f = &vfunctor_adapter;
  gf.df = &vdffunctor_adapter;
  gf.fdf = &vdffunctor_adapter2;
  gf.n = Initial.extent(0);
  gf.params = static_cast<void*>(const_cast<VFunctorWithDerivative*>(&F));
  int status;
  status = gsl_multiroot_fdfsolver_set(w.fsolver, &gf, x.gsl());
  gsl_check(status);
  status = GSL_CONTINUE;
  for(int i = 0; i < 1000 && status ==GSL_CONTINUE; ++i) {
    status = gsl_multiroot_fdfsolver_iterate(w.fsolver);
    if(status)
      throw ConvergenceFailure("gsl_multiroot_fsolver_iterate is stuck");
    status = gsl_multiroot_test_residual(w.fsolver->f, Residual);
  }
  if(status != GSL_SUCCESS)
    throw 
      ConvergenceFailure("gsl_root exceeded the maximum number of iterations");
  Array<double, 1> res(Initial.shape());
  GslVector resv(w.fsolver->x, false);
  res = resv.blitz_array();
  return res;
}

//-----------------------------------------------------------------------
/// This finds the root of a Double -> Double function, without a
/// derivative available.  The solution found is in the bracketed
/// range Xmin <= X <= Xmax. We find a solution when we have bracketed
/// it within a range xlow, xhigh, with xhigh - xlow < Eps.
///
/// If we can't find a solution, we throw a ConvergenceFailure
/// exception. 
//-----------------------------------------------------------------------

class StoppingCriteria {
public:
  StoppingCriteria(double Eps, double Eps_abs)
    :eps(Eps), eps_abs(Eps_abs)
  {}
  bool operator()(double a, double b) const
  { return fabs(a-b) < eps_abs + eps * std::min(fabs(a), fabs(b)); }
private:
  double eps, eps_abs;
};

class FWrap {
public:
  FWrap(const DFunctor& F) : f(F) { }
  double operator()(double x) const {return f(x);}
private:
  const DFunctor& f;
};
    
double GeoCal::root(const DFunctor& F, double Xmin, double Xmax, 
		    double Eps, double Eps_abs)
{
  StoppingCriteria c(Eps, Eps_abs);
  boost::uintmax_t max_iter = 1000;
  std::pair<double, double> res =
    boost::math::tools::toms748_solve(FWrap(F), Xmin, Xmax, c, max_iter);
  if(max_iter == 1000) {
    ConvergenceFailure e;
    e << "toms748_solve exceeded the maximum number of iterations";
    throw e;
  }
  return (res.first + res.second) / 2;
}

//-----------------------------------------------------------------------
/// This finds the root of a function, and propagates the derivative
/// of the solution with respect to any parameters in the function
/// (i.e., we *aren't* talking about the derivative wrt X here).
//-----------------------------------------------------------------------

AutoDerivative<double> 
GeoCal::root_with_derivative
(const DFunctorWithDerivative& F, double Xmin, double Xmax, 
 double Eps, double Eps_abs)
{
  double xroot = root(F, Xmin, Xmax, Eps, Eps_abs);
  Array<double, 1> grad = F.f_with_derivative(xroot).gradient();
  grad /= -F.df(xroot);
  return AutoDerivative<double>(xroot, grad);
}

//-----------------------------------------------------------------------
/// This will find a (possible empty) list of roots of a function,
/// where the roots have a seperation of at least the supplied minimum
/// separation. 
///
/// This function is useful for finding roots when you don't know how
/// many solutions there are in the given range. It will find all
/// roots, provided that they have a seperation larger then
/// Root_minimum_spacing, and return the list of solutions. This list
/// is ordered from smallest to greatest.
///
/// This function works by sampling the Functor with a spacing of
/// Root_minimum_spacing. If the function changes sign between one
/// spacing and the next, the routine root is called between those
/// spacings and the results is added to the root list.
///
/// This will not finds roots that are closer together then the
/// supplied minimum spacing. (Limitation)
//-----------------------------------------------------------------------

std::vector<double> GeoCal::root_list(const DFunctor& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Eps)
{
  range_max_check(Xmin, Xmax);
  range_max_check(0.0, Root_minimum_spacing);
  std::vector<double> res;
  double a = Xmin;
  double last_root = a - 10 * Eps;
				// Last root found.
  for(double b = a + Root_minimum_spacing; b < Xmax; 
      b += Root_minimum_spacing) {
    if(F(a) * F(b) <= 0.0) {
      double rt = root(F, a, b, Eps);
      if(fabs(rt - last_root) > Eps) {
				// Reject root if it is too close to
				// the last root. We have trouble
				// finding roots twice near the boundary 
				// [a, b] & [b, c] with rt close to b
	res.push_back(rt);
	last_root = rt;
      }
    }
    a = b;
  }
  if(F(a) * F(Xmax) <= 0.0) {
    double rt = root(F, a, Xmax, Eps);
    if(fabs(rt - last_root) > Eps) {
				// Reject root if it is too close to
				// the last root. We have trouble
				// finding roots twice near the boundary 
				// [a, b] & [b, c] with rt close to b
      res.push_back(rt);
      last_root = rt;
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// This will find a (possible empty) list of roots of a function,
/// where the roots have a seperation of at least the supplied minimum
/// separation. 
///
/// This function is useful for finding roots when you don't know how
/// many solutions there are in the given range. It will find all
/// roots, provided that they have a seperation larger then
/// Root_minimum_spacing, and return the list of solutions. This list
/// is ordered from smallest to greatest.
///
/// This function works by sampling the Functor with a spacing of
/// Root_minimum_spacing. If the function changes sign between one
/// spacing and the next, the routine root is called between those
/// spacings and the results is added to the root list.
///
/// This will not finds roots that are closer together then the
/// supplied minimum spacing. (Limitation)
//-----------------------------------------------------------------------

std::vector<AutoDerivative<double> > GeoCal::root_list(const DFunctorWithDerivative& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Eps)
{
  range_max_check(Xmin, Xmax);
  range_max_check(0.0, Root_minimum_spacing);
  std::vector<AutoDerivative<double> > res;
  double a = Xmin;
  double last_root = a - 10 * Eps;
				// Last root found.
  for(double b = a + Root_minimum_spacing; b < Xmax; 
      b += Root_minimum_spacing) {
    if(F(a) * F(b) <= 0.0) {
      AutoDerivative<double> rt = root_with_derivative(F, a, b, Eps);
      if(fabs(rt.value() - last_root) > Eps) {
				// Reject root if it is too close to
				// the last root. We have trouble
				// finding roots twice near the boundary 
				// [a, b] & [b, c] with rt close to b
	res.push_back(rt);
	last_root = rt.value();
      }
    }
    a = b;
  }
  if(F(a) * F(Xmax) <= 0.0) {
    AutoDerivative<double> rt = root_with_derivative(F, a, Xmax, Eps);
    if(fabs(rt.value() - last_root) > Eps) {
				// Reject root if it is too close to
				// the last root. We have trouble
				// finding roots twice near the boundary 
				// [a, b] & [b, c] with rt close to b
      res.push_back(rt);
      last_root = rt.value();
    }
  }
  return res;
}
