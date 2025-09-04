#include "orbit_offset_correction.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include "constant.h"
#include "ecr.h"
#include <boost/foreach.hpp>
#include <boost/make_shared.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitOffsetCorrection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(OrbitCorrection)
    & GEOCAL_NVP(att_corr)
    & GEOCAL_NVP(pos_corr)
    & GEOCAL_NVP_(outside_is_error)
    & GEOCAL_NVP_(fit_position_x)
    & GEOCAL_NVP_(fit_position_y)
    & GEOCAL_NVP_(fit_position_z)
    & GEOCAL_NVP_(fit_yaw)
    & GEOCAL_NVP_(fit_pitch)
    & GEOCAL_NVP_(fit_roll)
    & GEOCAL_NVP_(use_local_north_coordinate);
}

GEOCAL_IMPLEMENT(OrbitOffsetCorrection);
#endif

//-----------------------------------------------------------------------
/// Position correction with derivative.
//-----------------------------------------------------------------------

boost::array<AutoDerivative<double>, 3 > 
OrbitOffsetCorrection::pcorr_with_derivative(
const TimeWithDerivative& Tm,
const CartesianFixed& Pos_uncorr
) const
{
  boost::array<AutoDerivative<double>, 3 > pcorr;
  if(pos_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      for(int j = 0; j < 3; ++j)
	pcorr[j] = 0;
  } else {
    pos_map_type::const_iterator i = pos_corr.lower_bound(Tm.value());
    if(i == pos_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << Tm.value() << " is outside of range of time points";
	throw e;
      } else {
	pcorr = pos_corr.rbegin()->second;
      }
    } else if(i == pos_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << Tm.value() << " is outside of range of time points";
	throw e;
      } else {
	pcorr = i->second;
      }
    } else {
      boost::array<AutoDerivative<double>, 3 > p2 = i->second;
      Time t2 = i->first;
      --i;
      boost::array<AutoDerivative<double>, 3 > p1 = i->second;
      Time t1 = i->first;
      AutoDerivative<double> tspace = (Tm - t1) / (t2 - t1);
      for(int j = 0; j < 3; ++j)
	pcorr[j] = p1[j] + (p2[j] - p1[j]) * tspace;
    }
  }
  if(use_local_north_coordinate_)  {
    if(!pos_corr_is_cf())
      throw Exception("Can't use local north coordinates if underlying orbit uses CartesianInertial");
    LnLookVectorWithDerivative lv(pcorr);
    CartesianFixedLookVectorWithDerivative clv = lv.to_cf(Pos_uncorr);
    pcorr = clv.look_vector;
  }
  return pcorr;
}

//-----------------------------------------------------------------------
/// Position correction.
//-----------------------------------------------------------------------

boost::array<double, 3 > 
OrbitOffsetCorrection::pcorr(
const Time& Tm,
const CartesianFixed& Pos_uncorr
) const
{
  boost::array<double, 3 > pcorr;
  if(pos_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      for(int j = 0; j < 3; ++j)
	pcorr[j] = 0;
  } else {
    pos_map_type::const_iterator i = pos_corr.lower_bound(Tm);
    if(i == pos_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << Tm << " is outside of range of time points";
	throw e;
      } else {
	for(int j = 0; j < 3; ++j)
	  pcorr[j] = pos_corr.rbegin()->second[j].value();
      }
    } else if(i == pos_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << Tm << " is outside of range of time points";
	throw e;
      } else {
	for(int j = 0; j < 3; ++j)
	  pcorr[j] = i->second[j].value();
      }
    } else {
      boost::array<double, 3 > p2;
      for(int j = 0; j < 3; ++j)
	p2[j] = i->second[j].value();
      Time t2 = i->first;
      --i;
      boost::array<double, 3 > p1;
      for(int j = 0; j < 3; ++j)
	p1[j] = i->second[j].value();
      Time t1 = i->first;
      double tspace = (Tm - t1) / (t2 - t1);
      for(int j = 0; j < 3; ++j)
	pcorr[j] = p1[j] + (p2[j] - p1[j]) * tspace;
    }
  }
  if(use_local_north_coordinate_)  {
    if(!pos_corr_is_cf())
      throw Exception("Can't use local north coordinates if underlying orbit uses CartesianInertial");
    LnLookVector lv(pcorr);
    CartesianFixedLookVector clv = lv.to_cf(Pos_uncorr);
    pcorr = clv.look_vector;
  }
  return pcorr;
}

//-----------------------------------------------------------------------
/// Attitude correction with derivative.
//-----------------------------------------------------------------------

