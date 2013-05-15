// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "functor.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::DFunctor);
%geocal_shared_ptr(GeoCal::VFunctor);

namespace GeoCal {
class DFunctor : public GenericObject {
  public:
    virtual double operator()(const double& X) const = 0;
  };

  class VFunctor : public GenericObject {
  public:
    virtual blitz::Array<double, 1> operator()(const blitz::Array<double, 
    1>& X) const = 0;
  };
}
