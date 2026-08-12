#ifndef ORBIT_DES_H
#define ORBIT_DES_H
#include "orbit.h"
#include "ecr.h"
#include <boost/utility.hpp>

namespace GeoCal {
class OrbitDes;			// Forward declaration
  
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
  enum PositionType { SAME_AS_ORBIT = 0, CARTESIAN_FIXED = 1,
		      CARTESIAN_INERTIAL = 2 };
  enum EphemerisSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  PosCsephb(const blitz::Array<double, 2>& Pos, const Time& Tmin, double Tstep,
	    bool Is_cf,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL);
  PosCsephb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL,
	    PositionType P_type = SAME_AS_ORBIT);
	    
  PosCsephb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    EphemerisDataQuality E_quality = EPHEMERIS_QUALITY_GOOD,
	    EphemerisSource E_source = ACTUAL,
	    PositionType P_type = SAME_AS_ORBIT);
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

  Time max_time() const { return min_time_ + (pos.rows() - 1) * time_step(); }
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Raw data
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& position_data() const { return pos; }

  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<PosCsephb> des_read(std::istream& In);

//-----------------------------------------------------------------------
/// The UUID for the DES that contains this object, if any. This is an
/// empty string we don't have a actual DES we are part of.
//-----------------------------------------------------------------------

  std::string id() const { return id_;}
  void id(const std::string& V) { id_ = V;}

private:
  friend OrbitDes;
  void convert_cf_and_ci(const OrbitDes& orb);  
  std::string id_;
  Time min_time_;
  double tstep_;
  bool is_cf_;
  InterpolationType itype_;
  LagrangeOrder lagrange_order_;
  EphemerisDataQuality e_quality_;
  EphemerisSource e_source_;
  blitz::Array<double, 2> pos;
  PosCsephb() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};

/****************************************************************//**
  This handles attitude reading, writing, and interpolation. This
  uses the NITF DES CSEATTB (See the SNIP documentation).

  Note that this class doesn't read and write the full DES, only the
  data portion. It works with the python code found in
  geocal_des_extension.py.

  Note that the the CSATTB data is like a NITF TRE. But because it
  is a DES, it is potentially much larger. For efficiency, we read and
  write the data as istream and ostream rather than return strings as
  we typically do for TREs. On the python side, this can be mapped
  from a io object like FileHandle or BytesIO.

  Note that NITF has different convention for quaternions than we use
  internally with OrbitData. We use the functions nitf_to_quaternion
  and quaternion_to_nitf to map back and forth, look at the
  documentation of those functions to see what the mapping does.
*******************************************************************/
class AttCsattb : public Printable<AttCsattb>, boost::noncopyable {
public:
  enum AttitudeDataQuality {ATTITUDE_QUALITY_SUSPECT = 0,
			     ATTITUDE_QUALITY_GOOD = 1};
  enum InterpolationType { NEAREST_NEIGHBOR = 0,
			   LINEAR = 1,
			   LAGRANGE = 2 };
  enum LagrangeOrder { NO_LAGRANGE = 0, LAGRANGE_1 = 1, LAGRANGE_3 = 3,
		       LAGRANGE_5 = 5,
		       LAGRANGE_7 = 7};
  enum AttitudeSource { PREDICTED = 0, ACTUAL = 1, REFINED = 2 };
  enum AttitudeType { SAME_AS_ORBIT = 0, CARTESIAN_FIXED = 1,
		      CARTESIAN_INERTIAL = 2 };
  AttCsattb(const blitz::Array<double, 2>& Att, const Time& Tmin, double Tstep,
	    bool Is_cf,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality A_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource A_source = ACTUAL);
  AttCsattb(const Orbit& Orb, double Tstep, InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality A_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource A_source = ACTUAL,
	    AttitudeType A_type = SAME_AS_ORBIT);
  AttCsattb(const Orbit& Orb, const Time& Min_time, const Time& Max_time,
	    double Tstep,
	    InterpolationType Itype = LINEAR,
	    LagrangeOrder Lagrange_order = NO_LAGRANGE,
	    AttitudeDataQuality A_quality = ATTITUDE_QUALITY_GOOD,
	    AttitudeSource A_source = ACTUAL,
	    AttitudeType A_type = SAME_AS_ORBIT);
  virtual ~AttCsattb() {}
  boost::math::quaternion<double> att_q(const Time& T) const;
  boost::math::quaternion<AutoDerivative<double> > 
  att_q(const TimeWithDerivative& T) const;

//-----------------------------------------------------------------------
/// Interpolation type.
//-----------------------------------------------------------------------

