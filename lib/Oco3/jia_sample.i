// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"

%{
#include "jia_sample.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::JiaSample);

namespace GeoCal {
class JiaSample : public GenericObject {
public:
  JiaSample(int Blah);
  void jia_func() const;
  std::string print_to_string() const;
  %python_attribute(blah, int);
  %pickle_serialization();
};
}


