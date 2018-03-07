// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

// In this case, we can actually just include the .h file.
%{
#include "wgs84_constant.h"
%}

%include "wgs84_constant.h"
// List of things "import *" will include
%python_export("wgs84_a", "wgs84_b", "wgs84_esq")
