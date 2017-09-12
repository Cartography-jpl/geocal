// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_rational_polynomial.h"
#include "geocal_rpc.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(rsm_base)
%import "image_coordinate.i"
%import "rsm_polynomial.i"
%import "geocal_rpc.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmRationalPolynomial);

namespace GeoCal {
class RsmRationalPolynomial : public RsmBase {
public:
  RsmRationalPolynomial(int Np_x, int Np_y, int Np_z, int Dp_x, int Dp_y,
			int Dp_z, int N_max_order = -1, int D_max_order = -1);
  std::string print_to_string() const;
  ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  blitz::Array<double, 2> image_coordinate(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 3> image_coordinate(const blitz::Array<double, 2>& X,
     const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  blitz::Array<double, 4> image_coordinate(const blitz::Array<double, 3>& X,
     const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
  void set_rpc_coeff(const Rpc& R);
  void fit_offset_and_scale(double Min_line, double Max_line, double Min_sample,
			    double Max_sample, double Min_x, double Max_x,
			    double Min_y, double Max_y, double Min_z,
			    double Max_z);
  void fit_data(const std::vector<double>& Line,
		const std::vector<double>& Sample,
		const std::vector<double>& X,
		const std::vector<double>& Y,
		const std::vector<double>& Z);
  void fit(const ImageGroundConnection& Igc,
	   const CoordinateConverter& Cconv,
	   double Min_height, double Max_height,
	   int Min_line, int Max_line, int Min_sample,
	   int Max_sample);
  blitz::Array<double, 2> image_coordinate_jacobian
  (double X, double Y, double Z) const;

  %python_attribute_with_set(number_line_fit, int);
  %python_attribute_with_set(number_sample_fit, int);
  %python_attribute_with_set(number_height_fit, int);
  %python_attribute_with_set(number_second_pass_fit, int);
  %python_attribute_with_set(ignore_igc_in_error_fit, bool);
  %python_attribute(line_offset, double);
  %python_attribute(line_scale, double);
  %python_attribute(sample_offset, double);
  %python_attribute(sample_scale, double);
  %python_attribute(x_offset, double);
  %python_attribute(x_scale, double);
  %python_attribute(y_offset, double);
  %python_attribute(y_scale, double);
  %python_attribute(z_offset, double);
  %python_attribute(z_scale, double);
  %python_attribute(line_numerator, const RsmPolynomial&)
  %python_attribute(line_denominator, const RsmPolynomial&)
  %python_attribute(sample_numerator, const RsmPolynomial&)
  %python_attribute(sample_denominator, const RsmPolynomial&)
  %pickle_serialization()
};
}
