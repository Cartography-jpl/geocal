#ifndef VELOCITY_ABERRATION_EXACT_H
#define VELOCITY_ABERRATION_EXACT_H
#include "velocity_aberration.h"

namespace GeoCal {
/****************************************************************//**
  This does an exact velocity aberration correction, rather than
  the first order correction we do in QuaternionOrbitData. This
  matches what the MSP library does.
*******************************************************************/
class VelocityAberrationExact : public VelocityAberration {
public:
  VelocityAberrationExact() {}
  virtual ~VelocityAberrationExact() {};

  CartesianFixedLookVector aberration_calc
  (const GroundCoordinate& Spacecraft_pos,
   const GroundCoordinate& Gc_uncorrected,
   const boost::array<double, 3> &Velocity_cf,
   bool Forward_calc = true) const;
  virtual CartesianFixedLookVector
  velocity_aberration_apply(const GroundCoordinate& Spacecraft_pos,
			    const GroundCoordinate& Gc_no_aberration,
			    const boost::array<double, 3> &Velocity_cf) const
  { return aberration_calc(Spacecraft_pos, Gc_no_aberration,
			   Velocity_cf, true);
  }
  virtual CartesianFixedLookVector
  velocity_aberration_reverse(const GroundCoordinate& Spacecraft_pos,
		      const GroundCoordinate& Gc_with_aberration,
		      const boost::array<double, 3> &Velocity_cf) const
  { return aberration_calc(Spacecraft_pos, Gc_with_aberration,
			   Velocity_cf, false);
  }
  virtual void print(std::ostream& Os) const
  { Os << "VelocityAberrationExact\n";}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(VelocityAberrationExact);
#endif
