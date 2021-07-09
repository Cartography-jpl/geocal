#include "geocal_gsl_fit.h"
#include "geocal_exception.h"
#include <gsl/gsl_multifit.h>
#include <gsl/gsl_linalg.h>

using namespace GeoCal;

namespace GeoCal {
//-----------------------------------------------------------------------
// Wrapper around workspace, so make sure it is cleanup properly.
//-----------------------------------------------------------------------
  class GslWorkspace {
  public:
    GslWorkspace(size_t n, size_t p) 
    {work_space = gsl_multifit_linear_alloc(n, p);}
    ~GslWorkspace() {gsl_multifit_linear_free(work_space);}
    gsl_multifit_linear_workspace* work_space;
  };
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// This does a linear fit to solve the system Y = X * C (in the least
/// squares sense). The data Y is given the weight W 
/// (i.e., W = 1 / (Y uncertainty)). We return the Cov and chisqr in
/// addition to the solution.
//-----------------------------------------------------------------------

void GeoCal::gsl_fit(const GslMatrix& X, const GslVector& W, 
		     const GslVector& Y, GslVector& C, GslMatrix& Cov, 
		     double& Chisq)
{
  GslWorkspace ws(X.blitz_array().extent(0), X.blitz_array().extent(1));
  if(C.blitz_array().extent(0) != X.blitz_array().extent(1)) {
    blitz::Array<double, 1> a(X.blitz_array().extent(1));
    C.reset(a);
  }
  if(Cov.blitz_array().extent(0) != X.blitz_array().extent(1) ||
     Cov.blitz_array().extent(1) != X.blitz_array().extent(1)) {
    blitz::Array<double, 2> a(X.blitz_array().extent(1), 
			      X.blitz_array().extent(1));
    Cov.reset(a);
  }
  int status = gsl_multifit_wlinear(X.gsl(), W.gsl(), Y.gsl(), C.gsl(), 
				    Cov.gsl(), &Chisq,
				    ws.work_space);
  gsl_check(status);
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// This does a linear fit to solve the system Y = X * C (in the least
/// squares sense). The data Y is given equal weight for each point.
/// We return the Cov and chisqr in addition to the solution.
//-----------------------------------------------------------------------

void GeoCal::gsl_fit(const GslMatrix& X, const GslVector& Y, 
		     GslVector& C, GslMatrix& Cov, double& Chisq)
{
  GslWorkspace ws(X.blitz_array().extent(0), X.blitz_array().extent(1));
  if(C.blitz_array().extent(0) != X.blitz_array().extent(1)) {
    blitz::Array<double, 1> a(X.blitz_array().extent(1));
    C.reset(a);
  }
  if(Cov.blitz_array().extent(0) != X.blitz_array().extent(1) ||
     Cov.blitz_array().extent(1) != X.blitz_array().extent(1)) {
    blitz::Array<double, 2> a(X.blitz_array().extent(1), 
			      X.blitz_array().extent(1));
    Cov.reset(a);
  }
  int status = gsl_multifit_linear(X.gsl(), Y.gsl(), C.gsl(), Cov.gsl(), 
				   &Chisq, ws.work_space);
  gsl_check(status);
}

//-----------------------------------------------------------------------
/// \ingroup GSL
/// Do a full inversion of a matrix.
//-----------------------------------------------------------------------

void GeoCal::gsl_invert(const GslMatrix& A, GslMatrix& Ainv)
{
  int size = A.blitz_array().rows();
  if(A.blitz_array().cols() != size)
    throw Exception("Matrix needs to be square");
  gsl_permutation *p = gsl_permutation_alloc(size);
  int s;
  // Matrix gets destroyed, so create a copy
  GslMatrix Acopy(A.blitz_array().copy());
  int status = gsl_linalg_LU_decomp(Acopy.gsl(), p, &s);
  gsl_check(status);
  blitz::Array<double, 2> t(size, size);
  Ainv.reset(t);
  status = gsl_linalg_LU_invert(Acopy.gsl(), p, Ainv.gsl());
  gsl_check(status);
  gsl_permutation_free(p);
}

blitz::Array<double, 2>
GeoCal::gsl_invert(const blitz::Array<double, 2>& A)
{
  GslMatrix agsl(A);
  GslMatrix res;
  gsl_invert(agsl, res);
  return res.blitz_array().copy();
}
