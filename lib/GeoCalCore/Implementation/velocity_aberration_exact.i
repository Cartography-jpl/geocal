// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "velocity_aberration_exact.h"
%}
%base_import(velocity_aberration)
%import "ground_coordinate.i"
%import "look_vector.i"
%geocal_shared_ptr(GeoCal::VelocityAberrationExact);
namespace GeoCal {
%feature("notabstract") VelocityAberrationExact;
class VelocityAberrationExact : public VelocityAberration {
public:
  VelocityAberrationExact();
  CartesianFixedLookVector aberration_calc
  (const GroundCoordinate& Spacecraft_pos,
   const GroundCoordinate& Gc_uncorrected,
   const boost::array<double, 3> &Velocity_cf,
   bool Forward_calc = true) const;
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberration,
			    const boost::array<double, 3> &Velocity_cf) const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("VelocityAberrationExact")
