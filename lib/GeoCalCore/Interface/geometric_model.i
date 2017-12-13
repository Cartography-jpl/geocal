// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "geometric_model.h"
%}
%base_import(generic_object)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::GeometricModel);
%geocal_shared_ptr(GeoCal::GeometricTiePoints);

namespace GeoCal {
%nodefaultctor GeometricModel;

class GeometricModel: public GenericObject {
public:
  virtual ImageCoordinate original_image_coordinate
  (const ImageCoordinate& Resampled_ic) const;
  virtual ImageCoordinate resampled_image_coordinate
  (const ImageCoordinate& Resampled_ic) const;
  std::string print_to_string() const;
  %pickle_serialization()
};

class GeometricTiePoints : public GenericObject {
public:
  GeometricTiePoints();
  void add_point(const ImageCoordinate& Resampled_ic,
		 const ImageCoordinate& Original_ic);
  void start_replacing();
  void remove_point(int Index);
  %python_attribute(x, blitz::Array<double, 2>)
  %python_attribute(y, blitz::Array<double, 2>)
  %python_attribute(number_point, int);
  std::string print_to_string() const;
  %pickle_serialization()
};
}

