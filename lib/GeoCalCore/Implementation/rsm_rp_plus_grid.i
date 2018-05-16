// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_rp_plus_grid.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_base)
%import "rsm_rational_polynomial.i"
%import "rsm_base.i"
%import "rsm_grid.i"
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmRpPlusGrid);

namespace GeoCal {
class RsmRpPlusGrid : public RsmBase {
public:
  RsmRpPlusGrid(const boost::shared_ptr<RsmBase>& Rp,
		const boost::shared_ptr<RsmGrid>& Rgrid);
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  %python_attribute(rational_polynomial,
		    boost::shared_ptr<RsmBase>);
  %python_attribute(correction_grid, boost::shared_ptr<RsmGrid>);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RsmRpPlusGrid")
