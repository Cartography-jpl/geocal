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
  RsmGrid(int N_x, int N_y, int N_z, bool Ignore_igc_error_in_fit=false,
	  int Total_number_row_digit = 11,
	  int Total_number_col_digit = 11,
	  int Number_fractional_row_digit = 2,
	  int Number_fractional_col_digit = 2,
	  int Row_section_number = 1,
	  int Col_section_number = 1,
	  const std::string& Image_identifier="",
	  const std::string& Rsm_support_data_edition="fake-1");
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

  virtual ImageCoordinate image_coordinate(double X, double Y, double Z,
					   bool Extrapolate_ok) const;
  virtual ImageCoordinate image_coordinate(double X, double Y, double Z) const;

  virtual blitz::Array<double, 2> image_coordinate
    (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
     const blitz::Array<double, 1>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 2> image_coordinate
    (const blitz::Array<double, 1>& X, const blitz::Array<double, 1>& Y,
     const blitz::Array<double, 1>& Z) const;

  virtual blitz::Array<double, 3> image_coordinate
    (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
     const blitz::Array<double, 2>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 3> image_coordinate
  (const blitz::Array<double, 2>& X, const blitz::Array<double, 2>& Y,
   const blitz::Array<double, 2>& Z) const;

  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 4> image_coordinate
  (const blitz::Array<double, 3>& X, const blitz::Array<double, 3>& Y,
   const blitz::Array<double, 3>& Z) const;

  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z, bool Extrapolate_ok) const;
  virtual blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;

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
  %python_attribute_with_set(total_number_row_digit, int);
  %python_attribute_with_set(total_number_col_digit, int);
  %python_attribute_with_set(number_fractional_row_digit, int);
  %python_attribute_with_set(number_fractional_col_digit, int);
  void extrapolate_x_direction();
  void extrapolate_y_direction();
  void extrapolate_z_direction();
  int tre_size() const;
  std::string tre_string() const;
  static boost::shared_ptr<RsmGrid>
  read_tre_string(const std::string& Tre_in);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RsmGrid")
