#ifndef SPICE_ORBIT_H
#define SPICE_ORBIT_H
#include "orbit.h"
#include "ecr.h"

namespace GeoCal {
/****************************************************************//**
  This is an orbit that uses a SPICE kernel as the underlying 
  implementation.

  This class is used with earth satellites. Compare this class with
  SpicePlanetOrbit. These could perhaps be merged at some point, but
  for right now just keep these separate. Don't have enough experience
  with these classes yet to know what the interface should be like, or
  if these really could be merged.
*******************************************************************/
class SpiceOrbit : public Orbit {
public:
  enum {ISS_ID = 25544};
  SpiceOrbit(int Satellite_id, const std::string& Kernel_name="",
	     int Body_id = Ecr::EARTH_NAIF_CODE);
  virtual ~SpiceOrbit() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const
  { 
    // Right now don't handle derivatives. We can revisit this if
    // this becomes an issue.
    return orbit_data(T.value());
  }
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// The satellite ID. This is used to generate the number that the
/// SPICE toolkit uses, for example the TLE spacecraft code. Note that
/// the SPICE number is actually -100000 - satellite ID
//-----------------------------------------------------------------------

  int satellite_id() const { return satellite_id_;}
protected:
  SpiceOrbit() {}
private:
  int body_id_;
  int satellite_id_;
  int satellite_spice_id() const { return -100000 - satellite_id_; }
  std::string kernel_name_;
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive& Ar, const unsigned int version) const;
  template<class Archive>
  void load(Archive& Ar, const unsigned int version);
  GEOCAL_SPLIT_MEMBER();
};
}

GEOCAL_EXPORT_KEY(SpiceOrbit);
#endif
