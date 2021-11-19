#ifndef REFRACTION_H
#define REFRACTION_H
#include "ground_coordinate.h"

namespace GeoCal {
class Refraction : public Printable<Refraction> {
public:
  Refraction() {}
  virtual ~Refraction() {}

//-----------------------------------------------------------------------
/// Apply a refraction correction, taking a ground coordinate w/o
/// refraction to one with refraction applied.
//-----------------------------------------------------------------------
  
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_apply(const GroundCoordinate& Spacecraft_pos,
  		   const GroundCoordinate& Gc_no_refraction) const = 0;

//-----------------------------------------------------------------------
/// Reverse of refraction_apply.
//-----------------------------------------------------------------------
  
  virtual boost::shared_ptr<GroundCoordinate>
  refraction_reverse(const GroundCoordinate& Spacecraft_pos,
  		     const GroundCoordinate& Gc_with_refraction) const = 0;

//-----------------------------------------------------------------------
/// Print to a stream.
//-----------------------------------------------------------------------

  virtual void print(std::ostream& Os) const
  { Os << "Refraction";}
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(Refraction);
#endif
