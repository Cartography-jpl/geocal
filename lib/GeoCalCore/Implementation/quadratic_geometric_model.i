// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include <std_vector.i>
%include "geocal_common.i"

%{
#include "quadratic_geometric_model.h"
%}
%base_import(geometric_model)
%import "image_coordinate.i"
%geocal_shared_ptr(GeoCal::QuadraticGeometricModel);

namespace GeoCal {
class QuadraticGeometricModel: public GeometricModel {
public:
  enum FitType {LINEAR = 0, QUADRATIC};
  QuadraticGeometricModel(const boost::shared_ptr<GeometricTiePoints>& Tp,
			  FitType ft = LINEAR,
			  double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0);
  QuadraticGeometricModel(FitType ft = LINEAR,
			  double Magnify_line = 1.0, 
			  double Magnify_sample = 1.0);
  %python_attribute(tie_points, boost::shared_ptr<GeometricTiePoints>);
  void fit_transformation(const GeometricTiePoints& Tp);
  %python_attribute(transformation, blitz::Array<double, 1>)
  %python_attribute(inverse_transformation, blitz::Array<double, 1>)
  %python_attribute_with_set(magnify_line, double)
  %python_attribute_with_set(magnify_sample, double)
  %python_attribute(fit_type, FitType)
  %pickle_serialization();
};

}

