// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "with_parameter.h"
%}
%base_import(generic_object)
%import "array_ad.i"
%geocal_shared_ptr(GeoCal::WithParameter);
namespace GeoCal {
class WithParameter: public virtual GenericObject {
public:
  %python_attribute_with_set(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set(parameter_with_derivative, ArrayAd<double, 1>);
  %python_attribute(parameter_name, std::vector<std::string>);
  %python_attribute_with_set(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, std::vector<std::string>);
  %python_attribute_with_set(parameter_mask, blitz::Array<bool, 1>);
};
}
