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
  SpiceKeplerOrbit(const blitz::Array<double, 1>& elements, Time min_time = Time::min_valid_time,
		   Time max_time = Time::max_valid_time);
  virtual ~SpiceKeplerOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const
  { 
    // Right now don't handle derivatives. We can revisit this if
    // this becomes an issue.
    return orbit_data(T.value());
  }
  const blitz::Array<double, 1>& elements() const { return elements_;}
  virtual void print(std::ostream& Os) const;
  static double mu(const std::string& Body_name);
  static blitz::Array<double, 1> spice_conics(const blitz::Array<double, 1>& elements, Time T);
  static blitz::Array<double, 1> spice_oscelt(const OrbitData& Od);
private:
  SpiceKeplerOrbit() {}
  blitz::Array<double, 1> elements_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(SpiceKeplerOrbit);
#endif


