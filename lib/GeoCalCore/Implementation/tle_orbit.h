#ifndef TLE_ORBIT_H
#define TLE_ORBIT_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
   This is an orbit that does the calculation based on a two-line
   element set (TLE, see
   https://en.wikipedia.org/wiki/Two-line_element_set).

   You can get TLE for a particular satellite and time from 
   https://www.space-track.org.
*******************************************************************/
class TleOrbit : public Orbit {
public:
//-----------------------------------------------------------------------
/// Constructor. The TLE should be in the standard two-line element
/// set format (see
/// https://en.wikipedia.org/wiki/Two-line_element_set).
//-----------------------------------------------------------------------

  TleOrbit(const std::string& Tle)
    : tle_(Tle), elem_filled_in(false) {}
  virtual ~TleOrbit() {}
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
/// The TLE that this orbit is using.  
//-----------------------------------------------------------------------

  const std::string& tle() const { return tle_;}

//-----------------------------------------------------------------------
/// The epoch for the TLE
//-----------------------------------------------------------------------

  const Time& epoch() const { fill_in_elems(); return epoch_; }

  int revolution_number_at_epoch() const;
private:
  static double geophs[8];
  std::string tle_;
  mutable bool elem_filled_in;
  mutable double elems[10];
  mutable Time epoch_;
  void fill_in_elems() const;
  TleOrbit() : elem_filled_in(false) {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(TleOrbit);
#endif
