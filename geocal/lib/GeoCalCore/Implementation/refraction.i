// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "common.i"

%{
#include "refraction.h"
%}
%base_import(generic_object)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::Refraction);
namespace GeoCal {
class Refraction : public GenericObject {
public:
  Refraction(double Altitude, double Latitude, 
	     double Index_refraction_surface = -1);
  %python_attribute(altitude, double)
  %python_attribute(latitude, double)
  %python_attribute_with_set(index_refraction_surface, double)
  double surface_zenith(double Space_zenith) const;
  double displacement(double Space_zenith) const;
  boost::shared_ptr<GroundCoordinate>
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const;
  boost::shared_ptr<GroundCoordinate>
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const;
  std::string print_to_string() const;
  %pickle_init(1, self.altitude, self.latitude, self.index_refraction_surface)
};
}
