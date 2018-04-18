// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "geocal_gsl_root.h"
%}
%import "functor.i"
%import "dfunctor_with_derivative.i"
%import "auto_derivative.i"

namespace GeoCal {
blitz::Array<double, 1> gsl_root(const VFunctor& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-6);

double root(const DFunctor& F, 
	    double Xmin, double Xmax,
	    double Eps = 1e-6,
	    double Eps_abs = 1e-8);

AutoDerivative<double> 
root_with_derivative(const DFunctorWithDerivative& F, 
		double Xmin, double Xmax,
		double Eps = 1e-6, double Eps_abs = 1e-8);

std::vector<double> root_list(const DFunctor& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Residual = 1e-6);
}

// List of things "import *" will include
%python_export("root","root_with_derivative","root_list")
