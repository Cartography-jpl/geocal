// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "orbit_des.h"
%}
%base_import(generic_object)
%base_import(orbit)
%import "geocal_time.i"
%include "geocal_time_include.i"
%geocal_shared_ptr(GeoCal::PosCsephb);
namespace GeoCal {
class PosCsephb : public GenericObject {
public:
  PosCsephb(Orbit& Orb, double Tstep);
  PosCsephb(Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep);
  %pickle_serialization();
  std::string print_to_string() const;
};
}
// List of things "import *" will include
%python_export("PosCsephb")
