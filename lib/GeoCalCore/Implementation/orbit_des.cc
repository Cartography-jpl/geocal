#include "orbit_des.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "eci.h"
#include "planet_coordinate.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
#include "nitf_support.h"
#include <boost/make_shared.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>
#include <boost/regex.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void PosCsephb::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(PosCsephb);
  ar & GEOCAL_NVP_(min_time) & GEOCAL_NVP_(tstep)
    & GEOCAL_NVP_(is_cf) & GEOCAL_NVP_(itype)
    & GEOCAL_NVP_(e_quality)
    & GEOCAL_NVP_(e_source)
    & GEOCAL_NVP_(lagrange_order)
    & GEOCAL_NVP(pos);
}

template<class Archive>
void AttCsattb::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(AttCsattb);
  ar & GEOCAL_NVP_(min_time) & GEOCAL_NVP_(tstep)
    & GEOCAL_NVP_(is_cf) & GEOCAL_NVP_(itype)
    & GEOCAL_NVP_(a_quality)
    & GEOCAL_NVP_(a_source)
    & GEOCAL_NVP_(lagrange_order)
    & GEOCAL_NVP(att);
}

template<class Archive>
void OrbitDes::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(pos) & GEOCAL_NVP_(att)
    & GEOCAL_NVP_(naif_code);
}

GEOCAL_IMPLEMENT(PosCsephb);
GEOCAL_IMPLEMENT(AttCsattb);
GEOCAL_IMPLEMENT(OrbitDes);
#endif


//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

PosCsephb::PosCsephb
(const blitz::Array<double, 2>& Pos, const Time& Tmin, double Tstep,
 bool Is_cf,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 EphemerisDataQuality E_quality,
 EphemerisSource E_source)
: min_time_(Tmin), tstep_(Tstep), is_cf_(Is_cf), 
  itype_(Itype), lagrange_order_(Lagrange_order),
  e_quality_(E_quality), e_source_(E_source),
  pos(Pos.copy())
{
}

//-----------------------------------------------------------------------
/// Constructor. We sample the position of the given Orbit at fixed
/// spaces times. This version goes from the min_time() of the Orbit,
/// up to max_time() (or more accurately, the largest time min_time()
/// + i * Tstep that is <= max_time()).
//-----------------------------------------------------------------------

PosCsephb::PosCsephb
(const Orbit& Orb, double Tstep,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 EphemerisDataQuality E_quality,
 EphemerisSource E_source,
 PositionType P_type)
  : min_time_(Orb.min_time()), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    e_quality_(E_quality), e_source_(E_source)
{
  if(P_type == SAME_AS_ORBIT)
    is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  else if(P_type == CARTESIAN_FIXED)
    is_cf_ = true;
  else
    is_cf_ = false;
  int sz = (int) floor((Orb.max_time() - min_time_) / tstep_) + 1;
  if((min_time_ + sz * tstep_) <= Orb.max_time())
    sz += 1;
  pos.resize(sz, 3);
  for(int i = 0; i < sz; ++i) {
    Time t = min_time_ + i * tstep_;
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      pos(i, blitz::Range::all()) =
	cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      pos(i, blitz::Range::all()) =
	ci->position[0], ci->position[1], ci->position[2];
    }
  }
}

//-----------------------------------------------------------------------
/// Constructor. We sample the position of the given Orbit at fixed
/// spaces times. This version goes from the Min_time,
/// up to Max_time (or more accurately, the largest time Min_time
/// + i * Tstep that is <= Max_time).w
//-----------------------------------------------------------------------

PosCsephb::PosCsephb
(const Orbit& Orb, const Time& Min_time, const Time& Max_time, double Tstep,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 EphemerisDataQuality E_quality,
 EphemerisSource E_source,
 PositionType P_type)
  : min_time_(Min_time), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    e_quality_(E_quality), e_source_(E_source)
{
  if(P_type == SAME_AS_ORBIT)
    is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  else if(P_type == CARTESIAN_FIXED)
    is_cf_ = true;
  else
    is_cf_ = false;
  int sz = (int) floor((Max_time - min_time_) / tstep_) + 1;
  if((min_time_ + sz * tstep_) <= Max_time)
    sz += 1;
  pos.resize(sz, 3);
  for(int i = 0; i < sz; ++i) {
    Time t = min_time_ + i * tstep_;
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      pos(i, blitz::Range::all()) =
	cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      pos(i, blitz::Range::all()) =
	ci->position[0], ci->position[1], ci->position[2];
    }
  }
}

