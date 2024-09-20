// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "with_parameter.h"
%}
%base_import(generic_object)
%import "array_ad.i"
%geocal_shared_ptr(GeoCal::WithParameter);
%geocal_shared_ptr(GeoCal::WithParameterNested);
%geocal_shared_ptr(GeoCal::WithParameterShare);
namespace GeoCal {
// Allow this class to be derived from in Python.
%feature("director") WithParameter;

class WithParameter: public virtual GenericObject {
public:
  %python_attribute_with_set_virtual(parameter, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name, virtual std::vector<std::string>);
  %python_attribute_with_set_virtual(parameter_subset, blitz::Array<double, 1>);
  %python_attribute_with_set_virtual(parameter_with_derivative_subset, 
			     ArrayAd<double, 1>);
  %python_attribute(parameter_name_subset, virtual std::vector<std::string>);
  %python_attribute(parameter_mask, virtual blitz::Array<bool, 1>);
  %pickle_serialization();
  void add_identity_gradient();
};

class WithParameterNested: public WithParameter {
public:
  WithParameterNested();
  void add_object(const boost::shared_ptr<WithParameter>& Obj);
  void clear_object();
};

class WithParameterShare: public WithParameter {
public:
  WithParameterShare();
  void add_object(const boost::shared_ptr<WithParameter>& Obj);
  void clear_object();
  %python_attribute_with_set_virtual(parameter_mask, blitz::Array<bool, 1>);
};
  
}

// Extra code for handling boost serialization/python pickle of
// director classes
%geocal_director_serialization(with_parameter, WithParameter)

// List of things "import *" will include
%python_export("WithParameter", "WithParameterNested", "WithParameterShare")
