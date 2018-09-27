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
%import "array_ad.i"
%import "auto_derivative.i"

namespace GeoCal {
class RsmAdjustableParameter : public WithParameter {
public:
  RsmAdjustableParameter(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const = 0;
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const = 0;
  std::string base_tre_string() const;
  void base_read_tre_string(std::istream& In);
  %python_attribute_with_set(image_identifier, std::string);
  %python_attribute_with_set(rsm_suport_data_edition, std::string);
  %python_attribute_with_set(triangulation_id, std::string);
  %python_attribute_with_set(naif_code, int);
  %python_attribute(has_ground_coordinate_parameter, bool);
  virtual std::string print_to_string() const;
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameter")
