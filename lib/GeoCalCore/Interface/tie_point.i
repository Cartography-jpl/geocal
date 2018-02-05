// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "tie_point.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%import "ground_coordinate.i"
%import "igc_collection.i"
%geocal_shared_ptr(GeoCal::TiePoint);
%geocal_shared_ptr(GeoCal::TiePointCollection);
%geocal_shared_ptr(std::vector<boost::shared_ptr<GeoCal::TiePoint> >);
namespace GeoCal {
class TiePoint : public GenericObject {
public:
  TiePoint(int Number_image);
  TiePoint();
  TiePoint(const TiePoint& Tp);
  %python_attribute_with_set(id, int);
  %python_attribute_with_set(is_gcp, bool);
  %python_attribute_with_set(ground_location,
			     boost::shared_ptr<GroundCoordinate>);
  %python_attribute(number_image, int);
  %python_attribute(number_image_location, int);
  boost::shared_ptr<ImageCoordinate> image_coordinate(int Image_index) const;
  void image_coordinate(int Image_index,
			const boost::shared_ptr<ImageCoordinate>& Ic,
			double Line_sigma = 0.1, double Sample_sigma = 0.1);
  double line_sigma(int Image_index) const;
  double sample_sigma(int Image_index) const;
  std::string print_to_string() const;
  %python_attribute(ic, blitz::Array<double, 2>);
  %python_attribute(ic_sigma, blitz::Array<double, 2>);
  blitz::Array<double, 2> ic_pred(const IgcCollection& Igccol) const;
  blitz::Array<double, 2> ic_diff(const IgcCollection& Igccol) const;
  %pickle_serialization();
};
}
%template(TiePointVector) std::vector<boost::shared_ptr<GeoCal::TiePoint> >;
namespace GeoCal {
class TiePointCollection : public GenericObject,
		   public std::vector<boost::shared_ptr<GeoCal::TiePoint> > {
public:
  TiePointCollection();
  TiePointCollection(const std::vector<boost::shared_ptr<GeoCal::TiePoint> >&
		     Tpv);
  %python_attribute(number_gcp, int);
  std::string print_to_string() const;
  %pickle_serialization();
};
}
