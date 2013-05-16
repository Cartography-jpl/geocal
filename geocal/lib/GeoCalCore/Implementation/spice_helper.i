// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "spice_helper.h"
%}
namespace GeoCal {
class SpiceHelper  {
public:
  static bool spice_available();
  static void spice_setup(const std::string& Kernel = "geocal.ker");
};
}