void PosCsephb::print(std::ostream& Os) const
{
  Os << "PosCsephb\n"
     << "  Min time:           " << min_time_ << "\n"
     << "  Max time:           " << max_time() << "\n"
     << "  Tstep:              " << tstep_ << "\n"
     << "  Is_cf:              " << is_cf_ << "\n"
     << "  Interpolation type: "
     << (itype_ == NEAREST_NEIGHBOR ? "Nearest Neighbor" :
	 (itype_ == LINEAR ? "Linear" : "Lagrange")) << "\n"
     << "  Lagrange order:     " << (int) lagrange_order_ << "\n"
     << "  Ephemeris quality:  " << (e_quality_ == EPHEMERIS_QUALITY_SUSPECT ?
				     "Suspect" : "Good") << "\n"
     << "  Ephemeris source:   " << (e_source_ == PREDICTED ?
				     "Predicted" :
				     (e_source_ == ACTUAL ?
				      "Actual, time of collection" :
				      "Refined")) << "\n";
}

//-----------------------------------------------------------------------
/// Check if we need a sign flip.
//-----------------------------------------------------------------------

inline bool need_sign_flip(const blitz::Array<double, 1>& a1,
			   const blitz::Array<double, 1>& a2)
{
  boost::math::quaternion<double> q1 = nitf_to_quaternion(a1);
  boost::math::quaternion<double> q2 = nitf_to_quaternion(a2);
  // See interpolate_quaternion_rotation for the logic here.
  boost::math::quaternion<double> delta_quat = q2 * conj(q1);
  double t = delta_quat.R_component_1();
  if(t < 0)
    return true;
  return false;
}

//-----------------------------------------------------------------------
/// Return attitude quaternion for the given time.
//-----------------------------------------------------------------------

boost::math::quaternion<double> AttCsattb::att_q(const Time& T) const
{
  blitz::Range ra = blitz::Range::all();
  range_check_inclusive(T, min_time_, max_time());
  int i = (int) floor((T - min_time_) / tstep_);
  if(i >= att.rows() - 1)
    --i;
  blitz::Array<double, 1> res(4);
  if(itype_ == NEAREST_NEIGHBOR)
    res = att(i, ra);
  if(itype_ == LINEAR) {
    double f = (T - min_time_) / tstep_ - i;
    blitz::Array<double, 1> a1 = att(i+1, ra).copy();
    blitz::Array<double, 1> a0 = att(i, ra).copy();
    // Handle sign change in attitude
    if(need_sign_flip(a0,a1))
      a1 *= -1;
    res = f * a1 + (1 - f) * a0;
  }
  if(itype_ == LAGRANGE) {
    int d = (lagrange_order_ - 1) / 2;
    int istart = std::max(0, i - d);
    int iend = std::min(i + d + 2, att.rows());
    std::vector<Time> tm;
    std::vector<blitz::Array<double, 1> > a;
    for(int j = istart; j < iend; ++j) {
      tm.push_back(min_time_ + j * tstep_);
      blitz::Array<double, 1> av = att(j,ra).copy();
      //  Handle sign flips
      if(a.size() > 0 && need_sign_flip(a[0], av))
	av *= -1;
      a.push_back(av);
    }
    res = Orbit::lagrangian_interpolation(tm.begin(), tm.end(), T, a.begin(), a.end());
    //  Translate back to whatever sign the closest point has
    if(need_sign_flip(a[0], att(i,ra)))
      res *= -1;
  }
  boost::math::quaternion<double> resq = nitf_to_quaternion(res);
  normalize(resq);
  return resq;
}

