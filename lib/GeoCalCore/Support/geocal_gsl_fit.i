// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "geocal_gsl_fit.h"
%}

namespace GeoCal {
class GslMatrix;
class GslVector;
void gsl_fit(const GslMatrix& X, const GslVector& W, const GslVector& Y, 
             GslVector& C, GslMatrix& Cov, double& Chisq);
void gsl_fit(const GslMatrix& X, const GslVector& Y, 
             GslVector& C, GslMatrix& Cov, double& Chisq);
}

// List of things "import *" will include
%python_export("gsl_fit")
