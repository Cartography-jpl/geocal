// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter.h"
#include "coordinate_converter.h"  
%}
%base_import(generic_object)
%base_import(with_parameter)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameter);
%import "coordinate_converter.i"

namespace GeoCal {
class RsmAdjustableParameter : public WithParameter {
public:
  RsmAdjustableParameter(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %python_attribute_with_set(triangulation_id, std::string);
  virtual std::string print_to_string() const;
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameter")
