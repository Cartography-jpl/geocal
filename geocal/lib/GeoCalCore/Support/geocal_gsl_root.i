// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "geocal_gsl_root.h"
%}

namespace GeoCal {
blitz::Array<double, 1> gsl_root(const VFunctor& F, 
				 const blitz::Array<double, 1>& Initial,
				 double Residual = 1e-6);

double gsl_root(const DFunctor& F, 
		double Xmin, double Xmax,
		double Residual = 1e-6);

std::vector<double> root_list(const DFunctor& F, 
	        double Xmin, double Xmax, double Root_minimum_spacing,
		double Residual = 1e-6);
}
