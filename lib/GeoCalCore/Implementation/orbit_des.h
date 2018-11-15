#ifndef ORBIT_DES_H
#define ORBIT_DES_H
#include "orbit.h"

namespace GeoCal {
/****************************************************************//**
  This handles position reading, writing, and interpolation. This
  uses the NITF DES CSEPHB (See the SNIP documentation).

  Note that this class doesn't read and write the full DES, only the
  data portion. It works with the python code found in
  geocal_des_extension.py.

  The CSEPHB DES doens't contain velocity. We calculate the velocity
  from the positions.
*******************************************************************/
class PosCsephb : public Printable<PosCsephb> {
public:
  PosCsephb(Orbit& Orb, double Tstep);
  PosCsephb(Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep);
  virtual ~PosCsephb() {}
  virtual void print(std::ostream& Os) const;
private:
  Time min_time_;
  double tstep_;
  bool is_cf_;
  std::vector<blitz::Array<double, 1> > pos;
  PosCsephb() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PosCsephb);
#endif
