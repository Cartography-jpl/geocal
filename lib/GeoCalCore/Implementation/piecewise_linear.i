// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "piecewise_linear.h"
%}
%base_import(with_parameter)
%import "geocal_time.i"
%geocal_shared_ptr(GeoCal::PiecewiseLinear);

namespace GeoCal {
class PiecewiseLinear : public WithParameter {
public:
  enum {LINEAR, CONSTANT, LINEAR_TO_ZERO} FunctionType;
  PiecewiseLinear(const std::vector<Time>& X,
		  const blitz::Array<int, 1>& T);
  double value(const Time& x) const;
  %pickle_serialization();
  std::string print_to_string() const;
%pythoncode {
def __init__(self, x, t):
    # vector, but right now this doesn not work correctly. So we just
    # replace init with a version that checks for this
    if(isinstance(x, geocal_swig.Vector_Time)):
         xv = x
    else:
        xv = geocal_swig.Vector_Time()
        for xi in x:
            xv.push_back(xi)
    _piecewise_linear.PiecewiseLinear_swiginit(self, _piecewise_linear.new_PiecewiseLinear(xv, t))
}
};

}
