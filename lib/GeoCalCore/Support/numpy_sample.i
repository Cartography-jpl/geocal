// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "numpy_sample.h"
%}

%base_import(generic_object)

%geocal_shared_ptr(GeoCal::NumpySample);

namespace GeoCal {
class NumpySample : public GenericObject {
public:
  NumpySample();
  NumpySample(const blitz::Array<double, 2>& Din);
  %python_attribute(data, blitz::Array<double, 2>);
};
}

// List of things "import *" will include
%python_export("NumpySample")
