#include "orbit_offset_correction.h"
#include "geocal_serialize_support.h"
#include "ostream_pad.h"
#include "constant.h"
#include <boost/foreach.hpp>
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitOffsetCorrection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Orbit)
    & GEOCAL_NVP(orb_uncorr)
    & GEOCAL_NVP(att_corr)
    & GEOCAL_NVP(pos_corr)
    & GEOCAL_NVP_(outside_is_error)
    & GEOCAL_NVP_(fit_position_x)
    & GEOCAL_NVP_(fit_position_y)
    & GEOCAL_NVP_(fit_position_z)
    & GEOCAL_NVP_(fit_yaw)
    & GEOCAL_NVP_(fit_pitch)
    & GEOCAL_NVP_(fit_roll);
}

GEOCAL_IMPLEMENT(OrbitOffsetCorrection);
#endif

//-----------------------------------------------------------------------
/// Constructor. This has no time points for doing corrections, but
/// you can add those using insert_time_point.
//-----------------------------------------------------------------------

OrbitOffsetCorrection::OrbitOffsetCorrection(
const boost::shared_ptr<Orbit> Orb_uncorr,
bool Outside_is_error,
bool Fit_position_x,
bool Fit_position_y,
bool Fit_position_z,
bool Fit_yaw,
bool Fit_pitch,
bool Fit_roll)
: Orbit(Orb_uncorr->min_time(), Orb_uncorr->max_time()),
  orb_uncorr(Orb_uncorr),
  outside_is_error_(Outside_is_error),
  fit_position_x_(Fit_position_x),
  fit_position_y_(Fit_position_y),
  fit_position_z_(Fit_position_z),
  fit_yaw_(Fit_yaw),
  fit_pitch_(Fit_pitch),
  fit_roll_(Fit_roll)
{
  boost::array<AutoDerivative<double>, 3> pc;
  pc[0] = 0;
  pc[1] = 0;
  pc[2] = 0;
  pos_corr[min_time()] = pc;
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

// See base class for description
boost::shared_ptr<OrbitData> OrbitOffsetCorrection::orbit_data(Time T) const
{
  boost::shared_ptr<QuaternionOrbitData> oc_uncorr = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
  if(!oc_uncorr)
    throw Exception("OrbitOffsetCorrection only works with orbits that return a QuaternionOrbitData");
  boost::array<AutoDerivative<double>, 3> poff;

  if(pos_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      poff[2] = poff[1] = poff[0] = 0;
  } else {
    pos_map_type::const_iterator i = pos_corr.lower_bound(T);
    if(i == pos_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	for(int j = 0; j < 3; ++j)
	  poff[j] = pos_corr.rbegin()->second[j].value();
      }
    } else if(i == pos_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	for(int j = 0; j < 3; ++j)
	  poff[j] = i->second[j].value();
      }
    } else {
      boost::array<AutoDerivative<double>, 3> p2 = i->second;
      Time t2 = i->first;
      --i;
      boost::array<AutoDerivative<double>, 3 > p1 = i->second;
      Time t1 = i->first;
      throw Exception("Not done yet");
      //blah;
      //acorr = value(interpolate(q1, q2, T - t1, t2 - t1));
    }
  }

  boost::math::quaternion<AutoDerivative<double> > acorr;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      acorr = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  } else {
    att_map_type::const_iterator i = att_corr.lower_bound(T);
    if(i == att_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	acorr = value(att_corr.rbegin()->second);
      }
    } else if(i == att_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	acorr = value(i->second);
      }
    } else {
      boost::math::quaternion<AutoDerivative<double> > q2 = i->second;
      Time t2 = i->first;
      --i;
      boost::math::quaternion<AutoDerivative<double> > q1 = i->second;
      Time t1 = i->first;
      acorr = value(interpolate(q1, q2, T - t1, t2 - t1));
    }
  }
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(*oc_uncorr, poff, acorr));
}

// See base class for description
boost::shared_ptr<OrbitData> 
OrbitOffsetCorrection::orbit_data(const TimeWithDerivative& T) const
{
  boost::shared_ptr<QuaternionOrbitData> oc_uncorr = 
    boost::dynamic_pointer_cast<QuaternionOrbitData>(orb_uncorr->orbit_data(T));
  if(!oc_uncorr)
    throw Exception("OrbitOffsetCorrection only works with orbits that return a QuaternionOrbitData");

  boost::array<AutoDerivative<double>, 3 > pcorr;
  if(pos_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      for(int j = 0; j < 3; ++j)
	pcorr[j] = 0;
  } else {
    pos_map_type::const_iterator i = pos_corr.lower_bound(T.value());
    if(i == pos_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T.value() << " is outside of range of time points";
	throw e;
      } else {
	pcorr = pos_corr.rbegin()->second;
      }
    } else if(i == pos_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
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
      throw Exception("Not done yet");
      //blah;
      //acorr = value(interpolate(q1, q2, T - t1, t2 - t1));
    }
  }

  boost::math::quaternion<AutoDerivative<double> > acorr;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      acorr = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  } else {
    att_map_type::const_iterator i = att_corr.lower_bound(T.value());
    if(i == att_corr.end()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T.value() << " is outside of range of time points";
	throw e;
      } else {
	acorr = att_corr.rbegin()->second;
      }
    } else if(i == att_corr.begin()) {
      if(outside_is_error_) {
	Exception e;
	e << "Time " << T << " is outside of range of time points";
	throw e;
      } else {
	acorr = i->second;
      }
    } else {
      boost::math::quaternion<AutoDerivative<double> > q2 = i->second;
      Time t2 = i->first;
      --i;
      boost::math::quaternion<AutoDerivative<double> > q1 = i->second;
      Time t1 = i->first;
      acorr = interpolate(q1, q2, T - t1, t2 - t1);
    }
  }
  return boost::shared_ptr<OrbitData>
    (new QuaternionOrbitData(*oc_uncorr, pcorr, acorr));
}

// See base class for description
ArrayAd<double, 1> OrbitOffsetCorrection::parameter_with_derivative() const
{
  blitz::Array<AutoDerivative<double>, 1> res(3 + 3 * att_corr.size());
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
  Os << "  Fit position x: " << (fit_position_x() ? "True" : "False") << "\n"
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

