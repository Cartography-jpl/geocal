// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "refraction.h"
%}
%base_import(generic_object)
%import "ground_coordinate.i"
%geocal_shared_ptr(GeoCal::Refraction);
namespace GeoCal {
class Refraction : public GenericObject {
public:
  Refraction();
  virtual CartesianFixedLookVector
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const = 0;
  virtual CartesianFixedLookVector
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const = 0;
  std::string print_to_string() const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("Refraction")
