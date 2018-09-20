// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter_a.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_adjustable_parameter)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameterA);
%import "coordinate_converter.i"

namespace GeoCal {
class RsmAdjustableParameterA : public RsmAdjustableParameter {
public:
  RsmAdjustableParameterA(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterA>
  read_tre_string(const std::string& Tre_in);
  %python_attribute_with_set(coordinate_converter, boost::shared_ptr<CoordinateConverter>);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameterA")
