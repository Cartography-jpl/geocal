// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_direct_covariance_a.h"
#include "rsm_id.h"
#include "coordinate_converter.h"  
#include "image_ground_connection.h"  
%}
%base_import(rsm_direct_covariance)
%geocal_shared_ptr(GeoCal::RsmDirectCovarianceA);
%import "local_rectangular_coordinate.i"
%import "image_ground_connection.i"
%import "rsm_id.i"

namespace GeoCal {
class RsmDirectCovarianceA : public RsmDirectCovariance {
public:
  RsmDirectCovarianceA(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  RsmDirectCovarianceA(const ImageGroundConnection& Igc,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="",
			  bool Activate_image_correction=false,
			  bool Activate_ground_rotation=false,
			  bool Activate_ground_correction=false);
  std::string tre_string() const;
  static boost::shared_ptr<RsmDirectCovarianceA>
  read_tre_string(const std::string& Tre_in);
  void activate_image_correction();
  void activate_ground_rotation();
  void activate_ground_correction();
  %python_attribute_with_set(covariance, blitz::Array<double, 2>);
  %python_attribute(parameter_name, std::vector<std::string>);
  %python_attribute_with_set(coordinate_converter, boost::shared_ptr<LocalRcConverter>);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmDirectCovarianceA")
