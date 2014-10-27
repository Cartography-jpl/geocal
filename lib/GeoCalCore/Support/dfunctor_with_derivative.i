// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "dfunctor_with_derivative.h"
%}

%base_import(functor)
%import "auto_derivative.i"
%geocal_shared_ptr(GeoCal::DFunctorWithDerivative);

namespace GeoCal {
class DFunctorWithDerivative : public DFunctor {
public:
  virtual double df(double X) const;
  virtual AutoDerivative<double> f_with_derivative(double X) const;
};
}
