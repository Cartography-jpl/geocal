#include "orbit_des.h"
#include "ostream_pad.h"
#include "ecr.h"
#include "eci.h"
#include "geocal_serialize_support.h"
#include "tre_support.h"
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
void OrbitDes::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP_(pos);
}

GEOCAL_IMPLEMENT(PosCsephb);
GEOCAL_IMPLEMENT(OrbitDes);
#endif

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
 EphemerisSource E_source)
  : min_time_(Orb.min_time()), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    e_quality_(E_quality), e_source_(E_source)
{
  is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  blitz::Array<double, 1> p(3);
  for(Time t = min_time_; t <= Orb.max_time(); t += tstep_) {
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      p = cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      p = ci->position[0], ci->position[1], ci->position[2];
    }
    pos.push_back(p.copy());
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
 EphemerisSource E_source)
  : min_time_(Min_time), tstep_(Tstep),
    itype_(Itype), lagrange_order_(Lagrange_order),
    e_quality_(E_quality), e_source_(E_source)
{
  is_cf_ = Orb.orbit_data(min_time_)->prefer_cf();
  blitz::Array<double, 1> p(3);
  for(Time t = min_time_; t <= Max_time; t += tstep_) {
    if(is_cf_) {
      boost::shared_ptr<CartesianFixed> cf = Orb.position_cf(t);
      p = cf->position[0], cf->position[1], cf->position[2];
    } else {
      boost::shared_ptr<CartesianInertial> ci = Orb.position_ci(t);
      p = ci->position[0], ci->position[1], ci->position[2];
    }
    pos.push_back(p.copy());
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
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<double, 1> PosCsephb::pos_vel(const Time& T) const
{
  range_check_inclusive(T, min_time_, max_time());
  // Just linear interpolation for now. We'll do more complicated
  // interpolation shortly.
  if(itype_ != LINEAR)
    throw Exception("Only do linear interpolation for now");
  int i = (int) floor((T - min_time_) / tstep_);
  if(i >= (int) pos.size() - 1)
    --i;
  double f = (T - min_time_) / tstep_ - i;
  blitz::Array<double, 1> res(6);
  res(blitz::Range(0,2)) = f * pos[i+1] + (1 - f) * pos[i];
  res(blitz::Range(3,5)) = (pos[i+1] - pos[i]) / tstep_;
  return res;
}

//-----------------------------------------------------------------------
/// Return position and velocity for the given time.
//-----------------------------------------------------------------------

blitz::Array<AutoDerivative<double>, 1> PosCsephb::pos_vel
(const TimeWithDerivative& T) const
{
  range_check_inclusive(T.value(), min_time_, max_time());
  // Just linear interpolation for now. We'll do more complicated
  // interpolation shortly.
  if(itype_ != LINEAR)
    throw Exception("Only do linear interpolation for now");
  int i = (int) floor((T.value() - min_time_) / tstep_);
  if(i >= (int) pos.size())
    --i;
  AutoDerivative<double> f = (T - min_time_) / tstep_ - i;
  blitz::Array<AutoDerivative<double>, 1> res(6);
  res(blitz::Range(0,2)) = f * pos[i+1] + (1 - f) * pos[i];
  res(blitz::Range(3,5)) = (pos[i+1] - pos[i]) / tstep_;
  return res;
}

static boost::format frontpart("%|1$01d|%|2$01d|");
static boost::format largangeorder("%|1$01d|");
static boost::format nextpart("%|1$01d|%|2$01d|%|3$013.9f|%|4$8s|%|5$16s|%|6$05d|");
static boost::format numformat("%|1$+012.2f|");
static boost::format resformat("%|1$05d|");

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
  std::string mtime = min_time_.to_string();
  // Split into date and time parts
  // The time string is something like "1998-06-30T10:51:28.32Z"
  std::size_t t = mtime.find("T");
  std::string d_mtime = mtime.substr(0, t);
  std::string t_mtime = mtime.substr(t+1, -1);
  // Chop off trailing "Z" in time part"
  t_mtime = t_mtime.substr(0, t_mtime.size() - 1);
  // Remove the "-" in the date.
  d_mtime = boost::regex_replace(d_mtime, boost::regex("-"), "");
  // Remove the ":" in the time.
  t_mtime = boost::regex_replace(t_mtime, boost::regex(":"), "");
  // Pad with trailing "0" to full size"
  while(t_mtime.size() < 16)
    t_mtime += "0";
  Os << str_check_size(nextpart % e_source_ % coor_frame % tstep_ % d_mtime %
		       t_mtime % (int) pos.size(), 1 + 1 + 13 + 8 + 16 + 5);
  typedef blitz::Array<double, 1> atype;
  BOOST_FOREACH(const atype& v, pos)
    Os << str_check_size(numformat % v(0), 12)
       << str_check_size(numformat % v(1), 12)
       << str_check_size(numformat % v(2), 12);
  Os << str_check_size(resformat % 0, 5);
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
  res->pos.reserve(sz);
  for(int i = 0; i < sz; ++i) {
    blitz::Array<double, 1> v(3);
    v(0) = read_size<double>(In, 12);
    v(1) = read_size<double>(In, 12);
    v(2) = read_size<double>(In, 12);
    res->pos.push_back(v);
  }
  int reserved_len = read_size<int>(In, 5);
  if(reserved_len > 0)
    std::string skipped = read_size<std::string>(In, reserved_len);
  return res;
}

//-----------------------------------------------------------------------
/// Constructor.
//-----------------------------------------------------------------------

OrbitDes::OrbitDes(const boost::shared_ptr<PosCsephb>& Pos)
  : Orbit(Pos->min_time(), Pos->max_time()),
    pos_(Pos)
{
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data(Time T) const
{
  // Temporary
  boost::math::quaternion<double> q(0,0,0,1);
  blitz::Array<double, 1> posvel = pos_->pos_vel(T);
  boost::array<double, 3> vel;
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(pos_->is_cf())
    return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Ecr>(posvel(0), posvel(1), posvel(2)), vel, q);
  return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Eci>(posvel(0), posvel(1), posvel(2)), vel, q);
}

boost::shared_ptr<OrbitData> OrbitDes::orbit_data
(const TimeWithDerivative& T) const
{
  // Temporary
  boost::math::quaternion<AutoDerivative<double> > q(0,0,0,1);
  blitz::Array<AutoDerivative<double>, 1> posvel = pos_->pos_vel(T);
  boost::array<AutoDerivative<double>, 3> pos;
  boost::array<AutoDerivative<double>, 3> vel;
  pos[0] = posvel(0);
  pos[1] = posvel(1);
  pos[2] = posvel(2);
  vel[0] = posvel(3);
  vel[1] = posvel(4);
  vel[2] = posvel(5);
  if(pos_->is_cf())
    return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Ecr>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value()),
	   pos, vel, q);
  return boost::make_shared<QuaternionOrbitData>(T,
	   boost::make_shared<Eci>(posvel(0).value(), posvel(1).value(),
				   posvel(2).value()),
	   pos, vel, q);
}

void OrbitDes::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitDes:\n"
     << "  pos_csephb:\n";
  opad << *pos_csephb() << "\n";
  opad.strict_sync();
}
