// -*- mode: c++; -*-
// (Not really c++, but closest emacs mode)

%include "geocal_common.i"

%{
#include "velocity_aberration.h"
%}
%base_import(generic_object)
%import "ground_coordinate.i"
%import "look_vector.i"
%geocal_shared_ptr(GeoCal::VelocityAberration);
%geocal_shared_ptr(GeoCal::NoVelocityAberration);
%geocal_shared_ptr(GeoCal::VelocityAberrationFirstOrder);
namespace GeoCal {
class VelocityAberration : public GenericObject {
public:
  VelocityAberration();
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
		    const GroundCoordinate& Gc_no_aberration,
		    const boost::array<double, 3> &Velocity_cf) const = 0;
  virtual CartesianFixedLookVector
  velocity_aberration_reverse(const GroundCoordinate& Spacecraft_pos,
		      const GroundCoordinate& Gc_with_aberration,
		      const boost::array<double, 3> &Velocity_cf) const;
  std::string print_to_string() const;
  %pickle_serialization()
};

class NoVelocityAberration : public VelocityAberration {
public:
  NoVelocityAberration();
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberration,
			    const boost::array<double, 3> &Velocity_cf) const;
  %pickle_serialization()
};

class VelocityAberrationFirstOrder : public VelocityAberration {
public:
  VelocityAberrationFirstOrder();
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberration,
			    const boost::array<double, 3> &Velocity_cf) const;
  %pickle_serialization()
};
}
// List of things "import *" will include
%python_export("VelocityAberration", "NoVelocityAberration", "VelocityAberrationFirstOrder")