  InterpolationType interpolation_type() const { return itype_; }
  void interpolation_type(InterpolationType Itype) { itype_ = Itype; }

//-----------------------------------------------------------------------
/// Attitude data quality
//-----------------------------------------------------------------------

  AttitudeDataQuality attitude_data_quality() const { return a_quality_; }
  void attitude_data_quality(AttitudeDataQuality A_quality)
  { a_quality_ = A_quality; }

//-----------------------------------------------------------------------
/// Attitude data source
//-----------------------------------------------------------------------

  AttitudeSource attitude_source() const { return a_source_; }
  void attitude_source(AttitudeSource A_source)
  { a_source_ = A_source; }

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
/// Time step between attitude data, in seconds.
//-----------------------------------------------------------------------

  double time_step() const { return tstep_; }

//-----------------------------------------------------------------------
/// Maximum time we have data for.
//-----------------------------------------------------------------------

  Time max_time() const { return min_time_ + (att.rows() - 1) * time_step(); }
  virtual void print(std::ostream& Os) const;

//-----------------------------------------------------------------------
/// Raw data
//-----------------------------------------------------------------------

  const blitz::Array<double, 2>& attitude_data() const { return att; }

  void des_write(std::ostream& Os) const;
  static boost::shared_ptr<AttCsattb> des_read(std::istream& In);

//-----------------------------------------------------------------------
/// The UUID for the DES that contains this object, if any. This is an
/// empty string we don't have a actual DES we are part of.
//-----------------------------------------------------------------------

  std::string id() const { return id_;}
  void id(const std::string& V) { id_ = V;}

private:
  friend OrbitDes;
  void convert_cf_and_ci(const OrbitDes& orb);
  std::string id_;
  Time min_time_;
  double tstep_;
  bool is_cf_;
  InterpolationType itype_;
  LagrangeOrder lagrange_order_;
  AttitudeDataQuality a_quality_;
  AttitudeSource a_source_;
  blitz::Array<double, 2> att;
  AttCsattb() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
  
/****************************************************************//**
  This combines a PosCsephb and AttCsattb into an Orbit. Note that
  we calculate things for every time point. If you are using this
  a bit you may want to create a OrbitQuaternionList from this Orbit
  to speed things up.

  Note that this orbit uses a different convention for the
  ScLookVector coordinates than we use in for example KeplerOrbit
  based on the EOS/MISR convention. We have +y in the line/along track
  direction and +x in the sample/cross track direction. The other
  convention we use in other orbits (e.g., MISR) has +x in the line
  direction and +y in the sample direction.
*******************************************************************/

class OrbitDes: public Orbit {
public:
  OrbitDes(const boost::shared_ptr<PosCsephb>& Pos,
	   const boost::shared_ptr<AttCsattb>& Att,
	   int Naif_code = Ecr::EARTH_NAIF_CODE);
  virtual ~OrbitDes() {}
  virtual boost::shared_ptr<OrbitData> orbit_data(Time T) const;
  virtual boost::shared_ptr<OrbitData> 
  orbit_data(const TimeWithDerivative& T) const;
  virtual void print(std::ostream& Os) const;
  const boost::shared_ptr<PosCsephb>& pos_csephb() const {return pos_;}
  const boost::shared_ptr<AttCsattb>& att_csattb() const {return att_;}
  int naif_code() const { return naif_code_; }
  void naif_code(int V) { naif_code_ = V; }

//-----------------------------------------------------------------------
/// Convert PosCsephb and AttCsattb to use CartesianFixed. If they
/// already do this, then this doesn't do anything.
//-----------------------------------------------------------------------

  void convert_to_cf()
  {
    if(!pos_->is_cf()) {
      att_->convert_cf_and_ci(*this);
      pos_->convert_cf_and_ci(*this);
      att_->is_cf_ = true;
    }
  }

//-----------------------------------------------------------------------
/// Convert PosCsephb and AttCsattb to use CartesianInertial. If they
/// already do this, then this doesn't do anything.
//-----------------------------------------------------------------------

  void convert_to_ci()
  {
    if(pos_->is_cf()) {
      att_->convert_cf_and_ci(*this);
      pos_->convert_cf_and_ci(*this);
      att_->is_cf_ = false;
    }
  }
  
private:
  boost::shared_ptr<PosCsephb> pos_;
  boost::shared_ptr<AttCsattb> att_;
  int naif_code_;
  OrbitDes() {}
  friend class boost::serialization::access;
  template<class Archive>
  void serialize(Archive & ar, const unsigned int version);
};
}

GEOCAL_EXPORT_KEY(PosCsephb);
GEOCAL_EXPORT_KEY(AttCsattb);
GEOCAL_EXPORT_KEY(OrbitDes);
#endif