//-----------------------------------------------------------------------
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<double, 1> PosCsephb::pos_vel(const Time& T) const
{
  blitz::Range ra = blitz::Range::all();
  range_check_inclusive(T, min_time_, max_time());
  int i = (int) floor((T - min_time_) / tstep_);
  if(i >= pos.rows() - 1)
    --i;
  blitz::Array<double, 1> res(6);
  if(itype_ == NEAREST_NEIGHBOR) {
    res(blitz::Range(0,2)) = pos(i, ra);
    // Use linear interpolation for velocity, otherwise this will
    // always be zero.
    res(blitz::Range(3,5)) = (pos(i+1, ra) - pos(i, ra)) / tstep_;
  }
  if(itype_ == LINEAR) {
    double f = (T - min_time_) / tstep_ - i;
    res(blitz::Range(0,2)) = f * pos(i+1, ra) + (1 - f) * pos(i, ra);
    res(blitz::Range(3,5)) = (pos(i+1, ra) - pos(i, ra)) / tstep_;
  }
  if(itype_ == LAGRANGE) {
    int d = (lagrange_order_ - 1) / 2;
    int istart = std::max(0, i - d);
    int iend = std::min(i + d + 2, pos.rows());
    std::vector<Time> tm;
    std::vector<blitz::Array<double, 1> > p;
    if(false)
      std::cerr << "i:      " << i << "\n"
		<< "istart: " << istart << "\n"
		<< "iend:   " << iend << "\n"
		<< "Time: " << T << "\n"
		<< "Points: " << "\n";
    for(int j = istart; j < iend; ++j) {
      if(false)
	std::cerr << min_time_ + j * tstep_ << "\n";
      tm.push_back(min_time_ + j * tstep_);
      p.push_back(pos(j,ra));
    }
    AutoDerivative<double> twd(T.pgs(), 0, 1);
    TimeWithDerivative t2 = TimeWithDerivative::time_pgs(twd);
    blitz::Array<AutoDerivative<double>, 1> pres =
      Orbit::lagrangian_interpolation(tm.begin(), tm.end(), t2, p.begin(),
				      p.end());
    res(0) = pres(0).value();
    res(1) = pres(1).value();
    res(2) = pres(2).value();
    res(3) = pres(0).gradient()(0);
    res(4) = pres(1).gradient()(0);
    res(5) = pres(2).gradient()(0);
  }
  
  return res;
}

//-----------------------------------------------------------------------
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<AutoDerivative<double>, 1> PosCsephb::pos_vel
(const TimeWithDerivative& T) const
{
  blitz::Range ra = blitz::Range::all();
  range_check_inclusive(T.value(), min_time_, max_time());
  int i = (int) floor((T.value() - min_time_) / tstep_);
  if(i >= pos.rows())
    --i;
  blitz::Array<AutoDerivative<double>, 1> res(6);
  if(itype_ == NEAREST_NEIGHBOR) {
    res(blitz::Range(0,2)) = pos(i, ra);
    // Use linear interpolation for velocity, otherwise this will
    // always be zero.
    res(blitz::Range(3,5)) = (pos(i+1, ra) - pos(i, ra)) / tstep_;
  }
  if(itype_ == LINEAR) {
    AutoDerivative<double> f = (T - min_time_) / tstep_ - i;
    res(blitz::Range(0,2)) = f * pos(i+1, ra) + (1 - f) * pos(i, ra);
    res(blitz::Range(3,5)) = (pos(i+1,ra) - pos(i,ra)) / tstep_;
  }
  if(itype_ == LAGRANGE) {
    int d = (lagrange_order_ - 1) / 2;
    int istart = std::max(0, i - d);
    int iend = std::min(i + d + 2, pos.rows());
    std::vector<Time> tm;
    std::vector<blitz::Array<double, 1> > p;
    for(int j = istart; j < iend; ++j) {
      tm.push_back(min_time_ + j * tstep_);
      p.push_back(pos(j,ra));
    }
    AutoDerivative<double> twd(T.pgs().value(), 0, 1);
    TimeWithDerivative t2 = TimeWithDerivative::time_pgs(twd);
    blitz::Array<AutoDerivative<double>, 1> pres =
      Orbit::lagrangian_interpolation(tm.begin(), tm.end(), T, p.begin(),
				      p.end());
    blitz::Array<AutoDerivative<double>, 1> pres2 =
      Orbit::lagrangian_interpolation(tm.begin(), tm.end(), t2, p.begin(),
				      p.end());
    res(0) = pres(0);
    res(1) = pres(1);
    res(2) = pres(2);
    res(3) = pres2(0).gradient()(0);
    res(4) = pres2(1).gradient()(0);
    res(5) = pres2(2).gradient()(0);
  }
  return res;
}

