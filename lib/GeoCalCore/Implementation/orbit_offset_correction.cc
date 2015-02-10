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
    & GEOCAL_NVP_(fit_position)
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
bool Fit_position,
bool Fit_yaw,
bool Fit_pitch,
bool Fit_roll)
: Orbit(Orb_uncorr->min_time(), Orb_uncorr->max_time()),
  orb_uncorr(Orb_uncorr),
  outside_is_error_(Outside_is_error),
  fit_position_(Fit_position),
  fit_yaw_(Fit_yaw),
  fit_pitch_(Fit_pitch),
  fit_roll_(Fit_roll)
{
  pos_corr[0] = 0;
  pos_corr[1] = 0;
  pos_corr[2] = 0;
}

//-----------------------------------------------------------------------
/// Return list of time points that we have corrections for.
//-----------------------------------------------------------------------

std::vector<Time> OrbitOffsetCorrection::time_point() const
{
  std::vector<Time> res;
  BOOST_FOREACH(map_pair_type e, att_corr)
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
  boost::array<AutoDerivative<double>, 3> poff = 
    {{pos_corr[0].value(), pos_corr[1].value(), pos_corr[2].value() }};
  boost::math::quaternion<AutoDerivative<double> > acorr;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      acorr = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  } else {
    map_type::const_iterator i = att_corr.lower_bound(T);
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
  boost::math::quaternion<AutoDerivative<double> > acorr;
  if(att_corr.size() == 0) {
    if(outside_is_error_)
      throw Exception("Can interpolate because we have no time points");
    else
      acorr = boost::math::quaternion<AutoDerivative<double> >(1,0,0,0);
  } else {
    map_type::const_iterator i = att_corr.lower_bound(T.value());
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
    (new QuaternionOrbitData(*oc_uncorr, pos_corr, acorr));
}

// See base class for description
ArrayAd<double, 1> OrbitOffsetCorrection::parameter_with_derivative() const
{
  blitz::Array<AutoDerivative<double>, 1> res(3 + 3 * att_corr.size());
  res(0) = pos_corr[0];
  res(1) = pos_corr[1];
  res(2) = pos_corr[2];
  int i = 3;
  BOOST_FOREACH(map_pair_type e, att_corr) {
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
  pos_corr[0] = Parm(0);
  pos_corr[1] = Parm(1);
  pos_corr[2] = Parm(2);
  int i = 3;
  map_type::iterator e;
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
  res.push_back("Position X Offset (meter)");
  res.push_back("Position Y Offset (meter)");
  res.push_back("Position Z Offset (meter)");
  BOOST_FOREACH(map_pair_type e, att_corr) {
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
  blitz::Array<bool, 1> res(3 + 3 * att_corr.size());
  res(0) = fit_position();
  res(1) = fit_position();
  res(2) = fit_position();
  for(int i = 0; i < (int) att_corr.size(); ++i) {
    res(3 + i * 3 + 0) = fit_yaw();
    res(3 + i * 3 + 1) = fit_pitch();
    res(3 + i * 3 + 2) = fit_roll();
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
  Os << "  Fit position: " << (fit_position() ? "True" : "False") << "\n"
     << "  Fit yaw:      " << (fit_yaw() ? "True" : "False") << "\n"
     << "  Fit pitch:    " << (fit_pitch() ? "True" : "False") << "\n"
     << "  Fit roll:     " << (fit_roll() ? "True" : "False") << "\n";
  Os << "  Parameter:\n";
  std::vector<std::string> pname = parameter_name();
  blitz::Array<double, 1> parm = parameter();
  const static int sv_num_width = 17;
  for(int i = 0; i < parm.rows(); ++i)
    Os << "     " << std::setprecision(sv_num_width - 7) 
       << std::setw(sv_num_width) << parm(i) << "  "
       << pname[i] << "\n";
}

