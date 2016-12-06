// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "piecewise_linear.h"
%}
%base_import(with_parameter)
%import "geocal_time.i"
%include "geocal_time_include.i"
%geocal_shared_ptr(GeoCal::PiecewiseLinear);

namespace GeoCal {
class PiecewiseLinear : public WithParameter {
public:
  enum {LINEAR, CONSTANT, LINEAR_TO_ZERO} FunctionType;
  PiecewiseLinear(const std::vector<Time>& X,
		  const blitz::Array<int, 1>& T);
  %python_attribute_with_set(parameter_mask, blitz::Array<bool, 1>);
  double value(const Time& x) const;
  std::string print_to_string() const;
  %pickle_serialization();
};

}
