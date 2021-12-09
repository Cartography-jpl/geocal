// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "velocity_aberration_exact.h"
%}
%base_import(generic_object)
%import "orbit.i"
%import "ground_coordinate.i"
%import "look_vector.i"
%geocal_shared_ptr(GeoCal::VelocityAberrationExact);
namespace GeoCal {
class VelocityAberrationExact : public GenericObject {
public:
  VelocityAberrationExact();
  CartesianFixedLookVector aberration_calc
  (const QuaternionOrbitData& Od,
   const ScLookVector& Sl,
   const GroundCoordinate& Gc_before_correction,
   bool Forward_calc = true) const;
  virtual CartesianFixedLookVector
  aberration_apply(const QuaternionOrbitData& Od,
		   const ScLookVector& Sl,
		   const GroundCoordinate& Gc_approx_aberration) const;
  virtual CartesianFixedLookVector
  aberration_reverse(const QuaternionOrbitData& Od,
		     const ScLookVector& Sl,
  		     const GroundCoordinate& Gc_full_aberration) const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("VelocityAberrationExact")
