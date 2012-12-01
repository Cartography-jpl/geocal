// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%module geocal
%{
#include "spice_helper.h"
%}

namespace GeoCal {
class SpiceHelper {
public:
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker");
};
}
