#ifndef ORBIT_DES_H
#define ORBIT_DES_H
#include "orbit.h"
#include <boost/utility.hpp>

namespace GeoCal {
/****************************************************************//**
  This handles position reading, writing, and interpolation. This
  uses the NITF DES CSEPHB (See the SNIP documentation).

  Note that this class doesn't read and write the full DES, only the
  data portion. It works with the python code found in
  geocal_des_extension.py.

  The CSEPHB DES doens't contain velocity. We calculate the velocity
  from the positions.

  Note that the the CSEPHB data is like a NITF TRE. But because it
  is a DES, it is potentially much larger. For efficiency, we read and
  write the data as istream and ostream rather than return strings as
  we typically do for TREs. On the python side, this can be mapped
  from a io object like FileHandle or BytesIO.
*******************************************************************/
class PosCsephb : public Printable<PosCsephb>, boost::noncopyable {
public:
  enum EphemerisDataQuality {EPHEMERIS_QUALITY_SUSPECT = 0,
			     EPHEMERIS_QUALITY_GOOD = 1};
  enum InterpolationType { NEAREST_NEIGHBOR = 0,
			   LINEAR = 1,
			   LAGRANGE = 2 };
  enum LagrangeOrder { NO_LAGRANGE = 0, LAGRANGE_1 = 1, LAGRANGE_3 = 3,
		       LAGRANGE_5 = 5,
		       LAGRANGE_7 = 7};
  enum EphemerisSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  PosCsephb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  virtual ~PosCsephb() {}
  blitz::Array<double, 1> pos_vel(const Time& T) const;
  blitz::Array<AutoDerivative<double>, 1>
  pos_vel(const TimeWithDerivative& T) const;

//-----------------------------------------------------------------------
/// Interpolation type.
//-----------------------------------------------------------------------

  InterpolationType interpolation_type() const { return itype_; }
  void interpolation_type(InterpolationType Itype) { itype_ = Itype; }

//-----------------------------------------------------------------------
/// Ephemeris data quality
//-----------------------------------------------------------------------

  EphemerisDataQuality ephemeris_data_quality() const { return e_quality_; }
  void ephemeris_data_quality(EphemerisDataQuality E_quality)
  { e_quality_ = E_quality; }

//-----------------------------------------------------------------------
/// Ephemeris data source
//-----------------------------------------------------------------------

  EphemerisSource ephemeris_source() const { return e_source_; }
  void ephemeris_source(EphemerisSource E_source)
  { e_source_ = E_source; }

//-----------------------------------------------------------------------
/// Lagrange polynomial order
//-----------------------------------------------------------------------

  LagrangeOrder lagrange_order() const { return lagrange_order_; }
  void lagrange_order(LagrangeOrder Lagrange_order)
  { lagrange_order_ = Lagrange_order; }
  
//-----------------------------------------------------------------------
/// True if data is CartesianFixed. If false, then data is
/// CartesianInertial. 
//-----------------------------------------------------------------------

  bool is_cf() const { return is_cf_; }

//-----------------------------------------------------------------------
/// Minimum time we have data for.
//-----------------------------------------------------------------------

  const Time& min_time() const { return min_time_; }

  void min_time_split(std::string& d_mtime, std::string& t_mtime) const;
  
//-----------------------------------------------------------------------
/// Time step between position data, in seconds.
//-----------------------------------------------------------------------

  double time_step() const { return tstep_; }

//-----------------------------------------------------------------------
/// Maximum time we have data for.
//-----------------------------------------------------------------------

  Time max_time() const { return min_time_ + (pos.size() - 1) * time_step(); }
  virtual void print(std::ostream& Os) const;

  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<PosCsephb> des_read(std::istream& In);
private:
  Time min_time_;
  double tstep_;
  bool is_cf_;
  InterpolationType itype_;
  LagrangeOrder lagrange_order_;
  EphemerisDataQuality e_quality_;
  EphemerisSource e_source_;
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
