// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "misc_array_function.h"
%}

namespace GeoCal {
blitz::Array<double, 2> array_local_median(const blitz::Array<double, 2>& In,
					   int Window_nrow, int Window_ncol);
}

// List of things "import *" will include
%python_export("array_local_median")
