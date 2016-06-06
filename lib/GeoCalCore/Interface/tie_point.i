// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "tie_point.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::TiePoint);
namespace GeoCal {
class TiePoint {
  TiePoint(int Number_image);
  %python_attribute_with_set(id, int);
  %python_attribute_with_set(is_gcp, bool);
  %python_attribute_with_set(ground_location,
			     boost::shared_ptr<GroundCoordinate>);
  %python_attribute(number_image);
  %python_attribute(number_image_location);
  boost::shared_ptr<ImageCoordinate> image_coordinate(int Image_index) const;
  void image_coordinate(int Image_index,
			const boost::shared_ptr<ImageCoordinate>& Ic,
			double Line_sigma = 0.1, double Sample_sigma = 0.1);
  double line_sigma(int Image_index) const;
  double sample_sigma(int Image_index) const;
  std::string print_to_string() const;
  %pickle_serialization();
};
}