//-----------------------------------------------------------------------
/// Return attitude quaternion for the given time.
//-----------------------------------------------------------------------

boost::math::quaternion<AutoDerivative<double> > AttCsattb::att_q
(const TimeWithDerivative& T) const
{
  blitz::Range ra = blitz::Range::all();
  range_check_inclusive(T.value(), min_time_, max_time());
  int i = (int) floor((T.value() - min_time_) / tstep_);
  if(i >= att.rows() - 1)
    --i;
  blitz::Array<AutoDerivative<double>, 1> res(4);
  if(itype_ == NEAREST_NEIGHBOR)
    res = att(i, ra);
  if(itype_ == LINEAR) {
    AutoDerivative<double> f = (T - min_time_) / tstep_ - i;
    res = f * att(i+1, ra) + (1 - f) * att(i, ra);
  }
  if(itype_ == LAGRANGE) {
    int d = (lagrange_order_ - 1) / 2;
    int istart = std::max(0, i - d);
    int iend = std::min(i + d + 2, att.rows());
    std::vector<Time> tm;
    std::vector<blitz::Array<double, 1> > a;
    for(int j = istart; j < iend; ++j) {
      tm.push_back(min_time_ + j * tstep_);
      a.push_back(att(j,ra));
    }
    res = Orbit::lagrangian_interpolation(tm.begin(), tm.end(), T, a.begin(),
					  a.end());
  }
  boost::math::quaternion<AutoDerivative<double> > resq =
    nitf_to_quaternion(res);
  normalize(resq);
  return resq;
}

static boost::format frontpart("%|1$01d|%|2$01d|");
static boost::format largangeorder("%|1$01d|");
static boost::format nextpart("%|1$01d|%|2$01d|%|3$013.9f|%|4$8s|%|5$16s|%|6$05d|");
static boost::format numformat("%|1$+012.2f|");
static boost::format attnumformat("%|1$+18.15f|");
static boost::format resformat("%|1$09d|");

//-----------------------------------------------------------------------
/// Return min_time split into the component pieces the DES requires.
//-----------------------------------------------------------------------

void PosCsephb::min_time_split(std::string& d_mtime, std::string& t_mtime) const
{
  std::string mtime = min_time_.to_string();
  // Split into date and time parts
  // The time string is something like "1998-06-30T10:51:28.32Z"
  std::size_t t = mtime.find("T");
  d_mtime = mtime.substr(0, t);
  t_mtime = mtime.substr(t+1, -1);
  // Chop off trailing "Z" in time part"
  t_mtime = t_mtime.substr(0, t_mtime.size() - 1);
  // Remove the "-" in the date.
  d_mtime = boost::regex_replace(d_mtime, boost::regex("-"), "");
  // Remove the ":" in the time.
  t_mtime = boost::regex_replace(t_mtime, boost::regex(":"), "");
  // Pad with trailing "0" to full size"
  while(t_mtime.size() < 16)
    t_mtime += "0";
}

//-----------------------------------------------------------------------
/// Write out the DES data to the given stream.
//-----------------------------------------------------------------------

void PosCsephb::des_write(std::ostream& Os) const
{
  Os << str_check_size(frontpart % e_quality_ % itype_, 2);
  if(itype_ == LAGRANGE)
    Os << str_check_size(largangeorder % lagrange_order_, 1);
  // Indicate 1 for ECF, 0 for ECI
  int coor_frame = (is_cf_ ? 1 : 0);
  std::string d_mtime, t_mtime;
  min_time_split(d_mtime, t_mtime);
  Os << str_check_size(nextpart % e_source_ % coor_frame % tstep_ % d_mtime %
		       t_mtime % pos.rows(), 1 + 1 + 13 + 8 + 16 + 5);
  for(int i = 0; i < pos.rows(); ++i)
    Os << str_check_size(numformat % pos(i,0), 12)
       << str_check_size(numformat % pos(i,1), 12)
       << str_check_size(numformat % pos(i,2), 12);
  Os << str_check_size(resformat % 0, 9);
}

//-----------------------------------------------------------------------
/// Read the DES data  the given stream.
//-----------------------------------------------------------------------

