// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "vfunctor_with_derivative.h"
%}

%geocal_shared_ptr(VFunctorWithDerivative);

namespace GeoCal {
class VFunctorWithDerivative : public VFunctor {
public:
  virtual blitz::Array<double, 2> df(const blitz::Array<double, 1>& X) const;
  virtual void f_and_df(const blitz::Array<double, 1>& X, 
			blitz::Array<double, 1>& OUT,
			blitz::Array<double, 2>& OUT) const;
};
}
