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
  RsmGrid(int N_x, int N_y, int N_z, bool Ignore_igc_error_in_fit=false);
  virtual void fit(const ImageGroundConnection& Igc,
		   const CoordinateConverter& Cconv,
		   double Min_height, double Max_height,
		   int Min_line, int Max_line, int Min_sample,
		   int Max_sample);
  void fit_corr(const ImageGroundConnection& IGc,
		const CoordinateConverter& Cconv,
		const RsmBase& Rb);
  %python_attribute_with_set(ignore_igc_error_in_fit, bool);
  %python_attribute(line_grid, blitz::Array<double, 3>);
  %python_attribute(sample_grid, blitz::Array<double, 3>);
  int number_x(int Zindex) const;
  int number_y(int Zindex) const;
  int x_offset(int Zindex) const;
  int y_offset(int Zindex) const;
  %python_attribute(number_z, int);
  %python_attribute(x_start, double);
  %python_attribute(x_delta, double);
  %python_attribute(y_start, double);
  %python_attribute(y_delta, double);
  %python_attribute(z_start, double);
  %python_attribute(z_delta, double);
  %python_attribute(total_number_row_digit, int);
  %python_attribute(total_number_col_digit, int);
  %python_attribute(number_fractional_row_digit, int);
  %python_attribute(number_fractional_col_digit, int);
  std::string tre_string() const;
  static boost::shared_ptr<RsmGrid>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
