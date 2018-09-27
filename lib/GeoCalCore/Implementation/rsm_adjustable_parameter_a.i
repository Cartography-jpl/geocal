// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter_a.h"
#include "rsm_id.h"
#include "coordinate_converter.h"  
#include "image_ground_connection.h"  
%}
%base_import(rsm_adjustable_parameter)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameterA);
%import "local_rectangular_coordinate.i"
%import "image_ground_connection.i"
%import "rsm_id.i"

namespace GeoCal {
class RsmAdjustableParameterA : public RsmAdjustableParameter {
public:
  RsmAdjustableParameterA(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  RsmAdjustableParameterA(const ImageGroundConnection& Igc,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="",
			  bool Activate_image_correction=false,
			  bool Activate_ground_rotation=false,
			  bool Activate_ground_correction=false);
  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const;
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterA>
  read_tre_string(const std::string& Tre_in);
  void activate_image_correction();
  void activate_ground_rotation();
  void activate_ground_correction();
  %python_attribute_with_set(coordinate_converter, boost::shared_ptr<LocalRcConverter>);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameterA")
