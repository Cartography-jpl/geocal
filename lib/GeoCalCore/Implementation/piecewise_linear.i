// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "piecewise_linear.h"
%}
%base_import(with_parameter)
%geocal_shared_ptr(GeoCal::PiecewiseLinear);

namespace GeoCal {
class PiecewiseLinear : public WithParameter {
public:
  enum {LINEAR, CONSTANT, LINEAR_TO_ZERO} FunctionType;
  PiecewiseLinear(const blitz::Array<double, 1>& X, 
		  const ArrayAd<double, 1>& Y,
		  const blitz::Array<int, 1>& T);
  %pickle_serialization();
};

}
