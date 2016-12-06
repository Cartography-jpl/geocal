// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)
%include "geocal_common.i"
%{
#include "coordinate_converter.h"
%}

%base_import(generic_object)
%import "look_vector.i"
%import "ground_coordinate.i"

%geocal_shared_ptr(GeoCal::CoordinateConverter);
%geocal_shared_ptr(GeoCal::GeodeticConverter);

namespace GeoCal {
class CoordinateConverter : public GenericObject {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const = 0;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const = 0;
  virtual bool is_same(const CoordinateConverter& Conv) const;
  std::string print_to_string() const;
};

class GeodeticConverter : public CoordinateConverter {
public:
  virtual boost::shared_ptr<GroundCoordinate>
    convert_from_coordinate(double X, double Y, double Height = 0) const;
  virtual void convert_to_coordinate(const GroundCoordinate& Gc, 
  double& OUTPUT, double& OUTPUT, double& OUTPUT) const;
  %pickle_serialization();
};
}