boost::math::quaternion<AutoDerivative<double> > 
OrbitOffsetCorrection::acorr_with_derivative(const TimeWithDerivative& T) const
{
  boost::math::quaternion<AutoDerivative<double> > res;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      res = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  } else {
    att_map_type::const_iterator i = att_corr.lower_bound(T.value());
    if(i == att_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T.value() << " is outside of range of time points";
	throw e;
      } else {
	res = att_corr.rbegin()->second;
      }
    } else if(i == att_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	res = i->second;
      }
    } else {
      boost::math::quaternion<AutoDerivative<double> > q2 = i->second;
      Time t2 = i->first;
      --i;
      boost::math::quaternion<AutoDerivative<double> > q1 = i->second;
      Time t1 = i->first;
      res = interpolate(q1, q2, T - t1, t2 - t1);
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// Attitude correction.
//-----------------------------------------------------------------------

boost::math::quaternion<double> 
OrbitOffsetCorrection::acorr(const Time& T) const
{
  boost::math::quaternion<double> res;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      res = boost::math::quaternion<double>(1,0,0,0);
  } else {
    att_map_type::const_iterator i = att_corr.lower_bound(T);
    if(i == att_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	res = value(att_corr.rbegin()->second);
      }
    } else if(i == att_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	res = value(i->second);
      }
    } else {
      boost::math::quaternion<double> q2 = value(i->second);
      Time t2 = i->first;
      --i;
      boost::math::quaternion<double> q1 = value(i->second);
      Time t1 = i->first;
      res = interpolate(q1, q2, T - t1, t2 - t1);
    }
  }
  return res;
}

//-----------------------------------------------------------------------
/// Constructor. This has no time points for doing corrections, but
/// you can add those using insert_time_point.
//-----------------------------------------------------------------------

OrbitOffsetCorrection::OrbitOffsetCorrection(
const boost::shared_ptr<Orbit> Orb_uncorr,
bool Outside_is_error,
bool Use_local_north_coordinate,
bool Fit_position_x,
bool Fit_position_y,
bool Fit_position_z,
bool Fit_yaw,
bool Fit_pitch,
bool Fit_roll)
: OrbitCorrection(Orb_uncorr),
  outside_is_error_(Outside_is_error),
  fit_position_x_(Fit_position_x),
  fit_position_y_(Fit_position_y),
  fit_position_z_(Fit_position_z),
  fit_yaw_(Fit_yaw),
  fit_pitch_(Fit_pitch),
  fit_roll_(Fit_roll),
  use_local_north_coordinate_(Use_local_north_coordinate)
{
}

//-----------------------------------------------------------------------
/// Return list of time points that we have attitude corrections for.
//-----------------------------------------------------------------------

std::vector<Time> OrbitOffsetCorrection::attitude_time_point() const
{
  std::vector<Time> res;
  BOOST_FOREACH(att_map_pair_type e, att_corr)
    res.push_back(e.first);
  return res;
}

//-----------------------------------------------------------------------
/// Return the parameters to use for a new attitude time point at Tm
/// to match the current value (e.g, useful for adding a new
/// breakpoint w/o changing the calculated values.
//-----------------------------------------------------------------------

blitz::Array<double, 1> OrbitOffsetCorrection::att_parm_to_match(const Time& Tm) const
{
  blitz::Array<double, 1> res(3);
  quat_to_ypr(acorr(Tm), res(0), res(1), res(2));
  res /= Constant::arcsecond_to_rad;
  return res;
}

//-----------------------------------------------------------------------
/// Return list of time points that we have position corrections for.
//-----------------------------------------------------------------------

std::vector<Time> OrbitOffsetCorrection::position_time_point() const
{
  std::vector<Time> res;
  BOOST_FOREACH(pos_map_pair_type e, pos_corr)
    res.push_back(e.first);
  return res;
}

// See base class for description
ArrayAd<double, 1> OrbitOffsetCorrection::parameter_with_derivative() const
{
  blitz::Array<AutoDerivative<double>, 1> res(3 * pos_corr.size() + 3 * att_corr.size());
  int i = 0;
  BOOST_FOREACH(pos_map_pair_type e, pos_corr) {
    res(i + 0) = e.second[0];
    res(i + 1) = e.second[1];
    res(i + 2) = e.second[2];
    i += 3;
  }
  BOOST_FOREACH(att_map_pair_type e, att_corr) {
    quat_to_ypr(e.second, res(i + 0), res(i + 1), res(i + 2));
    res(i + 0) /=  Constant::arcsecond_to_rad;
    res(i + 1) /=  Constant::arcsecond_to_rad;
    res(i + 2) /=  Constant::arcsecond_to_rad;
    i += 3;
  }
  return res;
}

// See base class for description
void OrbitOffsetCorrection::parameter_with_derivative
(const ArrayAd<double, 1>& Parm)
{
  int i = 0;
  pos_map_type::iterator p;
  for(p = pos_corr.begin(); p != pos_corr.end(); ++p, i += 3) {
    p->second[0] = Parm(i + 0);
    p->second[1] = Parm(i + 1);
    p->second[2] = Parm(i + 2);
  }
  att_map_type::iterator e;
  for(e = att_corr.begin(); e != att_corr.end(); ++e, i += 3)
    e->second = quat_rot("xyz", Parm(i + 1) * Constant::arcsecond_to_rad, 
			 Parm(i + 2) * Constant::arcsecond_to_rad, 
			 Parm(i + 0) * Constant::arcsecond_to_rad);
  notify_update();
}

