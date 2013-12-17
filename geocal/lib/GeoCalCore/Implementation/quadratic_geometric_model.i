// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "common.i"

%{
#include "quadratic_geometric_model.h"
%}
%base_import(geometric_model)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::QuadraticGeometricModel);

namespace GeoCal {
class QuadraticGeometricModel: public GeometricModel {
public:
  QuadraticGeometricModel(double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0);
  QuadraticGeometricModel(const blitz::Array<double, 1>& Transformation,
			  double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0);
  %python_attribute(transformation, blitz::Array<double, 1>)
  %python_attribute(magnify_line, double)
  %python_attribute(magnify_sample, double)
  %pickle_init(1, self.transformation, self.magnify_line, self.magnify_sample);
};

}

