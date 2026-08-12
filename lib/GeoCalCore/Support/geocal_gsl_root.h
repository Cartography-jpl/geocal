#ifndef GEOCAL_GSL_ROOT_H
#define GEOCAL_GSL_ROOT_H
#include "functor.h"
#include "dfunctor_with_derivative.h"
#include "vfunctor_with_derivative.h"
#include <vector>

namespace GeoCal {
blitz::Array<double, 1> gsl_root(const VFunctor& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-8);
blitz::Array<double, 1> gsl_root(const VFunctorWithDerivative& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-8);
double root(const DFunctor& F, 
	    double Xmin, double Xmax,
	    double Eps = 1e-8, double Eps_abs = 1e-10);
AutoDerivative<double> 
root_with_derivative(const DFunctorWithDerivative& F, 
		     double Xmin, double Xmax,
		     double Eps = 1e-8, double Eps_abs = 1e-10);

std::vector<double> root_list(const DFunctor& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Eps = 1e-8);

std::vector<AutoDerivative<double> > root_list(const DFunctorWithDerivative& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Eps = 1e-8);
}
#endif
