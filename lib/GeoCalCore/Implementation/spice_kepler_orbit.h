#ifndef SPICE_KEPLER_ORBIT_H
#define SPICE_KEPLER_ORBIT_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This wraps around the JPL SPICE kepler orbit calculation data,
  making it into a geocal Orbit.
*******************************************************************/
class SpiceKeplerOrbit : public Orbit {
public:
  SpiceKeplerOrbit();
  virtual ~SpiceKeplerOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const
  { 
    // Right now don't handle derivatives. We can revisit this if
    // this becomes an issue.
    return orbit_data(T.value());
  }
  virtual void print(std::ostream& Os) const;
private:
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpiceKeplerOrbit);
#endif


