// This goes with geocal_time.i. This should be %include rather than
// %import
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "geocal_time.h"
%}

%import "geocal_time.i"

%template(Vector_Time) std::vector<GeoCal::Time>;
%template(Vector_Time2) std::vector<boost::shared_ptr<GeoCal::Time> >;
%extend std::vector<GeoCal::Time> {
  %pickle_vector();
};

// Conversion of a python sequence to a std::vector<Time> is handled
// by %shared_ptr_value_traits in geocal_common.i
