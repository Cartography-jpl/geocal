#ifndef VELOCITY_ABERRATION_EXACT_H
#define VELOCITY_ABERRATION_EXACT_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This does an exact velocity aberration correction, rather than
  the first order correction we do in QuaternionOrbitData. This
  matches what the MSP library does.

  We may change this interface a bit an introduce other
  implementation, this particular class is written to give a reference
  to compare other things to and isn't written for speed at all.
  We may introduce a base class like we have for Refraction, but since
  we don't really know what interface we want here we'll have this as
  a stand alone class for now. We may be able to combine this with
  Refraction into a some sort of "PointingCorrection" class or
  something like that, but again for now we'll keep these separate.
*******************************************************************/
class VelocityAberrationExact : public Printable<VelocityAberrationExact> {
public:
  VelocityAberrationExact() {}
  virtual ~VelocityAberrationExact() {};

  CartesianFixedLookVector aberration_calc
  (const QuaternionOrbitData& Od,
   const ScLookVector& Sl,
   const GroundCoordinate& Gc_before_correction,
   bool Forward_calc = true) const;
  virtual CartesianFixedLookVector
  aberration_apply(const QuaternionOrbitData& Od,
		   const ScLookVector& Sl,
		   const GroundCoordinate& Gc_approx_aberration) const
  { return aberration_calc(Od, Sl, Gc_approx_aberration, true); }
  virtual CartesianFixedLookVector
  aberration_reverse(const QuaternionOrbitData& Od,
		     const ScLookVector& Sl,
  		     const GroundCoordinate& Gc_full_aberration) const
  { return aberration_calc(Od, Sl, Gc_full_aberration, false); }
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(VelocityAberrationExact);
#endif
