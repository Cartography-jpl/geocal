// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "dem.h"
%}

%base_import(generic_object)
%import "look_vector.i"
%import "ground_coordinate.i"

%geocal_shared_ptr(GeoCal::Dem);
namespace GeoCal {
class Dem : public GenericObject {
public:
  virtual double distance_to_surface(const GroundCoordinate& Gp) const = 0;
  virtual double height_reference_surface(const GroundCoordinate& Gp) 
    const = 0;
  virtual boost::shared_ptr<CartesianFixed> intersect(const CartesianFixed& Cf,
      const CartesianFixedLookVector& Lv, double Resolution,
      double Max_height = 9000) const;
  virtual boost::shared_ptr<CartesianFixed> 
  intersect_start_length (const CartesianFixed& Cf,
			  const CartesianFixedLookVector& Lv, 
			  double Resolution, double Start_length) const;
  virtual void height_range(const GroundCoordinate& Ulc,
			    const GroundCoordinate& Lrc,
			    double &OUTPUT, double &OUTPUT,
			    double H_pad = 10.0) const;
  virtual boost::shared_ptr<GroundCoordinate> 
  surface_point(const GroundCoordinate& Gp) const = 0;
  std::string print_to_string() const;
};
}
// List of things "import *" will include
%python_export("Dem")
