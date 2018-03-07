// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"
%{
#include "constant.h"
%}

// In this case, we can actually just include the .h file.
%include "constant.h"

// List of things "import *" will include
%python_export("pi","deg_to_rad","rad_to_deg","arcsecond_to_rad","rad_to_arcsecond","speed_of_light")
