// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "ray_caster.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RayCaster);
namespace GeoCal {
class RayCaster: public GenericObject {
public:
  %python_attribute(start_position, int);
  %python_attribute(number_position, int);
  %python_attribute(last_position, bool);
  %python_attribute(current_position, int);
  virtual blitz::Array<double, 6> next_position() const = 0;
  std::string print_to_string() const;
};
}

