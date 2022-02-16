#ifndef VELOCITY_ABERRATION_H
#define VELOCITY_ABERRATION_H
#include "ground_coordinate.h"

namespace GeoCal {
class VelocityAberration : public Printable<VelocityAberration> {
public:
  VelocityAberration() {}
  virtual ~VelocityAberration() {}

//-----------------------------------------------------------------------
/// Apply a velocity aberration correction, taking a ground coordinate w/o
/// aberration. We return the CartesianFixedLookVector including
/// aberration.
//-----------------------------------------------------------------------
  
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberation,
			    const boost::array<double, 3> &Velocity_cf)
    const = 0;

//-----------------------------------------------------------------------
/// Reverse of velocity_aberration_apply.
//-----------------------------------------------------------------------
  
  virtual CartesianFixedLookVector
  velocity_aberration_reverse(const GroundCoordinate& Spacecraft_pos,
		      const GroundCoordinate& Gc_with_aberation,
		      const boost::array<double, 3> &Velocity_cf) const = 0;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "VelocityAberration\n";}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

//-----------------------------------------------------------------------
/// Implementation that doesn't do a correction. This isn't something
/// you would normally want, VelocityAberration is generally an
/// important effect to include. But this can be useful in test code,
/// e.g., for comparing to the MSP library with velocity aberration
/// turned off in order to focus on other differences.
//-----------------------------------------------------------------------
  
class NoVelocityAberration : public VelocityAberration {
public:
  NoVelocityAberration() {}
  virtual ~NoVelocityAberration() {}
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberation,
			    const boost::array<double, 3> &Velocity_cf)
    const
  { return CartesianFixedLookVector(Spacecraft_pos, Gc_no_aberation); }

//-----------------------------------------------------------------------
/// Reverse of velocity_aberration_apply.
//-----------------------------------------------------------------------
  
  virtual CartesianFixedLookVector
  velocity_aberration_reverse(const GroundCoordinate& Spacecraft_pos,
		      const GroundCoordinate& Gc_with_aberation,
		      const boost::array<double, 3> &Velocity_cf) const
  { return CartesianFixedLookVector(Spacecraft_pos, Gc_with_aberation); }

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "NoVelocityAberration\n";}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(VelocityAberration);
GEOCAL_EXPORT_KEY(NoVelocityAberration);
#endif
