// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_adjustable_parameter_b.h"
#include "rsm_id.h"
#include "coordinate_converter.h"  
#include "image_ground_connection.h"  
%}
%base_import(rsm_adjustable_parameter)
%base_import(with_parameter)
%geocal_shared_ptr(GeoCal::RsmAdjustableParameterB);
%geocal_shared_ptr(GeoCal::RsmBParameterDesc);
%import "local_rectangular_coordinate.i"
%import "image_ground_connection.i"
%import "rsm_id.i"

namespace GeoCal {
class RsmBParameterDesc : public WithParameter {
public:
  RsmBParameterDesc();
  RsmBParameterDesc(const ImageGroundConnection& Igc,
		    double H_min, double H_max);
  %python_attribute(number_parameter, int);
  %python_attribute_with_set(coordinate_converter, boost::shared_ptr<LocalRcConverter>);
  %python_attribute(normalization_scale, blitz::Array<double, 1>);
  %python_attribute(normalization_offset, blitz::Array<double, 1>);
  %python_attribute(using_basis, bool);
  %python_attribute_with_set(row_power, blitz::Array<int, 2>);
  %python_attribute_with_set(col_power, blitz::Array<int, 2>);
  std::string print_to_string() const;
  %pickle_serialization();
};
 
class RsmAdjustableParameterB : public RsmAdjustableParameter,
                                public RsmBParameterDesc {
public:
  RsmAdjustableParameterB(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  RsmAdjustableParameterB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  virtual void adjustment(const GroundCoordinate& Gc,
			  boost::shared_ptr<GroundCoordinate>& Gc_adjusted,
			  double& Lndelta, double& Smpdelta) const;
  virtual void adjustment_with_derivative(const GroundCoordinate& Gc,
	  ArrayAd<double, 1>& Cf_adjusted, AutoDerivative<double>& Lndelta,
	  AutoDerivative<double>& Smpdelta) const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmAdjustableParameterB>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmAdjustableParameterB", "RsmBParameterDesc")
