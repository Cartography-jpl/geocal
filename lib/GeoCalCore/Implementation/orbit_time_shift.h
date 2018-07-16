#ifndef ORBIT_TIME_SHIFT_H
#define ORBIT_TIME_SHIFT_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This is a simple adapter for an underlying orbit that allows us to
  "shift" the time of orbit. For a time "t", this returns orbit data
  from the underlying data at t-toffset, but with the time for the
  orbit data changed to "t".

  This is useful for simulations, where we may have an underlying
  orbit that otherwise matches some desired characteristics but that
  is at the wrong time (e.g., the orbit passes over a desired point
  but at local night when a simulation requires local day).  It is
  possible this might end up having other uses (e.g., correct for a
  known time error), but the simulation was what we originally
  developed this for.
*******************************************************************/
class OrbitTimeShift : public Orbit {
public:
  OrbitTimeShift(const boost::shared_ptr<Orbit> Orbit_original,
		 double Time_shift);
  virtual ~OrbitTimeShift() {}
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
/// The original orbit
//-----------------------------------------------------------------------

  const boost::shared_ptr<Orbit> orbit_original() const
  {return orbit_original_; }

//-----------------------------------------------------------------------
/// Time shift applied to orbit, in seconds.
//-----------------------------------------------------------------------

  double time_shift() const {return time_shift_;}
protected:
  OrbitTimeShift() {}
private:
  boost::shared_ptr<Orbit>  orbit_original_;
  double time_shift_;
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(OrbitTimeShift);
#endif


