// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_grid.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_base)
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmGrid);

namespace GeoCal {
class RsmGrid : public RsmBase {
public:
  RsmGrid(int N_x, int N_y, int N_z);
  virtual void fit(const ImageGroundConnection& Igc,
	   const CoordinateConverter& Cconv,
	   double Min_height, double Max_height,
	   int Min_line, int Max_line, int Min_sample,
	   int Max_sample,
	   int Nline = 20, int Nsample = 20, int Nheight = 20,
	   bool Skip_masked_point = false,
		   bool Ignore_error = false);
  %python_attribute(number_x, int);
  %python_attribute(number_y, int);
  %python_attribute(number_z, int);
  %python_attribute(x_start, double);
  %python_attribute(x_delta, double);
  %python_attribute(y_start, double);
  %python_attribute(y_delta, double);
  %python_attribute(z_start, double);
  %python_attribute(z_delta, double);
  %pickle_serialization()
};
}