boost::shared_ptr<PosCsephb> PosCsephb::des_read(std::istream& In)
{
  boost::shared_ptr<PosCsephb> res(new PosCsephb());
  res->e_quality_ = EphemerisDataQuality(read_size<int>(In, 1));
  res->itype_ = InterpolationType(read_size<int>(In, 1));
  res->lagrange_order_ = NO_LAGRANGE;	
  if(res->itype_ == LAGRANGE)
    res->lagrange_order_ = LagrangeOrder(read_size<int> (In, 1));
  res->e_source_ = EphemerisSource(read_size<int>(In, 1));
  int coor_frame = read_size<int>(In, 1);
  res->is_cf_ = (coor_frame == 1);
  res->tstep_ = read_size<double>(In,13);
  std::string d_mtime = read_size<std::string>(In, 8);
  std::string t_mtime = read_size<std::string>(In, 16);
  // Add "-" and ":"
  d_mtime.insert(6, "-");
  d_mtime.insert(4, "-");
  t_mtime.insert(4, ":");
  t_mtime.insert(2, ":");
  res->min_time_ = Time::parse_time(d_mtime + "T" + t_mtime + "Z");
  int sz = read_size<int>(In, 5);
  res->pos.resize(sz, 3);
  for(int i = 0; i < sz; ++i) {
    res->pos(i,0) = read_size<double>(In, 12);
    res->pos(i,1) = read_size<double>(In, 12);
    res->pos(i,2) = read_size<double>(In, 12);
  }
  int reserved_len = read_size<int>(In, 9);
  if(reserved_len > 0)
    std::string skipped = read_size<std::string>(In, reserved_len);
  return res;
}

//-----------------------------------------------------------------------
/// Constructor
//-----------------------------------------------------------------------

AttCsattb::AttCsattb
(const blitz::Array<double, 2>& Att, const Time& Tmin, double Tstep,
 bool Is_cf,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 AttitudeDataQuality A_quality,
 AttitudeSource A_source)
  : min_time_(Tmin), tstep_(Tstep), is_cf_(Is_cf),
    itype_(Itype), lagrange_order_(Lagrange_order),
    a_quality_(A_quality), a_source_(A_source), att(Att.copy())
{
}

//-----------------------------------------------------------------------
/// Constructor. We sample the attitude of the given Orbit at fixed
/// spaces times. This version goes from the min_time() of the Orbit,
/// up to max_time() (or more accurately, the largest time min_time()
/// + i * Tstep that is <= max_time()).
//-----------------------------------------------------------------------

AttCsattb::AttCsattb
(const Orbit& Orb, double Tstep,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 AttitudeDataQuality A_quality,
 AttitudeSource A_source,
 AttitudeType A_type)
  : min_time_(Orb.min_time()), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    a_quality_(A_quality), a_source_(A_source)
{
  if(A_type == SAME_AS_ORBIT)
    is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  else if(A_type == CARTESIAN_FIXED)
    is_cf_ = true;
  else
    is_cf_ = false;
  int sz = (int) floor((Orb.max_time() - min_time_) / tstep_) + 1;
  if((min_time_ + sz * tstep_) <= Orb.max_time())
    sz += 1;
  att.resize(sz, 4);
  for(int i = 0; i < sz; ++i) {
    Time t = min_time_ + i * tstep_;
    boost::shared_ptr<QuaternionOrbitData> od =
      boost::dynamic_pointer_cast<QuaternionOrbitData>(Orb.orbit_data(t));
    if(!od)
      throw Exception("AttCsattb only works with orbits that return a QuaternionOrbitData");
    if(is_cf_)
      att(i, blitz::Range::all()) = quaternion_to_nitf(od->sc_to_cf());
    else 
      att(i, blitz::Range::all()) = quaternion_to_nitf(od->sc_to_ci());
  }
}

//-----------------------------------------------------------------------
/// Constructor. We sample the attitude of the given Orbit at fixed
/// spaces times. This version goes from the Min_time,
/// up to Max_time (or more accurately, the largest time Min_time
/// + i * Tstep that is <= Max_time).w
//-----------------------------------------------------------------------