// See base class for description
std::vector<std::string> OrbitOffsetCorrection::parameter_name() const
{
  std::vector<std::string> res;
  BOOST_FOREACH(pos_map_pair_type e, pos_corr) {
    res.push_back("Position X Offset time " + e.first.to_string() + 
		  "(meter)");
    res.push_back("Position Y Offset time " + e.first.to_string() + 
		  "(meter)");
    res.push_back("Position Z Offset time " + e.first.to_string() + 
		  "(meter)");
  }
  BOOST_FOREACH(att_map_pair_type e, att_corr) {
    res.push_back("Yaw correction time " + e.first.to_string() + 
		  " (arcseconds)");
    res.push_back("Pitch correction time " + e.first.to_string() +
		  " (arcseconds)");
    res.push_back("Roll correction time " + e.first.to_string() +
		  " (arcseconds)");
  }
  return res;
}

// See base class for description
blitz::Array<bool, 1> OrbitOffsetCorrection::parameter_mask() const
{
  blitz::Array<bool, 1> res(3 * pos_corr.size() + 3 * att_corr.size());
  int j = 0;
  for(int i = 0; i < (int) pos_corr.size(); ++i, j+=3) {
    res(j + 0) = fit_position_x();
    res(j + 1) = fit_position_y();
    res(j + 2) = fit_position_z();
  }
  for(int i = 0; i < (int) att_corr.size(); ++i, j+=3) {
    res(j + 0) = fit_yaw();
    res(j + 1) = fit_pitch();
    res(j + 2) = fit_roll();
  }
  return res;
}

// See base class for description
void OrbitOffsetCorrection::print(std::ostream& Os) const
{
  OstreamPad opad(Os, "    ");
  Os << "OrbitOffsetCorrection\n"
     << "  Underlying orbit:\n";
  opad << *orbit_uncorrected() << "\n";
  opad.strict_sync();
  Os << " Use LocalNorth:  " 
     << (use_local_north_coordinate() ? "True" : "False") << "\n"
     << "  Fit position x: " << (fit_position_x() ? "True" : "False") << "\n"
     << "  Fit position y: " << (fit_position_y() ? "True" : "False") << "\n"
     << "  Fit position z: " << (fit_position_z() ? "True" : "False") << "\n"
     << "  Fit yaw:        " << (fit_yaw() ? "True" : "False") << "\n"
     << "  Fit pitch:      " << (fit_pitch() ? "True" : "False") << "\n"
     << "  Fit roll:       " << (fit_roll() ? "True" : "False") << "\n";
  Os << "  Parameter:\n";
  std::vector<std::string> pname = parameter_name();
  blitz::Array<double, 1> parm = parameter();
  const static int sv_num_width = 17;
  for(int i = 0; i < parm.rows(); ++i)
    Os << "     " << std::setprecision(sv_num_width - 7) 
       << std::setw(sv_num_width) << parm(i) << "  "
       << pname[i] << "\n";
}

//-----------------------------------------------------------------------
/// Return the time points and corrections. This is primarily of use
/// for python wrappers, to give access to the same kind of
/// information that std::cout << *this gives.
///
/// We return the attitude correction in arcseconds, one row per time
/// point. The columns are yaw, pitch, and roll. The position
/// correction is returned in meters, one row  per time point. The
/// columns are X, Y, Z offset.
//-----------------------------------------------------------------------

void OrbitOffsetCorrection::orbit_correction_parameter
(std::vector<boost::shared_ptr<Time> >& Attitude_time_point,
 blitz::Array<double, 2>& Attitude_corr,
 std::vector<boost::shared_ptr<Time> >&
 Position_time_point,
 blitz::Array<double, 2>& Position_corr)
{
  Attitude_time_point.clear();
  Position_time_point.clear();
  BOOST_FOREACH(att_map_pair_type e, att_corr)
    Attitude_time_point.push_back(boost::make_shared<Time>(e.first));
  BOOST_FOREACH(pos_map_pair_type e, pos_corr)
    Position_time_point.push_back(boost::make_shared<Time>(e.first));
  Attitude_corr.resize((int) Attitude_time_point.size(), 3);
  Position_corr.resize((int) Position_time_point.size(), 3);
  int i = 0;
  BOOST_FOREACH(att_map_pair_type e, att_corr) {
    AutoDerivative<double> y,p,r;
    quat_to_ypr(e.second, y,p,r);
    Attitude_corr(i, 0) = y.value() / Constant::arcsecond_to_rad;
    Attitude_corr(i, 1) = p.value() / Constant::arcsecond_to_rad;
    Attitude_corr(i, 2) = r.value() / Constant::arcsecond_to_rad;
    ++i;
  }
  i = 0;
  BOOST_FOREACH(pos_map_pair_type e, pos_corr) {
    Position_corr(i, 0) = e.second[0].value();
    Position_corr(i, 1) = e.second[1].value();
    Position_corr(i, 2) = e.second[2].value();
    ++i;
  }
}
