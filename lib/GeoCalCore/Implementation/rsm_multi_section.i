// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_multi_section.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_base)
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmMultiSection);

namespace GeoCal {
class RsmMultiSection : public RsmBase {
public:
  RsmMultiSection(int Nline, int Nsamp, int Nrow_section, int Ncol_section,
		  const RsmBase& Rsm_prototype, int Border=5);
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  %pickle_serialization()
};
}