AttCsattb::AttCsattb
(const Orbit& Orb, const Time& Min_time, const Time& Max_time, double Tstep,
 InterpolationType Itype,
 LagrangeOrder Lagrange_order,
 AttitudeDataQuality A_quality,
 AttitudeSource A_source,
 AttitudeType A_type)
  : min_time_(Min_time), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    a_quality_(A_quality), a_source_(A_source)
{
  if(A_type == SAME_AS_ORBIT)
    is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  else if(A_type == CARTESIAN_FIXED)
    is_cf_ = true;
  else
    is_cf_ = false;
  int sz = (int) floor((Max_time - min_time_) / tstep_) + 1;
  if((min_time_ + sz * tstep_) <= Max_time)
    sz += 1;
  att.resize(sz, 4);
  for(int i = 0; i < sz; ++i) {
    Time t = min_time_ + i * tstep_;
    boost::shared_ptr<QuaternionOrbitData> od =
      boost::dynamic_pointer_cast<QuaternionOrbitData>(Orb.orbit_data(t));
    if(!od)
      throw Exception("AttCsattb only works with orbits that return a QuaternionOrbitData");
    if(is_cf_)
      att(i, blitz::Range::all()) = quaternion_to_nitf(od->sc_to_cf());
    else
      att(i, blitz::Range::all()) = quaternion_to_nitf(od->sc_to_ci());
  }
}

void AttCsattb::print(std::ostream& Os) const
{
  Os << "AttCsattb\n"
     << "  Min time:           " << min_time_ << "\n"
     << "  Max time:           " << max_time() << "\n"
     << "  Tstep:              " << tstep_ << "\n"
     << "  Is_cf:              " << is_cf_ << "\n"
     << "  Interpolation type: "
     << (itype_ == NEAREST_NEIGHBOR ? "Nearest Neighbor" :
	 (itype_ == LINEAR ? "Linear" : "Lagrange")) << "\n"
     << "  Lagrange order:     " << (int) lagrange_order_ << "\n"
     << "  Attitude quality:  " << (a_quality_ == ATTITUDE_QUALITY_SUSPECT ?
				     "Suspect" : "Good") << "\n"
     << "  Attitude source:   " << (a_source_ == PREDICTED ?
				     "Predicted" :
				     (a_source_ == ACTUAL ?
				      "Actual, time of collection" :
				      "Refined")) << "\n";
}

//-----------------------------------------------------------------------
/// Return min_time split into the component pieces the DES requires.
//-----------------------------------------------------------------------

void AttCsattb::min_time_split(std::string& d_mtime, std::string& t_mtime) const
{
  std::string mtime = min_time_.to_string();
  // Split into date and time parts
  // The time string is something like "1998-06-30T10:51:28.32Z"
  std::size_t t = mtime.find("T");
  d_mtime = mtime.substr(0, t);
  t_mtime = mtime.substr(t+1, -1);
  // Chop off trailing "Z" in time part"
  t_mtime = t_mtime.substr(0, t_mtime.size() - 1);
  // Remove the "-" in the date.
  d_mtime = boost::regex_replace(d_mtime, boost::regex("-"), "");
  // Remove the ":" in the time.
  t_mtime = boost::regex_replace(t_mtime, boost::regex(":"), "");
  // Pad with trailing "0" to full size"
  while(t_mtime.size() < 16)
    t_mtime += "0";
}

//-----------------------------------------------------------------------
/// Write out the DES data to the given stream.
//-----------------------------------------------------------------------

void AttCsattb::des_write(std::ostream& Os) const
{
  Os << str_check_size(frontpart % a_quality_ % itype_, 2);
  if(itype_ == LAGRANGE)
    Os << str_check_size(largangeorder % lagrange_order_, 1);
  // Indicate 1 for ECF, 0 for ECI
  int coor_frame = (is_cf_ ? 1 : 0);
  std::string d_mtime, t_mtime;
  min_time_split(d_mtime, t_mtime);
  Os << str_check_size(nextpart % a_source_ % coor_frame % tstep_ % d_mtime %
		       t_mtime % att.rows(), 1 + 1 + 13 + 8 + 16 + 5);
  for(int i = 0; i < att.rows(); ++i)
    Os << str_check_size(attnumformat % att(i,0), 18)
       << str_check_size(attnumformat % att(i,1), 18)
       << str_check_size(attnumformat % att(i,2), 18)
       << str_check_size(attnumformat % att(i,3), 18);
  Os << str_check_size(resformat % 0, 9);
}

//-----------------------------------------------------------------------
/// Read the DES data  the given stream.
//-----------------------------------------------------------------------

