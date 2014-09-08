#ifndef GEOCAL_GSL_ROOT_H
#define GEOCAL_GSL_ROOT_H
#include "functor.h"
#include "vfunctor_with_derivative.h"
#include <vector>

namespace GeoCal {
blitz::Array<double, 1> gsl_root(const VFunctor& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-6);
blitz::Array<double, 1> gsl_root(const VFunctorWithDerivative& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-6);
double gsl_root(const DFunctor& F, 
		double Xmin, double Xmax,
		double Eps = 1e-6, double Eps_abs = 1e-8);

std::vector<double> root_list(const DFunctor& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Eps = 1e-6);
}
#endif
