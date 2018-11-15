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
  PosCsephb(const Orbit& Orb, double Tstep);
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep);
  virtual ~PosCsephb() {}
  blitz::Array<double, 1> pos_vel(const Time& T) const;
  blitz::Array<AutoDerivative<double>, 1>
  pos_vel(const TimeWithDerivative& T) const;

//-----------------------------------------------------------------------
/// True if data is CartesianFixed. If false, then data is
/// CartesianInertial. 
//-----------------------------------------------------------------------

  bool is_cf() const { return is_cf_; }

//-----------------------------------------------------------------------
/// Minimum time we have data for.
//-----------------------------------------------------------------------

  const Time& min_time() const { return min_time_; }

//-----------------------------------------------------------------------
/// Time step between position data, in seconds.
//-----------------------------------------------------------------------

  double time_step() const { return tstep_; }

//-----------------------------------------------------------------------
/// Maximum time we have data for.
//-----------------------------------------------------------------------

  Time max_time() const { return min_time_ + (pos.size() - 1) * time_step(); }
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

/****************************************************************//**
  This combines a PosCsephb and AttCsattb into an Orbit. Note that
  we calculate things for every time point. If you are using this
  a bit you may want to create a OrbitQuaternionList from this Orbit
  to speed things up.
*******************************************************************/

class OrbitDes: public Orbit {
public:
  OrbitDes(const boost::shared_ptr<PosCsephb>& Pos);
  virtual ~OrbitDes() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;
  const boost::shared_ptr<PosCsephb>& pos_csephb() const {return pos_;}
private:
  boost::shared_ptr<PosCsephb> pos_;
  OrbitDes() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PosCsephb);
GEOCAL_EXPORT_KEY(OrbitDes);
#endif
