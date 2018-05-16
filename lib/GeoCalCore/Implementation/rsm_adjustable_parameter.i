// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter.h"
%}
%base_import(generic_object)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameter);

namespace GeoCal {
class RsmAdjustableParameter : public GenericObject {
public:
  RsmAdjustableParameter();
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameter>
  read_tre_string(const std::string& Tre_in);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameter")
