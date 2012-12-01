#ifndef GEOCAL_GSL_FIT_H
#define GEOCAL_GSL_FIT_H
#include "geocal_gsl_matrix.h"

namespace GeoCal {

/** \defgroup GSL Wrappers around GNU Scientific Library */

void gsl_fit(const GslMatrix& X, const GslVector& W, const GslVector& Y, 
             GslVector& C, GslMatrix& Cov, double& Chisq);
void gsl_fit(const GslMatrix& X, const GslVector& Y, 
             GslVector& C, GslMatrix& Cov, double& Chisq);

}
#endif