boost::shared_ptr<AttCsattb> AttCsattb::des_read(std::istream& In)
{
  boost::shared_ptr<AttCsattb> res(new AttCsattb());
  res->a_quality_ = AttitudeDataQuality(read_size<int>(In, 1));
  res->itype_ = InterpolationType(read_size<int>(In, 1));
  res->lagrange_order_ = NO_LAGRANGE;	
  if(res->itype_ == LAGRANGE)
    res->lagrange_order_ = LagrangeOrder(read_size<int> (In, 1));
  res->a_source_ = AttitudeSource(read_size<int>(In, 1));
  int coor_frame = read_size<int>(In, 1);
  res->is_cf_ = (coor_frame == 1);
  res->tstep_ = read_size<double>(In,13);
  std::string d_mtime = read_size<std::string>(In, 8);
  std::string t_mtime = read_size<std::string>(In, 16);
  // Add "-" and ":"
  d_mtime.insert(6, "-");
  d_mtime.insert(4, "-");
  t_mtime.insert(4, ":");
  t_mtime.insert(2, ":");
  res->min_time_ = Time::parse_time(d_mtime + "T" + t_mtime + "Z");
  int sz = read_size<int>(In, 5);
  res->att.resize(sz, 4);
  for(int i = 0; i < sz; ++i) {
    res->att(i,0) = read_size<double>(In, 18);
    res->att(i,1) = read_size<double>(In, 18);
    res->att(i,2) = read_size<double>(In, 18);
    res->att(i,3) = read_size<double>(In, 18);
  }
  int reserved_len = read_size<int>(In, 9);
  if(reserved_len > 0)
    std::string skipped = read_size<std::string>(In, reserved_len);
  return res;
}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

OrbitDes::OrbitDes(const boost::shared_ptr<PosCsephb>& Pos,
		   const boost::shared_ptr<AttCsattb>& Att,
		   int Naif_code)
  : Orbit(std::max(Pos->min_time(), Att->min_time()),
	  std::min(Pos->max_time(), Att->max_time())),
    pos_(Pos), att_(Att), naif_code_(Naif_code)
{
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data(Time T) const
{
  boost::math::quaternion<double> att_q = att_->att_q(T);
  blitz::Array<double, 1> posvel = pos_->pos_vel(T);
  boost::array<double, 3> vel;
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(!(pos_->is_cf() && att_->is_cf()) &&
     !(!pos_->is_cf() && !att_->is_cf()))
    throw Exception("pos_ and att_ need to either both be for cartesian fixed or both for cartesian inertial");
  boost::shared_ptr<QuaternionOrbitData> od;
  if(pos_->is_cf()) {
    boost::shared_ptr<CartesianFixed> cf;
    if(naif_code_ == Ecr::EARTH_NAIF_CODE)
      cf = boost::make_shared<Ecr>(posvel(0), posvel(1), posvel(2));
    else
      cf = boost::make_shared<PlanetFixed>(posvel(0), posvel(1), posvel(2),
					   naif_code_);
    od = boost::make_shared<QuaternionOrbitData>(T, cf, vel, att_q);
  } else {
    boost::shared_ptr<CartesianInertial> ci;
    if(naif_code_ == Ecr::EARTH_NAIF_CODE)
      ci = boost::make_shared<Eci>(posvel(0), posvel(1), posvel(2));
    else
      ci = boost::make_shared<PlanetInertial>(posvel(0), posvel(1), posvel(2),
					      naif_code_);
    od = boost::make_shared<QuaternionOrbitData>(T, ci, vel, att_q);
  }
  return od;
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data
(const TimeWithDerivative& T) const
{
  boost::math::quaternion<AutoDerivative<double> > att_q = att_->att_q(T);
  blitz::Array<AutoDerivative<double>, 1> posvel = pos_->pos_vel(T);
  boost::array<AutoDerivative<double>, 3> pos;
  boost::array<AutoDerivative<double>, 3> vel;
  pos[0] = posvel(0);
  pos[1] = posvel(1);
  pos[2] = posvel(2);
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(!(pos_->is_cf() && att_->is_cf()) &&
     !(!pos_->is_cf() && !att_->is_cf()))
    throw Exception("pos_ and att_ need to either both be for cartesian fixed or both for cartesian inertial");
  boost::shared_ptr<QuaternionOrbitData> od;
  if(pos_->is_cf()) {
    boost::shared_ptr<CartesianFixed> cf;
    if(naif_code_ == Ecr::EARTH_NAIF_CODE)
      cf = boost::make_shared<Ecr>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value());
    else
      cf = boost::make_shared<PlanetFixed>(posvel(0).value(), posvel(1).value(),
					   posvel(2).value(), naif_code_);
    od = boost::make_shared<QuaternionOrbitData>(T, cf, pos, vel, att_q);
  } else {
    boost::shared_ptr<CartesianInertial> ci;
    if(naif_code_ == Ecr::EARTH_NAIF_CODE)
      ci = boost::make_shared<Eci>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value());
    else
      ci = boost::make_shared<PlanetInertial>(posvel(0).value(),
					      posvel(1).value(),
					      posvel(2).value(), naif_code_);
    od = boost::make_shared<QuaternionOrbitData>(T, ci, pos, vel, att_q);
  }
  return od;
}

