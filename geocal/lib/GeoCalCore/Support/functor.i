// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "functor.h"
%}

%geocal_shared_ptr(DFunctor);
%geocal_shared_ptr(VFunctor);

namespace GeoCal {
  class DFunctor {
  public:
    virtual double operator()(const double& X) const = 0;
  };

  class VFunctor {
  public:
    virtual blitz::Array<double, 1> operator()(const blitz::Array<double, 
    1>& X) const = 0;
  };
}
