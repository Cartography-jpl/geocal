// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "rsm_low_order_polynomial.h"
#include "image_ground_connection.h"
#include "coordinate_converter.h"  
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "image_ground_connection.i"
%import "coordinate_converter.i"

%geocal_shared_ptr(GeoCal::RsmLowOrderPolynomial);

namespace GeoCal {
class RsmLowOrderPolynomial : public GenericObject {
public:
  RsmLowOrderPolynomial(int Nline_fit=10, int Nsample_fit=10,
			int Nheight_fit=10,
			bool Ignore_igc_error_in_fit = false);
  std::string print_to_string() const;
  ImageCoordinate image_coordinate(double X, double Y, double Z) const;
  blitz::Array<double, 2> image_coordinate(const blitz::Array<double, 1>& X,
     const blitz::Array<double, 1>& Y, const blitz::Array<double, 1>& Z) const;
  blitz::Array<double, 3> image_coordinate(const blitz::Array<double, 2>& X,
     const blitz::Array<double, 2>& Y, const blitz::Array<double, 2>& Z) const;
  blitz::Array<double, 4> image_coordinate(const blitz::Array<double, 3>& X,
     const blitz::Array<double, 3>& Y, const blitz::Array<double, 3>& Z) const;
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
  %python_attribute_with_set(min_line, int);
  %python_attribute_with_set(max_line, int);
  %python_attribute_with_set(min_sample, int);
  %python_attribute_with_set(max_sample, int);
  %python_attribute_with_set(number_line_fit, int);
  %python_attribute_with_set(number_sample_fit, int);
  %python_attribute_with_set(number_height_fit, int);
  %python_attribute_with_set(ignore_igc_error_in_fit, bool);
  %python_attribute(parameter_line, blitz::Array<double, 1>);
  %python_attribute(parameter_sample, blitz::Array<double, 1>);
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("RsmLowOrderPolynomial")
