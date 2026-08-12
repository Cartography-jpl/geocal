// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "vfunctor_with_derivative.h"
%}

%base_import(functor)
%geocal_shared_ptr(GeoCal::VFunctorWithDerivative);

namespace GeoCal {
class VFunctorWithDerivative : public VFunctor {
public:
  virtual blitz::Array<double, 2> df(const blitz::Array<double, 1>& X) const;
  virtual void f_and_df(const blitz::Array<double, 1>& X, 
			blitz::Array<double, 1>& OUTPUT,
			blitz::Array<double, 2>& OUTPUT) const;
};
}
// List of things "import *" will include
%python_export("VFunctorWithDerivative")
