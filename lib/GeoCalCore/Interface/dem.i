// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "dem.h"
%}

%base_import(generic_object)
%import "ground_coordinate.i"

%geocal_shared_ptr(GeoCal::Dem);
namespace GeoCal {
class Dem : public GenericObject {
public:
  virtual double distance_to_surface(const GroundCoordinate& Gp) const = 0;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const = 0;
  boost::shared_ptr<CartesianFixed> intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height = 9000) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const = 0;
  std::string print_to_string() const;
};
}
