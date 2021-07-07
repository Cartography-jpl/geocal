// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_indirect_covariance_b.h"
#include "rsm_id.h"
#include "coordinate_converter.h"  
#include "image_ground_connection.h"  
%}
%base_import(rsm_indirect_covariance)
%base_import(rsm_adjustable_parameter_b)
%geocal_shared_ptr(GeoCal::RsmIndirectCovarianceB);
%import "local_rectangular_coordinate.i"
%import "image_ground_connection.i"
%import "rsm_id.i"

namespace GeoCal {
class RsmIndirectCovarianceB : public RsmIndirectCovariance,
                                public RsmBParameterDesc {
public:
  RsmIndirectCovarianceB(const std::string& Image_identifier="",
			 const std::string& Rsm_support_data_edition="fake-1",
			 const std::string& Triangulation_id="");
  RsmIndirectCovarianceB(const ImageGroundConnection& Igc,
			  double H_min, double H_max,
			  const RsmId& Rsm_id,
			  const std::string& Triangulation_id="");
  std::string tre_string() const;
  static boost::shared_ptr<RsmIndirectCovarianceB>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}

// List of things "import *" will include
%python_export("RsmIndirectCovarianceB")