void OrbitDes::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitDes:\n"
     << "  naif_code: " << naif_code() << "\n"
     << "  pos_csephb:\n";
  opad << *pos_csephb() << "\n";
  opad.strict_sync();
  Os << "  att_csattb:\n"; 
  opad << *att_csattb() << "\n";
  opad.strict_sync();
}

//-----------------------------------------------------------------------
/// Switch from CartesianInertial to CartesianFixed or vice versa.
//-----------------------------------------------------------------------

void AttCsattb::convert_cf_and_ci(const OrbitDes& orb)
{
  blitz::Range ra = blitz::Range::all();
  blitz::Array<double, 2> att_new(att.shape());
  for(int i = 0; i < att.rows(); ++i) {
    Time t = min_time_ + tstep_ * i;
    boost::shared_ptr<QuaternionOrbitData> od =
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb.orbit_data(t));
    boost::math::quaternion<double> attq = nitf_to_quaternion(att(i, ra));
    normalize(attq);
    if(is_cf_) {
      QuaternionOrbitData od2(od->time(), od->position_cf(),
			      od->velocity_cf(), attq);
      att_new(i, ra) = quaternion_to_nitf(od2.sc_to_ci());
    } else {
      QuaternionOrbitData od2(od->time(), od->position_ci(),
			      od->velocity_ci(), attq);
      att_new(i, ra) = quaternion_to_nitf(od2.sc_to_cf());
    }
  }
  att.reference(att_new);
  // We lie briefly. OrbitDes which is a friend of this class sets
  // this once PosCsephb is also converted
  // if(is_cf_)
  //   is_cf_ = false;
  // else
  //   is_cf_ = true;
}

//-----------------------------------------------------------------------
/// Switch from CartesianInertial to CartesianFixed or vice versa.
//-----------------------------------------------------------------------

void PosCsephb::convert_cf_and_ci(const OrbitDes& orb)
{
  blitz::Array<double, 2> pos_new(pos.shape());
  for(int i = 0; i < pos.rows(); ++i) {
    Time t = min_time_ + tstep_ * i;
    boost::shared_ptr<QuaternionOrbitData> od =
      boost::dynamic_pointer_cast<QuaternionOrbitData>(orb.orbit_data(t));
    boost::array<double, 3> p;
    p[0] = pos(i,0);
    p[1] = pos(i,1);
    p[2] = pos(i,2);
    if(is_cf_) {
      QuaternionOrbitData od2(od->time(), od->position_cf()->create(p),
			      od->velocity_cf(), od->sc_to_cf());
      pos_new(i, 0) = od2.position_ci()->position[0];
      pos_new(i, 1) = od2.position_ci()->position[1];
      pos_new(i, 2) = od2.position_ci()->position[2];
    } else {
      QuaternionOrbitData od2(od->time(), od->position_ci()->create(p),
			      od->velocity_ci(), od->sc_to_ci());
      pos_new(i, 0) = od2.position_cf()->position[0];
      pos_new(i, 1) = od2.position_cf()->position[1];
      pos_new(i, 2) = od2.position_cf()->position[2];
    }
  }
  pos.reference(pos_new);
  if(is_cf_)
    is_cf_ = false;
  else
    is_cf_ = true;
}

